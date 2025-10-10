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
#include <stdio.h>
#include "mqtt_command_handler.h"
#include "publisher_task.h"
#include "subscriber_task.h"
#include "comm_manager.h"
#include "mqtt_task.h"
#include "wireless_manager.h"

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
extern char current_OTA_version[MAX_FW_VERSION_LEN];

static connectivity_medium_t current_interface = CONNECTIVITY_NONE;

const unsigned int common_get_command = SHIFT_LEFT(DEVICE_STATUS) | SHIFT_LEFT(TARGETED_TEMP) |
									SHIFT_LEFT(GETDEVICE_CURRENT_HUMIDITY) | SHIFT_LEFT(GETDEVICE_CURRENT_CO2LEVEL) |
									SHIFT_LEFT(FIRMWARE_UPDATE_PROGRESS) | SHIFT_LEFT(FIRMWARE_UPDATE_STATUS);

typedef struct
{
	mqtt_commandId_e command;
	operation_type_e type;
	bool is_numeric;
	double value;
	char *buff;
}jsonpyload_t;
/**
 * Static Function Declarations
 */
/**
 * @brief Parses a numeric value from the specified JSON message using the given token key.
 */
static double json_parsenumeric(const char *message, const char *token);

/**
 * @brief Parses a JSON string to extract a string value.
 *
 * This function takes a JSON message and a token (key) as input. It attempts
 * to parse the message as JSON and then looks for the specified token. If the
 * token is found and its value is a string, the function returns a dynamically
 * allocated copy of that string. Otherwise, it prints an error and returns NULL.
 *
 * @param message The JSON formatted string to parse.
 * @param token The key name to search for.
 * @return A dynamically allocated string containing the key's value if found, otherwise NULL.
 * The caller is responsible for freeing the returned string using free().
 */
static char* json_parsestring(const char *message, const char *token);

/**
 * @brief Handles the DEVICE_STATUS command and responds with the current status of the device.
 */
static void handle_devicestatus_command(void);

/**
 * @brief Handles the Set date time command and responds with the status.
 */
static void handle_datetime_command(char *value);


/**
 * @brief Handles to get current FW version from cloud
 */
static void handle_current_FW_version_command(char *FW_version);

/**
 * @brief Main dispatcher for incoming MQTT commands string values; routes them to appropriate handlers.
 */
static void handle_mqtt_command_str(mqtt_commandId_e command, operation_type_e type, char *value);

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
 * @brief Sends the final status of the firmware update process (success/failure).
 */
static void handle_firmwareupdatefinalstatus_command(void);

/**
 * @brief Sends the final status of the firmware update process (success/failure).
 */
static void handle_currentfirmwareversion_command(void);

/**
 * @brief Processes the FIRMWARE_UPDATE_PROGRESS command to send update progress details.
 */
static void handle_timerremaining_command(void);

/**
 * @brief Processes the FIRMWARE_UPDATE_PROGRESS command to send update progress details.
 */
static void handle_getdeviceconfig_command(void);

/**
 * @brief Processes the temp unit command.
 */
static void handle_tempunit_command(operation_type_e type, temp_unit_t unit);

/**
 * @brief Main dispatcher for incoming MQTT commands; routes them to appropriate handlers.
 */
static void handle_mqtt_command(mqtt_commandId_e command, operation_type_e type, uint32_t value);

/**
 * @brief Sends a response back over MQTT with the specified command, type, and string data.
 */
void send_response_String(mqtt_commandId_e cmd, operation_type_e type, char *string_data);

/**
 * @brief Parses a date and time string into a DateTime structure.
 *
 * This function takes a string in "YYYY-MM-DD HH:MM:SS" format and
 * extracts the year, month, day, hour, minute, and second.
 *
 * @param datetime_str The date and time string to parse.
 * @param parsed_time A pointer to a DateTime structure to store the results.
 * @return true if parsing was successful, false otherwise.
 */
static bool parse_datetime_string(const char *datetime_str, DateTime *parsed_time);

/**
 * @brief Creates a JSON-formatted response string for the given command, type, and value.
 * @return Pointer to a dynamically allocated JSON response string. Caller must free.
 */
static char *json_create_response(jsonpyload_t pyload);

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
			LOG_INFO(CYLF_DEF, "Parsed value: %g for tokan %s\n", number, tokan);
			cJSON_Delete(json);
		} else {
			LOG_ERROR(CYLF_DEF, "Failed to parse incoming json message for tokan: %s.\n", tokan);
		}
	
    return number;
}

static char* json_parsestring(const char *message, const char *token)
{
    char* result = NULL;
   
		cJSON *json = NULL;
		cJSON *item = NULL;

    // Parse the JSON payload from the message string.
    json = cJSON_Parse(message);
    if (json == NULL) {
        LOG_ERROR(CYLF_DEF, "Failed to parse incoming JSON message.\r\n");
        return result;
    }

    // Get the specified item from the parsed JSON object.
    item = cJSON_GetObjectItem(json, token);
    if (item == NULL) {
        LOG_INFO(CYLF_DEF, "Failed to find token '%s' in JSON message.\r\n", token);
        cJSON_Delete(json);
        return result;
    }

    // Check if the found item is a string and retrieve its value.
    if (cJSON_IsString(item)) {
        // Explicitly allocate memory and copy the string.
        const char *valuestring = item->valuestring;
        if (valuestring != NULL) {
            size_t len = strlen(valuestring);
            result = (char*)malloc(len + 1); // +1 for the null terminator
            if (result != NULL) {
                memcpy(result, valuestring, len + 1);
                LOG_INFO(CYLF_DEF, "Parsed value: '%s' for token '%s'\r\n", result, token);
            } else {
                LOG_ERROR(CYLF_DEF, "Memory allocation failed for string value.\r\n");
            }
        }
    } else {
        LOG_ERROR(CYLF_DEF, "Token '%s' is not a string. Returning NULL.\r\n", token);
    }

    // Clean up the cJSON object to free memory.
    cJSON_Delete(json);
  
    return result;
}

static void handle_devicestatus_command(void)
{
	send_response_numeric(DEVICE_STATUS, OPERATION_READ, (uint32_t)1);
}

static bool parse_datetime_string(const char *datetime_str, DateTime *parsed_time)
{
    if (datetime_str == NULL || parsed_time == NULL) {
        LOG_ERROR(CYLF_DEF, "Error: Invalid input pointers.\r\n");
        return false;
    }

    // Use sscanf to parse the string with the specified format.
    // The format string "%d-%d-%d %d:%d:%d" matches the input pattern exactly.
    int result = sscanf(datetime_str, "%d-%d-%d %d:%d:%d",
                        &parsed_time->year,
                        &parsed_time->month,
                        &parsed_time->day,
                        &parsed_time->hour,
                        &parsed_time->minute,
                        &parsed_time->second);

    // sscanf returns the number of successfully matched items.
    // We expect 6 items (year, month, day, hour, minute, second).
    if (result == 6) {
        return true;
    } else {
        LOG_ERROR(CYLF_DEF, "Failed to parse date and time string. Expected 6 items, got %d.\r\n", result);
        return false;
    }
}

static void handle_datetime_command(char *value)
{
    DateTime parsed_time = {0};

    if(true == parse_datetime_string(value, &parsed_time))
    {
        send_response_numeric(SET_DATE_TIME, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);

        /* Update parsed date time info. on UI */
        set_datetime_ui(&parsed_time);
    }
    else
    {
        send_response_numeric(SET_DATE_TIME, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_JSON_PARSE_ERROR);
    }
}


static void handle_current_FW_version_command(char *FW_version)
{
	send_available_FW_version(FW_version);
}

static void handle_mqtt_command_str(mqtt_commandId_e command, operation_type_e type, char *value)
{
    switch(command)
    {
        case SET_DATE_TIME:
            LOG_INFO(CYLF_DEF, "Handling SET_DATE_TIME...\n");
            handle_datetime_command(value);
            break;

        case CURRENT_FIRMWARE_VERSION:
            LOG_INFO(CYLF_DEF, "Handling CURRENT_FIRMWARE_VERSION...\n");
            handle_current_FW_version_command(value);
            break;

        default:
            LOG_INFO(CYLF_DEF, "Un-handled string values.\n");
            break;
    }
}


static void handle_devicemode_command(operation_type_e type, thermostat_mode_t mode)
{

	//Send device mode to the M55 for UI update
	if(OPERATION_READ == type)
	{
		//send mode
		send_response_numeric(DEVICE_MODE, OPERATION_READ, (uint32_t)device_status.thermostat_settings.mode);
	}
	else if(OPERATION_WRITE == type)
	{
		//Call Set fan API
		send_response_numeric(DEVICE_MODE, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
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
	send_response_numeric(TARGETED_TEMP, OPERATION_READ, (uint32_t)device_status.environment.target_temp);
}

static void handle_settempupdown_command(temp_updown_e updown)
{
	//Call set temp by +-1 API

	(updown) ? device_status.environment.current_temp++ : device_status.environment.current_temp--;
	device_status.environment.target_temp = device_status.environment.current_temp;
	send_response_numeric(SETDEVICE_TEMP_UPDOWN, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
	set_device_temp(device_status.environment.current_temp);
}

static void handle_fanspeed_command(operation_type_e type, fan_speed_t speed)
{
	if(OPERATION_READ == type)
	{
		send_response_numeric(FAN_SPEED, OPERATION_READ, (uint32_t)device_status.thermostat_settings.fan_speed);
	}
	else if(OPERATION_WRITE == type)
	{
		//Call Set fan API
		device_status.thermostat_settings.fan_speed = (fan_speed_t)speed;
		send_response_numeric(FAN_SPEED, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		set_fan_speed(speed);
	}
}

void handle_currenttemp_command(operation_type_e type, uint32_t value)
{
	if(OPERATION_WRITE == type)
	{
		device_status.environment.current_temp = device_status.environment.target_temp = value;
		send_response_numeric(CURRENT_TEMP, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		set_device_temp(value);
	}
	else if(OPERATION_READ == type)
	{
		//add check for invalid temp
		send_response_numeric(CURRENT_TEMP, OPERATION_READ, (uint32_t)device_status.environment.current_temp);
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
		    LOG_ERROR(CYLF_DEF, "Unknown parameter!");
			break;
		}
	}
	send_response_numeric(cmd, OPERATION_READ, (uint32_t)currentpara);
}

static void handle_brightness_command(operation_type_e type, uint8_t brightness)
{
	if(OPERATION_READ == type)
	{
		send_response_numeric(DEVICE_BRIGHTNESS, OPERATION_READ, (uint32_t)device_status.preferences.display_brightness);
	}
	else if(OPERATION_WRITE == type)
	{
		device_status.preferences.display_brightness = brightness;
		send_response_numeric(DEVICE_BRIGHTNESS, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		set_device_brightness(brightness);
	}
}

static void handle_deviceaudio_command(operation_type_e type, uint8_t level)
{
	if(OPERATION_READ == type)
	{
		send_response_numeric(DEVICE_AUDIO, OPERATION_READ, (uint32_t)device_status.preferences.audio_level);
	}
	else if(OPERATION_WRITE == type)
	{
		//Call Set audio level
		device_status.preferences.audio_level = (audio_level_t)level;
		send_response_numeric(DEVICE_AUDIO, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		set_device_audio((audio_level_t)level);
	}
}

void mqtt_ota_task(void *arg)
{
	device_status.ota.ota_progress = 0;
	device_status.ota.ota_final_status = 1;
	for(;;)
	{
		device_status.ota.ota_progress+=10;
		send_response_numeric(FIRMWARE_UPDATE_PROGRESS, OPERATION_READ, (uint32_t)device_status.ota.ota_progress);
		LOG_INFO(CYLF_DEF, "Updating FW Progress\n");
		vTaskDelay(1000);
		if(device_status.ota.ota_progress >= 100)
		{
			device_status.ota.ota_final_status = 0;
			send_response_numeric(FIRMWARE_UPDATE_STATUS, OPERATION_READ, (uint32_t)device_status.ota.ota_final_status);
			LOG_INFO(CYLF_DEF, "OTA Task Deleted\n");
			vTaskDelete(NULL);
		}
	}
}


static void handle_triggerfirmwareupdate_command(void)
{
	//Call device firmware update API
	if(device_status.ota.ota_final_status != 0)
	{
		send_response_numeric(DEVICE_FIRMWARE_UPDATE, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_UNKNOWN_ERROR);
	}
	else
	{
		if(get_device_provision_state())
		{
			OTA_Tigger_On_ui();
			send_response_numeric(DEVICE_FIRMWARE_UPDATE, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		}
		else
		{
			no_internet_connected_send_to_ui();
			send_response_numeric(DEVICE_FIRMWARE_UPDATE, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_UNKNOWN_ERROR);
		}
	}

}

void handle_firmwareupdateprogress_command(uint8_t per)
{
	if(per != device_status.ota.ota_progress)
	{
		device_status.ota.ota_progress = per;
		send_response_numeric(FIRMWARE_UPDATE_PROGRESS, OPERATION_READ, (uint32_t)device_status.ota.ota_progress);
	}
}

static void handle_firmwareupdatefinalstatus_command(void)
{
	//Call get final status for firmware update API

	send_response_numeric(FIRMWARE_UPDATE_STATUS, OPERATION_READ, (uint32_t)device_status.ota.ota_final_status);
}

static void handle_currentfirmwareversion_command(void)
{
	send_response_String(CURRENT_FIRMWARE_VERSION, OPERATION_READ, current_OTA_version);
}

void handle_pairingremove_command(bool only_wifi)
{
	if(only_wifi)
	{
		send_response_numeric(DEVICE_STATUS, OPERATION_READ, (uint32_t)0);
	}
	else
	{
		send_response_numeric(DEVICE_PAIRING_DISCONECT, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
	}

	vTaskDelay(500);

	mqtt_task_cmd_t mqtt_task_cmd = HANDLE_DEPROVISIONING;
	xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
}

static void handle_timerremaining_command(void)
{
	send_response_numeric(TIME_REMAINING, OPERATION_READ, (uint32_t)device_status.thermostat_settings.time_remains);
}

static void handle_getdeviceconfig_command(void)
{
	char temp[5] = {0};
	
		cJSON *json = cJSON_CreateObject();
		char *json_data = NULL;

		if(NULL != json)
		{
			snprintf(temp, sizeof(temp), "%d",DEVICE_STATUS);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)true))
			{
				//Can display error to user
				return;
			}

			snprintf(temp, sizeof(temp), "%d",CURRENT_TEMP);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.environment.current_temp))
			{
				//Can display error to user
				return;
			}

			snprintf(temp, sizeof(temp), "%d",TARGETED_TEMP);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.environment.target_temp))
			{
				return;
			}

			snprintf(temp, sizeof(temp), "%d",GETDEVICE_CURRENT_CO2LEVEL);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.environment.current_co2_level))
			{
				return;
			}

			snprintf(temp, sizeof(temp), "%d",GETDEVICE_CURRENT_HUMIDITY);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.environment.current_humidity))
			{
				return;
			}

			snprintf(temp, sizeof(temp), "%d",DEVICE_MODE);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.thermostat_settings.mode))
			{
				return;
			}

			snprintf(temp, sizeof(temp), "%d",FAN_SPEED);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.thermostat_settings.fan_speed))
			{
				return;
			}

			snprintf(temp, sizeof(temp), "%d",DEVICE_BRIGHTNESS);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.preferences.display_brightness))
			{
				return;
			}

			snprintf(temp, sizeof(temp), "%d",DEVICE_AUDIO);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.preferences.audio_level))
			{
				return;
			}

			snprintf(temp, sizeof(temp), "%d",TEMP_UNIT);
			if(NULL == cJSON_AddNumberToObject(json, (const char *)temp, (uint32_t)device_status.thermostat_settings.temp_unit))
			{
				return;
			}

		snprintf(temp, sizeof(temp), "%d",CURRENT_FIRMWARE_VERSION);
		if(NULL == cJSON_AddStringToObject(json, (const char *)temp, current_OTA_version))
		{
			return;
		}

			json_data = cJSON_Print(json);
			cJSON_Delete(json);

		if(NULL != json_data)
		{
			if(CONNECTIVITY_MQTT_CLOUD == current_interface)
			{
				if(!(get_mqtt_status() & FLAG_MQTT_CONNECTION_SUCCESS))
				{
				    LOG_INFO(CYLF_DEF, "Cloud not connected\n");
					return;
				}

				publisher_q_data.cmd = PUBLISH_MQTT_MSG;
				publisher_q_data.data = json_data;

				xQueueSend(publisher_task_q, &publisher_q_data, portMAX_DELAY);
			}
			else if(CONNECTIVITY_BLE == current_interface)
			{
				app_bt_send_message(json_data, strlen(json_data));
			}
			else
			{
			    LOG_INFO(CYLF_DEF, "No interface Found for data exchange!\n");
			}
		}
		else
		{
		    LOG_INFO(CYLF_DEF, "Unable to create a JSON string!\n");
		}
	}
}

void handle_tempunit_command(operation_type_e type, temp_unit_t unit)
{
	if(OPERATION_READ == type)
	{
		send_response_numeric(TEMP_UNIT, OPERATION_READ, (uint32_t)device_status.thermostat_settings.temp_unit);
	}
	else if(OPERATION_WRITE == type)
	{
		device_status.thermostat_settings.temp_unit = (temp_unit_t)unit;
		send_response_numeric(TEMP_UNIT, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_SUCCESS);
		set_device_temp_unit(unit);
	}
}

void send_response_numeric(mqtt_commandId_e cmd, operation_type_e type, uint32_t response)
{
	jsonpyload_t pyload;
	pyload.is_numeric = true;
	pyload.command = cmd;
	pyload.type = type;
	pyload.value = (double)response;

    /* Wait for sensor task to acquire the I2C bus. */
    if (xSemaphoreTake(uplink_mutex, pdMS_TO_TICKS(500)) == pdTRUE)
    {
        char *json_data = json_create_response(pyload);
        if(CONNECTIVITY_MQTT_CLOUD == current_interface)
        {
            if(!(get_mqtt_status() & FLAG_MQTT_CONNECTION_SUCCESS))
            {
                LOG_INFO(CYLF_DEF, "send_response_numeric Cloud not connected\n");

                /* Release Mutex */
                xSemaphoreGive(uplink_mutex);
                return;
            }

            publisher_q_data.cmd = PUBLISH_MQTT_MSG;
            publisher_q_data.data = json_data;
            xQueueSend(publisher_task_q, &publisher_q_data, portMAX_DELAY);
        }
        else if(CONNECTIVITY_BLE == current_interface)
        {
            app_bt_send_message(json_data, strlen(json_data));
        }
        else
        {
            LOG_INFO(CYLF_DEF, "No interface Found for data exchange!\n");
        }

        /* Release Mutex */
        xSemaphoreGive(uplink_mutex);
    }
}

void send_response_String(mqtt_commandId_e cmd, operation_type_e type, char *string_data)
{
	jsonpyload_t pyload;
	pyload.is_numeric = false;
	pyload.command = cmd;
	pyload.type = type;
	pyload.buff = string_data;
	char *json_data = json_create_response(pyload);

	if(json_data != NULL)
	{
		if(CONNECTIVITY_MQTT_CLOUD == current_interface)
		{
			if(!(get_mqtt_status() & FLAG_MQTT_CONNECTION_SUCCESS))
			{
			    LOG_INFO(CYLF_DEF, "send_response_String Cloud not connected\n");
				return;
			}

			publisher_q_data.cmd = PUBLISH_MQTT_MSG;
			publisher_q_data.data = json_data;
			xQueueSend(publisher_task_q, &publisher_q_data, portMAX_DELAY);
		}
		else if(CONNECTIVITY_BLE == current_interface)
		{
			app_bt_send_message(json_data, strlen(json_data));
		}
		else
		{
		    LOG_INFO(CYLF_DEF, "No interface Found for data exchange!\n");
		}
	}
	else
	{
	    LOG_INFO(CYLF_DEF, "Unable to create a Json string!\n");
	}
}

static void handle_mqtt_command(mqtt_commandId_e command, operation_type_e type, uint32_t value)
{
    if((IS_BIT_SET(common_get_command, command)) && (OPERATION_READ != type))
    {
    	//Return with error if type of operation is invalid
    	send_response_numeric(command, OPERATION_RESPONSE, (uint32_t)MQTT_PARSER_INVALID_OPERATION_TYPE);
    	return;
    }

    switch (command) {
        case DEVICE_STATUS:
            LOG_INFO(CYLF_DEF, "Handling DEVICE_STATUS...\n");
			handle_devicestatus_command();
            break;

        case DEVICE_MODE:
            LOG_INFO(CYLF_DEF, "Handling DEVICE_MODE...\n");
            handle_devicemode_command(type, value);
            break;

        case TARGETED_TEMP:
            LOG_INFO(CYLF_DEF, "Handling TARGETED_TEMP...\n");
            handle_gettergettemp_command();
            break;

        case SETDEVICE_TEMP_UPDOWN:
            LOG_INFO(CYLF_DEF, "Handling SETDEVICE_TEMP_UPDOWN...\n");
        	handle_settempupdown_command(value);//handle up or down
            break;

        case FAN_SPEED:
            LOG_INFO(CYLF_DEF, "Handling FAN_SPEED...\n");
        	handle_fanspeed_command(type, value);
            break;

        case CURRENT_TEMP:
            LOG_INFO(CYLF_DEF, "Handling GETDEVICE_CURRENT_TEMP...\n");
            handle_currenttemp_command(type, value);
            break;

        case GETDEVICE_CURRENT_HUMIDITY:
            LOG_INFO(CYLF_DEF, "Handling GETDEVICE_CURRENT_HUMIDITY...\n");
        	handle_getcurrentpara_command(command);
            break;

        case GETDEVICE_CURRENT_CO2LEVEL:
            LOG_INFO(CYLF_DEF, "Handling GETDEVICE_CURRENT_CO2LEVEL...\n");
        	handle_getcurrentpara_command(command);
            break;

        case DEVICE_BRIGHTNESS:
            LOG_INFO(CYLF_DEF, "Handling DEVICE_BRIGHTNESS...\n");
        	handle_brightness_command(type, value);
            break;

        case DEVICE_AUDIO:
            LOG_INFO(CYLF_DEF, "Handling DEVICE_AUDIO...\n");
            handle_deviceaudio_command(type, value);
            break;

        case DEVICE_FIRMWARE_UPDATE:
            LOG_INFO(CYLF_DEF, "Handling DEVICE_FIRMWARE_UPDATE...\n");
            handle_triggerfirmwareupdate_command();
            break;

        case FIRMWARE_UPDATE_PROGRESS:
            LOG_INFO(CYLF_DEF, "Handling FIRMWARE_UPDATE_PROGRESS...\n");
            handle_firmwareupdateprogress_command((uint8_t)device_status.ota.ota_progress);
            break;

        case FIRMWARE_UPDATE_STATUS:
            LOG_INFO(CYLF_DEF, "Handling FIRMWARE_UPDATE_STATUS...\n");
            handle_firmwareupdatefinalstatus_command();
            break;

        case DEVICE_PAIRING_DISCONECT:
            LOG_INFO(CYLF_DEF, "Handling DEVICE_PAIRING_DISCONNECT...\n");
            handle_pairingremove_command(false);
            break;

        case TIME_REMAINING:
            LOG_INFO(CYLF_DEF, "Handling TIME_REMAINING...\n");
            handle_timerremaining_command();
        	break;

        case CURRENT_FIRMWARE_VERSION:
            LOG_INFO(CYLF_DEF, "Handling CURRENT_FIRMWARE_VERSION...\n");
        	handle_currentfirmwareversion_command();
        	break;

        case TEMP_UNIT:
            LOG_INFO(CYLF_DEF, "Handling TEMP_UNIT...\n");
        	handle_tempunit_command(type, value);
        	break;

        case GET_DEVICE_CONFIGURATION:
            LOG_INFO(CYLF_DEF, "Handling GET_DEVICE_CONFIGURATION...\n");
            handle_getdeviceconfig_command();
        	break;

        default:
            LOG_INFO(CYLF_DEF, "Unknown MQTT command received: %d\n", command);
            break;
    }
}

/**
 * Functions Definitions
 */
mqtt_parser_errors_e parse_received_command(const char *message, size_t message_len)
{
    double type, cmdId, value = 0;
    char *value_str;

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

    /* If cmd is date time, parse the string value  */
    if(SET_DATE_TIME == cmdId || CURRENT_FIRMWARE_VERSION == cmdId)
    {
        /* Parse the string value */
        value_str = json_parsestring(message, JSON_KEY_FOR_VALUE);
        handle_mqtt_command_str(cmdId, type, value_str);
    // The memory allocated by json_parsestring must be freed.
        if (value_str != NULL)
        {
            free(value_str);
        }
    }
    else
    {
        value = json_parsenumeric(message, JSON_KEY_FOR_VALUE);
        if(value < 0)
        {
            return MQTT_PARSER_JSON_PARSE_ERROR;
        }
        handle_mqtt_command(cmdId, type, (uint32_t)value);
    }

    return MQTT_PARSER_SUCCESS;
}


static char *json_create_response(jsonpyload_t payload)
{
		cJSON *json = cJSON_CreateObject();

    if(NULL != json)
    {
        if(NULL == cJSON_AddNumberToObject(json, JSON_KEY_FOR_TYPEOFOPERATION, payload.type))
        {
        	cJSON_Delete(json);
        	return NULL;
        }

        if(NULL == cJSON_AddNumberToObject(json, JSON_KEY_FOR_COMMAND, payload.command))
        {
        	cJSON_Delete(json);
        	return NULL;
        }

        if(payload.is_numeric)
        {
			if(NULL == cJSON_AddNumberToObject(json, JSON_KEY_FOR_VALUE, payload.value))
			{
				cJSON_Delete(json);
				return NULL;
			}
        }
        else
        {
			if(NULL == cJSON_AddStringToObject(json, JSON_KEY_FOR_VALUE, payload.buff))
			{
				cJSON_Delete(json);
				return NULL;
			}
        }

        char *command_str = cJSON_Print(json);
        cJSON_Delete(json);
        return command_str;
    }
    else
    {
        LOG_INFO(CYLF_DEF, "Unable to generate response for %d command\n", (int)payload.command);
    }
   
	return NULL;
}

void update_comm_interface(connectivity_medium_t interface)
{
	if(current_interface != interface)
	{
		current_interface = interface;
		LOG_INFO(CYLF_DEF, "Communication Interface Updated To : %d\n", current_interface);
	}
}

void send_device_config_info(void)
{
    handle_getdeviceconfig_command();
}
