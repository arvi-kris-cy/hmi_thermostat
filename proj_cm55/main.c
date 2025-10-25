/*******************************************************************************
* File Name        : main.c
*
* Description      : This source file contains the main routine for 
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
#include "ipc_communication.h"
#include "app_common.h"
#include "thermostat_events.h"
#include "comm_manager.h"
#include "app_eeprom.h"
#include "app_sensor.h"
#include "app_speaker.h"
#include "xensiv_pasco2_mtb.h"

/* Application related includes */
#include "app_voice_control.h"
#include "voice_assistant.h"
#include "app_rtc.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define GPU_INT_PRIORITY                    (3U)
#define DC_INT_PRIORITY                     (3U)

#define GFX_TASK_NAME                       ("CM55 Gfx Task")
/* stack size in words */
#define GFX_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE * 20)

#define GFX_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)

// #define VOICE_ASSISTANT_TASK_NAME               ("VoiceTask")
// #define VOICE_ASSISTANT_TASK_STACK_SIZE         (configMINIMAL_STACK_SIZE * 32)
// #define VOICE_ASSISTANT_TASK_PRIORITY           (configMAX_PRIORITIES - 2)

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
#define I2C_CONTROLLER_IRQ_PRIORITY         (2UL)
#define VG_PARAMS_POS                       (0UL)
#define RESET_VAL                   (0U)
/* Enabling or disabling a MCWDT requires a wait time of upto 2 CLK_LF cycles
 * to come into effect. This wait time value will depend on the actual CLK_LF
 * frequency set by the BSP.
 */
#define LPTIMER_1_WAIT_TIME_USEC            (62U)

/* Define the LPTimer interrupt priority number. '1' implies highest priority.*/
#define APP_LPTIMER_INTERRUPT_PRIORITY      (1U)

#if ( configGENERATE_RUN_TIME_STATS == 1 )
#define TCPWM_TIMER_INT_PRIORITY            (1U)
#endif

/*******************************************************************************
* Global Variables
*******************************************************************************/
static bool cm55_pipe2_msg_received = false;
static ipc_msg_t *ipc_recv_msg;

static volatile uint32_t msg_val = RESET_VAL;
static volatile uint32_t msg_cmd = RESET_VAL;

bool is_device_provisioned = false;

/* Heap memory for VGLite to allocate memory for buffers, command, and
 * tessellation buffers 
 */
CY_SECTION(".cy_gpu_buf") uint8_t contiguous_mem[VGLITE_HEAP_SIZE] = { 0xFF };

volatile void *vglite_heap_base = &contiguous_mem;

TaskHandle_t rtos_cm55_gfx_task_handle = NULL;
TaskHandle_t rtos_cm55_voice_task_handle = NULL;
TaskHandle_t rtos_cm55_sensor_task_handle = NULL;

char m55_current_OTA_version[MAX_FW_VERSION_LEN] = "-.-.-";
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

cy_stc_scb_i2c_context_t disp_touch_i2c_controller_context;

#if defined(MTB_DISPLAY_R4INCH_TFT)
cy_stc_sysint_t disp_touch_i2c_controller_irq_cfg =
{
    .intrSrc      = CYBSP_I2C_CONTROLLER_2_IRQ,
    .intrPriority = I2C_CONTROLLER_IRQ_PRIORITY,
};
#else
cy_stc_sysint_t disp_touch_i2c_controller_irq_cfg =
{
    .intrSrc      = CYBSP_I2C_CONTROLLER_IRQ,
    .intrPriority = I2C_CONTROLLER_IRQ_PRIORITY,
};
#endif

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

/* LPTimer HAL object */
static mtb_hal_lptimer_t lptimer_obj;

lv_obj_t *label;
uint8_t brightness_level = 100;
audio_level_t audio_level = AUDIO_MED;
/* RTC HAL object */
static mtb_hal_rtc_t rtc_obj;
char new_FW_version[MAX_FW_VERSION_LEN];
extern lv_display_t * disp;
/* Mutex to guard the I2C instance for Sensor/Touhpad */
SemaphoreHandle_t i2c_mutex = NULL;

/* VA Variables */
char *intent_text;

bool handle_ww_for_ui = false;
bool handle_command_for_ui = false;

char weather_sync_value[32];
char location_sync_value[32];
char hour_sync_value[8];
char minute_sync_value[8];
char second_sync_value[8];
char date_sync_value[8];
char month_sync_value[8];
char vaar_sync_value[8];
char year_sync_value[8];
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

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

#if ( configGENERATE_RUN_TIME_STATS == 1 )
/*******************************************************************************
* Function Name: setup_run_time_stats_timer
********************************************************************************
* Summary:
*  This function configuresTCPWM 0 GRP 0 Counter 0 as the timer source for  
*  FreeRTOS runtime statistics.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void setup_run_time_stats_timer(void)
{
    /* Initialze TCPWM block with required timer configuration */
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_Counter_Init(CYBSP_GENERAL_PURPOSE_TIMER_HW, 
        CYBSP_GENERAL_PURPOSE_TIMER_NUM, 
        &CYBSP_GENERAL_PURPOSE_TIMER_config))
    {
        handle_app_error();
    }

    /* Enable the initialized counter */
    Cy_TCPWM_Counter_Enable(CYBSP_GENERAL_PURPOSE_TIMER_HW, 
                            CYBSP_GENERAL_PURPOSE_TIMER_NUM);

    /* Start the counter */
    Cy_TCPWM_TriggerStart_Single(CYBSP_GENERAL_PURPOSE_TIMER_HW, 
                                 CYBSP_GENERAL_PURPOSE_TIMER_NUM);
}


/*******************************************************************************
* Function Name: get_run_time_counter_value
********************************************************************************
* Summary:
*  Function to fetch run time counter value. This will be used by FreeRTOS for 
*  run time statistics calculation.
*
* Parameters:
*  void
*
* Return:
*  uint32_t: TCPWM 0 GRP 0 Counter 0 value
*
*******************************************************************************/
uint32_t get_run_time_counter_value(void)
{
   return (Cy_TCPWM_Counter_GetCounter(CYBSP_GENERAL_PURPOSE_TIMER_HW, 
                                       CYBSP_GENERAL_PURPOSE_TIMER_NUM));
}


/*******************************************************************************
* Function Name: calculate_idle_percentage
********************************************************************************
* Summary:
*  Function to calculate CPU idle percentage. This function is used by LVGL to  
*  showcase CPU usage.
*
* Parameters:
*  void
*
* Return:
*  uint32_t: CPU idle percentage
*
*******************************************************************************/
uint32_t calculate_idle_percentage(void)
{

    static uint32_t previousIdleTime = 0;
    static TickType_t previousTick = 0;
    uint32_t time_diff = 0;
    uint32_t idle_percent = 0;

    uint32_t currentIdleTime = ulTaskGetIdleRunTimeCounter();
    TickType_t currentTick = portGET_RUN_TIME_COUNTER_VALUE();

    time_diff = currentTick - previousTick;

    if((currentIdleTime >= previousIdleTime) && (currentTick > previousTick))
    {
        idle_percent = ((currentIdleTime - previousIdleTime) * 100)/time_diff;
    }

    previousIdleTime = ulTaskGetIdleRunTimeCounter();
    previousTick = portGET_RUN_TIME_COUNTER_VALUE();

    return idle_percent;
}
#endif

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
        char time_str[3];
        char date_str[20];

        /** Update the hour labels for the main screen and low-power screen. */
        snprintf(time_str, sizeof(time_str), "%02u", ui_current_time.hour);
        lv_label_set_text(ui_TimeHactive, time_str);
        lv_label_set_text(ui_TimeHLP, time_str);

        /** Update the minute labels. */
        snprintf(time_str, sizeof(time_str), "%02u", ui_current_time.min);
        lv_label_set_text(ui_TimeMactive, time_str);
        lv_label_set_text(ui_TimeMLP, time_str);

        /** Update the second label on the low-power screen. */
        snprintf(time_str, sizeof(time_str), "%02u", ui_current_time.sec);
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
        lv_calendar_set_today_date(ui_dtCalendar, year, ui_current_time.month, ui_current_time.date);
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

void update_date_labels(void)
{
    if (strlen(month_sync_value) > 0 && strlen(vaar_sync_value) > 0 && strlen(date_sync_value) > 0)
    {
        char date_str[32];
        snprintf(date_str, sizeof(date_str), "%s %s %s", vaar_sync_value, date_sync_value, month_sync_value);

        // Update the UI labels
        lv_label_set_text(ui_Dateactive, date_str);
        lv_label_set_text(ui_DateLP, date_str);

        LOG_INFO(CYLF_DEF, "Updated date labels: %s\n", date_str);
    }
}

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
                    update_presence_detection(1); // TODO get actual person count
                }
                else if ((presence_status_t) msg_val == ABSENCE_DETECTED)
                {
                    /** Update absence status. */
                    update_presence_detection(0);
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
                ui_FWUpdateScreen_update_msg("Rebooting device as error with\n firmware update or no \nupdate available!\n");
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

                case SCREEN_SETTINGS:
                    _ui_screen_change(&ui_SystemSettings, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_SystemSettings_screen_init);
                    LOG_INFO(CYLF_DEF, "Displaying Settings Screen\n");
                    break;

                case SCREEN_FW:
                    _ui_screen_change(&ui_FWUpdateScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_FWUpdateScreen_screen_init);
                    LOG_INFO(CYLF_DEF, "Displaying Firmware Screen\n");
                    break;

                case SCREEN_DATE_TIME:
                    _ui_screen_change(&ui_DateTimeSettings, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_DateTimeSettings_screen_init);
                    LOG_INFO(CYLF_DEF, "Displaying Date & Time Screen\n");
                    break;

                case SCREEN_SETTINGS_SYSTEM:
                    _ui_screen_change(&ui_SystemSettings, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_SystemSettings_screen_init);
                    LOG_INFO(CYLF_DEF, "Displaying System Settings Subscreen\n");
                    break;

                case SCREEN_SETTINGS_AUDIO:
                    _ui_screen_change(&ui_AudioSettings, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_AudioSettings_screen_init);
                    LOG_INFO(CYLF_DEF, "Displaying Audio Settings Subscreen\n");
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

                // Update UI label
                lv_label_set_text(ui_container2text, weather_sync_value);
                break;

            case IPC_CMD_LOCATION_SYNC:
                LOG_INFO(CYLF_DEF, "\nLocation data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the location data
                memset(location_sync_value, 0, sizeof(location_sync_value));
                strncpy(location_sync_value, ipc_recv_msg->char_value, sizeof(location_sync_value) - 1);

                lv_label_set_text(ui_WeatherTextactive2, location_sync_value);
                break;

            case IPC_CMD_HOUR_SYNC:
                LOG_INFO(CYLF_DEF, "\nHour sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the hour sync data
                memset(hour_sync_value, 0, sizeof(hour_sync_value));
                strncpy(hour_sync_value, ipc_recv_msg->char_value, sizeof(hour_sync_value) - 1);

                // Update Hour Sync Text UI
                lv_label_set_text(ui_TimeHactive, hour_sync_value);
                lv_label_set_text(ui_TimeHLP, hour_sync_value);
                break;

            case IPC_CMD_MINUTE_SYNC:
                LOG_INFO(CYLF_DEF, "\nMinute sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the minute sync data
                memset(minute_sync_value, 0, sizeof(minute_sync_value));
                strncpy(minute_sync_value, ipc_recv_msg->char_value, sizeof(minute_sync_value) - 1);

                // Update Minute Sync Text UI
                lv_label_set_text(ui_TimeMactive, minute_sync_value);
                lv_label_set_text(ui_TimeMLP, minute_sync_value);
                break;

            case IPC_CMD_SECOND_SYNC:
                LOG_INFO(CYLF_DEF, "\nSecond sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                // Clear and copy the second sync data
                memset(second_sync_value, 0, sizeof(second_sync_value));
                strncpy(second_sync_value, ipc_recv_msg->char_value, sizeof(second_sync_value) - 1);

                // Update Second Sync Text UI
                lv_label_set_text(ui_TimeSLP, second_sync_value);
                break;

            case IPC_CMD_DATE_SYNC:
                LOG_INFO(CYLF_DEF, "Month sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(date_sync_value, 0, sizeof(date_sync_value));
                strncpy(date_sync_value, ipc_recv_msg->char_value, sizeof(date_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Date stored: %s\n", date_sync_value);
                update_date_labels();
                break;

            case IPC_CMD_MONTH_SYNC:
                LOG_INFO(CYLF_DEF, "Month sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(month_sync_value, 0, sizeof(month_sync_value));
                strncpy(month_sync_value, ipc_recv_msg->char_value, sizeof(month_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Month stored: %s\n", month_sync_value);
                update_date_labels();
                break;

            case IPC_CMD_VAAR_SYNC:
                LOG_INFO(CYLF_DEF, "Vaar sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(vaar_sync_value, 0, sizeof(vaar_sync_value));
                strncpy(vaar_sync_value, ipc_recv_msg->char_value, sizeof(vaar_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Vaar stored: %s\n", vaar_sync_value);
                update_date_labels();
                break;

            case IPC_CMD_YEAR_SYNC:
                LOG_INFO(CYLF_DEF, "Year sync data received in CM55: %s\n", ipc_recv_msg->char_value);

                memset(year_sync_value, 0, sizeof(year_sync_value));
                strncpy(year_sync_value, ipc_recv_msg->char_value, sizeof(year_sync_value) - 1);

                LOG_INFO(CYLF_DEF, "Year stored: %s\n", year_sync_value);
                break;

            default:
                break;
        }
        cm55_pipe2_msg_received = false;
    }
}

/*******************************************************************************
* Function Name: cm33_msg_callback
********************************************************************************
* Summary:
*  Callback function called when endpoint-2 (CM55) has received a message
*
* Parameters:
*  msg_data: Message data received throuig IPC
*
* Return :
*  void
*
*******************************************************************************/
void cm55_msg_callback(uint32_t * msgData)
{
    if (msgData != NULL)
    {
        /* Cast the message received to the IPC structure */
        ipc_recv_msg = (ipc_msg_t *) msgData;

        /* Extract the command to be processed in the main loop */
        msg_val = ipc_recv_msg->data;
        msg_cmd = ipc_recv_msg->cmd;
    }

    cm55_pipe2_msg_received = true;
}

/*******************************************************************************
* Function Name: lptimer_interrupt_handler
********************************************************************************
* Summary:
*  Interrupt handler function for LPTimer instance.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void lptimer_interrupt_handler(void)
{
    mtb_hal_lptimer_process_interrupt(&lptimer_obj);
}


/*******************************************************************************
* Function Name: setup_tickless_idle_timer
********************************************************************************
* Summary:
*  1. This function first configures and initializes an interrupt for LPTimer.
*  2. Then it initializes the LPTimer HAL object to be used in the RTOS
*     tickless idle mode implementation to allow the device enter deep sleep
*     when idle task runs. LPTIMER_1 instance is configured for CM55 CPU.
*  3. It then passes the LPTimer object to abstraction RTOS library that
*     implements tickless idle mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void setup_tickless_idle_timer(void)
{
    /* Interrupt configuration structure for LPTimer */
    cy_stc_sysint_t lptimer_intr_cfg =
    {
        .intrSrc = CYBSP_CM55_LPTIMER_1_IRQ,
        .intrPriority = APP_LPTIMER_INTERRUPT_PRIORITY
    };

    /* Initialize the LPTimer interrupt and specify the interrupt handler. */
    cy_en_sysint_status_t interrupt_init_status =
                                    Cy_SysInt_Init(&lptimer_intr_cfg,
                                                    lptimer_interrupt_handler);

    /* LPTimer interrupt initialization failed. Stop program execution. */
    if(CY_SYSINT_SUCCESS != interrupt_init_status)
    {
//        handle_app_error();
        APP_ERROR(1);
    }

    /* Enable NVIC interrupt. */
    NVIC_EnableIRQ(lptimer_intr_cfg.intrSrc);

    /* Initialize the MCWDT block */
    cy_en_mcwdt_status_t mcwdt_init_status =
                                    Cy_MCWDT_Init(CYBSP_CM55_LPTIMER_1_HW,
                                                &CYBSP_CM55_LPTIMER_1_config);

    /* MCWDT initialization failed. Stop program execution. */
    if(CY_MCWDT_SUCCESS != mcwdt_init_status)
    {
//        handle_app_error();
        APP_ERROR(mcwdt_init_status);
    }

    /* Enable MCWDT instance */
    Cy_MCWDT_Enable(CYBSP_CM55_LPTIMER_1_HW,
                    CY_MCWDT_CTR_Msk,
                    LPTIMER_1_WAIT_TIME_USEC);

    /* Setup LPTimer using the HAL object and desired configuration as defined
     * in the device configurator. */
    cy_rslt_t result = mtb_hal_lptimer_setup(&lptimer_obj,
                                            &CYBSP_CM55_LPTIMER_1_hal_config);

    /* LPTimer setup failed. Stop program execution. */
    if(CY_RSLT_SUCCESS != result)
    {
//        handle_app_error();
        APP_ERROR(result);
    }

    /* Pass the LPTimer object to abstraction RTOS library that implements
     * tickless idle mode
     */
    cyabs_rtos_set_lptimer(&lptimer_obj);
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
* Function Name: disp_touch_i2c_controller_interrupt
********************************************************************************
* Summary:
*  I2C controller ISR which invokes Cy_SCB_I2C_Interrupt to perform I2C transfer
*  as controller.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void disp_touch_i2c_controller_interrupt(void)
{
#if defined(MTB_DISPLAY_R4INCH_TFT)
    Cy_SCB_I2C_Interrupt(CYBSP_I2C_CONTROLLER_2_HW, &disp_touch_i2c_controller_context);
#else
	Cy_SCB_I2C_Interrupt(CYBSP_I2C_CONTROLLER_HW, &disp_touch_i2c_controller_context);
#endif
}

static void init_i2c_controller()
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    cy_en_scb_i2c_status_t i2c_result = CY_SCB_I2C_SUCCESS;
    cy_en_sysint_status_t sysint_status = CY_SYSINT_SUCCESS;

    i2c_result = Cy_SCB_I2C_Init(CYBSP_I2C_CONTROLLER_2_HW,
                    &CYBSP_I2C_CONTROLLER_2_config, &disp_touch_i2c_controller_context);

    if (CY_SCB_I2C_SUCCESS != i2c_result)
    {
        LOG_ERROR(CYLF_DEF, "I2C controller initialization failed !!\n");
        CY_ASSERT(0);
    }

    /* Initialize the I2C interrupt */
    sysint_status = Cy_SysInt_Init(&disp_touch_i2c_controller_irq_cfg,
                                       &disp_touch_i2c_controller_interrupt);

    if (CY_SYSINT_SUCCESS != sysint_status)
    {
        LOG_ERROR(CYLF_DEF, "I2C controller interrupt initialization failed\r\n");
        CY_ASSERT(0);
    }

    /* Enable the I2C interrupts. */
    NVIC_EnableIRQ((IRQn_Type)disp_touch_i2c_controller_irq_cfg.intrSrc);

    /* Enable the I2C */
    Cy_SCB_I2C_Enable(CYBSP_I2C_CONTROLLER_2_HW);


    i2c_result = mtb_hal_i2c_setup(&CYBSP_I2C_CONTROLLER_2_hal_obj,
                                    &CYBSP_I2C_CONTROLLER_2_hal_config,
                                        &disp_touch_i2c_controller_context,
                                            NULL);

    if(CY_RSLT_SUCCESS != result)
    {
        LOG_ERROR(CYLF_DEF, "I2C HAL setup failed with error code: 0x%08X\r\n", (unsigned int)result);
//        handle_app_error();
        APP_ERROR(result);
    }
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
static void cm55_gfx_task(void *arg)
{
    CY_UNUSED_PARAMETER(arg);

    uint32_t time_till_next = 0;
    static bool boot_config = true;
    cy_en_sysint_status_t sysint_status = CY_SYSINT_SUCCESS;
    cy_en_gfx_status_t gfx_status = CY_GFX_SUCCESS;
    vg_lite_error_t vglite_status = VG_LITE_SUCCESS;

#if defined(MTB_DISPLAY_WS7P0DSI_RPI)
    cy_rslt_t status = CY_RSLT_SUCCESS;
#elif defined(MTB_DISPLAY_EK79007AD3) || defined(MTB_DISPLAY_R4INCH_TFT)
    cy_en_mipidsi_status_t mipi_status = CY_MIPIDSI_SUCCESS;
#endif

    cy_en_scb_i2c_status_t i2c_result = CY_SCB_I2C_SUCCESS;

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


	    if(CY_RSLT_SUCCESS != i2c_result)
	    {
	        printf("I2C HAL setup failed with error code: 0x%08X\r\n", (unsigned int)i2c_result);
	        handle_app_error();
	    }

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
            printf("st7701s 4-inch display init failed with status = %d\r\n", mipi_status);
            CY_ASSERT(0);
        }

        if (CY_TCPWM_SUCCESS !=
                mtb_display_st7701s_backlight_init(&st7701s_pwm_cfg))
        {
            /* Handle possible errors */
        printf("failed pwm init");
            CY_ASSERT(0);
        }
        mtb_display_st7701s_set_brightness(brightness_level);
#endif
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
            /* Initialize LVGL library */
            lv_init();
            lv_port_disp_init();
            lv_port_indev_init();
            ui_demo_init();
            ui_timer_init();

           /* Start sensor task */
            app_sensor_task_init();

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

        // ww_to_ui();
        // intent_to_ui(intent_text);
        
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

    	/* Hide connectivity pop-up screen
    	 * if BLE/Cloud connected state  */
    	if (hide_conn_screen)
        {
            hide_connectivity_screen();
            hide_conn_screen = false;
        }
    }
}


/*******************************************************************************
* Function Name: setup_clib_support
********************************************************************************
* Summary:
*    1. This function configures and initializes the Real-Time Clock (RTC)).
*    2. It then initializes the RTC HAL object to enable CLIB support library 
*       to work with the provided Real-Time Clock (RTC) module.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void setup_clib_support(void)
{
    /* RTC Initialization is done in CM33 non-secure project */

    /* Initialize the ModusToolbox CLIB support library */
    mtb_clib_support_init(&rtc_obj);
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  This is the main function for CM55 non-secure application.
*    1. It initializes the device and board peripherals.
*    2. It sets up the LPtimer instance for CM55 CPU and initializes debug UART. 
*    3. It creates the FreeRTOS application task 'cm55_gfx_task'.
*    4. It starts the RTOS task scheduler.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result       = CY_RSLT_SUCCESS;
    BaseType_t task_return = pdFAIL;
    cy_en_ipc_pipe_status_t pipeStatus;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
//        handle_app_error();
        APP_ERROR(result);
    }

    /* Setup CLIB support library. */
    setup_clib_support();
    /* Setup the LPTimer instance for CM55 */
    setup_tickless_idle_timer();

    /* Initialize retarget-io middleware */
    init_retarget_io();
    /* Enable global interrupts */
    __enable_irq();
	    result = cy_log_init(CY_LOG_ERR, NULL, NULL);
    if (CY_RSLT_SUCCESS != result)
    {
        printf("cy_log_init failed with Error : [0x%X] \n", (unsigned int) result);
    }
    else
    {
        cy_log_set_facility_level(CYLF_DRIVER, CY_LOG_WARNING);
        cy_log_set_facility_level(CYLF_DEF, CY_LOG_INFO);
        cy_log_set_facility_level(CYLF_MIDDLEWARE, CY_LOG_WARNING);
    }

    /* Initialize RTC */
    app_rtc_init();

    /* Setup IPC communication for CM55*/
    cm55_ipc_communication_setup();

    Cy_SysLib_Delay(50);

    /* Register a callback function to handle events on the CM55 IPC pipe */
    pipeStatus = Cy_IPC_Pipe_RegisterCallback(CM55_IPC_PIPE_EP_ADDR, &cm55_msg_callback,
                                                      (uint32_t)CM55_IPC_PIPE_CLIENT_ID);

    if(CY_IPC_PIPE_SUCCESS != pipeStatus)
    {
//        handle_app_error();
        APP_ERROR(pipeStatus);
    }

    /* Power pasco2 sensor */
    power_co2_sensor();

    /* Initialize I2C SCB */
    init_i2c_controller();

    /* Create a binary semaphore to act as a I2C mutex
     * guarding touchpad and sensor. */
    i2c_mutex = xSemaphoreCreateMutex();
    if (i2c_mutex == NULL) {
        LOG_INFO(CYLF_DEF, "I2C mutex creation error.\n");
    }

    /* Initialize Speaker */
    app_speaker_init();

    /* Initialize Emulated EEPROM */
    app_eeprom_init();

    /* Read configuration from Emulated EEPROM */
    device_settings_t rd_settings = {0};
    app_eeprom_read(&rd_settings);

    if(rd_settings.is_available != true) {

        device_settings_t settings = {0};

    	/* Load default configuration */
        get_default_device_setting(&settings);
        settings.is_available = true;

        app_eeprom_write(&settings);
    	set_current_device_setting(&rd_settings);

    } else {
    	set_current_device_setting(&rd_settings);
    }

    dev_info.environment.target_temp = dev_info.environment.current_temp;

    /* Create the FreeRTOS Task */
    task_return = xTaskCreate(cm55_gfx_task, GFX_TASK_NAME,
                              GFX_TASK_STACK_SIZE, NULL,
                              GFX_TASK_PRIORITY, &rtos_cm55_gfx_task_handle);
    
    if (pdPASS != task_return)
    {
        printf("Error: Failed to create cm55_gfx_task.\r\n");
//        handle_app_error();
        APP_ERROR(1);
    }

//     task_return = xTaskCreate(voice_assistant_task,
//                             	VOICE_ASSISTANT_TASK_NAME,
//                             	VOICE_ASSISTANT_TASK_STACK_SIZE, NULL,
//                             	VOICE_ASSISTANT_TASK_PRIORITY, &rtos_cm55_voice_task_handle);
  	
//   	if (pdPASS != task_return)
//     {
//         printf("Error: Failed to create voice_assistant_task.\r\n");
// //        handle_app_error();
//         APP_ERROR(1);
//     }

	printf("****************** "
           "PSOC Edge MCU: HMI Thermostat Demo "
           "****************** \r\n\n");

    /* Start the RTOS Scheduler */
    vTaskStartScheduler();

    /* Should never get here! */
    handle_app_error();
}


/* [] END OF FILE */
