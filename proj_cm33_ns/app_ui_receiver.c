/*******************************************************************************
 * File Name:  app_ui_receiver.c
 *
 * Description: UI receiver module implementation.
 *
 * This file implements the UI receiver task, which handles events and messages
 * from various sources, including the inter-processor communication (IPC) pipe.
 *
 *
 *******************************************************************************
* Copyright 2025, Cypress Semiconductor Corporation (an Infineon company) or
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

/*******************************************************************************
 *                              INCLUDES
 ******************************************************************************/
#include "app_ui_receiver.h"
#include "wifi_task.h"
#include "wireless_manager.h"
#include "mqtt_command_handler.h"
#include "mqtt_task.h"
#include "publisher_task.h"

/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/
#define UI_RX_TASK_NAME                       ("UIRxTask")

/* stack size in words */
#define UI_RX_TASK_STACK_SIZE                 (1 * 1024)

#define UI_RX_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)
#define MAX_BLE_RETRY                           20U
#define UI_RX_QUEUE_LENGTH                      20U

/*******************************************************************************
 *                              GLOBAL VARIABLES
 ******************************************************************************/
/* Task handler for communication with the UI_RX task */
TaskHandle_t cm33_ui_rx_task_handle = NULL;

extern ipc_msg_t *ipc_recv_msg;

extern bool is_wifi_connection_cancel;
extern bool is_cloud_connection_cancel;
extern connection_retries_t auto_reconnect_retry;
extern bool device_provisioned;

extern uint8_t wifi_mac[6];

bool update_credential = false;
QueueHandle_t xUiRxQueue = NULL;

/*******************************************************************************
 *                              STATIC VARIABLES
 ******************************************************************************/
static volatile uint32_t msg_val = 0;
static volatile uint32_t msg_cmd = 0;

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/
static void ui_rx_task(void *arg);
/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

void get_latet_FW_version(void)
{
    char payload[50];

    if((cy_wcm_is_connected_to_ap() == 0) || (!(get_mqtt_status() & FLAG_MQTT_CONNECTION_SUCCESS)))
    {
        no_internet_connected_send_to_ui();
        return;
    }

    snprintf(payload, sizeof(payload), "{\"Message\":\"Update Version\",\"UID\":\"%X%X%X\"}", wifi_mac[3], wifi_mac[4], wifi_mac[5]);
    publish_msg_for_FW_version(payload);
}

static void ui_rx_task(void *arg)
{
    (void) arg;
    mqtt_task_cmd_t mqtt_task_cmd;
    uint32_t notificationValue;
    ipc_msg_t received_msg = {0};

    /* Create the queue for IPC msg */
    xUiRxQueue = xQueueCreate(UI_RX_QUEUE_LENGTH, sizeof(ipc_msg_t));
    if (xUiRxQueue == NULL)
    {
        LOG_ERROR(CYLF_DEF, "UI RX Queue creation failed!\n");
    }

    while (1)
    {
        /* Wait for events via the message queue (block indefinitely) */
        if (xQueueReceive(xUiRxQueue, &received_msg, portMAX_DELAY) == pdPASS)
        {
            /* Message successfully received. */
            LOG_INFO(CYLF_DEF, "Received message CMD: %lu\n", received_msg.cmd);

            /* Parse the received msg command and value */
            msg_val = received_msg.data;
            msg_cmd = received_msg.cmd;

            switch (msg_cmd)
            {
                case IPC_CMD_GET_UID:
                    LOG_INFO(CYLF_DEF, "Request to send UID received\n");
                    LOG_INFO(CYLF_DEF, "BLE Dev name: %s\n", (char*) ble_name);
                    response_uid_req((char*) ble_name);
                    break;

                case IPC_CMD_SET_DISPLAY_BRIGHTNESS:
                    LOG_INFO(CYLF_DEF, "Rx Brightness : %u\n", msg_val);
                    send_response_numeric(DEVICE_BRIGHTNESS, OPERATION_READ, (uint32_t) msg_val);
                    break;

                case IPC_CMD_RESET_WIFI_SSID_PASS:
                    LOG_INFO(CYLF_DEF, "Rx Wi-Fi credential erase request.\n");
                    handle_pairingremove_command(true);
                    break;

                case IPC_CMD_SET_FAN_SPEED:
                    device_status.thermostat_settings.fan_speed = msg_val;
                    send_response_numeric(FAN_SPEED, OPERATION_READ,
                            (uint32_t) device_status.thermostat_settings.fan_speed);
                    LOG_INFO(CYLF_DEF, "Rx Fan Mode : %u\n", msg_val);
                    break;

                case IPC_CMD_SET_THERMOSTAT_MODE:
                    device_status.thermostat_settings.mode = msg_val;
                    send_response_numeric(DEVICE_MODE, OPERATION_READ, (uint32_t) device_status.thermostat_settings.mode);
                    LOG_INFO(CYLF_DEF, "Rx Thermostat Mode : %u\n", msg_val);
                    break;

                case IPC_CMD_SET_CURRENT_TEMP:
                    device_status.environment.current_temp = msg_val;
                    send_response_numeric(CURRENT_TEMP, OPERATION_READ, (uint32_t) device_status.environment.current_temp);
                    LOG_INFO(CYLF_DEF, "Rx Current temp : %u\n", msg_val);
                    break;

                case IPC_CMD_SET_TARGET_TEMP:
                    device_status.environment.target_temp = msg_val;
                    send_response_numeric(TARGETED_TEMP, OPERATION_READ, (uint32_t) device_status.environment.target_temp);
                    LOG_INFO(CYLF_DEF, "Rx Target temp : %u\n", msg_val);
                    break;

                case IPC_CMD_SET_TEMPERATURE_DATA:
                    LOG_INFO(CYLF_DEF, "Rx IPC_CMD_SET_TEMPERATURE_DATA.\n");

                    static device_state_t temp_data = { 0 };
                    static device_state_t prev_data = { 0 };
                    static int32_t last_sent_time_minutes = -1;

                    temp_data.environment.current_temp = ipc_recv_msg->device_config.environment.current_temp;
                    temp_data.environment.target_temp = ipc_recv_msg->device_config.environment.target_temp;
                    temp_data.thermostat_settings.time_remains =
                            ipc_recv_msg->device_config.thermostat_settings.time_remains;

                    LOG_INFO(CYLF_DEF, "Current T: %u, Target T: %u, RS: %u\n", temp_data.environment.current_temp,
                            temp_data.environment.target_temp, temp_data.thermostat_settings.time_remains);

					if (temp_data.environment.target_temp != prev_data.environment.target_temp) 
					{
						prev_data.environment.target_temp = temp_data.environment.target_temp;
						device_status.environment.target_temp = temp_data.environment.target_temp;

                        /* Send target temperature multiple times
                         * to avoid delay on Mapp. */
                        for (int i = 0; i < 3; i++)
                        {
                            send_response_numeric(TARGETED_TEMP, OPERATION_READ,
                                                  (uint32_t) device_status.environment.target_temp);
                            vTaskDelay(pdMS_TO_TICKS(5));
                        }
					}

					if (temp_data.environment.current_temp != prev_data.environment.current_temp)
                    {
                        prev_data.environment.current_temp = temp_data.environment.current_temp;
                        device_status.environment.current_temp = temp_data.environment.current_temp;
                        send_response_numeric(CURRENT_TEMP, OPERATION_READ,
                                (uint32_t) device_status.environment.current_temp);
                    }

                    /* Check if the seconds value has changed. */
                    if (temp_data.thermostat_settings.time_remains != prev_data.thermostat_settings.time_remains)
                    {
                        /* Update the local status first. */
                        prev_data.thermostat_settings.time_remains = temp_data.thermostat_settings.time_remains;
                        device_status.thermostat_settings.time_remains = temp_data.thermostat_settings.time_remains;

                        /* Calculate the current number of minutes for comparison. */
                        int32_t current_minutes = device_status.thermostat_settings.time_remains / 60;

                        /* Only send to the cloud if the minute value has changed. */
                        if (current_minutes != last_sent_time_minutes) {
                            last_sent_time_minutes = current_minutes;

                            /* Send the actual value in seconds, as required by the cloud. */
                            send_response_numeric(TIME_REMAINING, OPERATION_READ,
                                    (uint32_t) device_status.thermostat_settings.time_remains);
                        }
                    }
                    break;

                case IPC_CMD_SWITCH_TO_BLE:
                    LOG_INFO(CYLF_DEF, "Rx IPC_CMD_SWITCH_TO_BLE.\n");
                    switch_to_ble_mqtt_disconn = true;
                    update_comm_interface(CONNECTIVITY_NONE);

                    /* Check if device connected over BLE */
                    if (conn_id != true)
                    {
                        /* Send the connection status to end device */
                        for (int retry = 0; retry < 5; retry++)
                        {
                            send_response_numeric(DEVICE_SWITCH_TO_BLE, OPERATION_READ, 1);
                            vTaskDelay(200);
                        }
                    }
                    else
                    {
                        LOG_INFO(CYLF_DEF, "Device already connected over BLE\n");
                    }

                    mqtt_task_cmd_t mqtt_task_cmd = HANDLE_MANUAL_DISCONNECTION;
                    xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
                    break;

                case IPC_CMD_SET_REMAINING_TIME:
                    device_status.thermostat_settings.time_remains = msg_val;
                    send_response_numeric(TIME_REMAINING, OPERATION_READ,
                            (uint32_t) device_status.thermostat_settings.time_remains);
                    LOG_INFO(CYLF_DEF, "Rx Remaining time : %u\n", msg_val);
                    break;

                case IPC_CMD_UPDATE_CONN_RETRIES:
                    auto_reconnect_retry = (connection_retries_t) msg_val;
                    if (WIFI_CONN_RETRY_INFINITE == auto_reconnect_retry && get_device_provision_state())
                    {
                        update_conn_state(DEV_ST_WIFI_CONNECTING);
                        xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT), eSetValueWithOverwrite);
                    }
                    else if (CLOUD_CONN_RETRY_INFINITE == auto_reconnect_retry)
                    {
                        mqtt_task_cmd_t mqtt_task_cmd = HANDLE_CONNECT;
                        xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
                    }
                    LOG_INFO(CYLF_DEF, "Rx Retries : %u\n", msg_val);
                    break;

                case IPC_CMD_UPDATE_CONN_STATE:
                    switch ((device_connection_state_t) msg_val)
                    {
                        case DEV_ST_CLOUD_CONNECTION_CANCEL:
                            is_cloud_connection_cancel = true;
                            mqtt_task_cmd = HANDLE_MANUAL_DISCONNECTION;
                            xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
                            LOG_INFO(CYLF_DEF, "Rx DEV_ST_CLOUD_CONNECTION_CANCEL.\n");
                            update_comm_interface(CONNECTIVITY_NONE);
                            break;

                        case DEV_ST_WIFI_CONNECTION_CANCEL:
                            LOG_INFO(CYLF_DEF, "Rx DEV_ST_WIFI_CONNECTION_CANCEL.\n");
                            is_wifi_connection_cancel = true;
                            update_comm_interface(CONNECTIVITY_NONE);

                            /* Check if BLE connected or not, and initiate
                             * actions accordingly. */
                            if (conn_id != false)
                            {
                                LOG_INFO(CYLF_DEF, "DEV_ST_WIFI_CONNECTION_CANCEL BLE already connected.\n");
                                update_conn_state(DEV_ST_BLE_CONNECTED);
                                handle_connectivity_state(STATE_BLE_CONNECTED);
                            }
                            else
                            {
                                LOG_INFO(CYLF_DEF, "DEV_ST_WIFI_CONNECTION_CANCEL BLE start ADV.\n");
                                handle_connectivity_state(STATE_BLE_ADV);
                            }
                            break;

                        case DEV_ST_BLE_ADVERTISEMENT_CANCEL:
                            LOG_INFO(CYLF_DEF, "Rx DEV_ST_BLE_ADVERTISEMENT_CANCEL.\n");
                            ble_stop_advertising();
                            if (get_device_provision_state())
                            {
                                vTaskDelay(2500);
                                update_conn_state(DEV_ST_WIFI_CONNECTING);
                                xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT), eSetValueWithOverwrite);
                            }
                            else
                            {
                                update_conn_state(DEV_ST_UNPROVISIONED);
                                LOG_INFO(CYLF_DEF, "Wifi Credential not available.\n");
                            }
                            update_comm_interface(CONNECTIVITY_NONE);
                            break;

                        case DEV_ST_BLE_ADVERTISING:
                            LOG_INFO(CYLF_DEF, "Rx DEV_ST_BLE_ADVERTISING. Staring BLE ADV.\n");
                            handle_connectivity_state(STATE_START_PROVISIONING);
                            update_conn_state(DEV_ST_BLE_ADVERTISING);
                            break;

                        case DEV_ST_WIFI_CONNECTING:
                            LOG_INFO(CYLF_DEF, "Rx DEV_ST_WIFI_CONNECTING.\n");
                            wifi_credentials_t info = { 0 };

                            // Safe copy with null termination
                            strncpy(info.ssid, ipc_recv_msg->wifi_info.ssid, sizeof(info.ssid) - 1);
                            info.ssid[sizeof(info.ssid) - 1] = '\0';

                            strncpy(info.password, ipc_recv_msg->wifi_info.password, sizeof(info.password) - 1);
                            info.password[sizeof(info.password) - 1] = '\0';

                            LOG_INFO(CYLF_DEF, "Received SSID: %s, Password: %s\n", info.ssid, info.password);

                            /* Set the WiFi Connection parameters structure to 0 before copying
                             * data */
                            memset(&wifi_conn_param, 0, sizeof(cy_wcm_connect_params_t));

                            /* Copy the WiFi credentials to the global variable */
                            memcpy(wifi_conn_param.ap_credentials.SSID, &info.ssid[0], sizeof(info.ssid));

                            memcpy(wifi_conn_param.ap_credentials.password, &info.password[0], sizeof(info.password));

                            memset(&wifi_details, 0, sizeof(wifi_details));
                            // Copy SSID
                            uint8_t ssid_len = strlen(info.ssid);
                            if (ssid_len > CY_WCM_MAX_SSID_LEN)
                                ssid_len = CY_WCM_MAX_SSID_LEN;

                            memcpy(wifi_details.wifi_ssid, info.ssid, ssid_len);
                            wifi_details.ssid_len = ssid_len;

                            // Copy Password
                            uint8_t password_len = strlen(info.password);
                            if (password_len > CY_WCM_MAX_PASSPHRASE_LEN)
                                password_len = CY_WCM_MAX_PASSPHRASE_LEN;

                            memcpy(wifi_details.wifi_password, info.password, password_len);
                            wifi_details.password_len = password_len;
                            update_conn_state(DEV_ST_WIFI_CONNECTING);

                            if (device_provisioned)
                            {
                                mqtt_task_cmd = HANDLE_MANUAL_DISCONNECTION;
                                xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
                                update_credential = true;
                            }
                            else
                            {
                                /* Unblock WiFi task with notification */
                                xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT), eSetValueWithOverwrite);
                            }
                            break;

                        case DEV_ST_SWITCH_WIFI:
                            LOG_INFO(CYLF_DEF, "Rx DEV_ST_SWITCH_WIFI.\n");

                            /* Send the connection status to end device */
                            for (int retry = 0; retry < 1; retry++)
                            {
                                send_response_numeric(DEVICE_SWITCH_TO_WIFI, OPERATION_READ, 1);
                                vTaskDelay(200);
                            }

                            if (get_device_provision_state())
                            {
                                xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT), eSetValueWithOverwrite);
                            }
                            else
                            {
                                update_conn_state(DEV_ST_UNPROVISIONED);
                                LOG_INFO(CYLF_DEF, "Wifi Credential not available.\n");
                            }

                            LOG_INFO(CYLF_DEF, "Request to switch to WiFi connection\n");
                            break;

                        default:
                            break;
                    }
                    break;

                case IPC_CMD_DEVICE_CONFIG:
                    LOG_INFO(CYLF_DEF, "Rx IPC_CMD_DEVICE_CONFIG\n");
                    device_status = ipc_recv_msg->device_config;

                    /* Send received device info over BLE/MQTT */
                    send_device_config_info();
                    break;

                case IPC_CMD_SET_AUDIO_LEVEL:
                    LOG_INFO(CYLF_DEF, "Rx Audio level : %u\n", msg_val);
                    send_response_numeric(DEVICE_AUDIO, OPERATION_READ, (uint32_t) msg_val);
                    break;

                case IPC_CMD_SET_TEMP_UNIT:
                    device_status.thermostat_settings.temp_unit = (temp_unit_t) msg_val;
                    send_response_numeric(TEMP_UNIT, OPERATION_READ,
                            (uint32_t) device_status.thermostat_settings.temp_unit);
                    break;

                default:
                    break;
            }
        }
    }
}

void ui_rx_thread_init(void)
{
    BaseType_t task_return = pdFAIL;

    /* Create the FreeRTOS Task */
    task_return = xTaskCreate(ui_rx_task, UI_RX_TASK_NAME, UI_RX_TASK_STACK_SIZE, NULL,
            UI_RX_TASK_PRIORITY, &cm33_ui_rx_task_handle);
    if (task_return != pdPASS)
    {
        LOG_ERROR(CYLF_DEF, "UI_RX task create Error.\n");
    }
    else
    {
        LOG_INFO(CYLF_DEF, "UI_RX task create OK.\r\n");
    }
}

/* [] END OF FILE */
