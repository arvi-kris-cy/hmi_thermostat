#ifndef THERMOSTAT_EVENTS_H
#define THERMOSTAT_EVENTS_H
#endif

#include "ui/ui_events.h"
#include "ui/ui.h"
#include "display_driver/mtb_display_st7701s.h"

typedef enum {
    STATE_NONE,
    STATE_HEATING,
    STATE_COOLING
}animation_state_t;

static animation_state_t current_state = STATE_NONE;
extern lv_timer_t *temp_timer;

typedef enum {
    FAN_OFF,
    FAN_HIGH,
    FAN_MED,
    FAN_LOW
}fan_speed_t;

static fan_speed_t current_fan_speed = FAN_OFF;

static bool is_fan_dragged = false;
static bool is_fan_dragged_by_temp = false;
static bool is_fan_dragged_by_button = false;

extern uint8_t brightness_level;