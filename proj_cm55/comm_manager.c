#include "comm_manager.h"
#include "ipc_communication.h"

CY_SECTION_SHAREDMEM static ipc_msg_t cm55_msg_data;

void update_fan_speed_ipc(fan_speed_t value)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_FAN_SPEED;
	cm55_msg_data.data = value;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_device_mode_ipc(thermostat_mode_t value)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_THERMOSTAT_MODE;
	cm55_msg_data.data = value;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_current_temp_ipc(int temperature)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_CURRENT_TEMP;
	cm55_msg_data.data = temperature;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_brightness_ipc(uint8_t level)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_DISPLAY_BRIGHTNESS;
	cm55_msg_data.data = level;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_target_temp_ipc(int temperature)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_TARGET_TEMP;
	cm55_msg_data.data = temperature;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_remainig_time_ipc(uint32_t time_s)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_REMAINING_TIME;
	cm55_msg_data.data = time_s;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_temperature_data_ipc(device_state_t *data)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_TEMPERATURE_DATA;

	memset(&(cm55_msg_data.device_config), 0, sizeof(cm55_msg_data.device_config));
	memcpy(&(cm55_msg_data.device_config), data, sizeof(cm55_msg_data.device_config));

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_audio_level_ipc(uint16_t level)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_AUDIO_LEVEL;
	cm55_msg_data.data = level;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_ble_adv_ipc(device_connection_state_t state)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_UPDATE_CONN_STATE;
	cm55_msg_data.data = state;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_wifi_cred_ipc(const char *ssid, const char *passwd)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_UPDATE_CONN_STATE;
	cm55_msg_data.data = DEV_ST_WIFI_CONNECTING;

	memset(&(cm55_msg_data.wifi_info), 0, sizeof(cm55_msg_data.wifi_info));
	strncpy(cm55_msg_data.wifi_info.ssid, ssid, sizeof(cm55_msg_data.wifi_info.ssid) - 1);
	cm55_msg_data.wifi_info.ssid[sizeof(cm55_msg_data.wifi_info.ssid) - 1] = '\0';

	strncpy(cm55_msg_data.wifi_info.password, passwd, sizeof(cm55_msg_data.wifi_info.password) - 1);
	cm55_msg_data.wifi_info.password[sizeof(cm55_msg_data.wifi_info.password) - 1] = '\0';


	cm55_send_msg_cm33(&cm55_msg_data);
}

void request_uid_ipc(void)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_GET_UID;
	cm55_msg_data.data = 0;

	cm55_send_msg_cm33(&cm55_msg_data);
}

void request_wifi_delete_ipc(void)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_RESET_WIFI_SSID_PASS;
	cm55_msg_data.data = 0;

	cm55_send_msg_cm33(&cm55_msg_data);
}


void send_device_config(device_state_t config)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_DEVICE_CONFIG;
	cm55_msg_data.data = 0;
	cm55_msg_data.device_config = config;

	cm55_send_msg_cm33(&cm55_msg_data);
}
