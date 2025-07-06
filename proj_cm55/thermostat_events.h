#ifndef THERMOSTAT_EVENTS_H
#define THERMOSTAT_EVENTS_H
#endif

#include "ui/ui_events.h"
#include "ui/ui.h"
#include "display_driver/mtb_display_st7701s.h"
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
extern char device_unique_id[13];

void decrease_temp_step (lv_timer_t * timer);
void increase_temp_step (lv_timer_t * timer);
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
