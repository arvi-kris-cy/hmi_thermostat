#include "comm_manager.h"
#include "ipc_communication.h"

CY_SECTION_SHAREDMEM static ipc_msg_t cm33_msg_data;

void set_fan_speed(fan_speed_t value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SET_FAN_SPEED;
	cm33_msg_data.data = value;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}


void set_device_mode(thermostat_mode_t value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SET_THERMOSTAT_MODE;
	cm33_msg_data.data = value;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_device_temp(uint8_t value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SET_TARGET_TEMP;
	cm33_msg_data.data = value;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_device_brightness(uint8_t level)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SET_DISPLAY_BRIGHTNESS;
	cm33_msg_data.data = level;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_device_audio_level(uint8_t level)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SET_AUDIO_LEVEL;
	cm33_msg_data.data = level;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}

void display_ble_pin(uint32_t pin)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_CURRENT_EVENT;
	cm33_msg_data.data = pin;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}

void update_conn_state(uint32_t state)
{
	printf("Update conn. state: %ld\n", state);

	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_UPDATE_CONN_STATE;
	cm33_msg_data.data = state;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}

void response_uid_req(char *uid)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SET_UID;
	memcpy(cm33_msg_data.unique_id, (char*)uid, 13);

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_device_audio(audio_level_t value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SET_AUDIO_LEVEL;
	cm33_msg_data.data = value;

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_device_temp_unit(temp_unit_t value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SET_TEMP_UNIT;
	cm33_msg_data.data = value;

	cm33_send_msg_cm55(&cm33_msg_data);
}
