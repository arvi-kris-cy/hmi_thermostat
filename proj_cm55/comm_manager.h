<<<<<<< HEAD
/*
 * communication.h
 *
 */
=======
/*******************************************************************************
 * File Name:   comm_manager.h
 *
 * Description:  Public interface for updating thermostat events from
 *               CM55 core to CM33 core using IPC.
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
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97

#ifndef COMM_MANAGER_H_
#define COMM_MANAGER_H_

<<<<<<< HEAD
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
=======
/*******************************************************************************
 *                                INCLUDES
 *******************************************************************************/
#include "app_common.h"


/*******************************************************************************
 *                                MACROS
 *******************************************************************************/

/*******************************************************************************
 *                                CONSTANTS
 *******************************************************************************/

/*******************************************************************************
 *                                DATA TYPES
 *******************************************************************************/

/*******************************************************************************
 *                                GLOBAL VARIABLES
 *******************************************************************************/


/*******************************************************************************
 *                                FUNCTION PROTOTYPES
 *******************************************************************************/

/**
 * @brief Sends an IPC message to update the fan speed.
 *
 * @param value Fan speed value to set.
 */
void update_fan_speed_ipc(fan_speed_t value);

/**
 * @brief Sends an IPC message to update the device thermostat mode.
 *
 * @param value Thermostat mode to set.
 */
void update_device_mode_ipc(thermostat_mode_t value);

/**
 * @brief Sends an IPC message to update the current temperature.
 *
 * @param temperature Current temperature value.
 */
void update_current_temp_ipc(int temperature);

/**
 * @brief Sends an IPC message to update the display brightness.
 *
 * @param level Brightness level to set (0-100%).
 */
void update_brightness_ipc(uint8_t level);

/**
 * @brief Sends an IPC message to update the target temperature.
 *
 * @param temperature Target temperature value.
 */
void update_target_temp_ipc(int temperature);

/**
 * @brief Sends an IPC message to update the remaining time for operation.
 *
 * @param time_s Remaining time in seconds.
 */
void update_remainig_time_ipc(uint32_t time_s);

/**
 * @brief Sends an IPC message containing the complete temperature-related device state.
 *
 * @param data Pointer to the device state structure containing temperature data.
 */
void update_temperature_data_ipc(device_state_t *data);

/**
 * @brief Sends an IPC message to update the system temperature unit.
 *
 * @param unit Temperature unit (e.g., Celsius or Fahrenheit).
 */
void update_system_unit_ipc(temp_unit_t unit);

/**
 * @brief Sends an IPC message to update the audio level.
 *
 * @param level Audio level value.
 */
void update_audio_level_ipc(uint16_t level);

/**
 * @brief Sends an IPC message to update the BLE advertising or connection state.
 *
 * @param state Device connection state for BLE.
 */
void update_ble_adv_ipc(device_connection_state_t state);

/**
 * @brief Sends an IPC message to update Wi-Fi credentials.
 *
 * @param ssid   Pointer to the SSID string.
 * @param passwd Pointer to the password string.
 */
void update_wifi_cred_ipc(const char *ssid, const char *passwd);

/**
 * @brief Sends an IPC message to switch the device to Wi-Fi mode.
 */
void update_switch_wifi_ipc(void);

/**
 * @brief Sends an IPC request to get the unique device ID.
 */
void request_uid_ipc(void);

/**
 * @brief Sends an IPC request to delete the saved Wi-Fi credentials.
 */
void request_wifi_delete_ipc(void);

/**
 * @brief Sends an IPC message to update the number of connection retries.
 *
 * @param state Connection retry state (limited or infinite retries).
 */
void update_connection_retries_ipc(connection_retries_t state);

/**
 * @brief Sends an IPC message to update the device connection state.
 *
 * This function updates the current connection state of the device by
 * sending an IPC command to the CM33 core.
 *
 * @param state The new device connection state to set.
 */
void update_connection_state_ipc(device_connection_state_t state);

/**
 * @brief Sends an IPC message to update the entire device configuration.
 *
 * @param config The complete device configuration structure.
 */
void send_device_config(device_state_t config);

void send_switch_to_ble_cmd(void);

void trigger_ota_update(void);

/**
 * @brief Get Latest Firmware update version
 */
void get_latest_OTA_version(void);

#endif /* COMM_MANAGER_H_ */

/* [] END OF FILE */
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
