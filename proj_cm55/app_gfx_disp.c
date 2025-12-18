/*******************************************************************************
* File Name        : app_gfx_disp.c
*
* Description      : This source file contains the gfx task routine for 
*                    application running on CM55 CPU.
*
* Related Document : See README.md
*
********************************************************************************
* Copyright 2024-2025, Cypress Semiconductor Corporation (an Infineon company) or
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
* Header Files
*******************************************************************************/
#include "cybsp.h"

#include "retarget_io_init.h"

#include "vg_lite.h"
#include "vg_lite_platform.h"

/* RTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "cyabs_rtos.h"
#include "cyabs_rtos_impl.h"

#include "cy_time.h"
#include "semphr.h"
#include "lvgl.h"
#include "ui.h"
#if defined(MTB_DISPLAY_WS7P0DSI_RPI)
#include "mtb_disp_ws7p0dsi_drv.h"
#elif defined(MTB_DISPLAY_EK79007AD3)
#include "mtb_display_ek79007ad3.h"
#elif defined(MTB_DISPLAY_W4P3INCH_RPI)
#include "mtb_disp_dsi_waveshare_4p3.h"
#elif defined(MTB_DISPLAY_R4INCH_TFT)
#include "mtb_display_st7701s.h"
#endif
#include "lv_qrcode.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "demos/lv_demos.h"
#include "ui/ui.h"
#include "app_common.h"
#include "thermostat_events.h"
#include "comm_manager.h"
#include "app_eeprom.h"
#include "app_sensor.h"
#include "app_speaker.h"
#include "xensiv_pasco2_mtb.h"

/* Application related includes */
#include "app_gfx_disp.h"
#include "app_voice_control.h"
#include "voice_assistant.h"
#include "app_rtc.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define GPU_INT_PRIORITY                    (3U)
#define DC_INT_PRIORITY                     (3U)

#define APP_BUFFER_COUNT                    (2U)
/* 64 KB */
#define DEFAULT_GPU_CMD_BUFFER_SIZE         ((64U) * (1024U))

#if defined(MTB_DISPLAY_W4P3INCH_RPI)
#define DISP_H                              (480U)
#define DISP_W                              (832U)
#elif defined(MTB_DISPLAY_EK79007AD3) || defined(MTB_DISPLAY_WS7P0DSI_RPI)
#define DISP_H                              (600U)
#define DISP_W                              (1024U)
#elif defined(MTB_DISPLAY_R4INCH_TFT)
#define DISP_H                              (480U)
#define DISP_W                              (512U)
#define DISP_RESET_PORT      GPIO_PRT16
#define DISP_RESET_PIN       (6U)
#define PWM_GPIO
#define BACKLIGHT_PORT						 GPIO_PRT20
#define BACKLIGHT_PIN 						(6U)
#endif
#define GPU_TESSELLATION_BUFFER_SIZE        ((DISP_H) * 128U)

#define VGLITE_HEAP_SIZE                    (((DEFAULT_GPU_CMD_BUFFER_SIZE) * \
                                              (APP_BUFFER_COUNT)) + \
                                             ((GPU_TESSELLATION_BUFFER_SIZE) * \
                                              (APP_BUFFER_COUNT)))

#define GPU_MEM_BASE                        (0x0U)
#define VG_PARAMS_POS                       (0UL)

#define TARGET_NUM_FRAMES                   (45U)

#define WEATHER_CODE_ICON                   (0U)
/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Heap memory for VGLite to allocate memory for buffers, command, and
 * tessellation buffers 
 */
CY_SECTION(".cy_gpu_buf") uint8_t contiguous_mem[VGLITE_HEAP_SIZE] = { 0xFF };

volatile void *vglite_heap_base = &contiguous_mem;

static bool is_need_to_reboot = 0;
/* DC IRQ Config */
cy_stc_sysint_t dc_irq_cfg =
{
    .intrSrc      = GFXSS_DC_IRQ,
    .intrPriority = DC_INT_PRIORITY
};

/* GPU IRQ Config */
cy_stc_sysint_t gpu_irq_cfg =
{
    .intrSrc      = GFXSS_GPU_IRQ,
    .intrPriority = GPU_INT_PRIORITY
};

#if defined(MTB_DISPLAY_EK79007AD3)
mtb_display_ek79007ad3_pin_config_t ek79007ad3_pin_cfg =
{
    .reset_port = CYBSP_DISP_RST_PORT,
    .reset_pin  = CYBSP_DISP_RST_PIN,
};
#elif defined(MTB_DISPLAY_R4INCH_TFT)
mtb_display_st7701s_pin_config_t st7701s_pin_cfg =
{
    .reset_port = CYBSP_DISP_RST_PORT,
    .reset_pin  = CYBSP_DISP_RST_PIN,
};

mtb_display_st7701s_backlight_config_t st7701s_pwm_cfg =
{
	.bl_port = 0 ,
	.bl_pin = 0 ,
	.pwm_hw = TCPWM0 ,
	.pwm_num = CYBSP_PWM_DISP_BACKLIGHT_NUM ,
	.pwm_config = &CYBSP_PWM_DISP_BACKLIGHT_config,
};
#endif

extern lv_display_t * disp;
extern TaskHandle_t rtos_cm55_gfx_task_handle;

/* VA Variables */
char *intent_text;

/* Location and time variables */
char weather_sync_value[32] = "11";
char location_sync_value[32];
char hour_sync_value[8];
char minute_sync_value[8];
char second_sync_value[8];
char date_sync_value[8];
char month_sync_value[8];
char vaar_sync_value[8];
char year_sync_value[8];
char weather_code_sync_value[8];
DateTime rx_datetime;
rtc_time_t now;

uint32_t idle_percent = 0;

extern bool is_device_provisioned;
extern char new_FW_version[MAX_FW_VERSION_LEN];

extern bool cm55_pipe2_msg_received;
extern ipc_msg_t *ipc_recv_msg;

extern volatile uint32_t msg_val;
extern volatile uint32_t msg_cmd;

extern bool cur_voice_active;

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/

/**
 * @brief Handles the writing of device settings to EEPROM.
 *
 * This function checks a global flag to determine if a write operation to
 * the EEPROM is required. If the flag is set, it retrieves the current device
 * settings and writes them to the EEPROM for persistent storage.
 *
 * @param None
 */
static void handle_eeprom_write(void);

/**
 * @brief Handles touch events detected on the UI.
 *
 * This function is triggered by a touch event. It clears the touch detection
 * flag and restarts the inactivity timer to prevent the device from entering
 * a low-power state.
 *
 * @param None
 */
static void handle_touch_event(void);

/**
 * @brief Clears the speaker buffer.
 *
 * This function calls the application's speaker clear function to stop
 * audio playback and clear any associated buffers.
 *
 * @param None
 */
static void clear_speaker(void);

/**
 * @brief Updates the UI with the current time and date.
 *
 * This function checks a flag to determine if the timestamp has been updated.
 * If so, it reads the current time and date, formats the strings, and updates
 * the corresponding labels and the calendar widget on the user interface.
 *
 * @param None
 */
static void handle_time_update(void);

/**
 * @brief Handles updates from the system's sensor.
 *
 * This function checks if new sensor data is available. If so, it updates
 * the UI with the latest CO2 reading and sends the updated configuration
 * to other device components via IPC.
 *
 * @param None
 */
static void handle_sensor_update(void);

/**
 * @brief Helper function to encapsulate all system event handling.
 *
 * This function processes all incoming IPC messages from CM55 and routes
 * them to the appropriate handler functions. It also manages screen state
 * transitions based on specific commands.
 *
 * @param None
 */
static void handle_system_event(void);

void calculate_fps(void);
/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

static void handle_eeprom_write(void)
{
    /** Check if the EEPROM write setting flag is enabled. */
    if (eeprom_wr_setting)
    {
        /** A temporary structure to hold current device settings. */
        device_settings_t settings = { 0 };

        /** Get the current settings from the device's state. */
        get_current_device_setting(&settings);

        /** Write the settings to the application EEPROM. */
        app_eeprom_write(&settings);

        /* Clear flag */
        eeprom_wr_setting = false;
    }
}

static void handle_touch_event(void)
{
    /** Check if a touch event has been detected. */
    if (true == touch_detected)
    {
        /** Reset the touch detection flag. */
        touch_detected = false;

        /** Restart the inactivity timer to prevent device idle state. */
        start_inactivity_timer();
    }
}

static void clear_speaker(void)
{
    app_speaker_clear();
}

static void handle_time_update(void)
{
    /** Check if the timestamp update flag is set. */
    if (update_timestamp)
    {
        /** Clear the flag to prevent redundant updates. */
        update_timestamp = false;
        char time_str[6];
        char date_str[20];

        /** Update the hour labels for the main screen and low-power screen. */
        snprintf(time_str, sizeof(time_str), "%02u :", ui_current_time.hour);
        lv_label_set_text(ui_TimeHactive, time_str);

        snprintf(time_str, sizeof(time_str), "%02u :", ui_current_time.hour);
        lv_label_set_text(ui_TimeHLP, time_str);

        /** Update the minute labels. */
        snprintf(time_str, sizeof(time_str), "%02u", ui_current_time.min);
        lv_label_set_text(ui_TimeMactive, time_str);

        snprintf(time_str, sizeof(time_str), " %02u :", ui_current_time.min);
        lv_label_set_text(ui_TimeMLP, time_str);

        /** Update the second label on the low-power screen. */
        snprintf(time_str, sizeof(time_str), " %02u", ui_current_time.sec);
        lv_label_set_text(ui_TimeSLP, time_str);

        /** Use the standard C library for date formatting. */
        struct tm date_time;
        uint32_t year = 2000 + ui_current_time.year;

        /** Populate the tm structure with current RTC values. */
        date_time.tm_sec = ui_current_time.sec;
        date_time.tm_min = ui_current_time.min;
        date_time.tm_hour = ui_current_time.hour;
        date_time.tm_mday = ui_current_time.date;
        date_time.tm_mon = ui_current_time.month - 1u;
        date_time.tm_year = year - 1900;
        date_time.tm_wday = ui_current_time.dayOfWeek - 1u;

        /** Use strftime to format the date string. */
        strftime(date_str, sizeof(date_str), "%a %d %b", &date_time);

        /** Update the date labels. */
        lv_label_set_text(ui_Dateactive, date_str);
        lv_label_set_text(ui_DateLP, date_str);

        /** Update the calendar widget's selected date to the current date. */
        //lv_calendar_set_today_date(ui_dtCalendar, year, ui_current_time.month, ui_current_time.date);

        cy_rslt_t result = rtc_get_time(&now);
        if (result == CY_RSLT_SUCCESS) {
        //     printf("RTC Time: %02u:%02u:%02u  DOW:%u  %02u/%02u/%02u\r\n",
        //         now.hours, now.minutes, now.seconds,
        //         now.dow, now.day, now.month, now.year);
        } else {
            printf("RTC Read time failed: 0x%08lx\r\n", (unsigned long)result);
        }
    }
}

static void handle_sensor_update(void)
{
    /** Check if new sensor data is available. */
    if (sensor_data_available)
    {
        /** Update CO2 level on the UI. */
        update_co2_data_ui(read_ppm);
        

        /* Update sensor data if device is connected */
        if (true == is_device_connected)
        {
            if(dev_info.environment.current_temp == dev_info.environment.target_temp)
            {
                /* Update humidity and CO2 levels on MApp via IPC. */
                update_device_config_ipc();
            }
        }

        /** Clear the flag to indicate the data has been processed. */
        sensor_data_available = false;
    }
}

void sync_ui_via_rtc()
{
    rx_datetime.year   = now.year + 2000;
    rx_datetime.month  = now.month;
    rx_datetime.day    = now.day;
    rx_datetime.hour   = now.hours;
    rx_datetime.minute = now.minutes;
    rx_datetime.second = now.seconds;

    set_date_time_rtc(&rx_datetime);
    printf("Sync via RTC\n");
}

void sync_ui_via_http(int year, int month, int day, int hour, int minute, int second)
{
    rx_datetime.year   = year;
    rx_datetime.month  = month;
    rx_datetime.day    = day;
    rx_datetime.hour   = hour;
    rx_datetime.minute = minute;
    rx_datetime.second = second;

    set_date_time_rtc(&rx_datetime);
    printf("Sync via HTTPs\n");

    (void)rtc_set_time_safe_from_http(year, month, day, hour, minute, second);
}

void update_date_via_http(void)
{
    if (strlen(year_sync_value) > 0 && strlen(month_sync_value) > 0 &&
        strlen(date_sync_value) > 0 && strlen(hour_sync_value) > 0 &&
        strlen(minute_sync_value) > 0 && strlen(second_sync_value) > 0)
    {
        // Parse HTTP time
        int year   = atoi(year_sync_value);   // e.g., 2025
        int month  = atoi(month_sync_value);  // 1..12
        int day    = atoi(date_sync_value);   // 1..31
        int hour   = atoi(hour_sync_value);   // 0..23
        int minute = atoi(minute_sync_value); // 0..59
        int second = atoi(second_sync_value); // 0..59

        // HTTP is newer -> update UI and also program external RTC
        sync_ui_via_http(year, month, day, hour, minute, second);

        enqueue_notification(NOTIFY_HTTP_SYNC, NOTIF_SUCCESS, DEV_ST_SYNCED_HTTP);

        // Clear buffers
        hour_sync_value[0]   = '\0';
        minute_sync_value[0] = '\0';
        second_sync_value[0] = '\0';
        date_sync_value[0]   = '\0';
        month_sync_value[0]  = '\0';
        year_sync_value[0]   = '\0';
    }
}

void show_clear_icon()
{
    lv_obj_clear_flag(ui_Clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_PartlyCloudy, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Rain, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Snow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Thunder, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Fog, LV_OBJ_FLAG_HIDDEN);
}

void show_rain_icon()
{
    lv_obj_clear_flag(ui_Rain, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_PartlyCloudy, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Snow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Thunder, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Fog, LV_OBJ_FLAG_HIDDEN);
}

void show_snow_icon()
{
    lv_obj_clear_flag(ui_Snow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_PartlyCloudy, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Rain, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Thunder, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Fog, LV_OBJ_FLAG_HIDDEN);
}

void show_partlycloud_icon()
{
    lv_obj_clear_flag(ui_PartlyCloudy, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Rain, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Snow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Thunder, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Fog, LV_OBJ_FLAG_HIDDEN);
}

void show_thunder_icon()
{
    lv_obj_clear_flag(ui_Thunder, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_PartlyCloudy, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Rain, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Snow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Fog, LV_OBJ_FLAG_HIDDEN);
}

void show_fog_icon()
{
    lv_obj_clear_flag(ui_Fog, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_PartlyCloudy, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Rain, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Snow, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Thunder, LV_OBJ_FLAG_HIDDEN);
}


void update_weather_info_via_http()
{
    int weatherCode = atoi(weather_code_sync_value);

    switch (weatherCode) {
        case 0:
            printf("Clear Sky \n");
            show_clear_icon();
            break;  // Clear sky
        case 1: case 2: case 3:
            printf("Partly Cloudy \n");
            show_partlycloud_icon();
            break;  // Partly cloudy / Mainly Clear
        case 45: case 48:
            printf("Fog \n");
            show_fog_icon();
            break; // Cloudy / Foggy
        case 51: case 53: case 55: case 61: case 63: case 65: case 80: case 81: case 82:
            printf("Rain \n");
            show_rain_icon();
            break;  // Drizzle / Rain
        case 56: case 57: case 66: case 67: case 71: case 73: case 75: case 77: case 85: case 86:
            printf("Snowflake \n");
            show_snow_icon();
            break;  // Freezing Rain
        case 95: case 96: case 99:
            printf("Rain With Thunder \n");
            show_thunder_icon();
            break;  // Rain with Thunder
        default:
            printf("UNKNOWN WEATHER CODE GROUP!!! \n");
            break;  // Unknown weather code
    }
}

#if WEATHER_CODE_ICON
void check_weather_code_icons(int weathercode)
{
    switch (weathercode) 
    {
        case 1: printf("Clear Sky\n");          show_clear_icon();        break;
        case 2: printf("Partly Cloudy\n");      show_partlycloud_icon();  break;
        case 3: printf("Fog\n");                show_fog_icon();          break;
        case 4: printf("Rain\n");               show_rain_icon();         break;
        case 5: printf("Snowflake\n");          show_snow_icon();         break;
        case 6: printf("Rain With Thunder\n");  show_thunder_icon();      break;
        default: printf("UNKNOWN WEATHER CODE!\n");                       break;
    }
}

bool check_weather_check_button_pressed(void)
{
    static bool last_pressed = false;
    static uint32_t debounce_ticks = 0;
    const uint32_t debounce_ms = 30;

    // Read raw
    bool raw_pressed = (Cy_GPIO_Read(CYBSP_USER_BTN2_PORT, CYBSP_USER_BTN2_NUM) == 0);

    uint32_t now_ms = xTaskGetTickCount(); // or xTaskGetTickCount()*portTICK_PERIOD_MS
    static bool candidate_state = false;
    static uint32_t last_change_ms = 0;

    if (raw_pressed != candidate_state) {
        candidate_state = raw_pressed;
        last_change_ms = now_ms;
    }

    // Stable long enough?
    if ((now_ms - last_change_ms) >= debounce_ms) {
        // Debounced state is candidate_state
        if (candidate_state && !last_pressed) {
            last_pressed = true;
            return true; // one event per press
        } else if (!candidate_state && last_pressed) {
            last_pressed = false;
        }
    }
    return false;
}
#endif

/* Helper function to encapsulate all system event handling */
static void handle_system_event(void)
{
    if (cm55_pipe2_msg_received)
    {
        switch (msg_cmd)
        {
            case IPC_CMD_UPDATE_PRESENCE_STATUS:
                /** Update UI based on presence detection status. */
                stop_active_state_timer();
                if ((presence_status_t) msg_val == PRESENCE_DETECTED)
                {
                    /** Switch to Active screen and display presence status. */
                    switch_to_active_screen();
                    update_presence_detection(1);
                    lv_obj_set_style_bg_image_opa(ui_presencelbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                else if ((presence_status_t) msg_val == ABSENCE_DETECTED)
                {
                    /** Update absence status. */
                    update_presence_detection(0);
                    lv_obj_set_style_bg_image_opa(ui_presencelbl, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                break;

            case IPC_CMD_SET_UID:
                /** Set the device's unique ID from the IPC message. */
                LOG_INFO(CYLF_DEF, "Rx UID: %s\n", ipc_recv_msg->unique_id);
                memcpy(device_unique_id, ipc_recv_msg->unique_id, 13);
                update_device_config_ipc();
                break;

            case IPC_CMD_SET_DATE_TIME:
            {
                /** Switch to Active screen and display presence status. */
                switch_to_active_screen();

                /** Set the device's date and time from the IPC message. */
                DateTime rx_datetime;
                memset(&rx_datetime, 0, sizeof(rx_datetime));
                memcpy(&rx_datetime, &(ipc_recv_msg->datetime), sizeof(DateTime));
                set_date_time_rtc(&rx_datetime);
                break;
            }

            case IPC_CMD_SET_CURRENT_CO2_LEVEL:
                /** Update the UI with the current CO2 level. */
                update_co2_data_ui((uint16_t) msg_val);
                break;

            case IPC_CMD_DEVICE_CONFIG:
                /** Update the device configuration via IPC. */
                update_device_config_ipc();
                break;

            case IPC_CMD_SET_DISPLAY_BRIGHTNESS:
                /** Switch to Active screen and display presence status. */
                switch_to_active_screen();

                /** Set the display brightness. */
                update_display_brightness(msg_val);
                break;

            case IPC_CMD_SET_AUDIO_LEVEL:
                /** Switch to Active screen and display presence status. */
                switch_to_active_screen();

                /** Set the audio output level. */
                update_thermostat_volume((audio_level_t) msg_val);
                break;

            case IPC_CMD_SET_TEMP_UNIT:
                /** Switch to Active screen and display presence status. */
                switch_to_active_screen();

                /** Set the temperature unit and update IPC. */
                update_system_unit((temp_unit_t) msg_val);
                update_device_config_ipc();
                break;

            case IPC_CMD_UPDATE_CONN_STATE:
            {
                /** Update the device's connection state based on the IPC message. */
                switch ((device_connection_state_t) msg_val)
                {
                    case DEV_ST_BLE_ADVERTISING:
                    case DEV_ST_WIFI_CONNECTING:
                        /* Dont turn off screen if device is not provisioned */
                        if(true != is_device_provisioned)
                        {
                            stop_active_state_timer();
                        }
                        update_device_connection_state((device_connection_state_t) msg_val);
                        break;

                    case DEV_ST_UNPROVISIONED:
                    case DEV_ST_CLOUD_DISCONNECTED:
                    case DEV_ST_WIFI_DISCONNECTED:
                        update_device_connection_state((device_connection_state_t) msg_val);
                        start_inactivity_timer();
                        break;

                    case DEV_ST_CLOUD_CONNECTING:
                        update_device_connection_state((device_connection_state_t) msg_val);
                        break;

                    case DEV_ST_WIFI_CONNECTED:
                        /** Switch to Active screen and display presence status. */
                        switch_to_active_screen();
                        update_device_connection_state((device_connection_state_t) msg_val);
                        break;

                    case DEV_ST_BLE_CONNECTED:
                        /** Switch to Active screen and display presence status. */
                        switch_to_active_screen();

                        update_device_connection_state((device_connection_state_t) msg_val);
                        start_inactivity_timer();
                        break;

                    case DEV_ST_CLOUD_CONNECTED:
                        /** Switch to Active screen and display presence status. */
                        switch_to_active_screen();

                        update_device_connection_state((device_connection_state_t) msg_val);
                        request_uid_ipc();
                        start_inactivity_timer();
                        break;

                    default:
                        break;
                }
                break;
            }
            case IPC_CMD_SET_FAN_SPEED:
            {
                /** Switch to Active screen and display presence status. */
                switch_to_active_screen();

                /** Set the fan speed and update settings. */
                update_fan_mode((fan_speed_t) msg_val);
                current_settings.thermostat_setting.fan_mode = (fan_speed_t) msg_val;
                update_current_device_setting();
                break;
            }
            case IPC_CMD_SET_THERMOSTAT_MODE:
            {
                /** Switch to Active screen and display presence status. */
                switch_to_active_screen();

                /** Set the thermostat mode and update settings. */
                update_thermostat_mode((thermostat_mode_t) msg_val);
                current_settings.thermostat_setting.mode = (thermostat_mode_t) msg_val;
                current_settings.thermostat_setting.fan_mode = get_current_fan_mode();
                update_thermostat_mode_timer();
                update_current_device_setting();
                break;
            }
            case IPC_CMD_SET_TARGET_TEMP:
            {
                /** Switch to Active screen and display presence status. */
                switch_to_active_screen();

                /** Update the target temperature. */
                update_device_temp((uint8_t) msg_val);
                break;
            }
            case IPC_CMD_CURRENT_EVENT:
            {
                /** Display the BLE pairing window with a code. */
                char pin[7] = { 0 };
                snprintf(pin, sizeof(pin), "%06lu", (unsigned long) msg_val);
                if (msg_val > 1)
                {
                    /** Switch to Active screen and display presence status. */
                    switch_to_active_screen();
                    stop_active_state_timer();
                    display_ble_pairing_window(0, pin);
                }
                else
                {
                    display_ble_pairing_window(1, pin);
                }
                break;
            }

            case IPC_CMD_GET_CURRENT_TEMP:
                /** Get and update the current temperature via IPC. */
                update_current_temp_ipc(get_current_temperature());
                break;

            case IPC_CMD_GET_DISPLAY_BRIGHTNESS:
                /** Get and update the brightness via IPC. */
                update_brightness_ipc(get_current_brigthness());
                break;

            case IPC_CMD_GET_FAN_SPEED:
                /** Get and update the fan speed via IPC. */
                update_fan_speed_ipc(get_current_fan_mode());
                break;

            case IPC_CMD_GET_THERMOSTAT_MODE:
                /** Get and update the thermostat mode via IPC. */
                update_device_mode_ipc(get_current_device_mode());
                break;

            case IPC_CMD_UPDATE_PROVISION_STATE:
                /** Update the provisioned state of the device. */
                is_device_provisioned = msg_val;
                break;

            case IPC_CMD_OTA_VERSION:
                LOG_INFO(CYLF_DEF, "New OTA version is %s\n", ipc_recv_msg->fw_version);
                memset(new_FW_version, 0,sizeof(new_FW_version));
                memcpy(new_FW_version, ipc_recv_msg->fw_version, strlen(ipc_recv_msg->fw_version));
                break;

            case IPC_CMD_TRIGGER_OTA_START:
                _ui_screen_change(&ui_FWUpdateScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_FWUpdateScreen_screen_init);
                trigger_ota_update();
                LOG_INFO(CYLF_DEF, "OTA Event Received\n");
                break;

            case IPC_CMD_DISABLE_TOUCH:
                LOG_INFO(CYLF_DEF, "Disable the touch\n");
                Cy_SCB_I2C_DeInit(CYBSP_I2C_CONTROLLER_2_HW);
                break;

            case IPC_CMD_ABORT_OTA:
                //ui_FWUpdateScreen_update_msg("Rebooting device as error with\n firmware update or no \nupdate available!\n");
                //is_need_to_reboot = 1;
                /* Enable the I2C interrupts. */
//                Cy_SCB_I2C_Init(CYBSP_I2C_CONTROLLER_2_HW,
//                                    &CYBSP_I2C_CONTROLLER_2_config, &disp_touch_i2c_controller_context);
//                NVIC_EnableIRQ((IRQn_Type)i2c_scb_irq_cfg.intrSrc);
//
//                /* Enable the I2C */
//                Cy_SCB_I2C_Enable(CYBSP_I2C_CONTROLLER_2_HW);
//                /* Add same FW version error status in notification queue */
//                _ui_screen_change(&ui_ActiveScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 230, 0, &ui_ActiveScreen_screen_init);
//                enqueue_notification(NOTIFY_FW_UPDATE, NOTIF_FAIL, DEV_ST_FW_HAVE_SAME_VERSION);
                break;

            case IPC_CMD_NO_INTERNET_NOTIFY:
                _ui_screen_change(&ui_ActiveScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 230, 0, &ui_ActiveScreen_screen_init);
                enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_FAIL, DEV_ST_NO_INTERNET);
                revert_fw_update_screen();
                break;

            case IPC_CMD_UPDATE_CURRENT_SCREEN:
            {
                switch((screen_id_t)msg_val)
                {
                case SCREEN_MAIN:
                    // if((disp != NULL) && (lv_display_get_screen_active(disp) == ui_FWUpdateScreen))
                    // {
                    //     NVIC_EnableIRQ((IRQn_Type)i2c_scb_irq_cfg.intrSrc);
                    // }
                    // else
                    // {
                    //     LOG_INFO(CYLF_DEF, "No Display mem, power cycle the device!");
                    // }
                    // _ui_screen_change(&ui_ActiveScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ActiveScreen_screen_init);
                    // LOG_INFO(CYLF_DEF, "Displaying Main Screen\n");
                    break;

                case SCREEN_FW:
                    _ui_screen_change(&ui_FWUpdateScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_FWUpdateScreen_screen_init);
                    LOG_INFO(CYLF_DEF, "Displaying Firmware Screen\n");
                    break;

                case SCREEN_DATE_TIME:
                   // _ui_screen_change(&ui_DateTimeSettings, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_DateTimeSettings_screen_init);
                    LOG_INFO(CYLF_DEF, "Displaying Date & Time Screen\n");
                    break;


                default:
                    printf("Unknown Screen\n");
                    break;
                }
                break;
            }

            case IPC_CMD_WEATHER_SYNC:
                LOG_INFO(CYLF_DEF, "\nWeather data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the weather data
                memset(weather_sync_value, 0, sizeof(weather_sync_value));
                strncpy(weather_sync_value, ipc_recv_msg->char_value, sizeof(weather_sync_value) - 1);

                // int roundoffTemp = atoi(weather_sync_value);

                // // Update UI label
                // char label_text[32];
                // snprintf(label_text, sizeof(label_text), "%d?c", roundoffTemp);
                // lv_label_set_text(ui_OutdoorTempLabel, label_text);
                lv_label_set_text_fmt(ui_OutdoorTempLabel, "%d°c", atoi(weather_sync_value));
                break;

            case IPC_CMD_LOCATION_SYNC:
                LOG_INFO(CYLF_DEF, "\nLocation data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the location data
                memset(location_sync_value, 0, sizeof(location_sync_value));
                strncpy(location_sync_value, ipc_recv_msg->char_value, sizeof(location_sync_value) - 1);

                // Update Weather Text UI
                lv_label_set_text(ui_LocationLabel, location_sync_value);
                break;

            case IPC_CMD_HOUR_SYNC:
                LOG_INFO(CYLF_DEF, "\nHour sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the hour sync data
                memset(hour_sync_value, 0, sizeof(hour_sync_value));
                strncpy(hour_sync_value, ipc_recv_msg->char_value, sizeof(hour_sync_value) - 1);
                update_date_via_http();
                break;

            case IPC_CMD_MINUTE_SYNC:
                LOG_INFO(CYLF_DEF, "\nMinute sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the minute sync data
                memset(minute_sync_value, 0, sizeof(minute_sync_value));
                strncpy(minute_sync_value, ipc_recv_msg->char_value, sizeof(minute_sync_value) - 1);
                update_date_via_http();
                break;

            case IPC_CMD_SECOND_SYNC:
                LOG_INFO(CYLF_DEF, "\nSecond sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the second sync data
                memset(second_sync_value, 0, sizeof(second_sync_value));
                strncpy(second_sync_value, ipc_recv_msg->char_value, sizeof(second_sync_value) - 1);
                update_date_via_http();
                break;

            case IPC_CMD_DATE_SYNC:
                LOG_INFO(CYLF_DEF, "Month sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(date_sync_value, 0, sizeof(date_sync_value));
                strncpy(date_sync_value, ipc_recv_msg->char_value, sizeof(date_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Date stored: %s\n", date_sync_value);
                update_date_via_http();
                break;

            case IPC_CMD_MONTH_SYNC:
                LOG_INFO(CYLF_DEF, "Month sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(month_sync_value, 0, sizeof(month_sync_value));
                strncpy(month_sync_value, ipc_recv_msg->char_value, sizeof(month_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Month stored: %s\n", month_sync_value);
                update_date_via_http();
                break;

            case IPC_CMD_VAAR_SYNC:
                LOG_INFO(CYLF_DEF, "Vaar sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(vaar_sync_value, 0, sizeof(vaar_sync_value));
                strncpy(vaar_sync_value, ipc_recv_msg->char_value, sizeof(vaar_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Vaar stored: %s\n", vaar_sync_value);
                // update_date_via_http();
                break;

            case IPC_CMD_YEAR_SYNC:
                LOG_INFO(CYLF_DEF, "Year sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(year_sync_value, 0, sizeof(year_sync_value));
                strncpy(year_sync_value, ipc_recv_msg->char_value, sizeof(year_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Year stored: %s\n", year_sync_value);
                update_date_via_http();
                break;

            case IPC_CMD_WEATHER_CODE_SYNC:
                LOG_INFO(CYLF_DEF, "Year sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(weather_code_sync_value, 0, sizeof(weather_code_sync_value));
                strncpy(weather_code_sync_value, ipc_recv_msg->char_value, sizeof(weather_code_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Weather code stored: %s\n", weather_code_sync_value);
                update_weather_info_via_http();
                break;

            default:
                break;
        }
        cm55_pipe2_msg_received = false;
    }
}

/*******************************************************************************
* Function Name: calculate_fps
********************************************************************************
* Summary:  
*  This function calculates the frames per second (FPS) based on the number of
*  frames rendered and the elapsed time since the last calculation.
*  It resets the frame count and start time after reaching the target number of
*  frames.

* Parameters:
*  void   
*  
* Return:
*  None
*******************************************************************************/
void calculate_fps(void)
{
    static uint32_t start_time_ms = RESET_VAL;
    static uint32_t num_frames    = RESET_VAL;
    static uint32_t time_ms       = RESET_VAL;
    static uint32_t fps_x_1000    = RESET_VAL; 
    num_frames++;
    
    if (TARGET_NUM_FRAMES <= num_frames)
    {
        idle_percent = calculate_idle_percentage();
        time_ms = get_time_ms() - start_time_ms;
        fps_x_1000 = (num_frames * 1000 * 1000) / time_ms;

        // printf("\rFPS: %u.%03u | CPU usage: %3u%%", (uint8_t)(fps_x_1000 / 1000),
        //             (uint16_t)(fps_x_1000 % 1000),
        //             (uint8_t)(100 - idle_percent));
        // fflush(stdout);

        lv_label_set_text_fmt(ui_FPSlabel,"FPS: %d CPU: %2u%%", (uint8_t)(fps_x_1000 / 1000),
                    (uint8_t)(100 - idle_percent));
        lv_label_set_text_fmt(ui_FPSlabel2,"FPS: %d CPU: %2u%%", (uint8_t)(fps_x_1000 / 1000),
                    (uint8_t)(100 - idle_percent));
        lv_label_set_text_fmt(ui_FPSlabel3,"FPS: %d CPU: %2u%%", (uint8_t)(fps_x_1000 / 1000),
                    (uint8_t)(100 - idle_percent));
        
        num_frames = RESET_VAL;
        start_time_ms = get_time_ms();
    }

}
/*******************************************************************************
* Function Name: dc_irq_handler
********************************************************************************
* Summary:
*  Display Controller interrupt handler which gets invoked when the DC finishes
*  utilizing the current frame buffer.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void dc_irq_handler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    Cy_GFXSS_Clear_DC_Interrupt(GFXSS, &gfx_context);

    /* Notify the cm55_gfx_task */
    xTaskNotifyFromISR(rtos_cm55_gfx_task_handle, 1, eSetValueWithOverwrite, 
                       &xHigherPriorityTaskWoken);

    /* Perform a context switch if a higher-priority task was woken */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


/*******************************************************************************
* Function Name: gpu_irq_handler
********************************************************************************
* Summary:
*  GPU interrupt handler which gets invoked when the GPU finishes composing
*  a frame.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void gpu_irq_handler(void)
{
    Cy_GFXSS_Clear_GPU_Interrupt(GFXSS, &gfx_context);
    vg_lite_IRQHandler();
}

/*******************************************************************************
* Function Name: cm55_gfx_task
********************************************************************************
* Summary:
*   This is the FreeRTOS task callback function.
*   It initializes:  
*       - GFX subsystem.
*       - Configure the DC, GPU interrupts.
*       - Initialize I2C interface to be used for touch as well as 7-inch display
*         drivers. 
*       - Initializes the display panel selected through Makefile component and
*         vglite driver.
*       - Allocates vglite memory.
*       - Configures LVGL, low level display and touch driver.
*       - Finally invokes the UI application.
*
* Parameters:
*  void *arg: Pointer to the argument passed to the task (not used)
*
* Return:
*  void
*
*******************************************************************************/
void cm55_gfx_task(void *arg)
{
    CY_UNUSED_PARAMETER(arg);

    uint32_t time_till_next = 0;
    static bool boot_config = true;
    static bool RTCtimeUISynced = false;
    cy_en_sysint_status_t sysint_status = CY_SYSINT_SUCCESS;
    cy_en_gfx_status_t gfx_status = CY_GFX_SUCCESS;
    vg_lite_error_t vglite_status = VG_LITE_SUCCESS;
    
#if WEATHER_CODE_ICON
    static uint8_t current_weather_code = 1;
#endif

#if defined(MTB_DISPLAY_WS7P0DSI_RPI)
    cy_rslt_t status = CY_RSLT_SUCCESS;
#elif defined(MTB_DISPLAY_EK79007AD3) || defined(MTB_DISPLAY_R4INCH_TFT)
    cy_en_mipidsi_status_t mipi_status = CY_MIPIDSI_SUCCESS;
#endif

    cy_en_scb_i2c_status_t i2c_result = CY_SCB_I2C_SUCCESS;
    (void) i2c_result;

    /* GFXSS init */
    /* MIPI-DSI Display specific configs */
#if defined(MTB_DISPLAY_WS7P0DSI_RPI)
    GFXSS_config.mipi_dsi_cfg = &mtb_disp_ws7p0dsi_dsi_config;
#elif defined(MTB_DISPLAY_EK79007AD3)
    GFXSS_config.mipi_dsi_cfg = &mtb_display_ek79007ad3_mipidsi_config;
#elif defined(MTB_DISPLAY_W4P3INCH_RPI)
    GFXSS_config.mipi_dsi_cfg = &mtb_disp_waveshare_4p3_dsi_config;
#endif

    /* Set frame buffer address to the GFXSS configuration structure */
    GFXSS_config.dc_cfg->gfx_layer_config->buffer_address    = frame_buffer1;
    GFXSS_config.dc_cfg->gfx_layer_config->uv_buffer_address = frame_buffer1;

//    GFXSS_config.dc_cfg->gfx_layer_config->width = DISP_W;
//    GFXSS_config.dc_cfg->gfx_layer_config->height = DISP_H;

    /* Initialize Graphics subsystem as per the configuration */
    gfx_status = Cy_GFXSS_Init(GFXSS, &GFXSS_config, &gfx_context);

    if (CY_GFX_SUCCESS == gfx_status)
    {

        /* Initialize GFXSS DC interrupt */
        sysint_status = Cy_SysInt_Init(&dc_irq_cfg, dc_irq_handler);
 
        if (CY_SYSINT_SUCCESS != sysint_status)
        {
            LOG_ERROR(CYLF_DEF, "Error in registering DC interrupt: %d\r\n", sysint_status);
            CY_ASSERT(0);
        }

        /* Enable GFX DC interrupt in NVIC. */
        NVIC_EnableIRQ(GFXSS_DC_IRQ);

        /* Initialize GFX GPU interrupt */
        sysint_status = Cy_SysInt_Init(&gpu_irq_cfg, gpu_irq_handler);

        if (CY_SYSINT_SUCCESS != sysint_status)
        {
            LOG_ERROR(CYLF_DEF, "Error in registering GPU interrupt: %d\r\n", sysint_status);
            CY_ASSERT(0);
        }
 
        /* Enable GPU interrupt */
        Cy_GFXSS_Enable_GPU_Interrupt(GFXSS);

        /* Enable GFX GPU interrupt in NVIC. */
        NVIC_EnableIRQ(GFXSS_GPU_IRQ);

        LOG_INFO(CYLF_DEF, "[cm55_gfx_task] GFX subsystem initialized, configuring I2C\r\n");

#if defined(MTB_DISPLAY_R4INCH_TFT)
        /* Enable the I2C */
        Cy_SCB_I2C_Enable(CYBSP_I2C_CONTROLLER_2_HW);
#else
        /* Enable the I2C */
        Cy_SCB_I2C_Enable(CYBSP_I2C_CONTROLLER_HW);
#endif

        vTaskDelay(pdMS_TO_TICKS(500));

#if defined(MTB_DISPLAY_R4INCH_TFT)
		/* Initialize the R4INCH display */
		mipi_status =  mtb_display_st7701s_init(GFXSS_GFXSS_MIPIDSI,&st7701s_pin_cfg);
        if(CY_MIPIDSI_SUCCESS != mipi_status)
        {
            LOG_ERROR(CYLF_DEF, "[cm55_gfx_task] st7701s 4-inch display init failed with status = %d\r\n", mipi_status);
            CY_ASSERT(0);
        }

        if (CY_TCPWM_SUCCESS != mtb_display_st7701s_backlight_init(&st7701s_pwm_cfg))
        {
            /* Handle possible errors */
            LOG_ERROR(CYLF_DEF, "[cm55_gfx_task] failed pwm init\r\n");
            CY_ASSERT(0);
        }
        mtb_display_st7701s_set_brightness(brightness_level);
#endif
        LOG_INFO(CYLF_DEF, "[cm55_gfx_task] Initializing VGLite\r\n");
        /* Allocate memory for VGLite from the vglite_heap_base */
        vg_module_parameters_t vg_params;
        vg_params.register_mem_base = (uint32_t)GFXSS_GFXSS_GPU_GCNANO;
        vg_params.gpu_mem_base[VG_PARAMS_POS] = GPU_MEM_BASE;
        vg_params.contiguous_mem_base[VG_PARAMS_POS] = vglite_heap_base;
        vg_params.contiguous_mem_size[VG_PARAMS_POS] = VGLITE_HEAP_SIZE;

        /* Initialize VGlite memory. */
        vg_lite_init_mem(&vg_params);

        /* Initialize the memory and data structures needed for VGLite draw/blit
         * functions
         */
        vglite_status = vg_lite_init((MY_DISP_HOR_RES) / 4,
                             (MY_DISP_VER_RES) / 4);

        if (VG_LITE_SUCCESS == vglite_status)
        {
            LOG_INFO(CYLF_DEF, "[cm55_gfx_task] VGLite initialized, initializing LVGL\r\n");
            /* Initialize LVGL library */
            lv_init();
            lv_port_disp_init();
            lv_port_indev_init();
            ui_init();
            ui_timer_init();
            LOG_INFO(CYLF_DEF, "[cm55_gfx_task] LVGL initialized successfully, entering main loop\r\n");
        }
        else
        {
            LOG_ERROR(CYLF_DEF, "vg_lite_init failed, status: %d\r\n", vglite_status);

            /* Deallocate all the resources and free up all the memory */
            vg_lite_close();
            CY_ASSERT(0);
        }
    }
    else
    {
        LOG_ERROR(CYLF_DEF, "Graphics subsystem init failed, status: %d\r\n", gfx_status);
        CY_ASSERT(0);
    }

    LOG_INFO(CYLF_DEF, "[cm55_gfx_task] Entering main event loop\r\n");

    for (;;)
    {
        if(is_need_to_reboot)
        {
            vTaskDelay(5000);
            NVIC_SystemReset();
        }
        /* Process sensor update event */
        handle_sensor_update();

        /* Process system IPC events */
        handle_system_event();

        /* LVGL's timer handler function, to be called periodically to handle
         * LVGL tasks.
         */
        time_till_next = lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(time_till_next));
#if defined(USE_VOICE_ASSISTANT)
        /* Read the user button state */
        if (check_button_pressed() || is_mic_clicked)
        {
            /** Switch to Active screen  */
            switch_to_active_screen();

            printf("Push to Talk Button detected! Speak a command!\r\n");
            Cy_GPIO_Set(CYBSP_LED_BLUE_PORT, CYBSP_LED_BLUE_NUM);

            is_mic_clicked = false;
            cur_voice_active = true;
            voice_assistant_change_state(VA_RUN_CMD);
        }

        ww_to_ui();
        intent_to_ui(intent_text);
#endif
#if WEATHER_CODE_ICON
        if (check_weather_check_button_pressed()) {
            current_weather_code++;
            if (current_weather_code > 6) current_weather_code = 1;
            check_weather_code_icons(current_weather_code); // reuse your function
        }
#endif
    	if(boot_config)
    	{
    		load_thermostat_config(current_settings.thermostat_setting.mode);
    		update_device_config_ipc();
    		boot_config = false;
    	}

    	/* If eeprom write operation pending */
        handle_eeprom_write();

    	/* If touch event detected restart the inactivity timer */
        handle_touch_event();
    
        clear_speaker();

    	/* Update time on UI */
    	handle_time_update();

        calculate_fps();

    	/* Hide connectivity pop-up screen
    	 * if BLE/Cloud connected state  */
    	if (hide_conn_screen)
        {
            hide_connectivity_screen();
            hide_conn_screen = false;
        }
        
        /* FW Update available check from UI */
		if (fw_update_timeout == true)
		{
			/* Hide spinner and label */
			lv_obj_add_flag(ui_FWUpdatespinner, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(ui_Fwupdatespinrlabel, LV_OBJ_FLAG_HIDDEN);
		
			/* Check if new Firmware version available */
			if (is_new_fw_available == true)
			{
				char msg[50];
		
				/* Update the FW version */
				snprintf(msg, sizeof(msg), "F.W version %s available.",
						 new_FW_version);
				lv_label_set_text(ui_Fwupdatelatestlbl, msg);
				lv_obj_clear_flag(ui_Fwupdatelatestlbl, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui_fwdownloadbtnlbl, LV_OBJ_FLAG_HIDDEN);
			}
			else
			{
				lv_obj_clear_flag(ui_Fwupdatelatestlbl, LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(ui_fwdownloadbtnlbl, LV_OBJ_FLAG_HIDDEN);
			}
		
			fw_update_timeout = false;
		}

        if(!RTCtimeUISynced)
        {
            // Sync UI time with current RTC
            sync_ui_via_rtc();
            RTCtimeUISynced = true;
        }
    }
}


/* [] END OF FILE */
