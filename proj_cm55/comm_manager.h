/*
 * communication.h
 *
 */

#ifndef COMM_MANAGER_H_
#define COMM_MANAGER_H_

#include "app_common.h"

void update_fan_speed_ipc(fan_speed_t value);
void update_device_mode_ipc(thermostat_mode_t value);
void update_target_temp_ipc(int temperature);
void update_current_temp_ipc(int temperature);
void update_brightness_ipc(uint8_t level);
void update_remainig_time_ipc(uint32_t time_s);
void update_temperature_data_ipc(device_state_t *data);
void update_audio_level_ipc(uint16_t level);
void update_ble_adv_ipc(device_connection_state_t state);
void update_wifi_cred_ipc(const char *ssid, const char *passwd);
void request_uid_ipc(void);
void request_wifi_delete_ipc(void);
void send_device_config(device_state_t config);


#endif /* COMM_MANAGER_H_ */
