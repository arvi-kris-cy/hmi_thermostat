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
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_UPDATE_CONN_STATE;
	cm33_msg_data.data = state;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}


void update_current_screen(screen_id_t screen)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_UPDATE_CURRENT_SCREEN;
	cm33_msg_data.data = screen;
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

void set_device_co2_level(uint16_t value)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_SET_CURRENT_CO2_LEVEL;
    cm33_msg_data.data = value;

    cm33_send_msg_cm55(&cm33_msg_data);
}

void update_presence_detection_status(presence_status_t status)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_UPDATE_PRESENCE_STATUS;
    cm33_msg_data.data = status;

    cm33_send_msg_cm55(&cm33_msg_data);
}

void update_provision_state(bool state)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_UPDATE_PROVISION_STATE;
	cm33_msg_data.data = state;
	cm33_msg_data.data_len = 1;
	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_datetime_ui(DateTime *timestamp)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_SET_DATE_TIME;
    memcpy(&cm33_msg_data.datetime, (DateTime*)timestamp, sizeof(DateTime));

    cm33_send_msg_cm55(&cm33_msg_data);
}

void OTA_Tigger_On_ui(void)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_TRIGGER_OTA_START;
	cm33_msg_data.data = 0;
	cm33_msg_data.data_len = 1;

	cm33_send_msg_cm55(&cm33_msg_data);
}

void send_available_FW_version(char *version)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_OTA_VERSION;
    memset(&cm33_msg_data.fw_version, 0, strlen(version) + 1);
    memcpy(&cm33_msg_data.fw_version, version, strlen(version));

    cm33_send_msg_cm55(&cm33_msg_data);
}

void abort_OTA(void)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_ABORT_OTA;

    cm33_send_msg_cm55(&cm33_msg_data);
}

void no_internet_connected_send_to_ui(void)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_NO_INTERNET_NOTIFY;

    cm33_send_msg_cm55(&cm33_msg_data);
}


void disable_touch(void)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_DISABLE_TOUCH;

    cm33_send_msg_cm55(&cm33_msg_data);
}

void set_weather_sync(const char *value)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_WEATHER_SYNC;

    // Clear and copy the string safely
    memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
    strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

    cm33_send_msg_cm55(&cm33_msg_data);
}

void set_location_sync(const char *value)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_LOCATION_SYNC;

    // Clear and copy the string safely
    memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
    strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

    cm33_send_msg_cm55(&cm33_msg_data);
}

void set_hour_sync(const char *value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_HOUR_SYNC;

	// Clear and copy the string safely
	memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
	strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_minute_sync(const char *value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_MINUTE_SYNC;

	// Clear and copy the string safely
	memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
	strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_second_sync(const char *value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_SECOND_SYNC;

	// Clear and copy the string safely
	memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
	strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_date_sync(const char *value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_DATE_SYNC;

	// Clear and copy the string safely
	memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
	strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_month_sync(const char *value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_MONTH_SYNC;

	// Clear and copy the string safely
	memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
	strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_vaar_sync(const char *value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_VAAR_SYNC;

	// Clear and copy the string safely
	memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
	strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_year_sync(const char *value)
{
	cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
	cm33_msg_data.cmd = IPC_CMD_YEAR_SYNC;

	// Clear and copy the string safely
	memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
	strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

	cm33_send_msg_cm55(&cm33_msg_data);
}

void set_weather_code_sync(const char *value)
{
    cm33_msg_data.client_id = CM55_IPC_PIPE_CLIENT_ID;
    cm33_msg_data.cmd = IPC_CMD_WEATHER_CODE_SYNC;

    // Clear and copy the string safely
    memset(cm33_msg_data.char_value, 0, sizeof(cm33_msg_data.char_value));
    strncpy(cm33_msg_data.char_value, value, sizeof(cm33_msg_data.char_value) - 1);  // Null-terminate

    cm33_send_msg_cm55(&cm33_msg_data);
}

