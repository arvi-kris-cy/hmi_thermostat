/*
 * app_common.c
 *
 *  Created on: 18-Jun-2025
 *      Author: Adnan.Shaikh
 */
#include "app_common.h"
#include "ipc_communication.h"

uint32_t get_timeout_ms(idle_timeout_t timeout)
{
    switch (timeout) {
        case TIMEOUT_3S:     return 3000;
        case TIMEOUT_5S:     return 5000;
        case TIMEOUT_10S:    return 10000;
        case TIMEOUT_20S:    return 20000;
        case TIMEOUT_30S:    return 30000;
        case TIMEOUT_NEVER:  return 0; // Or 0 or some sentinel value
        default:             return 10000;
    }
}


void handle_ipc_command(ipc_msg_t *msg, device_state_t *state)
{
	if(NULL == msg || NULL == state)
	{
		//handle error
		return;
	}

	switch (msg->cmd)
	{
		case IPC_CMD_CURRENT_EVENT:
			break;

			// Wi-Fi
		case IPC_CMD_SET_WIFI_SSID:
			{
				if(sizeof(state->wifi.ssid) >= msg->data_len)
				{
					strncpy(state->wifi.ssid, (char *)msg->data, msg->data_len);
				}
				//manage error
				break;
			}
		case IPC_CMD_SET_WIFI_PASSWORD:
			{
				if(sizeof(state->wifi.password) >= msg->data_len)
				{
					strncpy(state->wifi.password, (char *)msg->data, msg->data_len);
				}
				//manage error
				break;
			}

			// Environmental data
		case IPC_CMD_GET_CURRENT_TEMP:
			{
				state->environment.current_temp = msg->data;
				break;
			}
		case IPC_CMD_GET_CURRENT_HUMIDITY:
			{
				state->environment.current_humidity = msg->data;
				break;
			}
		case IPC_CMD_GET_CURRENT_CO2_LEVEL:
			{
				state->environment.current_co2_level = msg->data;
				break;
			}
		case IPC_CMD_SET_TARGET_TEMP:
			{
				state->environment.target_temp = msg->data;
				break;
			}

			// Thermostat settings
		case IPC_CMD_SET_FAN_SPEED:
			{
				state->thermostat_settings.fan_speed = (fan_speed_t)msg->data;
				break;
			}
		case IPC_CMD_GET_FAN_SPEED:
			{
				state->thermostat_settings.fan_speed = (fan_speed_t)msg->data;
				break;
			}
		case IPC_CMD_SET_THERMOSTAT_MODE:
			{
				state->thermostat_settings.mode = (thermostat_mode_t)msg->data;
				break;
			}
		case IPC_CMD_GET_THERMOSTAT_MODE:
			{
				state->thermostat_settings.mode = (thermostat_mode_t)msg->data;
				break;
			}

			// User preferences
		case IPC_CMD_SET_DISPLAY_BRIGHTNESS:
			{
				state->preferences.display_brightness = (uint8_t)msg->data;
				break;
			}
		case IPC_CMD_GET_DISPLAY_BRIGHTNESS:
			{
				state->preferences.display_brightness = (uint8_t)msg->data;
				break;
			}
		case IPC_CMD_SET_AUDIO_LEVEL:
			{
				state->preferences.audio_level = (uint8_t)msg->data;
				break;
			}
		case IPC_CMD_GET_AUDIO_LEVEL:
			{
				state->preferences.audio_level = (uint8_t)msg->data;
				break;
			}

		default:
			{
				//handle error
				break;
			}
	}
}
