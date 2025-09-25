/*******************************************************************************
 * File Name:   thermostat_events.h
 *
 * Description:  Public interface for updating thermostat events on UI.
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

#ifndef THERMOSTAT_EVENTS_H
#define THERMOSTAT_EVENTS_H

/*******************************************************************************
 *                                INCLUDES
 *******************************************************************************/
#include "ui/ui_events.h"
#include "ui/ui.h"
#include "display-tft-st7701s/mtb_display_st7701s.h"
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
typedef enum {
    STATE_NONE,
    STATE_HEATING,
    STATE_COOLING
}animation_state_t;

typedef enum {
    MIC_DISABLED,
    MIC_IDLE,
    MIC_ACTIVE
}mic_state_t;

typedef enum {
    NOTIFY_NETWORK_STATUS,
    NOTIFY_TEMP_UPDATE,
    NOTIFY_MODE_UPDATE,
    NOTIFY_FAN_MODE_UPDATE,
} notification_type;

typedef enum {
    NOTIF_SUCCESS,
    NOTIF_FAIL,
} notification_status_t;

typedef enum {
    PROV_MAPP_BLE = 1,
    PROV_UI_KEYBOARD
} provisioning_method_t;

typedef struct {
    notification_type type;
    notification_status_t status;
    uint32_t value;
} notification_data_t;

/*******************************************************************************
 *                                GLOBAL VARIABLES
 *******************************************************************************/
extern lv_timer_t *temp_timer;
extern uint8_t brightness_level;
extern uint8_t audio_level;
extern char device_unique_id[13];
extern device_settings_t current_settings;
extern volatile application_state_t app_state;
extern volatile bool popup_overlay_visible;
extern volatile bool is_device_connected;
extern device_state_t dev_info;
extern volatile bool hide_conn_screen;
extern bool is_mic_clicked;
/*******************************************************************************
 *                                FUNCTION PROTOTYPES
 *******************************************************************************/
void display_mic_state(void);
void update_mic_state(mic_state_t state);
void hide_presence_icon(void);
void update_presence_detection(uint8_t presence_count);
void mic_icon_click_handler(lv_event_t * e);

/**
 * @brief Starts BLE advertising for provisioning.
 *
 * Function updates the user interface to indicate the advertising state,
 * and sends a command to the CM33 core to begin BLE advertising using
 * IPC messaging.
 *
 * @param e Pointer to the LVGL event that triggered this function.
 *
 * @return void
 */
void start_ble_adv(lv_event_t *e);

/**
 * @brief Starts the Wi-Fi connection process using UI-provided credentials.
 *
 * This function initiates the Wi-Fi provisioning process using the
 * SSID and password entered by the user in the UI text fields.
 *
 * @param e Pointer to the LVGL event that triggered the function (unused).
 *
 * @return void
 */
void connect_wifi(lv_event_t * e);

/**
 * @brief Event handler to update Wi-Fi credentials.
 *
 * This function is triggered by a UI event (e.g., button click) to initiate
 * a Wi-Fi connection update.
 *
 * @param e Pointer to the LVGL event structure (unused).
 */
void update_wifi_cred(lv_event_t *e);

/**
 * @brief Updates the device connection state and corresponding UI elements.
 *
 * This function manages and updates the UI to reflect the current device
 * connection status
 *
 * @param state The current device connection state
 *
 * @return void
 */
void update_device_connection_state(uint8_t state);

/**
 * @brief Handles thermostat mode toggle from the UI event.
 *
 * Cycles through the available thermostat modes.
 *
 * @param e Event data from the UI (unused).
 */
void toggle_mode(lv_event_t * e);

/**
 * @brief Updates the thermostat mode and corresponding UI elements.
 *
 * This function changes the current operating mode of the thermostat
 * (ECO, RAPID, AUTO, or OFF), and updates the mode-related UI components.
 *
 * @param mode The new thermostat mode to be set
 *
 * @return void
 */
void update_thermostat_mode(thermostat_mode_t mode);

/**
 * @brief Updates the target temperature and modifies the UI based on the
 *  new temperature setting.
 *
 * This function updates the target temperature for the thermostat. It adjusts
 * UI components such as temperature labels, animations, and state indicators
 * based on whether the device should heat, cool, or remain idle to reach the
 * target temperature.
 *
 * @param temp The new target temperature to set
 *
 * @return void
 */
void update_device_temp(uint8_t temp);

void display_ble_pairing_window(bool hide, char *code);

/**
 * @brief Updates the thermostat mode and reflects changes in the UI and system state.
 *
 * This function handles the logic for switching between different thermostat modes
 * (ECO, RAPID, AUTO, OFF).
 *
 * @param mode The thermostat mode to set
 */
void set_thermostat_mode(thermostat_mode_t mode);

/**
 * @brief Gets the current fan mode.
 *
 * @return Current fan mode.
 */
fan_speed_t get_current_fan_mode(void);

/**
 * @brief Update the fan mode
 *
 * This function sets the fan animation, updates the fan mode label and
 * sends the updated mode to the CM33 core via IPC.
 *
 * @param mode Fan speed mode to set (e.g., FAN_LOW, FAN_MED, FAN_HIGH, FAN_OFF).
 */
void update_fan_mode(fan_speed_t mode);

/**
 * @brief Handles fan button click event.
 *
 * Cycles through available fan modes if the device is not in OFF mode.
 *
 * @param e Pointer to the LVGL event (unused).
 */
void fan_clicked(lv_event_t * e);

/**
 * @brief Gets the current thermostat mode.
 *
 * @return Current thermostat mode.
 */
thermostat_mode_t get_current_device_mode(void);

/**
 * @brief Gets the current display brightness level.
 *
 * @return Brightness level (0–100).
 */
uint8_t get_current_brigthness(void);

/**
 * @brief Gets the current temperature.
 *
 * @return Current temperature value.
 */
int get_current_temperature(void);

/**
 * @brief Updates the notification label and image based on the operation result.
 *
 * This function sets the notification text and image to indicate device operation
 *  (e.g., temperature update, mode change, or network status)succeeded or failed.
 *
 * @param[in] type   The type of notification (e.g., network status, temperature update).
 * @param[in] status The status of the operation (success or failure).
 * @param[in] value  The value associated with the operation result.
 *                   - For NOTIFY_NETWORK_STATUS, this represents the connection state.
 *                   - For NOTIFY_TEMP_UPDATE, this represents the temperature value.
 *                   - For NOTIFY_MODE_UPDATE and NOTIFY_FAN_MODE_UPDATE, this may be mapped to mode text externally.
 */
void update_notifcation_label(notification_type type, notification_status_t status, uint32_t value);

/**
 * @brief Loads and applies thermostat configuration to the UI.
 *
 * This function updates the UI elements based on the current temperature unit
 * (Celsius or Fahrenheit), applies the specified thermostat mode, and restores
 * saved settings for fan mode, volume, brightness, and idle timeout.
 *
 * @param mode Thermostat mode to be applied
 */
void load_thermostat_config(thermostat_mode_t mode);

/**
 * @brief Handles the idle timeout change event from the UI.
 *
 * Reads the selected timeout value from the dropdown UI element,
 * updates the system idle timeout.
 *
 * @param e Pointer to the LVGL event structure (unused).
 */
void change_idle_timeout(lv_event_t * e);

/**
 * @brief Handles the event to update the target temperature from the UI.
 *
 * This function reads the temperature value from the temperature arc widget,
 * and updates the device's target temperature accordingly.
 *
 * @param e Pointer to the LVGL event structure (unused).
 */
void update_temperature(lv_event_t * e);

void update_final_temperature(lv_event_t *e);

/**
 * @brief Updates the temperature adjustment timer based on thermostat mode.
 *
 * This function starts a timer to gradually adjust the temperature
 * by calling either the increase or decrease step function.
 */
void update_thermostat_mode_timer(void);

/**
 * @brief Sets the system idle timeout duration.
 *
 * Updates the device's idle timeout based on the selected option.
 *
 * @param time The selected idle timeout
 */
void set_idle_timeout(idle_timeout_t time);

/**
 * @brief Handles the volume level change triggered from the UI drop-down event.
 *
 * This function reads the selected volume level from the audio level drop-down,
 * updates the system volume accordingly, and sends the updated audio level to
 * the CM33 core via IPC.
 *
 * @param e Pointer to the LVGL event object (unused).
 */
void change_volume(lv_event_t * e);

/**
 * @brief Updates the thermostat volume level.
 *
 * This function sets the system volume to the specified level, updates the
 * UI drop-down to reflect this level.
 *
 * @param level The new volume level to be set.
 */
void update_thermostat_volume(audio_level_t level);

/**
 * @brief Handles system temperature unit change (°C ↔ °F) triggered by UI toggle.
 *
 * This function is invoked when the user toggles the temperature unit switch in the UI.
 * It updates the system's temperature unit, performs value conversions if needed,
 * refreshes all related UI labels and arc range, and updates the current device
 * settings and configuration via IPC.
 *
 * @param e Pointer to the LVGL event (unused).
 */
void set_system_unit(lv_event_t * e);

/**
 * @brief Update the system temperature unit and refresh UI elements accordingly.
 *
 * This function updates the device's temperature unit to either Celsius or Fahrenheit.
 * It converts all related temperature values and updates the UI labels, arc range,
 * and the internal device structure. It also stores the updated setting in NVM.
 *
 * @param unit The system unit to be applied.
 */
void update_system_unit(temp_unit_t unit);

/**
 * @brief Retrieves the default device settings.
 *
 * This function initializes the provided settings structure with
 * the default device configuration values.
 *
 * @param[out] settings Pointer to a device_settings_t structure
 *               to store the default settings.
 */
void get_default_device_setting(device_settings_t *settings);

/**
 * @brief Retrieves the current device settings.
 *
 * This function copies the current runtime device settings into the
 * provided structure.
 *
 * @param[out] settings Pointer to a device_settings_t structure to
 *              store the current settings.
 */
void get_current_device_setting(device_settings_t *settings);

/**
 * @brief Updates the current device settings.
 *
 * This function replaces the existing current device settings with those
 * provided by the user.
 *
 * @param[in] settings Pointer to a device_settings_t structure
 *              containing new settings.
 */
void set_current_device_setting(device_settings_t *settings);

/**
 * @brief Flags the device to write the current settings to EEPROM.
 *
 * This function sets a flag indicating that the current device settings
 * should be written to non-volatile storage.
 */
void update_current_device_setting(void);

/**
 * @brief Performs a factory reset of the device.
 *
 * This function restores all device settings to their default values,
 * and updates the UI accordingly
 *
 * @param e Event pointer (unused).
 */
void device_factory_reset(lv_event_t * e);

/**
 * @brief Sends the current device configuration to the CM33 core.
 *
 * This function captures a snapshot of the current device settings and
 * sends it to CM33 core over IPC.
 */
void update_device_config_ipc(void);

/**
 * @brief Starts the inactivity timer based on the current idle timeout setting.
 *
 * This function starts the active state timer with a duration derived from
 * the configured timeout value.
 */
void start_inactivity_timer(void);

/**
 * @brief Handles cancel device connection event and updates the device state.
 *
 * This function is triggered when a cancel button event is received.
 *
 * @param e Pointer to the LVGL event structure.
 */
void cancel_dev_conn_current_opt(lv_event_t *e);

/**
 * @brief Handles the auto-reconnect switch event to set connection retries.
 *
 * This function is triggered when the auto-reconnect switch value changes.
 * If the switch is ON, it sets infinite connection retries for Wi-Fi or cloud
 * based on the current connection state. If the switch is OFF, it sets limited
 * connection retries instead.
 *
 * @param e Pointer to the LVGL event structure.
 */
void auto_reconnect_cb(lv_event_t *e);

/**
 * @brief Switches the device connection to Wi-Fi.
 *
 * This function updates the device state to Wi-Fi connecting.
 *
 * @param e Pointer to the LVGL event structure.
 */
void switch_to_wifi(lv_event_t *e);

/**
 * @brief Updates the CO2 PPM value on the UI label.
 *
 * This function updates the CO2 value on UI.
 *
 * @param ppm The CO2 concentration in parts per million.
 */
void update_co2_data_ui(uint16_t ppm);

void switch_to_active_screen(void);

void switch_to_ble(lv_event_t *e);

/**
 * @brief Increments the hour on the UI.
 *
 * This function is an event handler for a button. It increments the static
 * hour variable, wrapping around from 23 to 0, and then calls the
 * update_time_labels helper function to refresh the UI.
 *
 * @param e The LVGL event that triggered the function.
 */
void inc_thour(lv_event_t * e);

/**
 * @brief Increments the minute on the UI.
 *
 * This function is an event handler for a button. It increments the static
 * minute variable, wrapping around from 59 to 0, and then calls the
 * update_time_labels helper function to refresh the UI.
 *
 * @param e The LVGL event that triggered the function.
 */
void inc_tmin(lv_event_t * e);

/**
 * @brief Increments the second on the UI.
 *
 * This function is an event handler for a button. It increments the static
 * second variable, wrapping around from 59 to 0, and then calls the
 * update_time_labels helper function to refresh the UI.
 *
 * @param e The LVGL event that triggered the function.
 */
void inc_tsec(lv_event_t * e);

/**
 * @brief Decrements the hour on the UI.
 *
 * This function is an event handler for a button. It decrements the static
 * hour variable, wrapping around from 0 to 23, and then calls the
 * update_time_labels helper function to refresh the UI.
 *
 * @param e The LVGL event that triggered the function.
 */
void dec_thour(lv_event_t * e);

/**
 * @brief Decrements the minute on the UI.
 *
 * This function is an event handler for a button. It decrements the static
 * minute variable, wrapping around from 0 to 59, and then calls the
 * update_time_labels helper function to refresh the UI.
 *
 * @param e The LVGL event that triggered the function.
 */
void dec_tmin(lv_event_t * e);

/**
 * @brief Decrements the second on the UI.
 *
 * This function is an event handler for a button. It decrements the static
 * second variable, wrapping around from 0 to 59, and then calls the
 * update_time_labels helper function to refresh the UI.
 *
 * @param e The LVGL event that triggered the function.
 */
void dec_tsec(lv_event_t * e);

/**
 * @brief Reads the date and time from the UI and updates the RTC.
 *
 * This function is an event callback for a button (e.g., "Set Time"). It reads
 * the date from the calendar widget and the time from the LVGL labels. It
 * then calculates the day of the week, validates the input, and updates
 * the hardware RTC with a retry mechanism.
 *
 * @param e The LVGL event that triggered the function.
 */
void update_date_time_rtc(lv_event_t * e);

/**
 * @brief Updates a text area with the selected date from a calendar.
 *
 * This function is an event handler that reads a valid date from the calendar
 * widget. If a date is selected, it formats the date into a "DD/MM/YYYY" string
 * and sets the text of the specified LVGL text area.
 *
 * @param e The LVGL event that triggered the function.
 */
void update_calendar_date(lv_event_t * e);

void check_fw_update_version(lv_event_t * e);

void update_fw_download_status(uint8_t percent);

void trigger_ota(lv_event_t * e);

/**
 * @brief Starts the fan animation based on the current fan mode.
 *
 * This function calls a separate helper to update the fan animation,
 * which is responsible for displaying the correct visual feedback
 * (e.g., low, medium, high speed fan icon).
 *
 * @param None
 */
void display_fan_anim(void);

/**
 * @brief Stops all fan animations and hides them.
 *
 * This function is used to reset the fan animation state. It deletes
 * any running animations for all fan speed states and hides the
 * associated UI objects before showing the static fan image.
 *
 * @param None
 */
void stop_fan_anim(void);

/**
 * @brief Updates the UI to reflect the current connectivity state.
 *
 * This function is responsible for calling the main state handler
 * to update the icons and labels on the home screen based on the
 * device's current connection status.
 *
 * @param None
 */
void update_homescreen_connectivity_state(void);

/**
 * @brief Stops all connectivity animations and resets the UI state.
 *
 * This function hides all possible connectivity-related UI elements
 * (images, buttons, labels) and deletes any running animations. It
 * then selectively displays the correct UI based on the new
 * connectivity state.
 *
 * @param None
 */
void stop_homescreen_connectivity_state(void);

/**
 * @brief Displays the temperature change animation.
 *
 * This function reveals the heating (red) or cooling (blue) animation
 * container depending on the current temperature state.
 *
 * @param None
 */
void display_temp_change_anim(void);

/**
 * @brief Hides the temperature change animation.
 *
 * This function hides the heating (red) or cooling (blue) animation
 * container depending on the current temperature state.
 *
 * @param None
 */
void stop_temp_change_anim(void);

/**
 * @brief Initialize the UI one-shot timer.
 *
 * Creates a one-shot FreeRTOS software timer with a period of 2000 ms
 * (2 seconds). When the timer expires, the LVGL timer callback is executed.
 */
void ui_timer_init(void);

/**
 * @brief Stop the UI timer if it is running.
 */
void ui_timer_stop(void);

/**
 * @brief Start the UI timer.
 *
 * If the timer is already expired, this restarts it.
 */
void ui_timer_start(void);

/**
 * @brief Hide the connectivity popup overlay when the device is connected.
 *
 * This function hides the popup overlay if the device is either BLE
 * connected or cloud connected. It also triggers animations and UI
 * state updates (fan, temperature, microphone, and presence detection).
 */
void hide_connectivity_screen(void);


#endif /* THERMOSTAT_EVENTS_H */

/* [] END OF FILE */
