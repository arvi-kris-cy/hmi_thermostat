/*******************************************************************************
 * File Name:  thermostat_events.c
 *
 * Description:
 *
 * This file implements the Emulated EEPROM initialization, write
 * and read operation.
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
#include "ui/ui.h"
#include "ui/ui_events.h"
#include "thermostat_events.h"
#include "ipc_communication.h"
#include "app_main.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "qr_manager.h"
#include "lv_qrcode.h"
#include "comm_manager.h"
#include "ui/screens/ui_ActiveScreen.h"
#include "app_eeprom.h"
#include "app_sensor.h"
#include "app_speaker.h"
#include "app_rtc.h"

/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/
#define CM55_APP_DELAY_MS           (50U)
#define ECO_MODE_TEMP_TIMER_TIMEOUT     5000U
#define RAPID_MODE_TEMP_TIMER_TIMEOUT       1000U
#define AUTO_MODE_TEMP_TIMER_TIMEOUT        2500U
#define NOTIFICATION_TIMEOUT_MS     1500U

// Timeout in milliseconds (e.g., 2000ms = 2 seconds)
#define WIFI_ILABEL_TIMER_TIMEOUT_MS 2000

#define TEMPERATURE_DEG_C_MAX_VALUE 30
#define TEMPERATURE_DEG_C_MIN_VALUE 14

#define TEMPERATURE_DEG_F_MAX_VALUE  ((30 * 9 / 5) + 32)  // 86°F
#define TEMPERATURE_DEG_F_MIN_VALUE  ((14 * 9 / 5) + 32)  // 57.2°F ≈ 57°F

#define TEMP_CONVERSION_CORRECTION_FACTOR 0.5

#define CELSIUS_TO_FAHRENHEIT(c)  ((int)((((float)(c) * 9.0 / 5.0) + 32.0) + TEMP_CONVERSION_CORRECTION_FACTOR))
#define FAHRENHEIT_TO_CELSIUS(f)  ((int)((((float)(f) - 32.0) * 5.0 / 9.0) + TEMP_CONVERSION_CORRECTION_FACTOR))

#define AUTO_MODE_TARGET_TEMP_DEFAULT_C 22
#define AUTO_MODE_TARGET_TEMP_DEFAULT_F CELSIUS_TO_FAHRENHEIT(AUTO_MODE_TARGET_TEMP_DEFAULT_C)

/* Notification related */
#define MAX_NOTIF_QUEUE     5U

/* Temperature arc angle */
#define TEMPERATURE_ARC_START_ANGLE 120U
#define TEMPERATURE_ARC_END_ANGLE   420U
#define TEMPERATURE_ARC_ANGLE_PER_STEP 18U

/* CO2 level thresholds */
#define CO2_LEVEL_THRESHOLD_1_GOOD	    1000U
#define CO2_LEVEL_THRESHOLD_2_MODERATE	2000U
#define CO2_LEVEL_THRESHOLD_3_POOR	    5000U


/*******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/
bool is_mic_clicked = false;
volatile bool popup_overlay_visible = false;
volatile application_state_t app_state = APP_ST_ACTIVE;
device_state_t dev_info;

uint8_t current_max_temp = TEMPERATURE_DEG_C_MAX_VALUE;
uint8_t current_min_temp = TEMPERATURE_DEG_C_MIN_VALUE;
char subinfo[64] = { 0 };

char device_unique_id[13] = { 0 };
lv_timer_t *temp_timer = NULL;
lv_timer_t *start_listening_timer = NULL;
lv_timer_t *stop_listening_timer = NULL;
lv_timer_t *state_update_timer = NULL;

device_settings_t current_settings = { 0 };
bool is_device_provisioned = false;

char new_FW_version[MAX_FW_VERSION_LEN];
char m55_current_OTA_version[MAX_FW_VERSION_LEN];

/* Device connection state flag in CM55 core */
volatile bool is_device_connected = false;

/* Flag to check if the screen need to be hidden */
volatile bool hide_conn_screen = false;

static lv_timer_t *app_timer = NULL;
void start_active_state_timer(uint32_t timeout_ms);
void stop_active_state_timer(void);
volatile bool wifi_popup_state = false;
bool voice_popup_state = false;
bool fw_update_timeout = false;
bool is_new_fw_available = false;

/*******************************************************************************
 *                             STATIC VARIABLES
 ******************************************************************************/

/* Default thermostat configuration */
static device_settings_t default_config =
{
        .audio.level = AUDIO_MED,
        .display_setting.brightness = 100,
        .thermostat_setting.fan_mode = FAN_LOW,
        .thermostat_setting.mode = MODE_ECO,
        .system.idle_timeout = TIMEOUT_10S,
        .system.temperature_unit = TEMP_UNIT_CELSIUS,
};

static int temperature = 24;
static int current_temp = 24;
static int target_temp = 24;
static mic_state_t current_mic_state = MIC_DISABLED;

/* Current thermostat mode */
static thermostat_mode_t dev_current_mode = MODE_ECO;

/* Current fan mode */
static fan_speed_t dev_fan_mode = FAN_OFF;

/* Current selected temperature unit */
static temp_unit_t dev_unit = TEMP_UNIT_CELSIUS;

static volatile uint32_t deg2sec = ECO_MODE_TEMP_TIMER_TIMEOUT; /**/

/* Timer handle */
static TimerHandle_t wifi_ilabel_timer = NULL;
static TimerHandle_t wifi_kybd_label_timer = NULL;
static TimerHandle_t fw_update_check_timer = NULL;

static provisioning_method_t prov_method = PROV_MAPP_BLE;
static animation_state_t current_state = STATE_NONE;

static bool person_detected = false;
static uint8_t person_count = 0;
static int weather_temp = 11;
static uint32_t current_test_state_idx = 0;
static qr_manager_t qr_manager;

static notification_data_t notif_queue[MAX_NOTIF_QUEUE];
static int notif_head = 0;
static int notif_tail = 0;
static bool notif_showing = false;
static lv_timer_t *notif_timer = NULL;

static int hour = 0;
static int minute = 0;
static int second = 0;
int angle;
/* Device current and previous connection state */
device_connection_state_t dev_current_conn_state = DEV_ST_UNPROVISIONED;
device_connection_state_t dev_last_conn_state = DEV_ST_UNPROVISIONED;

/* Notification audio type */
static audio_type_t notify_audio_type = AUDIO_ERROR;


static TimerHandle_t lvgl_timer;

static volatile bool ble_conn_state = false;
static volatile bool wifi_conn_state = false;
static volatile bool cloud_conn_state = false;

uint8_t brightness_level = 100;
audio_level_t audio_level = AUDIO_MED;

extern char weather_sync_value[32];

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/

/**
 * @brief Get delay (in seconds) per degree of temperature change based on
 *          the thermostat mode.
 *
 * @param mode Thermostat mode (e.g., MODE_ECO, MODE_RAPID, MODE_AUTO).
 * @return Delay in seconds per degree. Returns -1 for unsupported modes.
 */
static int get_deg_delay_sec(thermostat_mode_t mode);

/**
 * @brief Calculate the total estimated time (in seconds) required to reach
 *        the set temperature.
 *
 * @param mode Thermostat mode in which the delay is calculated.
 * @param current_temp Current measured temperature.
 * @param set_temp Desired set temperature.
 *
 * @return Estimated time in seconds. Returns 0 if mode delay is invalid.
 */
static uint32_t calculate_remaining_time_sec(thermostat_mode_t mode, int current_temp, int set_temp);

/**
 * @brief Generate a human-readable string indicating time remaining to reach
 *       the set temperature.
 *
 * This function calculates the difference between the current and set temperature
 * and generates a formatted string such as:
 * - "5 min until heated to 24°c"
 * - "Temperature already at 72°F"
 *
 * @param mode Thermostat operating mode.
 * @param current_temp Current temperature.
 * @param set_temp Target temperature to reach.
 * @param out_str Output buffer for the message string.
 * @param len Length of the output buffer.
 */
static void generate_thermostat_time_str(thermostat_mode_t mode, int current_temp, int set_temp, char *out_str,
        size_t len);

/**
 * @brief Sets the system speaker volume based on the specified audio level.
 *
 * This function maps the provided audio level to the corresponding hardware
 * speaker volume.
 *
 * @param level The desired audio level to be applied.
 */
static void set_volume(audio_level_t level);

/**
 * @brief Turns the fan off and updates the UI accordingly.
 *
 * Sets the fan mode to OFF and shows the default fan image
 * with animation.
 */
static void fan_off(void);

/**
 * @brief Sets the fan to high speed and updates the UI.
 *
 * Sets the fan mode to HIGH and displays the high-speed fan animation.
 */
static void fan_high(void);

/**
 * @brief Sets the fan to medium speed and updates the UI.
 *
 * Sets the fan mode to MED and displays the medium-speed fan animation.
 */
static void fan_med(void);

/**
 * @brief Sets the fan to low speed and updates the UI.
 *
 * Sets the fan mode to LOW and displays the low-speed fan animation.
 */
static void fan_low(void);

/**
 * @brief Updates the mode label on the Active screen UI.
 *
 * Sets the text of the mode label (`ui_MainModeLabel`) based on the
 * currently selected thermostat mode.
 *
 * @param mode The current thermostat mode to display (ECO, RAPID, AUTO, OFF).
 */
static void update_mode_label(thermostat_mode_t mode);

/**
 * @brief Displays the next notification from the queue.
 *
 * Updates the label, shows the notification with animation,
 * and sets a timer to hide it after a predefined duration.
 */
static void show_next_notification(void);

/**
 * @brief Checks whether a given notification already exists in the queue.
 *
 * Prevents duplicate notifications from being enqueued.
 *
 * @param type Type of the notification.
 * @param status Status associated with the notification.
 * @param value Optional value for additional context.
 *
 * @return true if a duplicate is found, false otherwise.
 */
static bool is_notification_duplicate(notification_type type, notification_status_t status, uint32_t value);



/**
 * @brief Initiates the animation to hide the current notification.
 *
 * Creates and starts a vertical slide animation for the notification UI object,
 * and sets the completion callback to hide the object and show the next one.
 *
 * @param timer Pointer to the LVGL timer that triggered this function.
 */
static void hide_notification(lv_timer_t * timer);

/**
 * @brief Callback executed when the hide animation completes.
 *
 * This function hides the notification UI element, clears the current
 * notification, and displays the next notification in the queue, if any.
 *
 * @param a Pointer to the completed animation object.
 */
static void hide_notification_ready_cb(lv_anim_t *a);

static void increase_temp_step(lv_timer_t *timer);
static void decrease_temp_step(lv_timer_t *timer);
static int temp_to_heating_arc_angle(int temperature);
static int temp_to_cooling_arc_angle(int temperature);

/**
 * @brief This is a helper function to update the time labels on the UI.
 *
 * This function takes the current static hour, minute, and second variables
 * and formats them into a "00" string representation before setting the text
 * of the corresponding LVGL labels.
 *
 * @param None
 */
static void update_time_labels(void);

/**
 * @brief Callback for the firmware details timer.
 *
 * This function is called by the LVGL timer after a delay. It updates the
 * firmware version label and hides the spinner.
 *
 * @param timer The LVGL timer object.
 */
static void update_firmware_label(lv_timer_t *timer);

/**
 * @brief FreeRTOS timer callback for LVGL UI updates.
 *
 * This callback is triggered when the LVGL one-shot timer expires.
 * It sets a flag to indicate that the connectivity screen should be hidden.
 *
 * @param xTimer Handle of the timer that expired.
 */
static void hide_popup_timer_cb(TimerHandle_t xTimer);

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

static int get_deg_delay_sec(thermostat_mode_t mode)
{
    switch (mode)
    {
        case MODE_ECO:
            return 5;

        case MODE_RAPID:
            return 1;

        case MODE_AUTO:
            return 3;

        default:
            return -1;
    }
}

static uint32_t calculate_remaining_time_sec(thermostat_mode_t mode, int current_temp, int set_temp)
{
    int delay_per_deg = get_deg_delay_sec(mode);

    if (delay_per_deg <= 0)
    {
        return 0;
    }

    int temp_diff = abs(set_temp - current_temp);
    return temp_diff * delay_per_deg;
}

static void generate_thermostat_time_str(thermostat_mode_t mode, int current_temp, int set_temp, char *out_str,
        size_t len)
{
    static uint32_t update_time = 0;

    if (mode == MODE_OFF)
    {
        snprintf(out_str, len, "System is OFF");
        return;
    }

    int temp_diff = set_temp - current_temp;
    if (temp_diff == 0)
    {
        if (dev_unit == TEMP_UNIT_CELSIUS)
        {
            snprintf(out_str, len, "Temperature already at %d", set_temp);
        }
        else
        {
            snprintf(out_str, len, "Temperature already at %d", set_temp);
        }
        return;
    }

    uint32_t delay_sec = calculate_remaining_time_sec(mode, current_temp, set_temp);
    uint32_t delay_min = (delay_sec + 59) / 60;
    if (update_time != delay_min)
    {
        update_time = delay_min;
    }

    if (temp_diff < 0)
    {
        // Cooling
        if (dev_unit == TEMP_UNIT_CELSIUS)
        {
            snprintf(out_str, len, "%u min until cooled to %d", (unsigned int) delay_min, set_temp);
        }
        else
        {
            snprintf(out_str, len, "%u min until cooled to %d", (unsigned int) delay_min, set_temp);
        }
    }
    else
    {
        // Heating
        if (dev_unit == TEMP_UNIT_CELSIUS)
        {
            snprintf(out_str, len, "%u min until heated to %d", (unsigned int) delay_min, set_temp);
        }
        else
        {
            snprintf(out_str, len, "%u min until heated to %d", (unsigned int) delay_min, set_temp);
        }
    }

    dev_info.thermostat_settings.time_remains = delay_sec;
}

static void set_volume(audio_level_t level)
{
    /* Update speaker volume based on the received audio level */
    switch (level)
    {
        case AUDIO_OFF:
            app_speaker_audio_lvl_ctrl(AUDIO_LVL_OFF);
            break;

        case AUDIO_LOW:
            app_speaker_audio_lvl_ctrl(AUDIO_LVL_LOW);
            break;

        case AUDIO_MED:
            app_speaker_audio_lvl_ctrl(AUDIO_LVL_MED);
            break;

        case AUDIO_HIGH:
            app_speaker_audio_lvl_ctrl(AUDIO_LVL_HIGH);
            break;
    }

    audio_level = level;
    current_settings.audio.level = level;

    /* Store updated settings in NVM */
    update_current_device_setting();
}

static void fan_off(void)
{
    /* Update fan mode to off */
    dev_fan_mode = FAN_OFF;

    /* Delete fan mode animation */
    lv_anim_del(ui_fanoff, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    /* Hide all the images initially */
    lv_obj_add_flag(ui_fanoff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

    /* Clear the fan off image and start animation */
    lv_obj_clear_flag(ui_fanoff, LV_OBJ_FLAG_HIDDEN);
    fanspeed_Animation(ui_fanoff, 0);
}

static void fan_high(void)
{
    /* Update fan mode to high */
    dev_fan_mode = FAN_HIGH;

    /* Delete fan mode animation */
    lv_anim_del(ui_fanoff, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    /* Hide all the images initially */
    lv_obj_add_flag(ui_fanoff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

    /* Clear the fan high image and start animation */
    lv_obj_clear_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
    fanspeedhigh_Animation(ui_fanactivehigh, 0);
}

static void fan_med(void)
{
    /* Update fan mode to medium */
    dev_fan_mode = FAN_MED;

    /* Delete fan mode animation */
    lv_anim_del(ui_fanoff, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    /* Hide all the images initially */
    lv_obj_add_flag(ui_fanoff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

    /* Clear the fan medium image and start animation */
    lv_obj_clear_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);
    fanspeedmed_Animation(ui_fanactivemed, 0);
}

static void fan_low(void)
{
    /* Update fan mode to low */
    dev_fan_mode = FAN_LOW;

    /* Delete fan mode animation */
    lv_anim_del(ui_fanoff, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    /* Hide all the images initially */
    lv_obj_add_flag(ui_fanoff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

    /* Clear the fan low image and start animation */
    lv_obj_clear_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
    fanspeedlow_Animation(ui_fanactivelow, 0);
}

static void hide_notification_ready_cb(lv_anim_t *a)
{
    lv_obj_add_flag(ui_notification, LV_OBJ_FLAG_HIDDEN);

    /* Move to next message */
    notif_head = (notif_head + 1) % MAX_NOTIF_QUEUE;
    notif_showing = false;

    /* Clear old message (optional) */
    memset(&notif_queue[notif_head], 0, sizeof(notification_data_t));

    show_next_notification();
}

static void hide_notification(lv_timer_t *timer)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, ui_notification);
    lv_anim_set_values(&a, -197, -297);
    lv_anim_set_time(&a, 300);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
    lv_anim_set_ready_cb(&a, hide_notification_ready_cb);
    lv_anim_start(&a);

    lv_timer_del(timer);
    notif_timer = NULL;
}

static bool is_notification_duplicate(notification_type type, notification_status_t status, uint32_t value)
{
    int i = notif_head;
    while (i != notif_tail)
    {
        if (notif_queue[i].type == type && notif_queue[i].status == status && notif_queue[i].value == value)
        {
            return true;
        }
        i = (i + 1) % MAX_NOTIF_QUEUE;
    }
    return false;
}

void enqueue_notification(notification_type type, notification_status_t status, uint32_t value)
{
    if (is_notification_duplicate(type, status, value))
        return;

    int next_tail = (notif_tail + 1) % MAX_NOTIF_QUEUE;
    if (next_tail == notif_head)
    {
        /* Queue full, drop notification */
        return;
    }

    notif_queue[notif_tail].type = type;
    notif_queue[notif_tail].status = status;
    notif_queue[notif_tail].value = value;
    notif_tail = next_tail;

    if (!notif_showing)
    {
        show_next_notification();
    }
}

static void show_next_notification(void)
{
    if (notif_head == notif_tail)
    {
        /* Queue empty */
        notif_showing = false;
        return;
    }

    notification_data_t *n = &notif_queue[notif_head];
    update_notifcation_label(n->type, n->status, n->value);

    lv_obj_clear_flag(ui_notification, LV_OBJ_FLAG_HIDDEN);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, ui_notification);
    lv_anim_set_values(&a, -297, -197);
    lv_anim_set_time(&a, 300);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
    lv_anim_start(&a);

    notif_showing = true;

    if (notif_timer)
    {
        lv_timer_del(notif_timer);
    }
    notif_timer = lv_timer_create(hide_notification, NOTIFICATION_TIMEOUT_MS, NULL);

    /* Play notification chime based on notification type */
    app_speaker_play(notify_audio_type);
}


void update_setto_label(lv_event_t *e)
{
    if (temp_timer)
    {
        if (popup_overlay_visible == false)
        {
            //_ui_flag_modify(ui_settolbl, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        }
    }
}

void increase_temp(lv_event_t *e)
{
    if (dev_current_mode != MODE_OFF)
    {
        if (temperature < current_max_temp)
        {
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempControl, 35);
            target_temp = ++temperature;
            if (dev_unit == TEMP_UNIT_CELSIUS)
            {
                lv_label_set_text_fmt(ui_currenttemp, "%d°C", temperature);
            }
            else
            {
                lv_label_set_text_fmt(ui_currenttemp, "%d°F", temperature);
            }
            lv_obj_set_style_text_color(ui_currenttemp, lv_color_hex(0xF44336), LV_PART_MAIN | LV_STATE_DEFAULT);
            if (popup_overlay_visible == false)
            {
                //lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
                lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
            }

            current_state = STATE_HEATING;

            if (temp_timer)
            {
                lv_timer_del(temp_timer);
                temp_timer = NULL;
            }
            temp_timer = lv_timer_create(increase_temp_step, deg2sec, NULL);
            lv_label_set_text_fmt(ui_MainModeLabel, ". . . Heating . . .");

            dev_info.environment.target_temp = target_temp;
            /* Update sensor data if device is connected */
            if (true == is_device_connected)
            {
                update_temperature_data_ipc(&dev_info);
            }
            generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
            //lv_label_set_text(ui_homescreensubmsg, subinfo);
            //lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);

            lv_obj_remove_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempIndicator, 35); // Replace current_temp with your value
        }
    }
    else
    {
        LOG_INFO(CYLF_DEF, "Increase temperature error. Device in Off mode.\n");
    }
}

void decrease_temp(lv_event_t *e)
{
    if (dev_current_mode != MODE_OFF)
    {
        if (temperature > current_min_temp)
        {
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempControl, 15);
            target_temp = --temperature;

            if (dev_unit == TEMP_UNIT_CELSIUS)
            {
                lv_label_set_text_fmt(ui_currenttemp, "%d°C", temperature);
            }
            else
            {
                lv_label_set_text_fmt(ui_currenttemp, "%d°F", temperature);
            }

            lv_obj_set_style_text_color(ui_currenttemp, lv_color_hex(0xC6FFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            if (popup_overlay_visible == false)
            {
                //lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
                lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
            }
            current_state = STATE_COOLING;
            if (temp_timer)
            {
                lv_timer_del(temp_timer);
                temp_timer = NULL;
            }
            temp_timer = lv_timer_create(decrease_temp_step, deg2sec, NULL);
            lv_label_set_text_fmt(ui_MainModeLabel, ". . . Cooling . . .");
            dev_info.environment.target_temp = target_temp;
            /* Update sensor data if device is connected */
            if (true == is_device_connected)
            {
                update_temperature_data_ipc(&dev_info);
            }
            generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
            //lv_label_set_text(ui_homescreensubmsg, subinfo);
            //lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);

            lv_obj_remove_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempIndicator, 15); // Replace current_temp with your value
        }
        else
        {
            LOG_INFO(CYLF_DEF, "Thermostat Minimum temperature reached.\n");
        }
    }
    else
    {
        LOG_INFO(CYLF_DEF, "Decrease temperature error. Device in Off mode.\n");
    }
}

static void increase_temp_step(lv_timer_t *timer)
{

    if (dev_current_mode != MODE_OFF)
    {
        if (current_temp < target_temp)
        {
            current_temp++;
            if (dev_unit == TEMP_UNIT_CELSIUS)
            {
                lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
                lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
            }
            else
            {
                lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
                lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
            }

            lv_label_set_text_fmt(ui_MainModeLabel, ". . . Heating . . .");
            generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
            //lv_label_set_text(ui_homescreensubmsg, subinfo);
            //lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
            current_state = STATE_HEATING;

//            lv_obj_set_style_arc_color(ui_ArcTempControl, lv_color_hex(0xFF5A5A), LV_PART_INDICATOR);

            lv_obj_remove_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempIndicator, 35); // Replace current_temp with your value

//            lv_arc_set_value(ui_ArcTempIndicator, (target_temp - current_temp)); // Replace current_temp with your value

            //            lv_arc_set_value(ui_ArcTempIndicator, current_temp);
            int start_angle = temp_to_heating_arc_angle(current_temp)+1;
            int end_angle   = temp_to_heating_arc_angle(target_temp);

            printf("Updated Start angle: %d, End Angle: %d\n", start_angle, end_angle);
            printf("Updated CT: %d, TT: %d\n", current_temp, target_temp);

                 //       lv_arc_set_range(ui_ArcTempIndicator, 0, abs(target_temp - current_temp));
                        //lv_arc_set_value(ui_ArcTempIndicator, abs(target_temp - current_temp)); // Replace current_temp with your value

            lv_arc_set_bg_start_angle(ui_ArcTempIndicator, start_angle);
            lv_arc_set_bg_end_angle(ui_ArcTempIndicator, end_angle);

        }
        if (current_temp == target_temp)
        {
            /** Switch to Active screen  */
            switch_to_active_screen();

            lv_timer_del(timer);
            temp_timer = NULL;
            if (dev_unit == TEMP_UNIT_CELSIUS)
            {
                lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
                lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
            }
            else
            {
                lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
                lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
            }

            lv_arc_set_value(ui_ArcTempControl, current_temp);

            //lv_obj_add_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);

            lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
            update_mode_label(dev_current_mode);
            enqueue_notification(NOTIFY_TEMP_UPDATE, NOTIF_SUCCESS, current_temp);
            dev_info.thermostat_settings.time_remains = 0;
            current_state = STATE_NONE;

//            lv_obj_set_style_arc_color(ui_ArcTempControl, lv_color_hex(0x191C26), LV_PART_INDICATOR);
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
        }

        dev_info.environment.current_temp = current_temp;

        /* Update sensor data if device is connected */
        if (true == is_device_connected)
        {
            update_temperature_data_ipc(&dev_info);
        }
    }
}

static void decrease_temp_step(lv_timer_t *timer)
{
    if (dev_current_mode != MODE_OFF)
    {
        lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
//		lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
        if (current_temp > target_temp)
        {
            current_temp--;
            lv_label_set_text_fmt(ui_MainModeLabel, ". . . Cooling . . .");

            if (dev_unit == TEMP_UNIT_CELSIUS)
            {
                lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
                lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
            }
            else
            {
                lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
                lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
            }

            generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
            //lv_label_set_text(ui_homescreensubmsg, subinfo);
            //lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
            current_state = STATE_COOLING;

            lv_obj_remove_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempIndicator, 15); // Replace current_temp with your value

            int start_angle = temp_to_cooling_arc_angle(current_temp);
            int end_angle   = temp_to_cooling_arc_angle(target_temp);

            printf("Start angle: %d, End Angle: %d\n", start_angle, end_angle);
            printf("CT: %d, TT: %d\n", current_temp, target_temp);

      //     lv_arc_set_range(ui_ArcTempIndicator, 0, abs(target_temp - current_temp));
         //   lv_arc_set_value(ui_ArcTempIndicator, abs(target_temp - current_temp)); // Replace current_temp with your value

            lv_arc_set_bg_start_angle(ui_ArcTempIndicator, end_angle);
            lv_arc_set_bg_end_angle(ui_ArcTempIndicator, start_angle);

        }
        if (current_temp == target_temp)
        {
            /** Switch to Active screen  */
            switch_to_active_screen();

            lv_timer_del(timer);
            temp_timer = NULL;
            if (dev_unit == TEMP_UNIT_CELSIUS)
            {
                lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
                lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
            }
            else
            {
                lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
                lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
            }

            lv_arc_set_value(ui_ArcTempControl, current_temp);
            
            //lv_obj_add_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
            update_mode_label(dev_current_mode);
            enqueue_notification(NOTIFY_TEMP_UPDATE, NOTIF_SUCCESS, current_temp);
            dev_info.thermostat_settings.time_remains = 0;
            current_state = STATE_NONE;

            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
        }

        dev_info.environment.current_temp = current_temp;

        /* Update sensor data if device is connected */
        if (true == is_device_connected)
        {
            update_temperature_data_ipc(&dev_info);
        }
    }
}


void update_fan_mode(fan_speed_t mode)
{
    /* Set Active screen fan animation and helper label
     * based on selected mode */
    switch (mode)
    {
        case FAN_HIGH:
            fan_high();
            lv_label_set_text_fmt(ui_fanmodelabel, "HIGH");
            break;
        case FAN_LOW:
            fan_low();
            lv_label_set_text_fmt(ui_fanmodelabel, "LOW");
            break;
        case FAN_MED:
            fan_med();
            lv_label_set_text_fmt(ui_fanmodelabel, "MED");
            break;
        case FAN_OFF:
            fan_off();
            lv_label_set_text_fmt(ui_fanmodelabel, "OFF");
            current_state = STATE_NONE;
            break;

        default:
            fan_off();
            lv_label_set_text_fmt(ui_fanmodelabel, "OFF");
            current_state = STATE_NONE;
            break;
    }

    dev_fan_mode = mode;

    /* Update info on MApp or Cloud if connected */
    if (true == is_device_connected)
    {
        /* Update fan mode info. to CM33 core using IPC */
        update_fan_speed_ipc(dev_fan_mode);
    }
}

void fan_clicked(lv_event_t *e)
{
    UNUSED_PARAM(e);

    if (dev_current_mode != MODE_OFF)
    {
        /* Device is ON → fan cannot be OFF */
        if (dev_fan_mode == FAN_OFF || dev_fan_mode == FAN_HIGH)
        {
            dev_fan_mode = FAN_LOW;  // Start cycle at LOW
        }
        else if (dev_fan_mode == FAN_LOW)
        {
            dev_fan_mode = FAN_MED;
        }
        else if (dev_fan_mode == FAN_MED)
        {
            dev_fan_mode = FAN_HIGH;
        }
    }
    else
    {
        /* Device is OFF → allow full cycle including FAN_OFF */
        if (dev_fan_mode == FAN_LOW)
        {
            dev_fan_mode = FAN_OFF;
        }
        else
        {
            dev_fan_mode++;
        }
    }

    update_fan_mode(dev_fan_mode);
    current_settings.thermostat_setting.fan_mode = dev_fan_mode;

    /* Save updated setting */
    update_current_device_setting();
}



void show_presence_icon_bubble(void)
{
    // _ui_flag_modify(ui_presencecountlabel, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    // _ui_flag_modify(ui_presencecountcircle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
}

// void weatherup(lv_event_t *e)
// {
//     weather_temp++;

//     if (dev_unit == TEMP_UNIT_CELSIUS)
//     {
//         lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d", weather_temp);

//     }
//     else
//     {
//         lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d", weather_temp);
//     }

// }

// void weatherdown(lv_event_t *e)
// {
//     weather_temp--;
//     if (dev_unit == TEMP_UNIT_CELSIUS)
//     {
//         lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d", weather_temp);
//     }
//     else
//     {
//         lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d", weather_temp);
//     }
// }

void update_display_brightness(uint8_t level)
{
    lv_slider_set_value(ui_BrightnessSlider, level, LV_ANIM_OFF);
    mtb_display_st7701s_set_brightness(level);
    brightness_level = level;
    dev_info.preferences.display_brightness = level;
    current_settings.display_setting.brightness = level;
    update_current_device_setting();
}

void change_brightness(lv_event_t *e)
{
    uint8_t slider_val = lv_slider_get_value(ui_BrightnessSlider);

    if ((slider_val % 10) > 5)
    {
        brightness_level = ((slider_val / 10) * 10) + 10;
    }
    else
    {
        brightness_level = ((slider_val / 10) * 10);
    }

    LOG_INFO(CYLF_DEF, "Brightness Level: %d Actual Level: %d\n", brightness_level, slider_val);
    lv_slider_set_value(ui_BrightnessSlider, brightness_level, LV_ANIM_OFF);
    update_display_brightness(brightness_level);

    /* Is device connected to MApp or Cloud */
    if (true == is_device_connected)
    {
        update_brightness_ipc(brightness_level);
    }
}

void show_presence_icon_and_update_label(uint8_t person_count)
{
    char buf[10];
    sprintf(buf, "x %d", person_count);

    /* Set full opacity for the presence icon, and unhide the
     * presence label if count is >1 */
    lv_obj_set_style_opa(ui_presencelbl, LV_OPA_COVER, 0);

    // TODO: update presence count label only for first couple of seconds after detection
    if (person_count > 1)
    {
        show_presence_icon_bubble();
        //lv_obj_clear_flag(ui_presencecountlabel, LV_OBJ_FLAG_HIDDEN);
        //lv_label_set_text(ui_presencecountlabel, buf);
    }
}

void update_presence_detection(uint8_t presence_count)
{
    stop_active_state_timer();
    //LOG_INFO(CYLF_DEF, "Presence : %d\n", presence_count);
    if (presence_count == 0)
    {
        person_detected = false;
        //_ui_screen_change(&ui_LPScreen, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_LPScreen_screen_init);
        start_inactivity_timer_addn(3000); // 3 seconds
    }
    else 
    {
        // person_count = presence_count;
        person_detected = true;
        start_inactivity_timer();
        // show_presence_icon_and_update_label(presence_count);

    }
    display_presence_detection_status();
    return;
}

void hide_presence_icon(void)
{
    lv_obj_set_style_opa(ui_presencelbl, 60, 0);
    // lv_obj_add_flag(ui_presencecountlabel, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_presencecountcircle, LV_OBJ_FLAG_HIDDEN);
}

void display_presence_detection_status(void)
{
    if (person_detected == true)
    {
        show_presence_icon_and_update_label(person_count);
        // _ui_opacity_set(ui_presencelbl, 255);
    }
    else
    {
        hide_presence_icon();
    }
}


void update_pin_label(const char *pin)
{
    //lv_label_set_text(ui_pinlabel, pin);
}

static void hide_popup_timer_cb(TimerHandle_t xTimer)
{
    hide_conn_screen = true;
}

void hide_connectivity_screen(void)
{
    if((dev_current_conn_state == DEV_ST_BLE_CONNECTED) ||
            (dev_current_conn_state == DEV_ST_CLOUD_CONNECTED))
    {
        lv_obj_add_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
        wifi_popup_state = false;
        popup_overlay_visible = false;

        /* Start fan mode animation running in background */
        display_fan_anim();
        display_temp_change_anim();
        display_presence_detection_status();
//        _ui_flag_modify(ui_ArcTempControl, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_ArcGroup, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
}

void ui_timer_init(void)
{
    /* Create periodic timer of 10ms */
    lvgl_timer = xTimerCreate("lvglTimer",
                              pdMS_TO_TICKS(1500),   /* 1.5s period */
                              pdFALSE,              /* One shot */
                              NULL,
                              hide_popup_timer_cb);

    if(lvgl_timer == NULL)
    {
        LOG_INFO(CYLF_DEF, "ui_timer_init error.\n");
    }

}

void ui_timer_stop(void)
{
    if (lvgl_timer != NULL) {
        xTimerStop(lvgl_timer, 0);
    }
}

void ui_timer_start(void)
{
    if (lvgl_timer != NULL) {
        xTimerStart(lvgl_timer, 0);
    }
}

void update_device_connection_state(device_connection_state_t state)
{
    const char *state_text = "Unknown";

    /* Hide all hidden widgets (images, button, labels) initially */
    lv_obj_add_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_commissionKeyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_autoreconnectpanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleconmappinfolabel, LV_OBJ_FLAG_HIDDEN);
    ////lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_qrcodecontainer, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_cloudconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wificonnecting120, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_clouddisconn120, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleadv, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_devstateimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wificonnectedimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifidisabledimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleconnected120, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homebleconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleconnected50, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homecloudconnimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_qrcodebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_qrcodebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_cloudconnecting, LV_OBJ_FLAG_HIDDEN);
    //lv_obj_add_flag(ui_connectLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionprg1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionprg2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionprg3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_switchtowififromble, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_switchtowififrombleinfolbl, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_devconwificonnectionscreen, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleswitchbtn, LV_OBJ_FLAG_HIDDEN);
    //lv_obj_add_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_devicestatelabel, LV_OBJ_FLAG_HIDDEN);
    //lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);

    /* Delete all running animations */
    lv_anim_del(ui_bleadv, NULL);
    lv_anim_del(ui_wificonnecting120, NULL);
    lv_anim_del(ui_wifi, NULL);
    lv_anim_del(ui_homebleconnected, NULL);
    lv_anim_del(ui_wifi, NULL);
    lv_anim_del(ui_connectionprg1, NULL);
    lv_anim_del(ui_connectionprg2, NULL);
    lv_anim_del(ui_connectionprg3, NULL);


    /* Update UI based on received device state */
    switch (state)
    {
        case DEV_ST_CLOUD_CONNECTED:

            /* Update connection state in cm55 core */
            is_device_connected = true;

            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_autoreconnectpanel, LV_OBJ_FLAG_HIDDEN);

            //lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_cloudconnected, LV_OBJ_FLAG_HIDDEN);
            state_text = "Cloud Connected";
            //lv_obj_clear_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_clear_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_qrcodebtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
            
            lv_obj_clear_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);

            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homecloudconnimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_bleswitchbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homecloudconnimg, 255, 0);

            /* Add Cloud connected status in notification queue */
            enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_SUCCESS, DEV_ST_CLOUD_CONNECTED);

            /* Start timer to hide the pop-up screen */
            ui_timer_stop();
            ui_timer_start();
            ble_conn_state = false;
            cloud_conn_state = true;
            break;

        case DEV_ST_CLOUD_CONNECTING:

            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_cloudconnecting, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_autoreconnectpanel, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);

            state_text = "Cloud Connecting . . .";

            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homewifidisconnimg, 255, 0);
            cloud_conn_state = false;
            break;

        case DEV_ST_WIFI_CONNECTING:

            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wificonnecting120, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connectionprg1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connectionprg2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connectionprg3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_autoreconnectpanel, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
            progress1_Animation(ui_connectionprg1, 0);
            progress2_Animation(ui_connectionprg2, 0);
            progress3_Animation(ui_connectionprg3, 0);
            state_text = "Wi-Fi Connecting . . .";

            if(wifi_conn_state) {
                if(cloud_conn_state) {
                    update_device_connection_state(DEV_ST_CLOUD_CONNECTED); state_text = "Cloud connected";
                } 
                else {
                    update_device_connection_state(DEV_ST_CLOUD_DISCONNECTED); state_text = "Cloud Disconnected";
                }
                break;
            }

            /* Update icon on home screen */
            lv_obj_clear_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_wifi, 255, 0);
            wifi_Animation(ui_wifi, 0);
            wifi_conn_state = false;
            cloud_conn_state = false;
            break;

        case DEV_ST_CLOUD_DISCONNECTED:
            /* Update connection state in cm55 core */
            is_device_connected = false;

            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_clouddisconn120, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_autoreconnectpanel, LV_OBJ_FLAG_HIDDEN);
            state_text = "Cloud Disconnected";

            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_bleswitchbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homewifidisconnimg, 255, 0);

            /* Add Cloud disconnected status in notification queue */
            enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_FAIL, DEV_ST_CLOUD_DISCONNECTED);
            cloud_conn_state = false;
            break;

        case DEV_ST_BLE_ADVERTISING:

            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_devicestateimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_mappinfobutton, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_bleadv, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_clear_flag(ui_wificonnecting120, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_autoreconnectpanel, LV_OBJ_FLAG_HIDDEN);

            /* Only show the switch to WiFi option if device is provisioned  */
            if (true == is_device_provisioned)
            {
                lv_obj_clear_flag(ui_switchtowififromble, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_switchtowififrombleinfolbl, LV_OBJ_FLAG_HIDDEN);
            }
            bleadvpulse_Animation(ui_bleadv, 0);
            if(ble_conn_state){update_device_connection_state(DEV_ST_BLE_CONNECTED); state_text = "BLE Connected"; break;}
            state_text = "Waiting for mobile connection . . .";
            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homebleconnected, LV_OBJ_FLAG_HIDDEN);
            wifi_Animation(ui_homebleconnected, 0);
            lv_obj_set_style_opa(ui_homebleconnected, 255, 0);

            ble_conn_state = false;
            break;

        case DEV_ST_BLE_PAIRING:

            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_commissionKeyboard, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_qrcodecontainer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);

            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homebleconnected, LV_OBJ_FLAG_HIDDEN);
            //miclisteninganime_Animation(ui_homebleconnected, 0);
            lv_obj_set_style_opa(ui_homebleconnected, 255, 0);
            
            break;

        case DEV_ST_UNPROVISIONED:
            /* Update connection state in cm55 core */
            is_device_connected = false;

            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_qrcodecontainer, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_commissionKeyboard, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_devicestatelabel, LV_OBJ_FLAG_HIDDEN);
            
            //lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devicestateimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_mappinfobutton, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devicestatelabel, LV_OBJ_FLAG_HIDDEN);
            
            state_text = "Wi-Fi not connected\nTap to start mobile pair";

            /* Update icon on home screen */
            lv_obj_add_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_wifi, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            wifi_conn_state = false;
            ble_conn_state = false;
            cloud_conn_state = false;
            break;

        case DEV_ST_WIFI_CONNECTED:

            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wificonnectedimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_bleswitchbtn, LV_OBJ_FLAG_HIDDEN);
            state_text = "Wi-Fi Connected";

            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homewifidisconnimg, 255, 0);

            /* Add Wi-Fi connected status in notification queue */
            enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_SUCCESS, DEV_ST_WIFI_CONNECTED);
            wifi_conn_state = true;
            break;

        case DEV_ST_WIFI_DISCONNECTED:

            /* Update connection state on popup screen */
            
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifidisabledimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_autoreconnectpanel, LV_OBJ_FLAG_HIDDEN);
            state_text = "Wi-Fi_Disconnected";

            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homewifidisconnimg, 255, 0);

            /* Add Wi-Fi disconnected status in notification queue */
            enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_FAIL, DEV_ST_WIFI_DISCONNECTED);
            wifi_conn_state = false;
            break;

        case DEV_ST_BLE_CONNECTED:
            /* Update connection state in cm55 core */
            is_device_connected = true;
            
            /* Update connection state on popup screen */
            lv_obj_add_flag(ui_progressCancelBtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_bleconnected120, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_mappinfobutton, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_bleconmappinfolabel, LV_OBJ_FLAG_HIDDEN);

            /* Only show the switch to WiFi option if device is provisioned  */
            if ( true == is_device_provisioned || wifi_conn_state)
            {
                lv_obj_clear_flag(ui_switchtowififromble, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_switchtowififrombleinfolbl, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_bleconmappinfolabel, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                //lv_obj_clear_flag(ui_switchtoKeybd, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);
            }

            state_text = "BLE Connected";

            /* Update icon on home screen */
            lv_obj_clear_flag(ui_bleconnected50, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_bleconnected50, 190, 0);

            /* Start timer to hide the pop-up screen */
            ui_timer_stop();
            ui_timer_start();
            ble_conn_state = false;
            break;

        default:
            break;
    }

    /* Update device state label in popup screen */
    lv_label_set_text(ui_devstatelabel, state_text);
    
    /* Update device current connection state */
    dev_current_conn_state = state;
    dev_last_conn_state = state;
}

void start_ble_adv(lv_event_t *e)
{
    UNUSED_PARAM(e);
    LOG_INFO(CYLF_DEF, "CM55: Start BLE ADV\r\n");

    prov_method = PROV_MAPP_BLE;

    /* Send start BLE ADV CMD to CM33 core using IPC */
    update_ble_adv_ipc(DEV_ST_BLE_ADVERTISING);
}

void connect_wifi(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Get the SSID and Password from the text field */
    const char *ssid = lv_textarea_get_text(ui_ssidfield);
    const char *password = lv_textarea_get_text(ui_passwordfield);

    prov_method = PROV_UI_KEYBOARD;

    /* Update UI with Wi-Fi connecting status */
    update_device_connection_state(DEV_ST_WIFI_CONNECTING);

    /* Send start WiFi connection along with Wi-Fi credentials
    to CM33 core using IPC */
    update_wifi_cred_ipc(ssid, password);
}

void update_wifi_cred(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Get the SSID and Password from the text field */
    const char *ssid = lv_textarea_get_text(ui_devstatecontainerssidfield);
    const char *password = lv_textarea_get_text(ui_devstatecontainerpasswordfield);
    
    /* Update UI with Wi-Fi connecting status */
    update_device_connection_state(DEV_ST_WIFI_CONNECTING);

    /* Send start WiFi connection along with Wi-Fi credentials
    to CM33 core using IPC */
    update_wifi_cred_ipc(ssid, password);
}

void update_notifcation_label(notification_type type, notification_status_t status, uint32_t value)
{
    if (NOTIF_FAIL == status)
    {
        /* Set notification audio  */
        notify_audio_type = AUDIO_ERROR;

        /* Set image in notification label */
        lv_img_set_src(ui_notificationlblimg, &ui_img_incorrect_img_png);

        /* Set text in notification label */
        switch (type)
        {
            case NOTIFY_NETWORK_STATUS:
                switch ((device_connection_state_t) value)
                {
                    case DEV_ST_CLOUD_DISCONNECTED:
                        lv_label_set_text(ui_notificationlabel, "Disconnected from Cloud.");
                        break;

                    case DEV_ST_WIFI_DISCONNECTED:
                        lv_label_set_text(ui_notificationlabel, "Disconnected from WiFi network.");
                        break;

                    case DEV_ST_NO_INTERNET:
                        lv_label_set_text(ui_notificationlabel, "Oops! No Internet, Try again.");
                        break;

                    default:
                        break;
                }
                break;

            case NOTIFY_TEMP_UPDATE:
                lv_label_set_text(ui_notificationlabel, "Failed to update temperature.");
                break;

            case NOTIFY_MODE_UPDATE:
                lv_label_set_text(ui_notificationlabel, "Failed to set mode.");
                break;

            case NOTIFY_FAN_MODE_UPDATE:
                lv_label_set_text(ui_notificationlabel, "Failed to set fan mode.");
                break;

            case NOTIFY_FW_UPDATE:
                lv_label_set_text(ui_notificationlabel, "Latest firmware installed, no update.");
                break;

            case NOTIFY_HTTP_SYNC:
                lv_label_set_text(ui_notificationlabel, "Failed to sync Location, Weather, Timezone");
                break;

            default:
                break;
        }
    }
    else if (NOTIF_SUCCESS == status)
    {
        /* Set notification audio  */
        notify_audio_type = AUDIO_SUCCESS;

        /* Set image in notification label */
        lv_img_set_src(ui_notificationlblimg, &ui_img_correct_img_png);

        /* Set text in notification label */
        switch (type)
        {
            case NOTIFY_NETWORK_STATUS:
                switch ((device_connection_state_t) value)
                {
                    case DEV_ST_CLOUD_CONNECTED:
                        lv_label_set_text(ui_notificationlabel, "Connected to Cloud.");
                        break;

                    case DEV_ST_WIFI_CONNECTED:
                        lv_label_set_text(ui_notificationlabel, "Connected to WiFi network.");
                        break;

                    default:
                        break;
                }
                break;
            case NOTIFY_TEMP_UPDATE:
                if (dev_unit == TEMP_UNIT_CELSIUS)
                {
                    lv_label_set_text_fmt(ui_notificationlabel, "Temperature set-point reached %u°C.", value);
                }
                else
                {
                    lv_label_set_text_fmt(ui_notificationlabel, "Temperature set-point reached %u°F.", value);
                }

                break;
            case NOTIFY_MODE_UPDATE:
                lv_label_set_text_fmt(ui_notificationlabel, "Mode set to %u.", value);  // You may map value to text
                break;
            case NOTIFY_FAN_MODE_UPDATE:
                lv_label_set_text_fmt(ui_notificationlabel, "Fan mode set to %u.", value); // Same here
                break;

            case NOTIFY_HTTP_SYNC:
                lv_label_set_text(ui_notificationlabel, "Synced Location, Weather, Timezone");
                break;

            default:
                break;
        }
    }
}

// Function to hide the label after 2 seconds
void hide_mappinfolabel(lv_timer_t *timer)
{
    lv_obj_add_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);
}

// Button tap event handler
void process_mappinfobutton_ex(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Show the label
        lv_obj_clear_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);

        // Start a 2-second (2000 ms) timer to hide the label
        lv_timer_create(hide_mappinfolabel, 2000, NULL);
    }
}

// Function to hide the label after 2 seconds
void hide_keyboardinfolabel(lv_timer_t *timer)
{
    lv_obj_add_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN);
}

// Button tap event handler
void process_keyboardinfobutton_ex(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Show the label
        lv_obj_clear_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN);

        // Start a 2-second (2000 ms) timer to hide the label
        lv_timer_create(hide_mappinfolabel, 2000, NULL);
    }
}

// Timer callback function to hide the label
static void wifi_ilabel_timer_callback(TimerHandle_t xTimer)
{
//    lv_obj_add_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);
//    lv_obj_add_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN);
}

void start_wifi_settings_ilabel_timer_ex(void)
{
//    lv_obj_clear_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);

    if (wifi_ilabel_timer == NULL)
    {
        // Create the timer (auto-reload = pdFALSE for one-shot)
        wifi_ilabel_timer = xTimerCreate("WifiILabelTimer", pdMS_TO_TICKS(WIFI_ILABEL_TIMER_TIMEOUT_MS), pdFALSE,
        NULL, wifi_ilabel_timer_callback);
    }

    if (wifi_ilabel_timer != NULL)
    {
        // If already active, stop it first
        if (xTimerIsTimerActive(wifi_ilabel_timer))
        {
            xTimerStop(wifi_ilabel_timer, 0);
        }

        // Start (or restart) the timer
        xTimerStart(wifi_ilabel_timer, 0);
    }
}
void stop_wifi_settings_ilabel_timer_ex(void)
{
    if (wifi_ilabel_timer != NULL)
    {
        if (xTimerIsTimerActive(wifi_ilabel_timer))
        {
            xTimerStop(wifi_ilabel_timer, 0);
        }
    }
}

// Timer callback function to hide the label
static void wifi_kybd_label_timer_callback(TimerHandle_t xTimer)
{
//    lv_obj_add_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);
    if (!lv_obj_has_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN))
    {
        lv_obj_add_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN);
    }
}

// Timer callback function to hide the label
static void fw_update_check_cb(TimerHandle_t xTimer)
{	
	fw_update_timeout = true;
}

void start_wifi_settings_kybd_label_timer_ex(void)
{
    if (lv_obj_has_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN))
    {
        lv_obj_clear_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN);
    }
    if (wifi_kybd_label_timer == NULL)
    {
        // Create the timer (auto-reload = pdFALSE for one-shot)
        wifi_kybd_label_timer = xTimerCreate("WifiKybdLabelTimer", pdMS_TO_TICKS(WIFI_ILABEL_TIMER_TIMEOUT_MS), pdFALSE,
        NULL, wifi_kybd_label_timer_callback);
    }

    if (wifi_kybd_label_timer != NULL)
    {
        // If already active, stop it first
        if (xTimerIsTimerActive(wifi_kybd_label_timer))
        {
            xTimerStop(wifi_kybd_label_timer, 0);
        }

        // Start (or restart) the timer
        xTimerStart(wifi_kybd_label_timer, 0);
    }
}
void stop_wifi_settings_kybd_label_timer_ex(void)
{
    if (wifi_ilabel_timer != NULL)
    {
        if (xTimerIsTimerActive(wifi_kybd_label_timer))
        {
            xTimerStop(wifi_kybd_label_timer, 0);
        }
    }
}

void set_thermostat_mode(thermostat_mode_t mode)
{
    /* Update image and labels based on current mode */
    switch (mode)
    {
        case MODE_ECO:
            //lv_img_set_src(ui_ModeButton, &ui_img_eco_png);
            lv_obj_set_style_bg_image_src(ui_ModeButton, &ui_img_eco_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_img_set_src(ui_ecoLP, &ui_img_eco_png);
            deg2sec = ECO_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_LOW);
            LOG_INFO(CYLF_DEF, "Mode set to ECO\n");
            break;

        case MODE_RAPID:
            //lv_img_set_src(ui_ModeButton, &ui_img_mode_select_rapid_png);
            lv_obj_set_style_bg_image_src(ui_ModeButton, &ui_img_mode_select_rapid_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_img_set_src(ui_ecoLP, &ui_img_mode_select_rapid_png);
            deg2sec = RAPID_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_HIGH);
            LOG_INFO(CYLF_DEF, "Mode set to RAPID\n");
            break;

        case MODE_AUTO:
            //lv_img_set_src(ui_ModeButton, &ui_img_automode_png_png);
            lv_obj_set_style_bg_image_src(ui_ModeButton, &ui_img_automode_png_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_img_set_src(ui_ecoLP, &ui_img_automode_png_png);
            deg2sec = AUTO_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_MED);
            LOG_INFO(CYLF_DEF, "Mode set to AUTO\n");
            update_device_temp(AUTO_MODE_TARGET_TEMP_DEFAULT_C);
            break;

        case MODE_OFF:
            //lv_img_set_src(ui_ModeButton, &ui_img_mode_select_fan_png);
            lv_obj_set_style_bg_image_src(ui_ModeButton, &ui_img_mode_select_fan_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_img_set_src(ui_ecoLP, &ui_img_mode_select_fan_png);
            update_fan_mode(FAN_OFF);
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            LOG_INFO(CYLF_DEF, "Mode set to FAN\n");

            /* If temperature increase decrease timer running, stop it */
            if (temp_timer)
            {
                lv_timer_del(temp_timer);
                temp_timer = NULL;
            }

            target_temp = current_temp;
            temperature = current_temp;
            //lv_obj_add_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempControl, current_temp);
            break;

        default:
            break;
    }

    /* Update mode label on Active screen  */
    update_mode_label(mode);

    /* Update info on MApp or Cloud if connected */
    if (true == is_device_connected)
    {
        /* Send mode info to CM33 core using IPC */
        update_device_mode_ipc(mode);
    }

    /* Store updated setting in NVM */
    update_thermostat_mode_timer();
}

void toggle_mode(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Advance to next mode */
    dev_current_mode = (dev_current_mode + 1) % MODE_MAX;

    /* Update UI based on the selected mode */
    set_thermostat_mode(dev_current_mode);
    lv_obj_set_style_opa(ui_ModeButton, LV_OPA_COVER, 0);

    /* Update current settings accordingly */
    current_settings.thermostat_setting.mode = dev_current_mode;
    current_settings.thermostat_setting.fan_mode = dev_fan_mode;

    /* Store updated mode in NVM */
    update_current_device_setting();
}

void open_notifcaiton_ex(void)
{
    LOG_INFO(CYLF_DEF, "Notification popup clicked.\n");

}
void delete_wifi_cred(lv_event_t *e)
{
    request_wifi_delete_ipc();

    /* Update device provisioned state */
    is_device_provisioned = false;
    
}

void display_qrcode(lv_event_t *e)
{
    qr_manager_init(&qr_manager, ui_qrcodecontainer, 120);
    qr_manager_update(&qr_manager, device_unique_id);
    lv_label_set_text(ui_qrcodelabel, "Scan this QR code using the mobile app.");
}

void hide_qr(lv_event_t *e)
{
    qr_manager_deinit(&qr_manager);
}

static void update_mode_label(thermostat_mode_t mode)
{
    /* Update mode label on Active screen UI */
    switch (mode)
    {
        case MODE_ECO:
            lv_label_set_text_fmt(ui_MainModeLabel, "ECO");
            break;

        case MODE_RAPID:
            lv_label_set_text_fmt(ui_MainModeLabel, "RAPID");
            break;

        case MODE_AUTO:
            lv_label_set_text_fmt(ui_MainModeLabel, "AUTO");
            break;

        case MODE_OFF:
            lv_label_set_text_fmt(ui_MainModeLabel, "OFF");
            break;
        default:
            break;
    }
}

void update_thermostat_mode(thermostat_mode_t mode)
{
    /* Update mode and fan mode image along with functionality
     * based on current mode */
    switch (mode)
    {
        case MODE_ECO:
            //lv_img_set_src(ui_ModeButton, &ui_img_eco_png);
            lv_obj_set_style_bg_image_src(ui_ModeButton, &ui_img_eco_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_img_set_src(ui_ecoLP, &ui_img_eco_png);
            deg2sec = ECO_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_LOW);
            LOG_INFO(CYLF_DEF, "Mode set to ECO\n");
            break;

        case MODE_RAPID:
            //lv_img_set_src(ui_ModeButton, &ui_img_mode_select_rapid_png);
            lv_obj_set_style_bg_image_src(ui_ModeButton, &ui_img_mode_select_rapid_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_img_set_src(ui_ecoLP, &ui_img_mode_select_rapid_png);
            deg2sec = RAPID_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_HIGH);
            LOG_INFO(CYLF_DEF, "Mode set to RAPID\n");
            break;

        case MODE_AUTO:
            //lv_img_set_src(ui_ModeButton, &ui_img_mode_select_auto_png);
            lv_obj_set_style_bg_image_src(ui_ModeButton, &ui_img_mode_select_auto_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_img_set_src(ui_ecoLP, &ui_img_mode_select_auto_png);
            deg2sec = AUTO_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_MED);
            LOG_INFO(CYLF_DEF, "Mode set to AUTO\n");
            if (dev_unit == TEMP_UNIT_CELSIUS)
            {
                update_device_temp(AUTO_MODE_TARGET_TEMP_DEFAULT_C);
            }
            else
            {
                update_device_temp(AUTO_MODE_TARGET_TEMP_DEFAULT_F);
            }
            break;

        case MODE_OFF:
            //lv_img_set_src(ui_ModeButton, &ui_img_mode_select_fan_png);
            lv_obj_set_style_bg_image_src(ui_ModeButton, &ui_img_mode_select_fan_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_img_set_src(ui_ecoLP, &ui_img_mode_select_fan_png);
            update_fan_mode(FAN_OFF);
            LOG_INFO(CYLF_DEF, "Mode set to FAN\n");
            break;

        default:
            break;
    }

    /* Update current mode label on Active screen */
    update_mode_label(mode);
    dev_current_mode = mode;
}

static int temp_to_heating_arc_angle(int temperature)
{
    if(dev_unit==TEMP_UNIT_FAHRENHEIT){
        int temp_min, temp_max, angle_per_Step;
        temp_min = TEMP_MIN*9/5 + 32;
        temp_max = TEMP_MAX*9/5 + 32;
        angle_per_Step = 300/(temp_max-temp_min);
        if (temperature < temp_min) temperature = temp_min;
        if (temperature > temp_max) temperature = temp_max;
        angle = TEMPERATURE_ARC_START_ANGLE + (temperature - temp_min) * angle_per_Step;
        return clip_angle(angle);
    }
    else{
        if (temperature < TEMP_MIN) temperature = TEMP_MIN;
        if (temperature > TEMP_MAX) temperature = TEMP_MAX;
        angle = TEMPERATURE_ARC_START_ANGLE + (temperature - TEMP_MIN) * TEMPERATURE_ARC_ANGLE_PER_STEP;
        return clip_angle(angle);
    }
}

static int temp_to_cooling_arc_angle(int temperature)
{
    if(dev_unit==TEMP_UNIT_FAHRENHEIT){
        int temp_min, temp_max, angle_per_Step;
        temp_min = TEMP_MIN*9/5 + 32;
        temp_max = TEMP_MAX*9/5 + 32;
        angle_per_Step = 300/(temp_max-temp_min);
        if (temperature < temp_min) temperature = temp_min;
        if (temperature > temp_max) temperature = temp_max;
        angle = TEMPERATURE_ARC_END_ANGLE - (temp_max - temperature) * angle_per_Step;
        return clip_angle(angle);
    }
    else{
        if (temperature < TEMP_MIN) temperature = TEMP_MIN;
        if (temperature > TEMP_MAX) temperature = TEMP_MAX;
        angle = TEMPERATURE_ARC_END_ANGLE - ((TEMP_MAX - temperature) * TEMPERATURE_ARC_ANGLE_PER_STEP);
        return clip_angle(angle);
    }
}

static inline int clip_angle(int angle){
    int ARC_ANGLE_MAX = 405;
    int ARC_ANGLE_MIN = 130;

    if (angle > ARC_ANGLE_MAX ) return ARC_ANGLE_MAX;
    if (angle < ARC_ANGLE_MIN ) return ARC_ANGLE_MIN;
    return angle;
}
void update_device_temp(uint8_t temp)
{
	if((temp < current_min_temp) || (temp > current_max_temp) || (temp == current_temp))
	{
	    LOG_INFO(CYLF_DEF, "Temp is not in range or same as current temp!");
		return;
	}

    /* Update temperature only if thermostat is not in Off mode */
    if (dev_current_mode != MODE_OFF)
    {
        /* Udpate the target temperature */
        target_temp = temp;
        temperature = target_temp;
        dev_info.environment.target_temp = target_temp;

        /* Update temperature value in temperature Arc */
        lv_arc_set_value(ui_ArcTempControl, target_temp);

        /* Update label based on the current temperature unit */
        if (dev_unit == TEMP_UNIT_CELSIUS)
        {
            lv_label_set_text_fmt(ui_currenttemp, "%d°C", temperature);

        }
        else
        {
            lv_label_set_text_fmt(ui_currenttemp, "%d°F", temperature);
        }

        /* Delete temperature timer */
        if (temp_timer)
        {
            lv_timer_del(temp_timer);
            temp_timer = NULL;
        }

        /* If target temperature is greater then current temp., start
         * timer to increase the current temperature and display the
         * red arrows along with animation and heating status on UI. */
        if (target_temp > current_temp)
        {
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);

            lv_obj_set_style_text_color(ui_currenttemp, lv_color_hex(0xF44336), LV_PART_MAIN | LV_STATE_DEFAULT);
            temp_timer = lv_timer_create(increase_temp_step, deg2sec, NULL);
            if (popup_overlay_visible == false)
            {
                //lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
                lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
            }

            current_state = STATE_HEATING;

            /* Update UI with heating label */
            lv_label_set_text_fmt(ui_MainModeLabel, ". . . Heating . . .");

            /* Update UI with time remaining info to reach target temperature */
            generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
            //lv_label_set_text(ui_homescreensubmsg, subinfo);
            //lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);

            lv_obj_remove_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempIndicator, 35);
            
        
            int start_angle = temp_to_heating_arc_angle(current_temp)+1;
            int end_angle   = temp_to_heating_arc_angle(target_temp);

            printf("Start angle: %d, End Angle: %d\n", start_angle, end_angle);
            printf("CT: %d, TT: %d\n", current_temp, target_temp);
          //  lv_arc_set_range(ui_ArcTempIndicator, 0, abs(target_temp - current_temp));
          //  lv_arc_set_value(ui_ArcTempIndicator, abs(target_temp - current_temp)); // Replace current_temp with your value

            lv_arc_set_bg_start_angle(ui_ArcTempIndicator, start_angle);
            lv_arc_set_bg_end_angle(ui_ArcTempIndicator, end_angle);

        }
        /* If target temperature is less then current temp., start
         * timer to decrease the current temperature and display the
         * blue arrows with animation along with the cooling status on UI. */
        else if (target_temp < current_temp)
        {
            //lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);

            lv_obj_set_style_text_color(ui_currenttemp, lv_color_hex(0xC6FFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            if (popup_overlay_visible == false)
            {
                //lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
                lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
            }
            current_state = STATE_COOLING;
            temp_timer = lv_timer_create(decrease_temp_step, deg2sec, NULL);

            /* Update UI with cooling label */
            lv_label_set_text_fmt(ui_MainModeLabel, ". . . Cooling . . .");

            /* Update UI with time remaining info to reach target temperature */
            generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
            //lv_label_set_text(ui_homescreensubmsg, subinfo);
            //lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);

            lv_obj_remove_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_ArcTempIndicator, 15);

            int end_angle = temp_to_cooling_arc_angle(current_temp);
            int start_angle   = temp_to_cooling_arc_angle(target_temp);

            printf("Start angle: %d, End Angle: %d\n", start_angle, end_angle);
            printf("CT: %d, TT: %d\n", current_temp, target_temp);

         //   lv_arc_set_range(ui_ArcTempIndicator, 0, abs(target_temp - current_temp));
          //  lv_arc_set_value(ui_ArcTempIndicator, abs(target_temp - current_temp)); // Replace current_temp with your value

            lv_arc_set_bg_start_angle(ui_ArcTempIndicator, start_angle);
            lv_arc_set_bg_end_angle(ui_ArcTempIndicator, end_angle);
        }
        /* If target temperature is set to current temp., stop all timer
         * and hide all red/blue arrow animations from UI. */
        else
        {
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_ArcTempIndicator, LV_OBJ_FLAG_HIDDEN);

            current_state = STATE_NONE;

            /* If timer running delete the instance */
            if (temp_timer != NULL)
            {
                lv_timer_del(temp_timer);
                temp_timer = NULL;
            }

            /* Hide all the Heating/Cooling labels */
            //lv_obj_add_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
            //lv_obj_add_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);

            /* Update UI with current mode label */
            update_mode_label(dev_current_mode);
        }

        dev_info.environment.target_temp = target_temp;
        dev_info.environment.current_temp = current_temp;

    }
    else
    {
        LOG_INFO(CYLF_DEF, "Increase temperature error. Device in Off mode.\n");
    }

//    /* Update status over MApp/Cloud if connected. */
//    if(true == is_device_connected)
//    {
//        /* Send target temp., current temp. and remaining time data
//         *  to M33 core using IPC */
//        update_temperature_data_ipc(&dev_info);
//    }
}

void display_ble_pairing_window(bool hide, char *code)
{
    if(hide)
    {
        /* Check if Wi-Fi pop up is visible on screen.
         * If visible, don't hide connection screen. */
        if(wifi_popup_state != true)
        {
            lv_obj_add_flag(ui_popupoverlay,LV_OBJ_FLAG_HIDDEN);
//            lv_obj_clear_flag(ui_ArcTempControl,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_ArcGroup,LV_OBJ_FLAG_HIDDEN);
        }
    }
    else
    {
        lv_obj_add_flag(ui_ArcTempControl,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_ArcGroup,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_commissionMapp,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_commissionKeyboard,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_devconnstatecontianer,LV_OBJ_FLAG_HIDDEN);
        //lv_obj_add_flag(ui_blepairingcode,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_qrcodecontainer,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
        //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
        //lv_label_set_text_fmt(ui_pinlabel, "%s", code);
    }
}

fan_speed_t get_current_fan_mode(void)
{
    return dev_fan_mode;
}

thermostat_mode_t get_current_device_mode(void)
{
    return dev_current_mode;
}

uint8_t get_current_brigthness(void)
{
    return brightness_level;
}

int get_current_temperature(void)
{
    return current_temp;
}

void load_thermostat_config(thermostat_mode_t mode)
{
    /* Set Active screen UI components based on current
     * temperature unit. */
    if (current_settings.system.temperature_unit == TEMP_UNIT_FAHRENHEIT)
    {
        /* Update active screen UI components for degree Faranhiet unit */
        current_temp = CELSIUS_TO_FAHRENHEIT(current_temp);
        lv_obj_add_flag(ui_ArcTempIndicator,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_state(ui_tempunitswitch, LV_STATE_CHECKED);
        dev_unit = TEMP_UNIT_FAHRENHEIT;
        lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
        lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
        lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d°F", CELSIUS_TO_FAHRENHEIT(atoi(weather_sync_value)));
        lv_arc_set_range(ui_ArcTempControl, TEMPERATURE_DEG_F_MIN_VALUE, TEMPERATURE_DEG_F_MAX_VALUE);
        lv_arc_set_value(ui_ArcTempControl, current_temp);
        current_max_temp = TEMPERATURE_DEG_F_MAX_VALUE;
        current_min_temp = TEMPERATURE_DEG_F_MIN_VALUE;
        dev_info.environment.current_temp = current_temp;

    }
    else
    {
        /* Update active screen UI components for degree Celcius unit */
        current_temp = (current_temp);
        lv_obj_clear_state(ui_tempunitswitch, LV_STATE_CHECKED);
        dev_unit = TEMP_UNIT_CELSIUS;
        lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
        lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
        lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d°C", atoi(weather_sync_value));
        lv_arc_set_range(ui_ArcTempControl, TEMPERATURE_DEG_C_MIN_VALUE, TEMPERATURE_DEG_C_MAX_VALUE);
        lv_arc_set_value(ui_ArcTempControl, current_temp);
        current_max_temp = TEMPERATURE_DEG_C_MAX_VALUE;
        current_min_temp = TEMPERATURE_DEG_C_MIN_VALUE;
        dev_info.environment.current_temp = current_temp;
    }

    /* Update UI based on the received thermostat mode */
    switch (mode)
    {
        case MODE_ECO: break;
        case MODE_RAPID: break;
        case MODE_AUTO: break;
        case MODE_OFF:
            update_thermostat_mode(mode);
            lv_arc_set_value(ui_ArcTempControl, current_temp);
            target_temp = current_temp;
            temperature = current_temp;
            break;

        default:
            break;
    }

    /* Set fan mode as per the setting */
    update_fan_mode(current_settings.thermostat_setting.fan_mode);

    /* Set audio level as per the setting */
    update_thermostat_volume(current_settings.audio.level);

    /* Set display brightness as per the setting */
    update_display_brightness(current_settings.display_setting.brightness);

    /* Set screen idle timeout as per the setting */
    set_idle_timeout(current_settings.system.idle_timeout);
}

void change_idle_timeout(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Read timeout value from the UI drop down element */
    uint16_t timeout = lv_dropdown_get_selected(ui_timeoutdropdown);
    LOG_INFO(CYLF_DEF, "Idle Timeout: %d\n", timeout);

    /* Set screen timeout period */
    set_idle_timeout(timeout);

    /* Stop currently running timer */
    stop_active_state_timer();

    /* Start inactivity timer with updated timeout period */
    start_inactivity_timer();
}

void update_temperature(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Read set temperature value from temperature arc */
    int temperature = lv_arc_get_value(ui_ArcTempControl);
    if (dev_unit == TEMP_UNIT_CELSIUS)
    {
        LOG_INFO(CYLF_DEF, "Temperature arc value: %d\n", temperature);
    }
    else
    {
        LOG_INFO(CYLF_DEF, "Temperature arc value: %d\n", temperature);
    }

    /* Set the target temperature to the temperature set by
     *  the arc UI element */
    update_device_temp((uint8_t) temperature);
}

void update_final_temperature(lv_event_t *e)
{
    /* Update status over MApp/Cloud if connected. */
    if(true == is_device_connected)
    {
        /* Send target temp., current temp. and remaining time data
         *  to M33 core using IPC */
        update_temperature_data_ipc(&dev_info);
    }
}

void update_thermostat_mode_timer(void)
{
    /* Update the timer running instance based on the target
     * temperature, in case selected mode is not Off.  */
    if (dev_current_mode != MODE_OFF)
    {
        /* Stop the timer if no */
        if (current_temp != target_temp)
        {
            /* Delete the current running timer */
            if (temp_timer)
            {
                lv_timer_del(temp_timer);
                temp_timer = NULL;
            }

            /* In cooling state, start the timer to decrease
             * the current temperature */
            if (current_temp > target_temp)
            {
                temp_timer = lv_timer_create(decrease_temp_step, deg2sec, NULL);
            }
            /* In heating state, start the timer to increase
             * the current temperature */
            else
            {
                temp_timer = lv_timer_create(increase_temp_step, deg2sec, NULL);
            }
        }
    }
}

void set_idle_timeout(idle_timeout_t time)
{
    switch (time)
    {
        case TIMEOUT_3S:
            LOG_INFO(CYLF_DEF, "Timeout = 3 seconds\n");
            break;

        case TIMEOUT_5S:
            LOG_INFO(CYLF_DEF, "Timeout = 5 seconds\n");
            break;

        case TIMEOUT_10S:
            LOG_INFO(CYLF_DEF, "Timeout = 10 seconds\n");
            break;

        case TIMEOUT_20S:
            LOG_INFO(CYLF_DEF, "Timeout = 20 seconds\n");
            break;

        case TIMEOUT_30S:
            LOG_INFO(CYLF_DEF, "Timeout = 30 seconds\n");
            break;

        case TIMEOUT_NEVER:
            LOG_INFO(CYLF_DEF, "Timeout = Never\n");
            break;

        default:
            LOG_ERROR(CYLF_DEF, "Invalid timeout option\n");
            break;
    }

    /* Set device settings timeout as received */
    current_settings.system.idle_timeout = time;

    /* Update the UI element as per recieved timeout */
    lv_dropdown_set_selected(ui_timeoutdropdown, time);

    /* Store the updated setting in NVM */
    update_current_device_setting();
}

void change_volume(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Read audio level info from the drop-down UI element */
    audio_level_t level = lv_dropdown_get_selected(ui_VolumeDrodown);
    LOG_INFO(CYLF_DEF, "Selected Volume:%d\n", level);

    /* Update system volume according to the selected level */
    set_volume(level);

    /* Is device connected to MApp or Cloud */
    if (true == is_device_connected)
    {
        /* Send audio level update info to CM33 core using IPC */
        update_audio_level_ipc(level);
    }

    /* Update audio level info. */
    dev_info.preferences.audio_level = level;
}

void update_thermostat_volume(audio_level_t level)
{
    /* Update system volume according to the passed level */
    set_volume(level);

    /* Update the drop-down UI element to this level */
    lv_dropdown_set_selected(ui_VolumeDrodown, (uint16_t) level);

    /* Update audio level info. */
    dev_info.preferences.audio_level = level;
}

void set_system_unit(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Read temperature unit switch (enabled or not) */
    bool is_checked = lv_obj_has_state(ui_tempunitswitch, LV_STATE_CHECKED);

    /* If enabled, degree set to deg F */
    if (is_checked)
    {
        LOG_INFO(CYLF_DEF, "System Unit set to deg F.\n");
        current_settings.system.temperature_unit = TEMP_UNIT_FAHRENHEIT;

        /* If previous unit was deg C, convert to deg F */
        if (dev_unit == TEMP_UNIT_CELSIUS)
        {
            current_temp = CELSIUS_TO_FAHRENHEIT(current_temp);
            target_temp = CELSIUS_TO_FAHRENHEIT(target_temp);
            temperature = CELSIUS_TO_FAHRENHEIT(temperature);
        }
        //lv_obj_add_flag(ui_activeBGImg, LV_OBJ_FLAG_HIDDEN);
        /* Update UI for deg F temperature data */
        dev_unit = TEMP_UNIT_FAHRENHEIT;
        lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
        lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
        lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d°F", CELSIUS_TO_FAHRENHEIT(atoi(weather_sync_value)));
        lv_arc_set_range(ui_ArcTempControl, TEMPERATURE_DEG_F_MIN_VALUE, TEMPERATURE_DEG_F_MAX_VALUE);
        lv_arc_set_value(ui_ArcTempControl, target_temp);
        current_max_temp = TEMPERATURE_DEG_F_MAX_VALUE;
        current_min_temp = TEMPERATURE_DEG_F_MIN_VALUE;
        lv_label_set_text_fmt(ui_currenttemp, "%d°F", target_temp);

        dev_info.environment.target_temp = target_temp;
        dev_info.environment.current_temp = current_temp;
    }
    else
    {
        LOG_INFO(CYLF_DEF, "System Unit set to deg C.\n");
        current_settings.system.temperature_unit = TEMP_UNIT_CELSIUS;

        /* If previous unit was deg F, convert to deg C */
        if (dev_unit == TEMP_UNIT_FAHRENHEIT)
        {
            current_temp = FAHRENHEIT_TO_CELSIUS(current_temp);
            target_temp = FAHRENHEIT_TO_CELSIUS(target_temp);
            temperature = FAHRENHEIT_TO_CELSIUS(temperature);
        }
        //lv_obj_clear_flag(ui_activeBGImg, LV_OBJ_FLAG_HIDDEN);
        /* Update UI for deg C temperature data */
        dev_unit = TEMP_UNIT_CELSIUS;
        lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
        lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
        lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d°C", atoi(weather_sync_value));
        lv_arc_set_range(ui_ArcTempControl, TEMPERATURE_DEG_C_MIN_VALUE, TEMPERATURE_DEG_C_MAX_VALUE);
        lv_arc_set_value(ui_ArcTempControl, target_temp);
        current_max_temp = TEMPERATURE_DEG_C_MAX_VALUE;
        current_min_temp = TEMPERATURE_DEG_C_MIN_VALUE;
        lv_label_set_text_fmt(ui_currenttemp, "%d°C", target_temp);
        dev_info.environment.target_temp = target_temp;
        dev_info.environment.current_temp = current_temp;
    }

    /* Send updated device config. to CM33 core using IPC */
    update_device_config_ipc();

    /* Store updated device settings */
    update_current_device_setting();
}

void set_background(lv_event_t * e){
    UNUSED_PARAM(e);

    /* Read temperature unit switch (enabled or not) */
    bool is_checked = lv_obj_has_state(ui_BGswitch, LV_STATE_CHECKED);
    if(!is_checked){
        lv_obj_add_flag(ui_activeBGImg, LV_OBJ_FLAG_HIDDEN);
        //lv_obj_clear_flag(ui_TemperatureArcBgPanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_image_src(ui_TemperatureArcBgPanel, &ui_temp_arc, LV_PART_MAIN | LV_STATE_DEFAULT); 
    }
    else{
        lv_obj_clear_flag(ui_activeBGImg, LV_OBJ_FLAG_HIDDEN);
        //lv_obj_add_flag(ui_TemperatureArcBgPanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_image_src(ui_TemperatureArcBgPanel, NULL, LV_PART_MAIN | LV_STATE_DEFAULT); 
    }
}

void update_system_unit(temp_unit_t unit)
{
    /* Based on the selected system unit convert all temperature
     * reading on UI to Celcius or Faranhiet */
    if (TEMP_UNIT_FAHRENHEIT == unit)
    {
        /* If previous unit was C, convert data to F */
        if (TEMP_UNIT_CELSIUS == dev_unit)
        {
            current_temp = CELSIUS_TO_FAHRENHEIT(current_temp);
            target_temp = CELSIUS_TO_FAHRENHEIT(target_temp);
            temperature = CELSIUS_TO_FAHRENHEIT(temperature);
        }

        /* Update all labels on UI (Active and Sleep) screen */
        lv_obj_add_state(ui_tempunitswitch, LV_STATE_CHECKED);
        lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
        lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
        lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d°F", CELSIUS_TO_FAHRENHEIT(atoi(weather_sync_value)));
        lv_arc_set_range(ui_ArcTempControl, TEMPERATURE_DEG_F_MIN_VALUE, TEMPERATURE_DEG_F_MAX_VALUE);
        lv_arc_set_value(ui_ArcTempControl, target_temp);
        current_max_temp = TEMPERATURE_DEG_F_MAX_VALUE;
        current_min_temp = TEMPERATURE_DEG_F_MIN_VALUE;
        lv_label_set_text_fmt(ui_currenttemp, "%d°F", target_temp);
        dev_info.environment.target_temp = target_temp;
        dev_info.environment.current_temp = current_temp;
    }
    else if (TEMP_UNIT_CELSIUS == unit)
    {
        /* If previous unit was F, convert data to C */
        if (TEMP_UNIT_FAHRENHEIT == dev_unit)
        {
            current_temp = FAHRENHEIT_TO_CELSIUS(current_temp);
            target_temp = FAHRENHEIT_TO_CELSIUS(target_temp);
            temperature = FAHRENHEIT_TO_CELSIUS(temperature);
        }

        /* Update all labels on UI (Active and Sleep) screen */
        lv_obj_clear_state(ui_tempunitswitch, LV_STATE_CHECKED);
        lv_label_set_text_fmt(ui_TemperatureCurrValueLbl, "%d", current_temp);
        lv_label_set_text_fmt(ui_MainTemptextLP, "%d", current_temp);
        lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d°C", atoi(weather_sync_value));
        lv_arc_set_range(ui_ArcTempControl, TEMPERATURE_DEG_C_MIN_VALUE, TEMPERATURE_DEG_C_MAX_VALUE);
        lv_arc_set_value(ui_ArcTempControl, target_temp);
        current_max_temp = TEMPERATURE_DEG_C_MAX_VALUE;
        current_min_temp = TEMPERATURE_DEG_C_MIN_VALUE;
        lv_label_set_text_fmt(ui_currenttemp, "%d°C", target_temp);
        dev_info.environment.target_temp = target_temp;
        dev_info.environment.current_temp = current_temp;
    }

    /* Update current temperature unit */
    current_settings.system.temperature_unit = unit;
    dev_unit = unit;

    /* Store device settings in NVM */
    update_current_device_setting();
}

void get_default_device_setting(device_settings_t *settings)
{
    memset(settings, 0, sizeof(device_settings_t));

    /* Copy default device settings into passed argument */
    memcpy(settings, &default_config, sizeof(device_settings_t));
}

void get_current_device_setting(device_settings_t *settings)
{
    memset(settings, 0, sizeof(device_settings_t));

    /* Copy current device settings into passed argument */
    memcpy(settings, &current_settings, sizeof(device_settings_t));
}

void set_current_device_setting(device_settings_t *settings)
{
    /* Update current device settings with the ones given as argument */
    current_settings.audio.level = settings->audio.level;
    current_settings.display_setting.brightness = settings->display_setting.brightness;
    current_settings.thermostat_setting.fan_mode = settings->thermostat_setting.fan_mode;
    current_settings.thermostat_setting.mode = settings->thermostat_setting.mode;
    current_settings.is_available = settings->is_available;
    current_settings.system.idle_timeout = settings->system.idle_timeout;
    current_settings.system.temperature_unit = settings->system.temperature_unit;
}

void update_current_device_setting(void)
{
    eeprom_wr_setting = true;
}

void device_factory_reset(lv_event_t *e)
{
    UNUSED_PARAM(e);
    device_settings_t read_settings = { 0 };

    /* Read device default settings */
    get_default_device_setting(&read_settings);

    /* Write default settings to current settings */
    set_current_device_setting(&read_settings);

    /* Store updated current settings to NVM*/
    update_current_device_setting();

    /* Update flags as per the settings*/
    dev_current_mode = read_settings.thermostat_setting.mode;
    dev_fan_mode = read_settings.thermostat_setting.fan_mode;
    brightness_level = read_settings.display_setting.brightness;
    audio_level = read_settings.audio.level;

    /* Update UI as per the thermostat mode */
    update_thermostat_mode(dev_current_mode);

    /* Set default audio config */
    update_thermostat_volume(audio_level);

    /* Set brightness to default */
    update_display_brightness(brightness_level);

    /* Set screen timeout to default */
    set_idle_timeout(read_settings.system.idle_timeout);

    /* Update temperature unit settings */
    if (read_settings.system.temperature_unit == TEMP_UNIT_FAHRENHEIT)
    {
        lv_obj_add_state(ui_tempunitswitch, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(ui_tempunitswitch, LV_STATE_CHECKED);
    }

//    _ui_flag_modify(ui_ArcTempControl, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    _ui_flag_modify(ui_ArcGroup, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    lv_scr_load(ui_ActiveScreen);

    /* Send factory reset command over IPC
     * to delete the WiFi credentials */
    request_wifi_delete_ipc();

    /* Update device provisioned state */
    is_device_provisioned = false;
    
}

void update_device_config_ipc(void)
{
    /* Get snaphsot of current configuration */
    dev_info.environment.current_temp = current_temp;
    dev_info.environment.target_temp = target_temp;

    if(read_ppm != 0)
    {
        dev_info.environment.current_co2_level = read_ppm;
    }
    else
    {
        dev_info.environment.current_co2_level = 400;
    }

    if(read_humidity != 0)
    {
        dev_info.environment.current_humidity = read_humidity/1000;
    }
    else
    {
        dev_info.environment.current_humidity = 51;
    }

    dev_info.thermostat_settings.fan_speed = dev_fan_mode;
    dev_info.thermostat_settings.mode = dev_current_mode;
    dev_info.thermostat_settings.time_remains = 0;
    dev_info.preferences.display_brightness = brightness_level;
    dev_info.preferences.audio_level = audio_level;
    dev_info.thermostat_settings.temp_unit = dev_unit;

    /* Send configuration to CM33 core using IPC */
    send_device_config(dev_info);
}

void start_inactivity_timer(void)
{
    uint32_t addn_timeout_ms = 0;
    /* Start inactivity timer if Screen timeout is not set to Never */
    if (TIMEOUT_NEVER != current_settings.system.idle_timeout)
    {
        if (person_detected == true)
        {
            /* Add additional timeout when person detected so screen doesn't immediately transition */
            addn_timeout_ms = 5000 /* 5 seconds */;
        }
        /* Start timer with screen timeout duration */
        start_active_state_timer(addn_timeout_ms + get_timeout_ms(current_settings.system.idle_timeout));
    }
}

// TODO - this is temp, idle transition should be done on completion of absence UI update
void start_inactivity_timer_addn(uint32_t additional_timeout_ms)
{
    /* If configured timeout is set to never, do nothing */
    if (TIMEOUT_NEVER == current_settings.system.idle_timeout)
    {
        return;
    }

    uint32_t base_ms = get_timeout_ms(current_settings.system.idle_timeout);

    start_active_state_timer(base_ms + additional_timeout_ms);
}

void cancel_dev_conn_current_opt(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Update UI if cancel button event is received */
    if (DEV_ST_BLE_ADVERTISING == dev_current_conn_state)
    {
        /* Update cancel event over IPC */
        update_connection_state_ipc(DEV_ST_BLE_ADVERTISEMENT_CANCEL);

        /* If last state was un-provisioned, move back to un-provisioned state */
        if ((!is_device_provisioned) || (DEV_ST_UNPROVISIONED == dev_last_conn_state))
        {
            lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            update_device_connection_state(DEV_ST_UNPROVISIONED);
        }
        else if ((is_device_provisioned) && (DEV_ST_BLE_ADVERTISING == dev_last_conn_state))
        {
            /* Clear auto-reconnect switch */
            lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);

            /* Update UI with Wi-Fi connecting status */
            update_device_connection_state(DEV_ST_WIFI_CONNECTING);

            /* Send switch to Wi-Fi command to CM33 core using IPC */
//            update_switch_wifi_ipc();
        }
        else
        {
            lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            update_device_connection_state(DEV_ST_UNPROVISIONED);
        }
    }
    else if (DEV_ST_WIFI_CONNECTING == dev_current_conn_state)
    {
        /* Update cancel event over IPC */
        update_connection_state_ipc(DEV_ST_WIFI_CONNECTION_CANCEL);

        /* If last state was un-provisioned, move back to un-provisioned state */
        if (DEV_ST_UNPROVISIONED == dev_last_conn_state)
        {
            lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            update_device_connection_state(DEV_ST_UNPROVISIONED);
        }
        else if (DEV_ST_WIFI_CONNECTING == dev_last_conn_state)
        {
            /* Clear auto-reconnect switch */
            lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);

            /* Update UI with Wi-Fi disconnected status */
            if (true == ble_conn_state)
            {
                update_device_connection_state(DEV_ST_BLE_CONNECTED);
            }
            else
            {
                /* Check if connected to WiFi. */
                if (true == wifi_conn_state)
                {
                    update_device_connection_state(DEV_ST_WIFI_DISCONNECTED);
                }
                else
                {
                    update_device_connection_state(DEV_ST_BLE_ADVERTISING);
                }
            }
        }
        else
        {
            lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            update_device_connection_state(DEV_ST_UNPROVISIONED);
        }
    }
    else if (DEV_ST_CLOUD_CONNECTING == dev_current_conn_state)
    {
        /* Update cancel event over IPC */
        update_connection_state_ipc(DEV_ST_CLOUD_CONNECTION_CANCEL);

        /* If last state was un-provisioned, move back to un-provisioned state */
        if (DEV_ST_UNPROVISIONED == dev_last_conn_state)
        {
            lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            update_device_connection_state(DEV_ST_UNPROVISIONED);
        }
        else if (DEV_ST_CLOUD_CONNECTING == dev_last_conn_state)
        {
            /* Clear auto-reconnect switch */
            lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);

            /* Update UI with Cloud disconnected status */
            if (true == ble_conn_state)
            {
                update_device_connection_state(DEV_ST_BLE_CONNECTED);
            }
            else
            {
                /* If Wi-Fi connected */
                if(true == wifi_conn_state)
                {
                    update_device_connection_state(DEV_ST_CLOUD_DISCONNECTED);
                }
                else
                {
                    update_device_connection_state(DEV_ST_BLE_ADVERTISING);
                }
            }
        }
        else
        {
            lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            update_device_connection_state(DEV_ST_UNPROVISIONED);
        }
    }
    else
    {
        LOG_ERROR(CYLF_DEF, "Un-handled cancel event received: Current event: %d\n", dev_current_conn_state);
    }
}

void auto_reconnect_cb(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Update connection attempt based on the value of auto-reconnect switch */
    if (lv_obj_has_state(ui_Switch2, LV_STATE_CHECKED))
    {
        /* Update infinite connection attempts based on current conn state */
        if ((DEV_ST_WIFI_CONNECTING == dev_current_conn_state) ||
                (DEV_ST_WIFI_DISCONNECTED == dev_current_conn_state))
        {
            /* Update UI with Wi-Fi connection screen */
            update_device_connection_state(DEV_ST_WIFI_CONNECTING);

            /* Update connection retries over IPC */
            update_connection_retries_ipc(WIFI_CONN_RETRY_INFINITE);
        }
        else if ((DEV_ST_CLOUD_CONNECTING == dev_current_conn_state)
                || (DEV_ST_CLOUD_DISCONNECTED == dev_current_conn_state))
        {
            /* Update UI with Cloud connection screen */
            update_device_connection_state(DEV_ST_CLOUD_CONNECTING);

            /* Update connection retries over IPC */
            update_connection_retries_ipc(CLOUD_CONN_RETRY_INFINITE);
        }
        else if (DEV_ST_BLE_ADVERTISING == dev_current_conn_state)
        {
            /* Update UI with BLE advertising screen */
            update_device_connection_state(DEV_ST_BLE_ADVERTISING);

            /* Update connection retries over IPC */
            update_connection_retries_ipc(BLE_CONN_RETRY_INFINITE);
        }
    }
    else
    {
        /* Update limited connection attempts based on current conn. state */
        if ((DEV_ST_WIFI_CONNECTING == dev_current_conn_state) ||
                (DEV_ST_WIFI_DISCONNECTED == dev_current_conn_state))
        {
            update_connection_retries_ipc(WIFI_CONN_RETRY_LIMITED);
        }
        else if ((DEV_ST_CLOUD_CONNECTING == dev_current_conn_state)
                || (DEV_ST_CLOUD_DISCONNECTED == dev_current_conn_state))
        {
            update_connection_retries_ipc(CLOUD_CONN_RETRY_LIMITED);
        }
        else if (DEV_ST_BLE_ADVERTISING == dev_current_conn_state)
        {
            update_connection_retries_ipc(BLE_CONN_RETRY_LIMITED);
        }
    }
}

void switch_to_wifi(lv_event_t *e)
{
    UNUSED_PARAM(e);

    /* Update UI with Wi-Fi connecting status */
    update_device_connection_state(DEV_ST_WIFI_CONNECTING);

    /* Send switch to Wi-Fi command to CM33 core using IPC */
    update_switch_wifi_ipc();
}

void update_co2_data_ui(uint16_t ppm)
{
    char ppm_str[6];
    snprintf(ppm_str, sizeof(ppm_str), "%u", ppm);
    lv_label_set_text(ui_activeCO2, ppm_str);

    char ppm_lp_str[10];
    snprintf(ppm_lp_str, sizeof(ppm_lp_str), "%u ppm", ppm);
    lv_label_set_text(ui_Co2LP, ppm_lp_str);
}

void switch_to_active_screen(void)
{
    lv_obj_t *current_screen = lv_scr_act();

    if(current_screen == ui_LPScreen)
    {
        _ui_screen_change(&ui_ActiveScreen, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_ActiveScreen_screen_init);
        app_state = APP_ST_ACTIVE;
        
        /* Update sensor sampling interval to 1s */
        /* Set sensor sampling interval to IDLE state */
        set_sensor_sampling_interval(SENSOR_SAMPLING_INTERVAL_ACTIVE);
    }

    start_inactivity_timer();
}

void switch_to_ble(lv_event_t *e)
{
    /* Update UI with BLE ADV status */
    update_device_connection_state(DEV_ST_BLE_ADVERTISING);

    /* Send switch to BLE command to CM33 core using IPC */
    send_switch_to_ble_cmd();

    is_device_connected = false;
}

// static void update_time_labels(void)
// {
//     char buf[3];    // Enough for "00\0"

//     /* Update hour */
//     snprintf(buf, sizeof(buf), "%02d", hour);
//     lv_label_set_text(ui_htext, buf);

//     /* Update minute */
//     snprintf(buf, sizeof(buf), "%02d", minute);
//     lv_label_set_text(ui_mtext, buf);

//     /* Update second */
//     snprintf(buf, sizeof(buf), "%02d", second);
//     lv_label_set_text(ui_stext, buf);
// }

// void inc_thour(lv_event_t * e)
// {
//     LV_UNUSED(e);
//     sscanf(lv_label_get_text(ui_htext), "%d", &hour);
//     hour = (hour + 1) % 24;   // Wrap around 0-23

//     char buf[3];   // Enough for "00\0"
//     snprintf(buf, sizeof(buf), "%02d", hour);
//     lv_label_set_text(ui_htext, buf);
// }

// void inc_tmin(lv_event_t * e)
// {
//     LV_UNUSED(e);
//     sscanf(lv_label_get_text(ui_mtext), "%d", &minute);
//     minute = (minute + 1) % 60;   // Wrap around 0-59

//     char buf[3];   // Enough for "00\0"
//     snprintf(buf, sizeof(buf), "%02d", minute);
//     lv_label_set_text(ui_mtext, buf);
// }

// void inc_tsec(lv_event_t * e)
// {
//     LV_UNUSED(e);
//     sscanf(lv_label_get_text(ui_stext), "%d", &second);
//     second = (second + 1) % 60;   // Wrap around 0-59

//     char buf[3];   // Enough for "00\0"
//      snprintf(buf, sizeof(buf), "%02d", second);
//      lv_label_set_text(ui_stext, buf);
//  }

// void dec_thour(lv_event_t * e)
// {
//     LV_UNUSED(e);
//     sscanf(lv_label_get_text(ui_htext), "%d", &hour);
//     hour = (hour == 0) ? 23 : hour - 1;   // Wrap around backwards

//     char buf[3];   // Enough for "00\0"
//     snprintf(buf, sizeof(buf), "%02d", hour);
//     lv_label_set_text(ui_htext, buf);
// }

// void dec_tmin(lv_event_t * e)
// {
//     LV_UNUSED(e);
//     sscanf(lv_label_get_text(ui_mtext), "%d", &minute);
//     minute = (minute == 0) ? 59 : minute - 1;

//     char buf[3];   // Enough for "00\0"
//     snprintf(buf, sizeof(buf), "%02d", minute);
//     lv_label_set_text(ui_mtext, buf);
// }

// void dec_tsec(lv_event_t * e)
// {
//     LV_UNUSED(e);
//     sscanf(lv_label_get_text(ui_stext), "%d", &second);
//     second = (second == 0) ? 59 : second - 1;

//     char buf[3];   // Enough for "00\0"
//      snprintf(buf, sizeof(buf), "%02d", second);
//      lv_label_set_text(ui_stext, buf);
// }


void update_date_time_rtc(lv_event_t * e)
{
    // Ensure the event is a click event
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
    {
        return;
    }

    // A temporary structure to hold the new time and date
    cy_stc_rtc_config_t new_time = {0};
    cy_en_rtc_status_t rtc_status = CY_RTC_BAD_PARAM;
    uint32_t retry_count = 5;

    /* Get current RTC config. */
    Cy_RTC_GetDateAndTime(&new_time);

    // // Read the selected date from the calendar
    // lv_calendar_date_t sel_date;
    // if (lv_calendar_get_pressed_date(ui_dtCalendar, &sel_date))
    // {
    //     new_time.date = sel_date.day;
    //     new_time.month = sel_date.month;
    //     new_time.year = sel_date.year - RTC_CENTURY; // Using RTC_CENTURY macro
    // }

    // // Read values from the LVGL labels for time
    // // You should use lv_textarea_get_text if they are text areas
    // sscanf(lv_label_get_text(ui_htext), "%2d", &new_time.hour);
    // sscanf(lv_label_get_text(ui_mtext), "%2d", &new_time.min);
    // sscanf(lv_label_get_text(ui_stext), "%2d", &new_time.sec);

    // Calculate the day of the week from the date
    struct tm time_info = {
        .tm_sec = new_time.sec,
        .tm_min = new_time.min,
        .tm_hour = new_time.hour,
        .tm_mday = new_time.date,
        .tm_mon = new_time.month - 1, // struct tm uses 0-11 for months
        .tm_year = new_time.year + (RTC_CENTURY - TM_YEAR_BASE) // Using macros for offset
    };
    mktime(&time_info);

    // The tm_wday field now holds the day of the week (0=Sunday, 6=Saturday)
    new_time.dayOfWeek = time_info.tm_wday + 1; // Cy_RTC_SetDateAndTime expects 1-7


    // Validate the time values
    if (new_time.sec < 60 && new_time.min < 60 && new_time.hour < 24)
    {
        // Set the new date and time in the RTC with a retry mechanism
        do
        {
            rtc_status = Cy_RTC_SetDateAndTime(&new_time);
            retry_count--;
            if (rtc_status != CY_RTC_SUCCESS)
            {
                // Delay for 1ms before retrying to allow the RTC to become ready
                vTaskDelay(pdMS_TO_TICKS(1));
            }
        } while ((rtc_status != CY_RTC_SUCCESS) && (retry_count > 0));


        if (rtc_status == CY_RTC_SUCCESS)
        {
            /* Update current time stamp on UI */
            update_time_on_ui();
            update_timestamp = true;
        }
        else
        {
            // Handle error, e.g., RTC was busy
//            handle_app_error();
            APP_ERROR(1);
        }

        stop_minute_sync_timer();
        start_minute_sync_timer();

    }
    else
    {
        // Handle invalid time input
        // Example: display_notification("Invalid time entered!");
    }
}

// void update_calendar_date(lv_event_t * e)
// {
//     /* Get the selected date from calendar */
//     lv_obj_t * cal = ui_dtCalendar;
//     lv_calendar_date_t sel_date;
//     bool valid = lv_calendar_get_pressed_date(cal, &sel_date);

//     if(valid) {
//         char buf[16];  // Enough for "DD/MM/YYYY"
//         snprintf(buf, sizeof(buf), "%02d/%02d/%04d",
//                  sel_date.day,
//                  sel_date.month,
//                  sel_date.year);

//         /* Set to your text area */
//         lv_textarea_set_text(ui_datetimetextarea, buf);
//     }
// }

static void update_firmware_label(lv_timer_t *timer)
{
	if(new_FW_version[0] != 0)
	{

    // Write "2.0.0" to the hidden firmware details label.
    lv_label_set_text(ui_getfwverdetails, new_FW_version);

    // Hide the spinner once the text is updated.
    _ui_flag_modify(ui_fwcheckspinner, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);

    /* Un-Hide the fw details */
    _ui_flag_modify(ui_getfwverdetails, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);

    /* Hide the check update button */
    _ui_flag_modify(ui_checkupdtbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);

		/* UnHide the download update button */
		_ui_flag_modify(ui_downloadfwbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);

	}
	else
	{
		// Hide the spinner once the text is updated.
		_ui_flag_modify(ui_fwcheckspinner, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);

		revert_fw_update_screen();
	}
}

void revert_fw_update_screen()
{
	if(NULL != ui_checkupdtbtn)
	{
        /* UnHide the check update button */
        _ui_flag_modify(ui_checkupdtbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);

        /* UnHide the download update button */
        _ui_flag_modify(ui_downloadfwbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
	}
}

void check_fw_update_version(lv_event_t * e)
{
    // Create an LVGL timer to show the firmware version after 5 seconds
	memset(new_FW_version, 0, MAX_FW_VERSION_LEN);
	get_latest_OTA_version();
    lv_timer_create(update_firmware_label, 3000, NULL);
}

void update_fw_download_status(uint8_t percent)
{
    //lv_bar_set_value(ui_Bar2, percent, LV_ANIM_OFF);
}

void trigger_ota(lv_event_t * e)
{
	if(strcmp(m55_current_OTA_version, new_FW_version) == 0)
	{
        _ui_screen_change(&ui_ActiveScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 230, 0, &ui_ActiveScreen_screen_init);
        enqueue_notification(NOTIFY_FW_UPDATE, NOTIF_FAIL, DEV_ST_FW_HAVE_SAME_VERSION);
	}
	else
	{
        /* Hide spinner and label */
        lv_obj_add_flag(ui_FWUpdatespinner, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Fwupdatespinrlabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Fwupdatelatestlbl, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_fwdownloadbtnlbl, LV_OBJ_FLAG_HIDDEN);
        
        /* Update FW update screen */
        char msg[70];
        snprintf(msg, sizeof(msg), "Downloading F.W version %s\nPlease wait . . .", new_FW_version);
        lv_label_set_text(ui_Fwupdatelatestlbl, msg);
        lv_obj_clear_flag(ui_Fwupdatelatestlbl, LV_OBJ_FLAG_HIDDEN);
        
        /* Load FW update screen */
        _ui_screen_change(&ui_FWUpdateScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 230, 0, &ui_FWUpdateScreen_screen_init);

        trigger_ota_update();		
	}
}

void start_ota_process(lv_event_t * e)
{
	lv_obj_add_flag(ui_Fwupdatelatestlbl, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_fwdownloadbtnlbl, LV_OBJ_FLAG_HIDDEN);
	
	char msg[70];
	snprintf(msg, sizeof(msg), "Downloading F.W version %s\nPlease wait . . .", new_FW_version);       	

	lv_label_set_text(ui_Fwupdatelatestlbl, msg);
	lv_obj_clear_flag(ui_Fwupdatelatestlbl, LV_OBJ_FLAG_HIDDEN);
	
	/* Start OTA process */
	trigger_ota_update();		
}

void start_check_fw_update_timer(void)
{
    BaseType_t xStatus;

    /* Check if the timer handle exists (i.e., if it has been created) */
    if (fw_update_check_timer == NULL)
    {
        fw_update_check_timer = xTimerCreate(
            "FWUpdateCheckTimer",
            pdMS_TO_TICKS(10000),
            pdFALSE,
            NULL,
            fw_update_check_cb
        );

        if (fw_update_check_timer == NULL)
        {
			LOG_ERROR(CYLF_DEF, "Failed to create FWUpdateCheckTimer");
            return;
        }
    }

    /* Check if the timer is currently running (active) */
    if (xTimerIsTimerActive(fw_update_check_timer) == pdTRUE)
    {
        /* Stop the timer if it is active */
        xStatus = xTimerStop(fw_update_check_timer, 0);
        if (xStatus != pdPASS)
        {
 			LOG_ERROR(CYLF_DEF, "Failed to stop FWUpdateCheckTimer");
        }
    }

    /* Start the timer (or restart it if it was stopped) */
    xStatus = xTimerStart(fw_update_check_timer, 0);
    if (xStatus != pdPASS)
    {
		LOG_ERROR(CYLF_DEF, "Failed to start FWUpdateCheckTimer");
    }
}

void display_fan_anim(void)
{
    /* Update the fan animation based on the global fan mode variable. */
    update_fan_mode(dev_fan_mode);
}

void stop_fan_anim(void)
{
    /* Delete fan mode animation for all possible states. */
    lv_anim_del(ui_fanoff, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    /* Hide all the animated fan images initially. */
    lv_obj_add_flag(ui_fanoff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

    /* Clear the hidden flag for the main fan image and start the animation from a stopped state. */
     lv_obj_clear_flag(ui_fanoff, LV_OBJ_FLAG_HIDDEN);
     fanspeed_Animation(ui_fanoff, 0);
}

void update_homescreen_connectivity_state(void)
{
    update_device_connection_state(dev_current_conn_state);
}

void stop_homescreen_connectivity_state(void)
{
    const char *state_text = "Unknown";

    /* Hide all hidden widgets (images, button, labels) initially */
    lv_obj_add_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_commissionKeyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_autoreconnectpanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connstatekeyboardbtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
    //lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_qrcodecontainer, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_cloudconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wificonnecting120, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_clouddisconn120, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleadv, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_devstateimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wificonnectedimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifidisabledimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleconnected120, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homebleconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleconnected50, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homecloudconnimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_qrcodebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_qrcodebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_cloudconnecting, LV_OBJ_FLAG_HIDDEN);
    //lv_obj_add_flag(ui_connectLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionprg1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionprg2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionprg3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_switchtowififromble, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_switchtowififrombleinfolbl, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_devconwificonnectionscreen, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homebleconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleconnected50, LV_OBJ_FLAG_HIDDEN);
    //lv_obj_add_flag(ui_bleswitchbtnlbl, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleswitchbtn, LV_OBJ_FLAG_HIDDEN);

    /* Delete all running animations associated with connectivity. */
    lv_anim_del(ui_bleadv, NULL);
    lv_anim_del(ui_wificonnecting120, NULL);
    lv_anim_del(ui_wifi, NULL);
    lv_anim_del(ui_homebleconnected, NULL);
    lv_anim_del(ui_wifi, NULL);
    lv_anim_del(ui_connectionprg1, NULL);
    lv_anim_del(ui_connectionprg2, NULL);
    lv_anim_del(ui_connectionprg3, NULL);

    /* Update the UI based on the current device connection state. */
    switch (dev_current_conn_state)
    {
        case DEV_ST_CLOUD_CONNECTING:
            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homewifidisconnimg, 255, 0);
            break;

        case DEV_ST_UNPROVISIONED:
        case DEV_ST_WIFI_CONNECTING:
            /* Update icon on home screen */
            lv_obj_add_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_wifi, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            break;

        case DEV_ST_BLE_ADVERTISING:
        case DEV_ST_BLE_PAIRING:
            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homebleconnected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homebleconnected, 255, 0);
            break;

        case DEV_ST_WIFI_DISCONNECTED:
            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homewifidisconnimg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homewifidisconnimg, 255, 0);
            break;

        case DEV_ST_BLE_CONNECTED:
            /* Update icon on home screen */
            lv_obj_clear_flag(ui_homebleconnected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homebleconnected, 255, 0);
            break;

        default:
            break;
    }

    /* Update device state label in popup screen */
    lv_label_set_text(ui_devstatelabel, state_text);
}

void display_temp_change_anim(void)
{
    if (current_state == STATE_HEATING)
    {
        lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
        //lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
    }
    else if (current_state == STATE_COOLING)
    {
        lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
        //lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
    }
}

void app_timer_handler(lv_timer_t *timer)
{
    _ui_screen_change(&ui_LPScreen, LV_SCR_LOAD_ANIM_FADE_ON, 230, 0, &ui_LPScreen_screen_init);
    lv_timer_del(app_timer);
    app_timer = NULL;
    app_state = APP_ST_IDLE;

    /* Set sensor sampling interval to IDLE state */
    set_sensor_sampling_interval(SENSOR_SAMPLING_INTERVAL_IDLE);
}

void start_active_state_timer(uint32_t timeout_ms)
{
    if (app_timer == NULL)
    {
        // Create timer with LV_TIMER_RUN_ONCE flag
        app_timer = lv_timer_create_basic();  // Basic timer (manual config)
        lv_timer_set_cb(app_timer, app_timer_handler);
        lv_timer_set_repeat_count(app_timer, 1);  // One-shot
    }

    lv_timer_set_period(app_timer, timeout_ms);
    lv_timer_reset(app_timer);  // Restart timer countdown
    lv_timer_resume(app_timer); // Ensure it's active
    app_state = APP_ST_ACTIVE;
}

void stop_active_state_timer(void)
{
    if (app_timer != NULL)
    {
        lv_timer_pause(app_timer);
        lv_timer_reset(app_timer);
        //printf("Active state timer stopped.\n");
    }
}

void update_co2_aqi_indicator(uint32_t co2_val)
{
	/* Update color code based on CO2 value */
	if (co2_val < CO2_LEVEL_THRESHOLD_1_GOOD)
	{
		lv_image_set_src(ui_CO2LevelsImg, &ui_img_aqilevel_1_png);
	}
	else if (co2_val < CO2_LEVEL_THRESHOLD_2_MODERATE)
	{
		lv_image_set_src(ui_CO2LevelsImg, &ui_img_aqilevel_2_png);
	}
    else if (co2_val < CO2_LEVEL_THRESHOLD_3_POOR)
	{
		lv_image_set_src(ui_CO2LevelsImg, &ui_img_aqilevel_3_png);
	}
	else
	{
		lv_image_set_src(ui_CO2LevelsImg, &ui_img_aqilevel_4_png);
	}
}


/* [] END OF FILE */
