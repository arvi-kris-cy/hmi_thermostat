/******************************************************************************
* File Name:   mqtt_command_handler.c
*
* Description:   Implements MQTT command parsing and creation.
*
*******************************************************************************
* Copyright 2024-2025, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/**
 * Headers Includes
 */
#include "mqtt_command_handler.h"
#include "publisher_task.h"
#include "subscriber_task.h"
#include "wireless_manager.h"
#include "comm_manager.h"
#include "mqtt_task.h"

/**
 * Macros
 */
#define JSON_KEY_FOR_TYPEOFOPERATION			"type"
#define JSON_KEY_FOR_COMMAND					"cmd_id"
#define JSON_KEY_FOR_VALUE						"value"
#define SHIFT_LEFT(shift_amount) ((1) << (shift_amount))
#define IS_BIT_SET(value, bit) (((value) & (1U << (bit))) != 0)
/**
 * Global Declaration
 */
extern QueueHandle_t publisher_task_q;
extern QueueHandle_t mqtt_task_q;

publisher_data_t publisher_q_data;

device_state_t device_status;

const unsigned int common_get_command = SHIFT_LEFT(DEVICE_STATUS) | SHIFT_LEFT(TARGETED_TEMP) |
									SHIFT_LEFT(GETDEVICE_CURRENT_HUMIDITY) | SHIFT_LEFT(GETDEVICE_CURRENT_CO2LEVEL) |
									SHIFT_LEFT(FIRMWARE_UPDATE_PROGRESS) | SHIFT_LEFT(FIRMWARE_UPDATE_STATUS);
/**
 * Static Function Declarations
 */
/**
 * @brief Parses a numeric value from the specified JSON message using the given token key.
 */
static double json_parsenumeric(const char *message, const char *token);

/**
 * @brief Handles the DEVICE_STATUS command and responds with the current status of the device.
 */
static void handle_devicestatus_command(void);

/**
 * @brief Handles the DEVICE_MODE command to set or get the current device mode.
 */
static void handle_devicemode_command(operation_type_e type, thermostat_mode_t mode);

/**
 * @brief Handles the TARGETED_TEMP command to fetch the current target temperature.
 */
static void handle_gettergettemp_command(void);

/**
 * @brief Handles the SETDEVICE_TEMP_UPDOWN command to adjust temperature up or down.
 */
static void handle_settempupdown_command(temp_updown_e updown);

/**
 * @brief Handles the FAN_SPEED command to set or get the current fan speed.
 */
static void handle_fanspeed_command(operation_type_e type, fan_speed_t speed);

/**
 * @brief Handles commands to retrieve current environmental parameters such as humidity or CO₂.
 */
static void handle_getcurrentpara_command(mqtt_commandId_e cmd);

/**
 * @brief Handles commands to retrieve and set current temp.
 */
static void handle_currenttemp_command(operation_type_e type, uint32_t value);

/**
 * @brief Handles the DEVICE_BRIGHTNESS command to update or retrieve the display brightness level.
 */
static void handle_brightness_command(operation_type_e type, uint8_t brightness);

/**
 * @brief Handles the DEVICE_AUDIO command to set or retrieve the audio volume level.
 */
static void handle_deviceaudio_command(operation_type_e type, uint8_t level);

/**
 * @brief Initiates the firmware update sequence as triggered via MQTT.
 */
static void handle_triggerfirmwareupdate_command(void);

/**
 * @brief Processes the FIRMWARE_UPDATE_PROGRESS command to send update progress details.
 */
static void handle_firmwareupdateprogress_command(void);

/**
 * @brief Sends the final status of the firmware update process (success/failure).
 */
static void handle_firmwareupdatefinalstatus_command(void);

/**
 * @brief Processes the FIRMWARE_UPDATE_PROGRESS command to send update progress details.
 */
static void handle_timerremaining_command(void);

/**
 * @brief Main dispatcher for incoming MQTT commands; routes them to appropriate handlers.
 */
static void handle_mqtt_command(mqtt_commandId_e command, operation_type_e type, uint32_t value);

/**
 * @brief Creates a JSON-formatted response string for the given command, type, and value.
 * @return Pointer to a dynamically allocated JSON response string. Caller must free.
 */
static char *json_create_response(mqtt_commandId_e command, operation_type_e type, double value);

/**
 * Static Function Definitions
 */
static double json_parsenumeric(const char *message, const char *tokan)
{
	double number = -1;
    // Example: Parse JSON payload from MQTT message
    cJSON *json = cJSON_Parse(message);
    if (json) {
    	number = cJSON_GetObjectItem(json, tokan)->valuedouble;
        printf("Parsed value: %g for tokan %s\n", number, tokan);
        cJSON_Delete(json);
    } else {
        printf("Failed to parse incoming json message for tokan: %s.\n", tokan);
    }

    return number;
}

static void handle_devicestatus_command(void)
{
	send_response(DEVICE_STATUS, OPERATION_READ, (uint32_t)1);
}

static void handle_devicemode_command(operation_type_e type, thermostat_mode_t mode)
{

	//Send device mode to the M55 for UI update
	if(OPERATION_READ == type)
	{
		//send mode
		send_response(DEVICE_MODE, OPERATION_READ, (uint32_t)device_status.thermostat_settings.mode);
	}
	else if(OPERATION_WRITE == type)
	{
		//Call Set fan API
		send_response(DEVICE_MODE, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		device_status.thermostat_settings.mode = mode;
		if(MODE_OFF == mode)
		{
			device_status.thermostat_settings.fan_speed = FAN_OFF;
			set_fan_speed(FAN_OFF);
		}
		set_device_mode(mode);
	}
}

static void handle_gettergettemp_command(void)
{
	send_response(TARGETED_TEMP, OPERATION_READ, (uint32_t)device_status.environment.target_temp);
}

static void handle_settempupdown_command(temp_updown_e updown)
{
	//Call set temp by +-1 API

	(updown) ? device_status.environment.current_temp++ : device_status.environment.current_temp--;
	device_status.environment.target_temp = device_status.environment.current_temp;
	send_response(SETDEVICE_TEMP_UPDOWN, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
	set_device_temp(device_status.environment.current_temp);
}

static void handle_fanspeed_command(operation_type_e type, fan_speed_t speed)
{
	if(OPERATION_READ == type)
	{
		send_response(FAN_SPEED, OPERATION_READ, (uint32_t)device_status.thermostat_settings.fan_speed);
	}
	else if(OPERATION_WRITE == type)
	{
		//Call Set fan API
		device_status.thermostat_settings.fan_speed = (fan_speed_t)speed;
		send_response(FAN_SPEED, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		set_fan_speed(speed);
	}
}

void handle_currenttemp_command(operation_type_e type, uint32_t value)
{
	if(OPERATION_WRITE == type)
	{
		device_status.environment.current_temp = device_status.environment.target_temp = value;
		send_response(CURRENT_TEMP, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		set_device_temp(value);
	}
	else if(OPERATION_READ == type)
	{
		//add check for invalid temp
		send_response(CURRENT_TEMP, OPERATION_READ, (uint32_t)device_status.environment.current_temp);
	}
}

void handle_getcurrentpara_command(mqtt_commandId_e cmd)
{
	int currentpara = 0;
	switch (cmd)
	{
		case GETDEVICE_CURRENT_HUMIDITY:
		{
			if(device_status.environment.current_humidity > 99)
			{
				device_status.environment.current_humidity = 33;
			}
			currentpara = device_status.environment.current_humidity++;
			break;
		}
		case GETDEVICE_CURRENT_CO2LEVEL:
		{
			if(device_status.environment.current_humidity > 750)
			{
				device_status.environment.current_humidity = 310;
			}
			currentpara = device_status.environment.current_co2_level++;
			break;
		}

		default:
		{
			printf("Unknown parameter!");
			break;
		}
	}
	send_response(cmd, OPERATION_READ, (uint32_t)currentpara);
}

static void handle_brightness_command(operation_type_e type, uint8_t brightness)
{
	if(OPERATION_READ == type)
	{
		send_response(DEVICE_BRIGHTNESS, OPERATION_READ, (uint32_t)device_status.preferences.display_brightness);
	}
	else if(OPERATION_WRITE == type)
	{
		//Call Set brightness API
		device_status.preferences.display_brightness = brightness;
		send_response(DEVICE_BRIGHTNESS, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
	}
}

static void handle_deviceaudio_command(operation_type_e type, uint8_t level)
{
	if(OPERATION_READ == type)
	{
		send_response(DEVICE_AUDIO, OPERATION_READ, (uint32_t)device_status.preferences.audio_level);
	}
	else if(OPERATION_WRITE == type)
	{
		//Call Set audio level
		device_status.preferences.audio_level = level;
		send_response(DEVICE_AUDIO, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
	}
}

static void handle_triggerfirmwareupdate_command(void)
{
	//Call device firmware update API

	send_response(DEVICE_FIRMWARE_UPDATE, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
}

static void handle_firmwareupdateprogress_command(void)
{
	//Call get firmware update progress API

	send_response(FIRMWARE_UPDATE_PROGRESS, OPERATION_READ, (uint32_t)MQTT_PARSER_SUCCESS);
}

static void handle_firmwareupdatefinalstatus_command(void)
{
	//Call get final status for firmware update API

	send_response(FIRMWARE_UPDATE_STATUS, OPERATION_READ, (uint32_t)MQTT_PARSER_SUCCESS);
}

void handle_pairingremove_command(void)
{
	send_response(DEVICE_PAIRING_DISCONECT, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);

    mqtt_task_cmd_t mqtt_task_cmd = HANDLE_MANUAL_DISCONNECTION;
    xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
}

static void handle_timerremaining_command(void)
{
	send_response(TIME_REMAINING, OPERATION_READ, (uint32_t)device_status.thermostat_settings.time_remains);
}

void send_response(mqtt_commandId_e cmd, operation_type_e type, uint32_t response)
{
	if(!(get_mqtt_status() & FLAG_MQTT_CONNECTION_SUCCESS))
	{
		printf("Cloud not connected\n");
		return;
	}
	publisher_q_data.cmd = PUBLISH_MQTT_MSG;
	publisher_q_data.data = json_create_response(cmd, type, (double)response);
	xQueueSend(publisher_task_q, &publisher_q_data, portMAX_DELAY);
}

static void handle_mqtt_command(mqtt_commandId_e command, operation_type_e type, uint32_t value)
{
    if((IS_BIT_SET(common_get_command, command)) && (OPERATION_READ != type))
    {
    	//Return with error if type of operation is invalid
    	send_response(command, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_INVALID_OPERATION_TYPE);
    	return;
    }

    switch (command) {
        case DEVICE_STATUS:
            printf("Handling DEVICE_STATUS...\n");
			handle_devicestatus_command();
            break;

        case DEVICE_MODE:
            printf("Handling DEVICE_MODE...\n");
            handle_devicemode_command(type, value);
            break;

        case TARGETED_TEMP:
            printf("Handling TARGETED_TEMP...\n");
            handle_gettergettemp_command();
            break;

        case SETDEVICE_TEMP_UPDOWN:
            printf("Handling SETDEVICE_TEMP_UPDOWN...\n");
        	handle_settempupdown_command(value);//handle up or down
            break;

        case FAN_SPEED:
            printf("Handling FAN_SPEED...\n");
        	handle_fanspeed_command(type, value);
            break;

        case CURRENT_TEMP:
            printf("Handling GETDEVICE_CURRENT_TEMP...\n");
            handle_currenttemp_command(type, value);
            break;

        case GETDEVICE_CURRENT_HUMIDITY:
            printf("Handling GETDEVICE_CURRENT_HUMIDITY...\n");
        	handle_getcurrentpara_command(command);
            break;

        case GETDEVICE_CURRENT_CO2LEVEL:
            printf("Handling GETDEVICE_CURRENT_CO2LEVEL...\n");
        	handle_getcurrentpara_command(command);
            break;

        case DEVICE_BRIGHTNESS:
            printf("Handling DEVICE_BRIGHTNESS...\n");
        	handle_brightness_command(type, value);
            break;

        case DEVICE_AUDIO:
            printf("Handling DEVICE_AUDIO...\n");
            handle_deviceaudio_command(type, value);
            break;

        case DEVICE_FIRMWARE_UPDATE:
            printf("Handling DEVICE_FIRMWARE_UPDATE...\n");
            handle_triggerfirmwareupdate_command();
            break;

        case FIRMWARE_UPDATE_PROGRESS:
            printf("Handling FIRMWARE_UPDATE_PROGRESS...\n");
            handle_firmwareupdateprogress_command();
            break;

        case FIRMWARE_UPDATE_STATUS:
            printf("Handling FIRMWARE_UPDATE_STATUS...\n");
            handle_firmwareupdatefinalstatus_command();
            break;

        case DEVICE_PAIRING_DISCONECT:
            printf("Handling DEVICE_PAIRING_DISCONNECT...\n");
            handle_pairingremove_command();
            break;

        case TIME_REMAINING:
            printf("Handling TIME_REMAINING...\n");
            handle_timerremaining_command();
        	break;

        default:
            printf("Unknown MQTT command received: %d\n", command);
            break;
    }
}

/**
 * Functions Definitions
 */
mqtt_parser_errors_e parse_mqtt_command(const char *message, size_t message_len)
{
	static bool flag = 0;
	if(flag == 0)
	{
		device_status.environment.target_temp = 24;
		device_status.thermostat_settings.fan_speed = FAN_MED;
		device_status.thermostat_settings.mode = MODE_ECO;
		device_status.environment.current_temp = device_status.environment.target_temp;
		device_status.environment.current_co2_level = 310;
		device_status.environment.current_humidity = 51;
		device_status.preferences.display_brightness = 70;
		device_status.preferences.audio_level = 70;
		device_status.thermostat_settings.time_remains = 60*3;
		flag =1;
	}

	double type, cmdId, value = 0;
	type = json_parsenumeric(message, JSON_KEY_FOR_TYPEOFOPERATION);
	if(type < 0)
	{
		return MQTT_PARSER_JSON_PARSE_ERROR;
	}
	cmdId = json_parsenumeric(message, JSON_KEY_FOR_COMMAND);
	if(cmdId < 0)
	{
		return MQTT_PARSER_JSON_PARSE_ERROR;
	}
	value = json_parsenumeric(message, JSON_KEY_FOR_VALUE);
	if(value < 0)
	{
		return MQTT_PARSER_JSON_PARSE_ERROR;
	}

	handle_mqtt_command(cmdId, type, (uint32_t)value);
	return MQTT_PARSER_SUCCESS;
}

static char *json_create_response(mqtt_commandId_e command, operation_type_e type, double value)
{
    // Example: Create a JSON-formatted MQTT command
    cJSON *json = cJSON_CreateObject();

    if(NULL != json)
    {
        if(NULL == cJSON_AddNumberToObject(json, JSON_KEY_FOR_TYPEOFOPERATION, type))
        {
        	return NULL;
        }

        if(NULL == cJSON_AddNumberToObject(json, JSON_KEY_FOR_COMMAND, command))
        {
        	return NULL;
        }

        if(NULL == cJSON_AddNumberToObject(json, JSON_KEY_FOR_VALUE, value))
        {
        	return NULL;
        }

        char *command_str = cJSON_Print(json);
        cJSON_Delete(json);
        return command_str;
    }
    else
    {
    	printf("Unable to generate response for %d command\n", (int)command);
    }

	return NULL;
}
