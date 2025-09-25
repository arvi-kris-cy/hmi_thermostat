/*
 * communication.h
 *
 *  Created on: 20-Jun-2025
 *      Author: Adnan.Shaikh
 */

#ifndef COMM_MANAGER_H_
#define COMM_MANAGER_H_

#include "app_common.h"

void set_fan_speed(fan_speed_t value);
void set_device_mode(thermostat_mode_t value);
void set_device_temp(uint8_t value);
void set_device_brightness(uint8_t level);
void set_device_audio_level(uint8_t level);
void display_ble_pin(uint32_t pin);
void update_conn_state(uint32_t state);
void response_uid_req(char *uid);
void set_device_brightness(uint8_t value);
void set_device_audio(audio_level_t value);
void set_device_temp_unit(temp_unit_t value);
void set_device_co2_level(uint16_t value);
void update_presence_detection_status(presence_status_t status);
void update_provision_state(bool state);
void set_datetime_ui(DateTime *timestamp);



#endif /* COMM_MANAGER_H_ */
