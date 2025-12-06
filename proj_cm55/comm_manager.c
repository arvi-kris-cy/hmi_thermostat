<<<<<<< HEAD
#include "comm_manager.h"
#include "ipc_communication.h"

CY_SECTION_SHAREDMEM static ipc_msg_t cm55_msg_data;

=======
/*******************************************************************************
 * File Name:  comm_manager.c
 *
 * Description:
 *
 * This file implements the functions to send thermostat data over IPC
 * from CM55 core to CM33 core.
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
 *                                INCLUDES
 ******************************************************************************/
#include "comm_manager.h"
#include "ipc_communication.h"

/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/

/*******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/

/*******************************************************************************
 *                             STATIC VARIABLES
 ******************************************************************************/
CY_SECTION_SHAREDMEM static ipc_msg_t cm55_msg_data;

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
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

<<<<<<< HEAD
void update_system_unit_ipc(system_unit_t unit)
=======
void update_system_unit_ipc(temp_unit_t unit)
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_SET_TEMP_UNIT;
	cm55_msg_data.data = unit;

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

<<<<<<< HEAD

	cm55_send_msg_cm33(&cm55_msg_data);
}

=======
	cm55_send_msg_cm33(&cm55_msg_data);
}

void update_switch_wifi_ipc(void)
{
    cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
    cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
    cm55_msg_data.cmd = IPC_CMD_UPDATE_CONN_STATE;
    cm55_msg_data.data = DEV_ST_SWITCH_WIFI;
    cm55_send_msg_cm33(&cm55_msg_data);
}

>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
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

<<<<<<< HEAD
=======
void update_connection_retries_ipc(connection_retries_t state)
{
    cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
    cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
    cm55_msg_data.cmd = IPC_CMD_UPDATE_CONN_RETRIES;
    cm55_msg_data.data = state;

    cm55_send_msg_cm33(&cm55_msg_data);
}

void update_connection_state_ipc(device_connection_state_t state)
{
    cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
    cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
    cm55_msg_data.cmd = IPC_CMD_UPDATE_CONN_STATE;
    cm55_msg_data.data = state;

    cm55_send_msg_cm33(&cm55_msg_data);
}
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97

void send_device_config(device_state_t config)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_DEVICE_CONFIG;
	cm55_msg_data.data = 0;
	cm55_msg_data.device_config = config;

	cm55_send_msg_cm33(&cm55_msg_data);
}
<<<<<<< HEAD
=======


void send_switch_to_ble_cmd(void)
{
    cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
    cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
    cm55_msg_data.cmd = IPC_CMD_SWITCH_TO_BLE;
    cm55_msg_data.data = 0;

    cm55_send_msg_cm33(&cm55_msg_data);
}

void trigger_ota_update(void)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_TRIGGER_OTA_START;
	cm55_msg_data.data = 0;

    cm55_send_msg_cm33(&cm55_msg_data);
}

void get_latest_OTA_version(void)
{
	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = IPC_CMD_OTA_VERSION;
	cm55_msg_data.data = 0;

    cm55_send_msg_cm33(&cm55_msg_data);
}
/* [] END OF FILE */
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
