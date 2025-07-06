/*
 * app_UI_RX.c
 *
 *  Created on: 05-Jun-2025
 *      Author: Tejas.Patel
 */

/*******************************************************************************
 *                                INCLUDES
 ******************************************************************************/
#include "app_ui_receiver.h"
#include "wifi_task.h"
#include "wireless_manager.h"
#include "mqtt/mqtt_command_handler.h"

/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/
#define UI_RX_EVT_QUEUE_LENGTH   (10)

#define UI_RX_TASK_NAME                       ("CM33 UI RX Task")
/* stack size in words */
#define UI_RX_TASK_STACK_SIZE                 (4 * 1024)

#define UI_RX_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)

/*******************************************************************************
 *                             GLOBAL VARIAUI_RXS
 ******************************************************************************/
/* Queue handle for communication with the UI_RX task */
static QueueHandle_t app_ui_rx_q;

TaskHandle_t cm33_ui_rx_task_handle = NULL;

extern volatile bool cm33_pipe2_msg_received;

volatile uint32_t msg_val = 0;
volatile uint32_t msg_cmd = 0;

extern ipc_msg_t *ipc_recv_msg;


/*******************************************************************************
 *                             STATIC VARIAUI_RXS
 ******************************************************************************/

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/
static void ui_rx_app_evt_handler(void *evt);
static void ui_rx_task(void *arg);

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

static void ui_rx_app_evt_handler(void *evt) {
	app_event_t *q_evt = (app_event_t*) evt;

	switch (q_evt->type) {
	case EVT_BLE_START_ADV:
		printf("Received EVT_UI_RX_START_ADV\n");
		//TODO Call UI_RX advertising function
		break;

	case EVT_WIFI_CONNECT:
		wifi_details_t network_details = {0};
		memcpy(&network_details, &(q_evt->wifi_info), sizeof(wifi_details_t));
		break;

	default:
		printf("Received unhandled UI_RX event.\n");
		break;
	}

}

static void ui_rx_task(void *arg) {
	(void) arg;
	app_event_t event;

	/* Initialize UI_RX task queue */
	app_ui_rx_q = xQueueCreate(UI_RX_EVT_QUEUE_LENGTH, sizeof(app_event_t));
	if (app_ui_rx_q == NULL) {
		printf("UI_RX app. event queue creation Error.\n");
	} else {
		printf("UI_RX app. event queue creation OK.\n");
	}

	while (1) {
		/* Wait for events in task queue (block indefinitely) */
		if (xQueueReceive(app_ui_rx_q, &event, /*portMAX_DELAY*/pdMS_TO_TICKS(100)) == pdTRUE)
		{
			printf("Received UI_RX event: %d\n", event.type);
			switch (event.type)
			{
				case EVT_BLE_START_ADV:
				case EVT_WIFI_CONNECT:
					printf("Received UI_RX event : %d.\n", event.type);
					/* call UI_RX event handler */
					event.Handler(&event);
					break;

				default:
					printf("Received UI_RX Event not handled : %d.\n", event.type);
					break;
			}

		}
		else
		{
//			printf("CM33 running\n");
			if(cm33_pipe2_msg_received == true)
			{
				cm33_pipe2_msg_received = false;

				switch(msg_cmd) {
				case IPC_CMD_GET_UID:
					printf("Request to send UID received\n");
					printf("BLE Dev name: %s\n", (char*)ble_name);
					response_uid_req((char*)ble_name);
					break;

				case IPC_CMD_SET_DISPLAY_BRIGHTNESS:
					printf("Rx Brightness : %ld\n", msg_val);
					send_response(DEVICE_BRIGHTNESS, OPERATION_READ, (uint32_t)msg_val);
					break;

				case IPC_CMD_RESET_WIFI_SSID_PASS:
					printf("Rx Wi-Fi credential erase request.\n");
					handle_pairingremove_command();
					break;

				case IPC_CMD_SET_FAN_SPEED:
					device_status.thermostat_settings.fan_speed = msg_val;
					send_response(FAN_SPEED, OPERATION_READ, (uint32_t)device_status.thermostat_settings.fan_speed);
					printf("Rx Fan Mode : %ld\n", msg_val);
					break;

				case IPC_CMD_SET_THERMOSTAT_MODE:
					device_status.thermostat_settings.mode = msg_val;
					send_response(DEVICE_MODE, OPERATION_READ, (uint32_t)device_status.thermostat_settings.mode);
					printf("Rx Thermostat Mode : %ld\n", msg_val);
					break;

				case IPC_CMD_SET_CURRENT_TEMP:
					device_status.environment.current_temp = msg_val;
					send_response(CURRENT_TEMP, OPERATION_READ, (uint32_t)device_status.environment.current_temp);
					printf("Rx Current temp : %ld\n", msg_val);
					break;

				case IPC_CMD_SET_TARGET_TEMP:
					device_status.environment.target_temp = msg_val;
					send_response(TARGETED_TEMP, OPERATION_READ, (uint32_t)device_status.environment.target_temp);
					printf("Rx Target temp : %ld\n", msg_val);
					break;

				case IPC_CMD_SET_REMAINING_TIME:
					device_status.thermostat_settings.time_remains = msg_val;
					send_response(TIME_REMAINING, OPERATION_READ, (uint32_t)device_status.thermostat_settings.time_remains);
					printf("Rx Remaining time : %ld\n", msg_val);
					break;

				case IPC_CMD_UPDATE_CONN_STATE:
					switch((device_connection_state_t)msg_val)
					{
					case DEV_ST_BLE_ADVERTISING:
						printf("Rx DEV_ST_BLE_ADVERTISING. Staring BLE ADV.\n");
						ble_init();
						break;

					case DEV_ST_WIFI_CONNECTING:
						printf("Rx DEV_ST_WIFI_CONNECTING.\n");
						wifi_credentials_t info = {0};

						// Safe copy with null termination
						strncpy(info.ssid, ipc_recv_msg->wifi_info.ssid, sizeof(info.ssid) - 1);
						info.ssid[sizeof(info.ssid) - 1] = '\0';

						strncpy(info.password, ipc_recv_msg->wifi_info.password, sizeof(info.password) - 1);
						info.password[sizeof(info.password) - 1] = '\0';

						printf("Received SSID: %s, Password: %s\n", info.ssid, info.password);

						/* Set the WiFi Connection parameters structure to 0 before copying
						 * data */
						memset(&wifi_conn_param, 0, sizeof(cy_wcm_connect_params_t));

						/* Copy the WiFi credentials to the global variable */
						memcpy(wifi_conn_param.ap_credentials.SSID, &info.ssid[0],
							   sizeof(info.ssid));

						memcpy(wifi_conn_param.ap_credentials.password,
							   &info.password[0], sizeof(info.password));

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

						/* Unblock WiFi task with notification */
						xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT),
									eSetValueWithOverwrite);
						break;

					default:
						break;
					}
					break;

				default:
					break;
				}
			}
		}
	}
}

void put_ui_evt_task_q(app_event_t *event)
{
    app_event_t q_evt = {0};
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t result;

    /* Load event details */
    memcpy(&q_evt, event, sizeof(app_event_t));
    q_evt.Handler = ui_rx_app_evt_handler;

    if (xPortIsInsideInterrupt())
    {
        result = xQueueSendFromISR(app_ui_rx_q, (app_event_t*)&q_evt, &xHigherPriorityTaskWoken);
        if (result != pdPASS)
        {
            printf("UI_RX app. evt queue put Error (from ISR)\n");
        }
        else
        {
            printf("UI_RX app. evt queue put OK (from ISR)\n");

            /* If sending unblocked a higher-priority task, request context switch */
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else
    {
        result = xQueueSend(app_ui_rx_q, &q_evt, pdMS_TO_TICKS(100));
        if (result != pdPASS)
        {
            printf("UI_RX app. evt queue put Error\n");
        }
        else
        {
            printf("UI_RX app. evt queue put OK.\n");
        }
    }
}

void ui_rx_thread_init(void) {
	BaseType_t task_return = pdFAIL;

	/* Create the FreeRTOS Task */
	task_return = xTaskCreate(ui_rx_task, UI_RX_TASK_NAME,
		UI_RX_TASK_STACK_SIZE, NULL,
		UI_RX_TASK_PRIORITY, &cm33_ui_rx_task_handle);
	if (task_return != pdPASS) {
		printf("UI_RX task create Error.\n");
	} else {
		printf("UI_RX task create OK.\r\n");
	}
}

