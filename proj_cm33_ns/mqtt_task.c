/******************************************************************************
* File Name:   mqtt_task.c
*
* Description: This file contains the task that handles initialization & 
*              connection of Wi-Fi and the MQTT client. The task then starts 
*              the subscriber and the publisher tasks. The task also implements
*              reconnection mechanisms to handle WiFi and MQTT disconnections.
*              The task also handles all the cleanup operations to gracefully 
*              terminate the Wi-Fi and MQTT connections in case of any failure.
*
* Related Document: See README.md
*
*
*******************************************************************************
* * Copyright 2024-2025, Cypress Semiconductor Corporation (an Infineon company) or
* * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
* *
* * This software, including source code, documentation and related
* * materials ("Software") is owned by Cypress Semiconductor Corporation
* * or one of its affiliates ("Cypress") and is protected by and subject to
* * worldwide patent protection (United States and foreign),
* * United States copyright laws and international treaty provisions.
* * Therefore, you may use this Software only as provided in the license
* * agreement accompanying the software package from which you
* * obtained this Software ("EULA").
* * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* * non-transferable license to copy, modify, and compile the Software
* * source code solely for use in connection with Cypress's
* * integrated circuit products.  Any reproduction, modification, translation,
* * compilation, or representation of this Software except as specified
* * above is prohibited without the express written permission of Cypress.
* *
* * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* * reserves the right to make changes to the Software without notice. Cypress
* * does not assume any liability arising out of the application or use of the
* * Software or any product or circuit described in the Software. Cypress does
* * not authorize its products for use in any products where a malfunction or
* * failure of the Cypress product may reasonably be expected to result in
* * significant property damage, injury or death ("High Risk Product"). By
* * including Cypress's product in a High Risk Product, the manufacturer
* * of such system or application assumes all risk of such use and in doing
* * so agrees to indemnify Cypress against all liability.
*******************************************************************************/
#include "cybsp.h"

/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "task.h"

/* Task header files */
#include "mqtt_task.h"
#include "subscriber_task.h"
#include "publisher_task.h"

/* Configuration file for Wi-Fi and MQTT client */
#include "mqtt_client_config.h"

/* Middleware libraries */
#include "cy_retarget_io.h"
#include "cy_wcm.h"

#include "cy_mqtt_api.h"
#include "clock.h"

/* LwIP header files */
#include "lwip/netif.h"
#include "retarget_io_init.h"

#include "wireless_manager.h"
#include "mqtt/mqtt_command_handler.h"
#include "wifi_task.h"
/******************************************************************************
* Macros
******************************************************************************/
/* Queue length of a message queue that is used to communicate the status of 
 * various operations.
 */
#define MQTT_TASK_QUEUE_LENGTH           (3u)

/* Time in milliseconds to wait before creating the publisher task. */
#define TASK_CREATION_DELAY_MS           (2000u)

#define TIME_DIV_MS                                 (60000u)
#define APP_SDIO_INTERRUPT_PRIORITY                 (7U)
#define APP_HOST_WAKE_INTERRUPT_PRIORITY            (2U)
#define APP_SDIO_FREQUENCY_HZ                       (25000000U)
#define SDHC_SDIO_64BYTES_BLOCK                     (64U)

/*String that describes the MQTT handle that is being created in order to uniquely identify it*/
#define MQTT_HANDLE_DESCRIPTOR                       "MQTThandleID"

/* Macro to check if the result of an operation was successful and set the 
 * corresponding bit in the status_flag based on 'init_mask' parameter. When 
 * it has failed, print the error message and return the result to the 
 * calling function.
 */
#define CHECK_RESULT(result, init_mask, error_message...)      \
                     do                                        \
                     {                                         \
                         if ((int)result == CY_RSLT_SUCCESS)   \
                         {                                     \
                             status_flag |= init_mask;         \
                         }                                     \
                         else                                  \
                         {                                     \
                             printf(error_message);            \
                             return result;                    \
                         }                                     \
                     } while(0)

/******************************************************************************
* Global Variables
*******************************************************************************/
/* MQTT connection handle. */
cy_mqtt_t mqtt_connection;

/* Queue handle used to communicate results of various operations - MQTT 
 * Publish, MQTT Subscribe, MQTT connection, and Wi-Fi connection between tasks 
 * and callbacks.
 */
QueueHandle_t mqtt_task_q;

/* Flag to denote initialization status of various operations. */
uint32_t status_flag;

/* Pointer to the network buffer needed by the MQTT library for MQTT send and 
 * receive operations.
 */
uint8_t *mqtt_network_buffer = NULL;

mqtttopic_t mqtt_topics[NUMBERS_OF_TOPIC] = {0};

extern TaskHandle_t wifi_task_handle;
extern bool device_provisioned;
/******************************************************************************
* Function Prototypes
*******************************************************************************/

#if GENERATE_UNIQUE_CLIENT_ID
static cy_rslt_t mqtt_get_unique_client_identifier(char *mqtt_client_identifier);
#endif /* GENERATE_UNIQUE_CLIENT_ID */


/******************************************************************************
 * Function Name: cleanup
 ******************************************************************************
 * Summary:
 *  Function that invokes the deinit and cleanup functions for various
 *  operations based on the status_flag.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *
 ******************************************************************************/
void cleanup_mqtt(void)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    /* Disconnect the MQTT connection if it was established. */
    if (status_flag & FLAG_MQTT_CONNECTION_SUCCESS)
    {
        status = cy_mqtt_disconnect(mqtt_connection);

        if (CY_RSLT_SUCCESS == status)
        {
            printf("Disconnected from the MQTT Broker...\n");
        }
        else
        {
            printf("MQTT disconnect API failed unexpectedly.\n");
        }
    }
    /* Delete the MQTT instance if it was created. */
    if (status_flag & FLAG_MQTT_INSTANCE_CREATED)
    {
        status = cy_mqtt_delete(mqtt_connection);

        if (CY_RSLT_SUCCESS == status)
        {
            printf("Removed MQTT connection info from stack...\n");
        }
        else
        {
            printf("MQTT delete API failed unexpectedly.\n");
        }
    }
    /* Deallocate the network buffer. */
    if (status_flag & FLAG_BUFFER_INITIALIZED)
    {
        vPortFree((void *) mqtt_network_buffer);
    }
    /* Deinit the MQTT library. */
    if (status_flag & FLAG_LIBS_INITIALIZED)
    {
        status = cy_mqtt_deinit();

        if (CY_RSLT_SUCCESS == status)
        {
            printf("Deinitialized MQTT stack...\n");
        }
        else
        {
            printf("MQTT deinit API failed unexpectedly.\n");
        }
    }
    /* Disconnect from Wi-Fi AP. */
    if (status_flag & FLAG_WIFI_CONNECTED)
    {
        status = cy_wcm_disconnect_ap();

        if (CY_RSLT_SUCCESS == status)
        {
            printf("Disconnected from the Wi-Fi AP!\n");
        }
        else
        {
            printf("WCM disconnect AP failed unexpectedly.\n");
        }
    }
    /* De-initialize the Wi-Fi Connection Manager. */
    if (status_flag & FLAG_WCM_INITIALIZED)
    {
        status = cy_wcm_deinit();

        if (CY_RSLT_SUCCESS == status)
        {
            printf("Deinitialized Wifi connection...\n");
        }
        else
        {
            printf("WCM deinit API failed unexpectedly.\n");
        }
    }
}

/******************************************************************************
 * Function Name: mqtt_event_callback
 ******************************************************************************
 * Summary:
 *  Callback invoked by the MQTT library for events like MQTT disconnection,
 *  incoming MQTT subscription messages from the MQTT broker.
 *    1. In case of MQTT disconnection, the MQTT client task is communicated
 *       about the disconnection using a message queue.
 *    2. When an MQTT subscription message is received, the subscriber callback
 *       function implemented in subscriber_task.c is invoked to handle the
 *       incoming MQTT message.
 *
 * Parameters:
 *  cy_mqtt_t mqtt_handle : MQTT handle corresponding to the MQTT event (unused)
 *  cy_mqtt_event_t event : MQTT event information
 *  void *user_data : User data pointer passed during cy_mqtt_create() (unused)
 *
 * Return:
 *  void
 *
 ******************************************************************************/
static void mqtt_event_callback(cy_mqtt_t mqtt_handle, cy_mqtt_event_t event, void *user_data)
{
    cy_mqtt_publish_info_t *received_msg;
    mqtt_task_cmd_t mqtt_task_cmd;

    CY_UNUSED_PARAMETER(mqtt_handle);
    CY_UNUSED_PARAMETER(user_data);

    printf("\nMQTT Received Event: %d!\n",event.type);

    switch(event.type)
    {
        case CY_MQTT_EVENT_TYPE_DISCONNECT:
        {
            /* Clear the status flag bit to indicate MQTT disconnection. */
            status_flag &= ~(FLAG_MQTT_CONNECTION_SUCCESS);

            /* MQTT connection with the MQTT broker is broken as the client
             * is unable to communicate with the broker. Set the appropriate
             * command to be sent to the MQTT task.
             */
            printf("\nUnexpectedly disconnected from MQTT broker!\n");
            mqtt_task_cmd = HANDLE_DISCONNECTION;

            /* Send the message to the MQTT client task to handle the
             * disconnection.
             */
            xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
            break;
        }

        case CY_MQTT_EVENT_TYPE_SUBSCRIPTION_MESSAGE_RECEIVE:
        {
            status_flag |= FLAG_MQTT_MSG_RECEIVED;

            /* Incoming MQTT message has been received. Send this message to
             * the subscriber callback function to handle it.
             */

            received_msg = &(event.data.pub_msg.received_message);

            mqtt_subscription_callback(received_msg);
            break;
        }
        default :
        {
            /* Unknown MQTT event */
            printf("\nUnknown Event received from MQTT callback!\n");
            break;
        }
    }
}

/******************************************************************************
 * Function Name: mqtt_init
 ******************************************************************************
 * Summary:
 *  Function that initializes the MQTT library and creates an instance for the
 *  MQTT client. The network buffer needed by the MQTT library for MQTT send
 *  send and receive operations is also allocated by this function.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  cy_rslt_t : CY_RSLT_SUCCESS on a successful initialization, else an error
 *              code indicating the failure.
 *
 ******************************************************************************/
static cy_rslt_t mqtt_init(void)
{
    /* Variable to indicate status of various operations. */
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /* Initialize the MQTT library. */
    result = cy_mqtt_init();
    CHECK_RESULT(result, FLAG_LIBS_INITIALIZED, "\nMQTT library initialization failed!\n");

    /* Allocate buffer for MQTT send and receive operations. */
    mqtt_network_buffer = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MQTT_NETWORK_BUFFER_SIZE);
    if(NULL == mqtt_network_buffer)
    {
        result = ~CY_RSLT_SUCCESS;
    }
    CHECK_RESULT(result, FLAG_BUFFER_INITIALIZED, "Network Buffer allocation failed!\n\n");

    /* Create the MQTT client instance. */
    result = cy_mqtt_create(mqtt_network_buffer, MQTT_NETWORK_BUFFER_SIZE,
                            security_info, &broker_info,MQTT_HANDLE_DESCRIPTOR,
                            &mqtt_connection);

    CHECK_RESULT(result, FLAG_MQTT_INSTANCE_CREATED, "\nMQTT instance creation failed!\n");
    if(CY_RSLT_SUCCESS == result)
    {
        /* Register a MQTT event callback */
        result = cy_mqtt_register_event_callback( mqtt_connection, (cy_mqtt_callback_t)mqtt_event_callback, NULL );
        if(CY_RSLT_SUCCESS == result)
        {
            printf("\nMQTT library initialization successful.\n");
        }
    }
    return result;
}

/******************************************************************************
 * Function Name: mqtt_connect
 ******************************************************************************
 * Summary:
 *  Function that initiates MQTT connect operation. The connection is retried
 *  a maximum of 'MAX_MQTT_CONN_RETRIES' times with interval of
 *  'MQTT_CONN_RETRY_INTERVAL_MS' milliseconds.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  cy_rslt_t : CY_RSLT_SUCCESS upon a successful MQTT connection, else an
 *              error code indicating the failure.
 *
 ******************************************************************************/
static cy_rslt_t mqtt_connect(void)
{
    /* Variable to indicate status of various operations. */
    cy_rslt_t result = CY_RSLT_SUCCESS;
    bool mqtt_conn_status;

    /* MQTT client identifier string. */
    char mqtt_client_identifier[(MQTT_CLIENT_IDENTIFIER_MAX_LEN + 1)] = MQTT_CLIENT_IDENTIFIER;

    /* Configure the user credentials as a part of MQTT Connect packet */
    if (strlen(MQTT_USERNAME) > 0)
    {
        connection_info.username = MQTT_USERNAME;
        connection_info.password = MQTT_PASSWORD;
        connection_info.username_len = sizeof(MQTT_USERNAME) - 1;
        connection_info.password_len = sizeof(MQTT_PASSWORD) - 1;
    }

    /* Generate a unique client identifier with 'MQTT_CLIENT_IDENTIFIER' string
     * as a prefix if the `GENERATE_UNIQUE_CLIENT_ID` macro is enabled.
     */
#if GENERATE_UNIQUE_CLIENT_ID
    result = mqtt_get_unique_client_identifier(mqtt_client_identifier);
    CHECK_RESULT(result, 0, "Failed to generate unique client identifier for the MQTT client!\n");
#endif /* GENERATE_UNIQUE_CLIENT_ID */

    /* Set the client identifier buffer and length. */
    connection_info.client_id = mqtt_client_identifier;
    connection_info.client_id_len = strlen(mqtt_client_identifier);

    printf("\n'%.*s' connecting to MQTT broker '%.*s'...\n",
           connection_info.client_id_len,
           connection_info.client_id,
           broker_info.hostname_len,
           broker_info.hostname);

    connection_info.will_info->topic = mqtt_topics[1];
    connection_info.will_info->topic_len = MQTT_TOPIC_SIZE;
    connection_info.will_info->payload = "{\"cmd_id\":0,\"type\":0,\"value\":0}";
    connection_info.will_info->payload_len = (size_t)(strlen(connection_info.will_info->payload));

    mqtt_conn_status = false;
    while(false == mqtt_conn_status)
    {
        if (cy_wcm_is_connected_to_ap() == 0)
        {
            printf("\nUnexpectedly disconnected from Wi-Fi network! \nInitiating Wi-Fi reconnection...\n");
            status_flag &= ~(FLAG_WIFI_CONNECTED);

            /* Initiate Wi-Fi reconnection. */
            result = wifi_connect();
            if (CY_RSLT_SUCCESS != result)
            {
                break;
            }
        }

        /* Establish the MQTT connection. */
        result = cy_mqtt_connect(mqtt_connection, &connection_info);

        if (CY_RSLT_SUCCESS == result)
        {
            printf("MQTT connection successful.\r\n");

        	update_conn_state(DEV_ST_CLOUD_CONNECTED);

            /* Set the appropriate bit in the status_flag to denote successful
             * MQTT connection, and return the result to the calling function.
             */
            status_flag |= FLAG_MQTT_CONNECTION_SUCCESS;
            mqtt_conn_status = true;
            break;
        }

        printf("\nMQTT connection failed with error code 0x%0X.\n",
               (int)result);
        vTaskDelay(MQTT_CONN_RETRY_INTERVAL_MS);
    }
    return result;
}

/*******************************************************************************
* Function Definitions
*******************************************************************************/

/******************************************************************************
 * Function Name: terminate_tasks
 ******************************************************************************
 * Summary:
 *  Cleanup section: Delete subscriber and publisher tasks and perform
 *
 * Parameters:
 *  void
 *
 *
 * Return:
 *  void
 *
 ******************************************************************************/
void terminate_tasks(void)
{
    printf("\nTerminating Publisher and Subscriber tasks...\n");
    if (NULL != subscriber_task_handle )
    {
        vTaskDelete(subscriber_task_handle);
    }
    if (NULL != publisher_task_handle )
    {
        vTaskDelete(publisher_task_handle);
    }
    cleanup_mqtt();
    printf("\nCleanup Done\nTerminating the MQTT task...\n\n");
    vTaskDelete(NULL);
}

/******************************************************************************
 * Function Name: mqtt_client_task
 ******************************************************************************
 * Summary:
 *  Task for handling initialization & connection of Wi-Fi and the MQTT client.
 *  The task also creates and manages the subscriber and publisher tasks upon 
 *  successful MQTT connection. The task also handles the WiFi and MQTT 
 *  connections by initiating reconnection on the event of disconnections.
 *
 * Parameters:
 *  void *pvParameters : Task parameter defined during task creation (unused)
 *
 * Return:
 *  void
 *
 ******************************************************************************/
void mqtt_client_task(void *pvParameters)
{
    /* Structures that store the data to be sent/received to/from various
     * message queues.
     */
    mqtt_task_cmd_t mqtt_status;
    subscriber_data_t subscriber_q_data;
    publisher_data_t publisher_q_data;
    bool mqtt_init_status = false;
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /* To avoid compiler warnings */
    (void) pvParameters;

    /* Create a message queue to communicate with other tasks and callbacks. */
    mqtt_task_q = xQueueCreate(MQTT_TASK_QUEUE_LENGTH, sizeof(mqtt_task_cmd_t));

    /* Set the appropriate bit in the status_flag to denote successful 
     * WCM initialization.
     */
    status_flag |= FLAG_WCM_INITIALIZED;
    printf("\nWi-Fi Connection Manager initialized.\n");

	/* Create the subscriber task and cleanup if the operation fails. */
	if (pdPASS == xTaskCreate(subscriber_task, "Subscriber task", SUBSCRIBER_TASK_STACK_SIZE,
							  NULL, SUBSCRIBER_TASK_PRIORITY, &subscriber_task_handle))
	{
		/* Wait for the subscribe operation to complete. */
		vTaskDelay(pdMS_TO_TICKS(TASK_CREATION_DELAY_MS));

		/* Create the publisher task and cleanup if the operation fails. */
		if (pdPASS != xTaskCreate(publisher_task, "Publisher task", PUBLISHER_TASK_STACK_SIZE,
								  NULL, PUBLISHER_TASK_PRIORITY, &publisher_task_handle))
		{
	        handle_app_error();
		}
	}

    while (true)
    {
        /* Wait for results of MQTT operations from other tasks and callbacks. */
        if (pdTRUE == xQueueReceive(mqtt_task_q, &mqtt_status, portMAX_DELAY))
        {
            /* In this code example, the disconnection from the MQTT Broker or
             * the Wi-Fi network is handled by the case 'HANDLE_DISCONNECTION'.
             *
             * The publish and subscribe failures (`HANDLE_MQTT_PUBLISH_FAILURE`
             * and `HANDLE_MQTT_SUBSCRIBE_FAILURE`) does not initiate
             * reconnection in this example, but they can be handled as per the
             * application requirement in the following swich cases.
             */
            switch(mqtt_status)
            {
                case HANDLE_DISCONNECTION:
                {
                    /* Deinit the publisher before initiating reconnections. */
                    publisher_q_data.cmd = PUBLISHER_DEINIT;
                    xQueueSend(publisher_task_q, &publisher_q_data, portMAX_DELAY);

                    /* Although the connection with the MQTT Broker is lost,
                     * call the MQTT disconnect API for cleanup of threads and
                     * other resources before reconnection.
                     */
                    cy_mqtt_disconnect(mqtt_connection);
                	update_conn_state(DEV_ST_CLOUD_DISCONNECTED);

                    /* Check if Wi-Fi connection is active. If not, update the
                     * status flag and initiate Wi-Fi reconnection.
                     */
                    if (cy_wcm_is_connected_to_ap() == 0)
                    {
                        status_flag &= ~(FLAG_WIFI_CONNECTED);
                        printf("\nInitiating Wi-Fi Reconnection...\n");
                        if (CY_RSLT_SUCCESS == wifi_connect())
                        {
                        	update_conn_state(DEV_ST_WIFI_CONNECTED);
            				vTaskDelay(2000);

                            printf("\nInitiating MQTT Reconnection...\n");
                            if (CY_RSLT_SUCCESS == mqtt_connect())
                            {
                                /* Initiate MQTT subscribe post the reconnection. */
                                subscriber_q_data.cmd = SUBSCRIBE_TO_TOPIC;
                                xQueueSend(subscriber_task_q, &subscriber_q_data, portMAX_DELAY);

        						/* Send the connection status to end device */
        						send_response_numeric(DEVICE_STATUS, OPERATION_READ, 1);
                            }
                        }
                        else
                        {
                        	update_conn_state(DEV_ST_WIFI_DISCONNECTED);
                        }
                    }
                    else
                    {
                        printf("\nInitiating MQTT Reconnection...\n");
						if (CY_RSLT_SUCCESS == mqtt_connect())
						{
							/* Initiate MQTT subscribe post the reconnection. */
							subscriber_q_data.cmd = SUBSCRIBE_TO_TOPIC;
							xQueueSend(subscriber_task_q, &subscriber_q_data, portMAX_DELAY);

							/* Send the connection status to end device */
							send_response_numeric(DEVICE_STATUS, OPERATION_READ, 1);
						}
                    }

                    break;
                }

                /**
                 * Once WIFI connected try to connected with mqtt server
                 */
                case HANDLE_CONNECT:
                {
                	if(false == mqtt_init_status)
                	{
						result = mqtt_init();
	                	if(CY_RSLT_SUCCESS == result)
	                	{
	                		mqtt_init_status = true;
	                	}
	                	else
	                	{
	                        printf("\nMQTT Init failed with error 0x%0X\n\n",
	                               (int)result);
	                	}
                	}

                	update_conn_state(DEV_ST_CLOUD_CONNECTING);
		        	vTaskDelay(1500);

                	result = mqtt_connect();
					if(CY_RSLT_SUCCESS == result)
					{
						/* Initiate MQTT subscribe post the reconnection. */
						subscriber_q_data.cmd = SUBSCRIBE_TO_TOPIC;
						xQueueSend(subscriber_task_q, &subscriber_q_data, portMAX_DELAY);

						/* Send the connection status to end device */
						send_response_numeric(DEVICE_STATUS, OPERATION_READ, 1);

			        	vTaskDelay(1000);
			        	device_provisioned = true;
			        	ble_disconnect();

	                	update_conn_state(DEV_ST_CLOUD_CONNECTED);
					}
                	else
                	{
                        printf("\nMQTT connect failed with error 0x%0X\n\n",
                               (int)result);
                	}
                    break;
                }
                case HANDLE_MANUAL_DISCONNECTION:
                {
                	if(cy_wcm_is_connected_to_ap())
                	{
						/* Initiate MQTT subscribe post the reconnection. */
						subscriber_q_data.cmd = UNSUBSCRIBE_FROM_TOPIC;
						xQueueSend(subscriber_task_q, &subscriber_q_data, portMAX_DELAY);

						vTaskDelay(500);

						/* Although the connection with the MQTT Broker is lost,
						 * call the MQTT disconnect API for cleanup of threads and
						 * other resources before reconnection.
						 */
						cy_mqtt_disconnect(mqtt_connection);
						update_conn_state(DEV_ST_CLOUD_DISCONNECTED);
						vTaskDelay(2000);
                	}

                    xTaskNotify(wifi_task_handle, NOTIF_DISCONNECT | NOTIF_ERASE_DATA,
                                eSetValueWithOverwrite);
                    break;
                }
                default:
                    break;
            }
        }
        else
        {
        	vTaskDelay(500);
        }
    }
}

uint32_t get_mqtt_status(void)
{
	return status_flag;
}

#if GENERATE_UNIQUE_CLIENT_ID
/******************************************************************************
 * Function Name: mqtt_get_unique_client_identifier
 ******************************************************************************
 * Summary:
 *  Function that generates unique client identifier for the MQTT client by
 *  appending a timestamp to a common prefix 'MQTT_CLIENT_IDENTIFIER'.
 *
 * Parameters:
 *  char *mqtt_client_identifier : Pointer to the string that stores the 
 *                                 generated unique identifier
 *
 * Return:
 *  cy_rslt_t : CY_RSLT_SUCCESS on successful generation of the client 
 *              identifier, else a non-zero value indicating failure.
 *
 ******************************************************************************/
static cy_rslt_t mqtt_get_unique_client_identifier(char *mqtt_client_identifier)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    /* Check for errors from snprintf. */
    if (0 > snprintf(mqtt_client_identifier,
                     (MQTT_CLIENT_IDENTIFIER_MAX_LEN + 1),
                     MQTT_CLIENT_IDENTIFIER "%lu",
                     (long unsigned int)Clock_GetTimeMs()))
    {
        status = ~CY_RSLT_SUCCESS;
    }

    return status;
}
#endif /* GENERATE_UNIQUE_CLIENT_ID */

/* [] END OF FILE */
