/******************************************************************************
* File Name:   mqtt_command_handler.h
*
* Description:  Header file for MQTT command parsing and creation.
* 				Provides APIs to parse incoming MQTT messages and construct response commands.
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

#ifndef MQTT_MQTT_COMMAND_HANDLER_H_
#define MQTT_MQTT_COMMAND_HANDLER_H_

/**
 * Header Includes
 */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <cJSON.h>
#include "app_common.h"

/**
 * Global Declaration
 */
typedef enum mqtt_commandId
{
	DEVICE_STATUS 				= 0,
	DEVICE_MODE					= 1,
	TARGETED_TEMP				= 2,
	SETDEVICE_TEMP_UPDOWN		= 3,
	FAN_SPEED					= 4,
	CURRENT_TEMP				= 5,
	GETDEVICE_CURRENT_HUMIDITY	= 6,
	GETDEVICE_CURRENT_CO2LEVEL	= 7,
	DEVICE_BRIGHTNESS			= 8,
	DEVICE_AUDIO				= 9,
	DEVICE_FIRMWARE_UPDATE		= 10,
	FIRMWARE_UPDATE_PROGRESS	= 11,
	FIRMWARE_UPDATE_STATUS		= 12,
	DEVICE_PAIRING_DISCONECT	= 13,
	CURRENT_FIRMWARE_VERSION	= 14,
	GET_NEW_FIRMWARE_VERSION	= 15,
	TIME_REMAINING				= 16,

	MAX_COMMAND_ID
} mqtt_commandId_e;

// Enum for MQTT processing errors
typedef enum {
    MQTT_PARSER_SUCCESS = 0,             // No error
    MQTT_PARSER_NULL_INPUT_ERROR,        // Null input string
    MQTT_PARSER_EMPTY_INPUT_ERROR,       // Input string is empty
    MQTT_PARSER_JSON_PARSE_ERROR,        // Malformed JSON
    MQTT_PARSER_MEMORY_ALLOCATION_ERROR, // Memory allocation failure
    MQTT_PARSER_OBJECT_CREATION_ERROR,   // Failed to create JSON object
    MQTT_PARSER_KEY_NOT_FOUND_ERROR,     // Expected key missing
    MQTT_PARSER_TYPE_MISMATCH_ERROR,     // Key found but with unexpected type
    MQTT_PARSER_VALUE_OUT_OF_RANGE_ERROR,// Value exceeds allowed limits or bounds
    MQTT_PARSER_INVALID_COMMAND_ID,      // Unknown or unsupported command ID
    MQTT_PARSER_INVALID_OPERATION_TYPE,  // Read/Write operation type invalid
    MQTT_PARSER_ENCODING_ERROR,          // JSON string encoding failure
    MQTT_PARSER_BUILD_ERROR,             // Error during JSON construction
    MQTT_PARSER_UNKNOWN_ERROR            // Fallback for undefined issues
} mqtt_parser_errors_e;

// Enum for operation types
typedef enum operation_type{
    OPERATION_READ 	= 0,  	// Read operation
    OPERATION_WRITE = 1,  	// Write operation
    OPERATION_RESPONSE = 2, // response operation

	MAX_OPERATION
} operation_type_e;

// Enum for temperature up/down
typedef enum temp_updown{
    DECREASE 	= 0,
    INCREASE	= 1,

	MAX_TEMP_UPDOWN
} temp_updown_e;


extern device_state_t device_status;
/**
 * Functions declarations
 */
/**
 * @brief Parses the incoming MQTT message and extracts the command ID and associated parameters.
 *
 * @param message       Pointer to the raw JSON-formatted MQTT message.
 * @param message_len   Length of the message in bytes.
 *
 * @return mqtt_parser_errors_e Returns a status code indicating success or the type of parsing error encountered,
 *                              such as invalid format, missing fields, or unsupported command ID.
 */
mqtt_parser_errors_e parse_mqtt_command(const char *message, size_t message_len);


/**
 * @brief Sends a response back over MQTT with the specified command, type, and response value.
 */
void send_response(mqtt_commandId_e cmd, operation_type_e type, uint32_t response);

/**
 * @brief Handles the DEVICE_PAIRING_DISCONNECT command to unpair or reset Bluetooth/Wi-Fi pairing.
 */
void handle_pairingremove_command(void);

#endif /* MQTT_MQTT_COMMAND_HANDLER_H_ */
