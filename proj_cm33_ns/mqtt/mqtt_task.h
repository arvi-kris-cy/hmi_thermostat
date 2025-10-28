/******************************************************************************
* File Name:   mqtt_task.h
*
* Description: This file is the public interface of mqtt_task.c
*
* Related Document: See README.md
*
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

#ifndef MQTT_TASK_H_
#define MQTT_TASK_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "cy_mqtt_api.h"
#include "semphr.h"


/*******************************************************************************
* Macros
********************************************************************************/
/* Task parameters for MQTT Client Task. */
#define MQTT_TASK_NAME                             ("MQTT Client")
#define MQTT_CLIENT_TASK_PRIORITY       	        (1U)
#define MQTT_CLIENT_TASK_STACK_SIZE     	        (1024U * 2U)

#define MQTT_TOPIC_SIZE 					(16U)

#define NUMBERS_OF_TOPIC					(2U)

#define JSON_KEY_FOR_TYPEOFOPERATION			"type"
#define JSON_KEY_FOR_COMMAND					"cmd_id"
#define JSON_KEY_FOR_VALUE						"value"
/*******************************************************************************
* Global Variables
********************************************************************************/
/* Commands for the MQTT Client Task. */
typedef enum
{
    HANDLE_MQTT_SUBSCRIBE_FAILURE,
    HANDLE_MQTT_PUBLISH_FAILURE,
    HANDLE_DISCONNECTION,
    HANDLE_CONNECT,
	HANDLE_DEPROVISIONING,
	HANDLE_MANUAL_DISCONNECTION,
	START_OTA_REMOVE_MQTT_INSTANCE
} mqtt_task_cmd_t;

/**
 * @brief Flags for tracking the state of system components during cleanup.
 */
typedef enum
{
    FLAG_WCM_INITIALIZED            = (1lu << 0),
    FLAG_WIFI_CONNECTED             = (1lu << 1),
    FLAG_LIBS_INITIALIZED           = (1lu << 2),
    FLAG_BUFFER_INITIALIZED         = (1lu << 3),
    FLAG_MQTT_INSTANCE_CREATED      = (1lu << 4),
    FLAG_MQTT_CONNECTION_SUCCESS    = (1lu << 5),
    FLAG_MQTT_MSG_RECEIVED          = (1lu << 6)
} cleanup_flags_t;

typedef char mqtttopic_t[MQTT_TOPIC_SIZE + 1];

/*******************************************************************************
 * Extern variables
 ******************************************************************************/
extern cy_mqtt_t mqtt_connection;
extern QueueHandle_t mqtt_task_q;
extern SemaphoreHandle_t uplink_mutex;
extern bool switch_to_ble_mqtt_disconn;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void mqtt_client_task(void *pvParameters);
void cleanup_mqtt(void);
uint32_t get_mqtt_status(void);
void mqtt_diconnect(void);

#endif /* MQTT_TASK_H_ */

/* [] END OF FILE */
