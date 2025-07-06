/*
 * app_common.h
 *
 *  Created on: 06-Jun-2025
 *      Author: Tejas.Patel */

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ipc_communication.h"

#ifndef SHARED_INCLUDE_APP_COMMON_H_
#define SHARED_INCLUDE_APP_COMMON_H_

/*******************************************************************************
 *                                Data Types
 *******************************************************************************/
/* Function pointer for event handler */
typedef void (*EventHandler)(void*);

/* CM55 to CM33 */
typedef enum {
	CMD_CM55_UI_CONTROL,
	CMD_CM55_VOICE
} notify_type_t;

/* CM33 to CM55 */
typedef enum {
	DEVICE_STARTED_BLE_ADV,
	DEVICE_WIFI_CONN_SATRTED,
	DEVICE_WIFI_CONNECTED,
	DEVICE_MQTT_CONNECTED,
} device_status_t;

/* Event Notifier types  */
typedef enum {
    EVT_USER_BUTTON,
	EVT_UI_UPDATE,
    EVT_BLE_WIFI_CTRL,
    EVT_BLE_START_ADV,
    EVT_WIFI_CONNECT,

    NOTIF_UNKNOWN
} event_type_t;

/* CMDs evts from CM33 to CM55 */
typedef enum {
	UI_UPDATE_BLE_ADV_START,
	UI_UPDATE_BLE_CONNECTED,
	UI_UPDATE_WIFI_CONNECTING,
	UI_UPDATE_WIFI_CONNECTED,
	UI_UPDATE_WIFI_DISCONNECTED,
	UI_UPDATE_CLOUD_CONNECTED,
} ui_update_evt_t;

typedef enum {
    FAN_OFF = 0,
    FAN_HIGH,
    FAN_MED,
    FAN_LOW,
	FAN_MODE_MAX
}fan_speed_t;


typedef enum {
    MODE_ECO = 0,
    MODE_RAPID,
    MODE_AUTO,
	MODE_OFF,
	MODE_MAX
} thermostat_mode_t;

typedef enum {
    // BLE Events
    EVENT_BLE_ADVERTISING = 0,
    EVENT_BLE_SCANNING,
    EVENT_BLE_PAIRING_PIN_REQUEST,
    EVENT_BLE_PAIRED,

    // Wi-Fi Events
    EVENT_WIFI_SCANNING,
    EVENT_WIFI_CONNECTING,
    EVENT_WIFI_CONNECTED,

    // Cloud Events
    EVENT_CLOUD_CONNECTING,
    EVENT_CLOUD_CONNECTED,

    EVENT_MAX
} app_event_id_e;

typedef enum {
    DEV_ST_CLOUD_CONNECTED,
    DEV_ST_CLOUD_CONNECTING,
    DEV_ST_WIFI_CONNECTING,
    DEV_ST_WIFI_DISCONNECTED,
    DEV_ST_CLOUD_DISCONNECTED,
    DEV_ST_BLE_ADVERTISING,
	DEV_ST_BLE_PAIRING,
    DEV_ST_UNPROVISIONED,
	DEV_ST_PROVISIONED,
    DEV_ST_WIFI_CONNECTED,
    DEV_ST_BLE_CONNECTED
} device_connection_state_t;

typedef enum {
    IPC_CMD_CURRENT_EVENT = 0,              //Send a current event to UI

	IPC_CMD_GET_UID,						//Request UID
	IPC_CMD_SET_UID,						//Response of UID

	IPC_CMD_UPDATE_CONN_STATE,

    // Wi-Fi
    IPC_CMD_SET_WIFI_SSID,                  // Set SSID string
    IPC_CMD_SET_WIFI_PASSWORD,              // Set Wi-Fi password
	IPC_CMD_SET_WIFI_SSID_PASS,              // Set Wi-Fi password
	IPC_CMD_RESET_WIFI_SSID_PASS,              // Erase Wi-Fi password


    // Environmental data
    IPC_CMD_GET_CURRENT_TEMP,                // Get current temperature
    IPC_CMD_GET_CURRENT_HUMIDITY,           // Get current humidity
    IPC_CMD_GET_CURRENT_CO2_LEVEL,          // Get current CO2 reading
    IPC_CMD_SET_TARGET_TEMP,                // Set user-defined target temperature
	IPC_CMD_SET_CURRENT_TEMP,                // Set current temperature

	//TIME
	IPC_CMD_SET_REMAINING_TIME,               // Set remainig time to reach target temp.

    // HVAC settings
    IPC_CMD_SET_FAN_SPEED,                  // Set fan speed level
    IPC_CMD_GET_FAN_SPEED,                  // Get fan speed level
    IPC_CMD_SET_THERMOSTAT_MODE,            // Set operating mode
    IPC_CMD_GET_THERMOSTAT_MODE,            // Get operating mode

    // User preferences
    IPC_CMD_SET_DISPLAY_BRIGHTNESS,         // Set brightness level
	IPC_CMD_GET_DISPLAY_BRIGHTNESS,         // Get brightness level
    IPC_CMD_SET_AUDIO_LEVEL,                // Set audio output level
    IPC_CMD_GET_AUDIO_LEVEL,                // Get audio output level

    // OTA
    IPC_CMD_TRIGGER_OTA_START,              // Start OTA update
    IPC_CMD_OTA_PROGRESS,               	// Query OTA progress
    IPC_CMD_OTA_STATUS,                  	// Query OTA active status (in progress or not)

    IPC_CMD_MAX
} ipc_command_e;

/* Structure to hold WiFi details */
typedef struct {
    char ssid[33];     // SSID max length 32 + null
    char password[65]; // WPA2 max length 64 + null
} wifi_credentials_t;

/* Structure to hold BLE Pairing code details */
typedef struct {
    char pair_code[7];     // BLE Pairing code (6+1)
} ble_pairing_code_t;

/* Event structure */
typedef struct {
    event_type_t type;
    EventHandler Handler;
    uint8_t data;

    union {
    	wifi_credentials_t	wifi_info;
    	ble_pairing_code_t	pairing_code;
    };

} app_event_t;

typedef struct {
    uint32_t current_temp;
    uint32_t current_humidity;
    uint32_t current_co2_level;
    uint32_t target_temp;
} environmental_data_t;

typedef struct {
	uint32_t time_remains;
	fan_speed_t fan_speed;
    thermostat_mode_t mode;
} thermostate_settings_t;

typedef struct {
    uint8_t display_brightness;
    uint8_t audio_level;
} user_preferences_t;

typedef struct {
    bool ota_available;
    uint8_t ota_progress;
    bool ota_final_status;
} ota_status_t;

typedef struct {
    wifi_credentials_t wifi;
    environmental_data_t environment;
    thermostate_settings_t thermostat_settings;
    user_preferences_t preferences;
    ota_status_t ota;
} device_state_t;


#endif /* SHARED_INCLUDE_APP_COMMON_H_ */
