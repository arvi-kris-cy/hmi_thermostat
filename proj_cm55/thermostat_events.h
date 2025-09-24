#ifndef THERMOSTAT_EVENTS_H
#define THERMOSTAT_EVENTS_H
#endif

#include "ui/ui_events.h"
#include "ui/ui.h"
#include "display-tft-st7701s/mtb_display_st7701s.h"
#include "app_common.h"

typedef enum {
    STATE_NONE,
    STATE_HEATING,
    STATE_COOLING
}animation_state_t;

extern lv_timer_t *temp_timer;


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

extern uint8_t brightness_level;
extern uint8_t audio_level;
extern char device_unique_id[13];
extern device_settings_t current_settings;
extern volatile application_state_t app_state;
extern volatile bool popup_overlay_visible;

void decrease_temp_step (lv_timer_t * timer);
void increase_temp_step (lv_timer_t * timer);
void display_mic_state(void);
void mic_icon_click_handler(lv_event_t * e);
void connect_wifi(lv_event_t * e);
void hide_notification(lv_timer_t * timer);
void update_device_connection_state(uint8_t state);
void update_thermostat_mode(thermostat_mode_t mode);
void update_device_temp(uint8_t temp);
void display_ble_pairing_window(bool hide, char *code);
void set_thermostat_mode(thermostat_mode_t mode);
void update_fan_mode(fan_speed_t mode);
fan_speed_t get_current_fan_mode(void);
thermostat_mode_t get_current_device_mode(void);
uint8_t get_current_brigthness(void);
int get_current_temperature(void);
uint8_t get_target_temperature(void);
void update_notifcation_label(notification_type type, notification_status_t status, uint32_t value);
void load_thermostat_config(thermostat_mode_t mode);
void change_idle_timeout(lv_event_t * e);
void update_temperature(lv_event_t * e);
void update_thermostat_mode_timer(void);
void set_idle_timeout(idle_timeout_t time);
void fw_update_check_ui(lv_event_t * e);
void change_volume(lv_event_t * e);
void update_thermostat_volume(audio_level_t level);
void set_system_unit(lv_event_t * e);
void update_system_unit(system_unit_t unit);
void get_default_device_setting(device_settings_t *settings);
void get_current_device_setting(device_settings_t *settings);
void set_current_device_setting(device_settings_t *settings);
void update_current_device_setting(void);
void device_factory_reset(lv_event_t * e);
void update_device_config_ipc(void);
void start_inactivity_timer(void);
void show_presence_icon_and_update_label(uint8_t person_count);
void hide_presence_icon(void);
