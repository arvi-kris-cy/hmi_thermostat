
/* Header file includes */
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
#include "app_audio.h"

/******************************************************************************
 * Macros
 ******************************************************************************/
#define CM55_APP_DELAY_MS           (50U)
#define ECO_MODE_TEMP_TIMER_TIMEOUT		5000U
#define RAPID_MODE_TEMP_TIMER_TIMEOUT		1000U
#define AUTO_MODE_TEMP_TIMER_TIMEOUT		2500U
#define NOTIFICATION_TIMEOUT_MS		1500U

// Timeout in milliseconds (e.g., 2000ms = 2 seconds)
#define WIFI_ILABEL_TIMER_TIMEOUT_MS 2000

#define TEMPERATURE_MAX_VALUE 30
#define TEMPERATURE_MIN_VALUE 14

#define AUTO_MODE_TARGET_TEMP_DEFAULT	22U

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
static int temperature = 24;
static int current_temp = 24;
static int target_temp;

device_state_t dev_info;

lv_timer_t *temp_timer = NULL;

CY_SECTION_SHAREDMEM static ipc_msg_t cm55_msg_data;
CY_SECTION_SHAREDMEM static app_event_t cm55_app_evt;

static mic_state_t current_mic_state = MIC_DISABLED;
static thermostat_mode_t  dev_current_mode  = MODE_ECO;
static fan_speed_t  dev_fan_mode  = FAN_OFF;
static volatile uint32_t deg2sec = ECO_MODE_TEMP_TIMER_TIMEOUT;	/**/

// Timer handle (must be global or static)
static TimerHandle_t wifi_ilabel_timer = NULL;
static TimerHandle_t wifi_kybd_label_timer = NULL;
char subinfo[64] = {0};

char device_unique_id[13] = {0};
static int weather_temp = 28;

lv_timer_t *start_listening_timer = NULL;
lv_timer_t *stop_listening_timer = NULL;

static provisioning_method_t prov_method  = PROV_MAPP_BLE;
static animation_state_t current_state = STATE_NONE;

// Declare state array for cycling (without DEV_ST_IDLE but including cloud and wifi disconnects)
static const device_connection_state_t ble_prov_states[] = {
    DEV_ST_BLE_ADVERTISING,
	DEV_ST_BLE_PAIRING,
    DEV_ST_BLE_CONNECTED,
    DEV_ST_WIFI_CONNECTING,
    DEV_ST_WIFI_CONNECTED,
	DEV_ST_CLOUD_CONNECTING,
    DEV_ST_CLOUD_CONNECTED,
};
#define BLE_STATE_COUNT (sizeof(ble_prov_states) / sizeof(ble_prov_states[0]))

// Declare state array for cycling (without DEV_ST_IDLE but including cloud and wifi disconnects)
static const device_connection_state_t kybd_prov_states[] = {
    DEV_ST_WIFI_CONNECTING,
    DEV_ST_WIFI_CONNECTED,
	DEV_ST_CLOUD_CONNECTING,
    DEV_ST_CLOUD_CONNECTED,
};
#define KYBD_STATE_COUNT (sizeof(kybd_prov_states) / sizeof(kybd_prov_states[0]))


static uint32_t current_test_state_idx = 0;
lv_timer_t *state_update_timer = NULL;


static qr_manager_t qr_manager;

//Notifcation related
#define MAX_NOTIF_QUEUE 5

typedef struct {
    notification_type type;
    notification_status_t status;
    uint32_t value;
} notification_data_t;

static notification_data_t notif_queue[MAX_NOTIF_QUEUE];
static int notif_head = 0;
static int notif_tail = 0;
static bool notif_showing = false;
static lv_timer_t * notif_timer = NULL;

/*****************************************************************************
 * Function Declaration
 *****************************************************************************/
static int get_deg_delay_sec(thermostat_mode_t mode);
static uint32_t calculate_remaining_time_sec(thermostat_mode_t mode, int current_temp, int set_temp);
static void generate_thermostat_time_str(thermostat_mode_t mode, int current_temp, int set_temp, char *out_str, size_t len);
static void set_volume(audio_level_t level);
static void fan_off(void);
static void fan_high(void);
static void fan_med(void);
static void fan_low(void);
static void update_mode_label(thermostat_mode_t mode);
static void mic_stop_listening(void);
static void mic_activate_listening(void);
static void show_next_notification(void);
static void enqueue_notification(notification_type type, notification_status_t status, uint32_t value);

/*****************************************************************************
 * Function Definitions
 *****************************************************************************/
// Get time delay per degree change
static int get_deg_delay_sec(thermostat_mode_t mode) {
    switch (mode) {
        case MODE_ECO:   return 5;
        case MODE_RAPID: return 1;
        case MODE_AUTO:  return 3;  // Example: configurable
        default:         return -1; // MODE_OFF or invalid
    }
}

static uint32_t calculate_remaining_time_sec(thermostat_mode_t mode, int current_temp, int set_temp) {
    int delay_per_deg = get_deg_delay_sec(mode);

    if (delay_per_deg <= 0) {
        return 0;  // OFF mode or invalid
    }

    int temp_diff = abs(set_temp - current_temp);  // C2S
    return temp_diff * delay_per_deg;
}

static void generate_thermostat_time_str(thermostat_mode_t mode, int current_temp, int set_temp, char *out_str, size_t len) {
	static uint32_t update_time = 0;

    if (mode == MODE_OFF) {
        snprintf(out_str, len, "System is OFF");
        return;
    }

    int temp_diff = set_temp - current_temp;
    if (temp_diff == 0) {
        snprintf(out_str, len, "Temperature already at %d°C", set_temp);
        return;
    }

    uint32_t delay_sec = calculate_remaining_time_sec(mode, current_temp, set_temp);
    uint32_t delay_min = (delay_sec + 59) / 60;
    if(update_time != delay_min) {
    	update_time = delay_min;
//    	update_remainig_time_ipc(delay_sec);
    }

    if (temp_diff < 0) {
        // Cooling
        snprintf(out_str, len, "%u min until cooled to %d°C", (unsigned int)delay_min, set_temp);
    } else {
        // Heating
        snprintf(out_str, len, "%u min until heated to %d°C", (unsigned int)delay_min, set_temp);
    }

    dev_info.thermostat_settings.time_remains = delay_sec;
}

void update_setto_label(lv_event_t * e)
{
	if (temp_timer){
        _ui_flag_modify(ui_settolbl, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
	}
}


void increase_temp(lv_event_t * e)
{
	if(dev_current_mode != MODE_OFF) {
		if (temperature < TEMPERATURE_MAX_VALUE) {
			target_temp = ++temperature;
			lv_label_set_text_fmt(ui_currenttemp,"%d°c",temperature);
			lv_obj_set_style_text_color(ui_currenttemp, lv_color_hex(0xF44336), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_label_set_text_fmt(ui_MainTemptextLP, "%d°c",temperature);
			lv_obj_add_flag(ui_bluecontainer,LV_OBJ_FLAG_HIDDEN);
			lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
			lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
			_ui_flag_modify(ui_redcontainer, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
			lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
			red1anim_Animation(ui_red1, 1500);
			red2anim_Animation(ui_red2, 1000);
			red3anim_Animation(ui_red3, 500);
			red4anim_Animation(ui_red4, 200);
			current_state = STATE_HEATING;

		   if (temp_timer){
			 lv_timer_del(temp_timer);
			 temp_timer=NULL;
		   }
		   temp_timer=lv_timer_create(increase_temp_step,deg2sec,NULL);
		   lv_label_set_text_fmt(ui_Mainroomtextactive, ". . . Heating . . .");

 	      dev_info.environment.target_temp = target_temp;
 	      update_temperature_data_ipc(&dev_info);

		   generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
		   lv_label_set_text(ui_homescreensubmsg, subinfo);
		   lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
		}
	}
	else
	{
		printf("Increase temperature error. Device in Off mode.\n");
	}
}

 void decrease_temp(lv_event_t * e)
{
	 if(dev_current_mode != MODE_OFF) {
		 if (temperature > TEMPERATURE_MIN_VALUE) {
			target_temp = --temperature;
			lv_label_set_text_fmt(ui_currenttemp,"%d°c",temperature);
			lv_obj_set_style_text_color(ui_currenttemp, lv_color_hex(0xC6FFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_label_set_text_fmt(ui_MainTemptextLP, "%d°c",temperature);
			lv_obj_add_flag(ui_redcontainer,LV_OBJ_FLAG_HIDDEN);
			lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
			lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
			_ui_flag_modify(ui_bluecontainer, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
			lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
			blue1anim_Animation(ui_blue1, 300);
			blue2anim_Animation(ui_blue2, 500);
			blue3anim_Animation(ui_blue3, 800);
			blue4anim_Animation(ui_blue4, 1300);
			current_state = STATE_COOLING;
			if (temp_timer){
				lv_timer_del(temp_timer);
				temp_timer=NULL;
			}
			temp_timer=lv_timer_create(decrease_temp_step,deg2sec,NULL);
			lv_label_set_text_fmt(ui_Mainroomtextactive, ". . . Cooling . . .");
 	      dev_info.environment.target_temp = target_temp;
 	      update_temperature_data_ipc(&dev_info);
			generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
			lv_label_set_text(ui_homescreensubmsg, subinfo);
			lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
		 }
		 else
		 {
			    printf("Thermostat Minimum temperature reached.\n");
		 }
	}
 	else
 	{
 		printf("Decrease temperature error. Device in Off mode.\n");
 	}
}

void increase_temp_step (lv_timer_t * timer){

	if(dev_current_mode != MODE_OFF) {
		if (current_temp < target_temp){
			current_temp++;
			lv_label_set_text_fmt(ui_MainTempactive, "%d°c",current_temp);
		   lv_label_set_text_fmt(ui_Mainroomtextactive, ". . . Heating . . .");
//			lv_arc_set_value(ui_temperaturearc, current_temp);
		   generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
		   lv_label_set_text(ui_homescreensubmsg, subinfo);
		   lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
		}
		if (current_temp == target_temp){
			lv_timer_del(timer);
			temp_timer=NULL;
			lv_label_set_text_fmt(ui_MainTempactive, "%d°c",current_temp);
			lv_arc_set_value(ui_temperaturearc, current_temp);
			lv_obj_add_flag(ui_redcontainer,LV_OBJ_FLAG_HIDDEN);

		   lv_obj_add_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);

			lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
			update_mode_label(dev_current_mode);
			enqueue_notification(NOTIFY_TEMP_UPDATE, NOTIF_SUCCESS, current_temp);
   	        dev_info.thermostat_settings.time_remains = 0;

		}

	      dev_info.environment.current_temp = current_temp;
	      update_temperature_data_ipc(&dev_info);
	}
}

void decrease_temp_step (lv_timer_t * timer){
	if(dev_current_mode != MODE_OFF) {
		lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
//		lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
		if (current_temp > target_temp){
			current_temp--;
			lv_label_set_text_fmt(ui_Mainroomtextactive, ". . . Cooling . . .");

			lv_label_set_text_fmt(ui_MainTempactive, "%d°c",current_temp);
//			lv_arc_set_value(ui_temperaturearc, current_temp);

		   generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
		   lv_label_set_text(ui_homescreensubmsg, subinfo);
		   lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
		}
		if (current_temp == target_temp){
			lv_timer_del(timer);
			temp_timer=NULL;
			lv_label_set_text_fmt(ui_MainTempactive, "%d°c",current_temp);
			lv_arc_set_value(ui_temperaturearc, current_temp);
			lv_obj_add_flag(ui_bluecontainer,LV_OBJ_FLAG_HIDDEN);
  		   lv_obj_add_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
			update_mode_label(dev_current_mode);
			enqueue_notification(NOTIFY_TEMP_UPDATE, NOTIF_SUCCESS, current_temp);
			dev_info.thermostat_settings.time_remains = 0;
		}

	      dev_info.environment.current_temp = current_temp;
	      update_temperature_data_ipc(&dev_info);
	}
}

 void change_room(lv_event_t * e)
{
	typedef enum
	{
	  Bedroom,
	  Kitchen,
	  Dining,
	  Living
	} room_select;
	
	static int index = 0;
	static const char * rooms[]= {"Kitchen","Dining","Bedroom"};
	lv_label_set_text_fmt(ui_Roomtextactive,"%s",rooms[index]);
	room_select current_room= (room_select)index;
	index = (index + 1 ) % 3;

	switch (current_room)
	{
	case Bedroom:
	    lv_label_set_text(ui_RoomTempactive,"19°c");
		break;
	case Kitchen:
	    lv_label_set_text(ui_RoomTempactive,"24°c");
		break;
	case Dining:
		lv_label_set_text(ui_RoomTempactive,"22°c");
		break;	
	default:
		break;
	}
}

void weather_change(lv_event_t * e){
        static int index = 0; 
    
        // Hide all containers 
        lv_obj_add_flag(ui_weathercontainer1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_weathercontainer2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_weathercontainer3, LV_OBJ_FLAG_HIDDEN);
    
        switch (index) {
            case 0:
                lv_obj_clear_flag(ui_weathercontainer1, LV_OBJ_FLAG_HIDDEN);
                break;
            case 1:
                lv_obj_clear_flag(ui_weathercontainer2, LV_OBJ_FLAG_HIDDEN);
                break;
            case 2:
                lv_obj_clear_flag(ui_weathercontainer3, LV_OBJ_FLAG_HIDDEN);
                break;
        }
    
        index = (index + 1) % 3;
}

void update_fan_mode(fan_speed_t mode)
{
	switch (mode) {
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
		break;

	default:
		fan_off();
		lv_label_set_text_fmt(ui_fanmodelabel, "OFF");
		break;
	}

	dev_fan_mode = mode;

	update_fan_speed_ipc(dev_fan_mode);
}

void fan_clicked(lv_event_t * e)
{
	if(dev_current_mode != MODE_OFF)
	{
		/* Update fan mode */
		dev_fan_mode = (dev_fan_mode + 1) % FAN_MODE_MAX;
		update_fan_mode(dev_fan_mode);
	}
}

static void fan_off(void){
	dev_fan_mode = FAN_OFF;

    lv_anim_del(ui_fanactive, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    // Hide all images first
   lv_obj_add_flag(ui_fanactive, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

   lv_obj_clear_flag(ui_fanactive, LV_OBJ_FLAG_HIDDEN);
   fanspeed_Animation(ui_fanactive, 0);
}

static void fan_high(void){
    dev_fan_mode = FAN_HIGH;

    lv_anim_del(ui_fanactive, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    // Hide all images first
   lv_obj_add_flag(ui_fanactive, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

   lv_obj_clear_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
   fanspeedhigh_Animation(ui_fanactivehigh, 0);
}

static void fan_med(void){
	dev_fan_mode = FAN_MED;

    lv_anim_del(ui_fanactive, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    // Hide all images first
   lv_obj_add_flag(ui_fanactive, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

   lv_obj_clear_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);
   fanspeedmed_Animation(ui_fanactivemed, 0);
}

static void fan_low(void){
	dev_fan_mode = FAN_LOW;

    lv_anim_del(ui_fanactive, NULL);
    lv_anim_del(ui_fanactivehigh, NULL);
    lv_anim_del(ui_fanactivelow, NULL);
    lv_anim_del(ui_fanactivemed, NULL);

    // Hide all images first
   lv_obj_add_flag(ui_fanactive, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivehigh, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
   lv_obj_add_flag(ui_fanactivemed, LV_OBJ_FLAG_HIDDEN);

   lv_obj_clear_flag(ui_fanactivelow, LV_OBJ_FLAG_HIDDEN);
   fanspeedlow_Animation(ui_fanactivelow, 0);
}

void weatherup(lv_event_t * e){
    weather_temp++;
	lv_label_set_text_fmt(ui_container1text, "%d°c",weather_temp);
    lv_label_set_text_fmt(ui_container2text, "%d°c",weather_temp);
    lv_label_set_text_fmt(ui_container3text, "%d°c",weather_temp);
}

void weatherdown(lv_event_t * e){
    weather_temp--;
	lv_label_set_text_fmt(ui_container1text, "%d°c",weather_temp);
    lv_label_set_text_fmt(ui_container2text, "%d°c",weather_temp);
    lv_label_set_text_fmt(ui_container3text, "%d°c",weather_temp);
}

void update_display_brightness(uint8_t level)
{
	lv_slider_set_value(ui_Slider2, level, LV_ANIM_OFF);
    mtb_display_st7701s_set_brightness(level);
    brightness_level = level;
	dev_info.preferences.display_brightness = level;
}

void change_brightness(lv_event_t * e){
     uint8_t slider_val  = lv_slider_get_value(ui_Slider2);

     if((slider_val % 10) > 5)
     {
    	 brightness_level = ((slider_val/10)*10)+10;
     }
     else
     {
    	 brightness_level = ((slider_val/10)*10);
     }

     printf("Brightness Level: %d Actual Level: %d", brightness_level, slider_val);
     lv_slider_set_value(ui_Slider2, brightness_level, LV_ANIM_OFF);
     update_display_brightness(brightness_level);
     update_brightness_ipc(brightness_level);
}

void show_presence_icon_and_update_label(uint8_t person_count)
{
    char buf[10];
    sprintf(buf, "x %d", person_count);

    /* Set full opacity for the presence icon, and unhide the
     * presence label if count is >1 */
    lv_obj_set_style_opa(ui_presence, LV_OPA_COVER, 0);

    if(person_count > 1) {
        lv_obj_clear_flag(ui_presencecountlabel, LV_OBJ_FLAG_HIDDEN);

        lv_label_set_text(ui_presencecountlabel, buf);
    }
}

void hide_presence_icon(void) {
    lv_obj_set_style_opa(ui_presence, 60, 0);
    lv_obj_add_flag(ui_presencecountlabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_presencecountcircle, LV_OBJ_FLAG_HIDDEN);
}

void mic_stop_listening_cb(lv_timer_t *timer) {
    stop_listening_timer = NULL; // Timer is one-shot

    if (current_mic_state == MIC_ACTIVE) {
        mic_stop_listening();
    }
}

void mic_start_listening_cb(lv_timer_t *timer) {
    start_listening_timer = NULL; // Timer is one-shot

    if (current_mic_state == MIC_IDLE) {
        mic_activate_listening();

        // Start timer to stop listening after 7 seconds
        stop_listening_timer = lv_timer_create(mic_stop_listening_cb, 7000, NULL);
        lv_timer_set_repeat_count(stop_listening_timer, 1);  // One-shot
    }
}

void mic_icon_click_handler(lv_event_t * e) {

    // Switch state to next
	    switch(current_mic_state)
	    {
	        case MIC_DISABLED:
	            current_mic_state = MIC_IDLE;
	            break;
	        case MIC_IDLE:
	            current_mic_state = MIC_DISABLED;
	            break;
	        default:
	        	break;
	    }

	    // Always delete any ongoing animation on active listen image
	    lv_anim_del(ui_micactivelisten, NULL);

	    // Hide all images first
	    lv_obj_add_flag(ui_micdisabled, LV_OBJ_FLAG_HIDDEN);
	    lv_obj_add_flag(ui_micidle, LV_OBJ_FLAG_HIDDEN);
	    lv_obj_add_flag(ui_micactivelisten, LV_OBJ_FLAG_HIDDEN);

	    // Show selected state and handle animation if needed
	    switch(current_mic_state)
	    {
	        case MIC_DISABLED:
	            lv_obj_clear_flag(ui_micdisabled, LV_OBJ_FLAG_HIDDEN);
	            break;

	        case MIC_IDLE:
	            lv_obj_clear_flag(ui_micidle, LV_OBJ_FLAG_HIDDEN);
				start_listening_timer = lv_timer_create(mic_start_listening_cb, 3000, NULL);
				lv_timer_set_repeat_count(start_listening_timer, 1);  // One-shot
	            break;
	        default:
	        	break;
	    }
}


static void mic_activate_listening(void) {
    if (current_mic_state == MIC_IDLE) {

	    // Hide all images first
	    lv_obj_add_flag(ui_micdisabled, LV_OBJ_FLAG_HIDDEN);
	    lv_obj_add_flag(ui_micidle, LV_OBJ_FLAG_HIDDEN);
	    lv_obj_add_flag(ui_micactivelisten, LV_OBJ_FLAG_HIDDEN);
	    lv_obj_clear_flag(ui_micactivelisten, LV_OBJ_FLAG_HIDDEN);

//        lv_img_set_src(ui_micdisabled, &ui_img_mic_listening_png);
//        lv_obj_set_style_opa(ui_micdisabled, 255, 0); // Full visible
        miclisteninganime_Animation(ui_micactivelisten, 0);
        current_mic_state = MIC_ACTIVE;
    }
}

void recreate_mic_idle(void)
{
    // Delete the old object if it exists
    if (ui_micdisabled != NULL) {
        lv_obj_del(ui_micdisabled);
        ui_micdisabled = NULL;
    }

    // Recreate the mic image object
    ui_micdisabled = lv_img_create(ui_ActiveScreen);
    lv_img_set_src(ui_micdisabled, &ui_img_voice_home_icon_png);
    lv_obj_set_width(ui_micdisabled, LV_SIZE_CONTENT);   // 50
    lv_obj_set_height(ui_micdisabled, LV_SIZE_CONTENT);  // 50
    lv_obj_set_x(ui_micdisabled, 75);
    lv_obj_set_y(ui_micdisabled, 189);
    lv_obj_set_align(ui_micdisabled, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_micdisabled, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui_micdisabled, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_opa(ui_micdisabled, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    current_mic_state = MIC_IDLE;
}


static void mic_stop_listening(void)
{
    lv_obj_add_flag(ui_micdisabled, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_micidle, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_micactivelisten, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clear_flag(ui_micidle, LV_OBJ_FLAG_HIDDEN);

    current_mic_state = MIC_IDLE;
}


// Timer callback to cycle through states
static void device_state_test_cb(lv_timer_t * timer)
{
    if (PROV_MAPP_BLE == prov_method) {
        if (current_test_state_idx < BLE_STATE_COUNT) {
            update_device_connection_state(ble_prov_states[current_test_state_idx]);
            current_test_state_idx++;
        }
    }
    else if (PROV_UI_KEYBOARD == prov_method) {
        if (current_test_state_idx < KYBD_STATE_COUNT) {
            update_device_connection_state(kybd_prov_states[current_test_state_idx]);
            current_test_state_idx++;
        }
    }
}



// Call this function after your screen is initialized
void start_device_state_test_timer()
{
	// Start timer to stop listening after 7 seconds
	state_update_timer = lv_timer_create(device_state_test_cb, 5000, NULL);
	lv_timer_set_repeat_count(state_update_timer, 1);  // One-shot
}

void update_pin_label(const char *pin)
{
    lv_label_set_text(ui_pinlabel, pin);
}


void update_device_connection_state(device_connection_state_t state)
{
    const char* state_text = "Unknown";

	lv_obj_add_flag(ui_commissionMapp,LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_commissionKeyboard,LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_devconnstatecontianer,LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_blepairingcode,LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_qrcodecontainer,LV_OBJ_FLAG_HIDDEN);

//    lv_obj_clear_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

    // Clear any previous animation
     lv_anim_del(ui_bleadv, NULL);
     lv_anim_del(ui_wificonnecting120, NULL);
     lv_anim_del(ui_wifi, NULL);
     //homescreen icon anim
     lv_anim_del(ui_homebleadv, NULL);
     lv_anim_del(ui_wifi, NULL);

     lv_anim_del(ui_connectionprg1, NULL);
     lv_anim_del(ui_connectionprg2, NULL);
     lv_anim_del(ui_connectionprg3, NULL);


     // Hide all images first
    lv_obj_add_flag(ui_cloudconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wificonnecting120, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_clouddisconn120, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleadv, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_devstateimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wificonnectedimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifidisccconnectedimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_bleconnected120, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homebleadv, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homeclouddisconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homewifidisconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_homecloudconnected, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_qrcodebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_qrcodebtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_cloudconnecting, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_wifistatic, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_connectionprg1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionprg2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionprg3, LV_OBJ_FLAG_HIDDEN);

    // Show only the selected state
    switch(state)
    {
        case DEV_ST_CLOUD_CONNECTED:
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_cloudconnected, LV_OBJ_FLAG_HIDDEN);
            state_text = "Cloud Connected";
            lv_obj_clear_flag(ui_qrcodebtn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifideletebtn, LV_OBJ_FLAG_HIDDEN);

            /* home screen icon */
            lv_obj_clear_flag(ui_homecloudconnected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homecloudconnected, 255, 0);

            enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_SUCCESS, DEV_ST_CLOUD_CONNECTED);
            break;

        case DEV_ST_CLOUD_CONNECTING:
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_cloudconnecting, LV_OBJ_FLAG_HIDDEN);
            state_text = "Cloud Connecting . . .";

            /* home screen icon */
            lv_obj_clear_flag(ui_homeclouddisconnected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homeclouddisconnected, 255, 0);
        	break;

        case DEV_ST_WIFI_CONNECTING:
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_wificonnecting120, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connectionprg1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connectionprg2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_connectionprg3, LV_OBJ_FLAG_HIDDEN);
            progress1_Animation(ui_connectionprg1, 0);
            progress2_Animation(ui_connectionprg2, 0);
            progress3_Animation(ui_connectionprg3, 0);
            state_text = "Wi-Fi Connecting . . .";

            /* home screen icon */
            lv_obj_clear_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_wifi, 255, 0);
            wifi_Animation(ui_wifi, 0);
            break;

        case DEV_ST_CLOUD_DISCONNECTED:
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_clouddisconn120, LV_OBJ_FLAG_HIDDEN);
            state_text = "Cloud Disconnected";

            /* home screen icon */
            lv_obj_clear_flag(ui_homeclouddisconnected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homeclouddisconnected, 255, 0);

            enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_FAIL, DEV_ST_CLOUD_DISCONNECTED);
            break;

        case DEV_ST_BLE_ADVERTISING:
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_bleadv, LV_OBJ_FLAG_HIDDEN);
			bleadvpulse_Animation(ui_bleadv, 0);
//			miclisteninganime_Animation(ui_bleadv,0);
            state_text = "Waiting for mobile connection . . .";

            /* home screen icon */
            lv_obj_clear_flag(ui_homebleadv, LV_OBJ_FLAG_HIDDEN);
            bleadvpulse_Animation(ui_homebleadv,0);
            lv_obj_set_style_opa(ui_homebleadv, 255, 0);
            break;

        case DEV_ST_BLE_PAIRING:
        	lv_obj_add_flag(ui_commissionMapp,LV_OBJ_FLAG_HIDDEN);
        	lv_obj_add_flag(ui_commissionKeyboard,LV_OBJ_FLAG_HIDDEN);
        	lv_obj_add_flag(ui_devconnstatecontianer,LV_OBJ_FLAG_HIDDEN);
        	lv_obj_add_flag(ui_blepairingcode,LV_OBJ_FLAG_HIDDEN);
        	lv_obj_add_flag(ui_qrcodecontainer,LV_OBJ_FLAG_HIDDEN);
        	lv_obj_add_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
        	lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

        	lv_obj_clear_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
        	lv_obj_clear_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);

            /* home screen icon */
            lv_obj_clear_flag(ui_homebleadv, LV_OBJ_FLAG_HIDDEN);
            miclisteninganime_Animation(ui_homebleadv,0);
            lv_obj_set_style_opa(ui_homebleadv, 255, 0);
            break;

        case DEV_ST_UNPROVISIONED:
            lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_qrcodecontainer, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_commissionKeyboard, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_commissionMapp, LV_OBJ_FLAG_HIDDEN);

            /* home screen icon */
            lv_obj_add_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_wifistatic, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_wifistatic, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            break;

        case DEV_ST_WIFI_CONNECTED:
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_wificonnectedimg, LV_OBJ_FLAG_HIDDEN);
            state_text = "Wi-Fi Connected";

            /* home screen icon */
            lv_obj_clear_flag(ui_homeclouddisconnected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homeclouddisconnected, 255, 0);
            enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_SUCCESS, DEV_ST_WIFI_CONNECTED);
            break;

        case DEV_ST_WIFI_DISCONNECTED:
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_wifidisccconnectedimg, LV_OBJ_FLAG_HIDDEN);
            state_text = "Wi-Fi Disconnected";

            /* home screen icon */
            lv_obj_clear_flag(ui_homewifidisconnected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_homewifidisconnected, 255, 0);

            enqueue_notification(NOTIFY_NETWORK_STATUS, NOTIF_FAIL,DEV_ST_WIFI_DISCONNECTED);

        	break;

        case DEV_ST_BLE_CONNECTED:
            lv_obj_add_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(ui_bleconnected120, LV_OBJ_FLAG_HIDDEN);
            state_text = "BLE Connected";

            /* home screen icon */
            lv_obj_clear_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(ui_wifi, 255, 0);
            wifi_Animation(ui_wifi, 0);
            break;

        default:
            // Fallback safety: hide all
            break;
    }

    // Update label text
    lv_label_set_text(ui_devstatelabel, state_text);
}


void start_ble_adv(lv_event_t * e) {
	printf("CM55: Start BLE ADV\r\n");

	prov_method = PROV_MAPP_BLE;
	update_device_connection_state(DEV_ST_BLE_ADVERTISING);

	memset(&cm55_app_evt, 0, sizeof(cm55_app_evt));
	cm55_app_evt.type = EVT_BLE_START_ADV;

	cm55_msg_data.client_id = CM33_IPC_PIPE_CLIENT_ID;
	cm55_msg_data.intr_mask = CY_IPC_CYPIPE_INTR_MASK_EP2;
	cm55_msg_data.cmd = CMD_CM55_UI_CONTROL;
//	cm55_msg_data.value = (uintptr_t)&cm55_app_evt;

//	send_mg_cm33(&cm55_msg_data);

	update_ble_adv_ipc(DEV_ST_BLE_ADVERTISING);
}


void connect_wifi(lv_event_t * e)
{
    const char * ssid = lv_textarea_get_text(ui_ssidfield);
    const char * password = lv_textarea_get_text(ui_passwordfield);

	prov_method = PROV_UI_KEYBOARD;

    /* Start WiFi Animation */
    update_device_connection_state(DEV_ST_WIFI_CONNECTING);

    LV_LOG_USER("SSID: %s", ssid);
    LV_LOG_USER("Password: %s", password);

    update_wifi_cred_ipc(ssid, password);
}

// Separate callback (C89 compliant)
void hide_notification_ready_cb(lv_anim_t * a)
{
    lv_obj_add_flag(ui_notification, LV_OBJ_FLAG_HIDDEN);

    // ✅ Move to next message
    notif_head = (notif_head + 1) % MAX_NOTIF_QUEUE;
    notif_showing = false;

    // Clear old message (optional)
    memset(&notif_queue[notif_head], 0, sizeof(notification_data_t));

    show_next_notification();
}

void hide_notification(lv_timer_t * timer)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, ui_notification);
    lv_anim_set_values(&a, -197, -297);
    lv_anim_set_time(&a, 300);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_ready_cb(&a, hide_notification_ready_cb);
    lv_anim_start(&a);

    lv_timer_del(timer);
    notif_timer = NULL;
}



static bool is_duplicate(notification_type type, notification_status_t status, uint32_t value)
{
    int i = notif_head;
    while (i != notif_tail) {
        if (notif_queue[i].type == type &&
            notif_queue[i].status == status &&
            notif_queue[i].value == value) {
            return true;
        }
        i = (i + 1) % MAX_NOTIF_QUEUE;
    }
    return false;
}

static void enqueue_notification(notification_type type, notification_status_t status, uint32_t value)
{
    if (is_duplicate(type, status, value)) return;

    int next_tail = (notif_tail + 1) % MAX_NOTIF_QUEUE;
    if (next_tail == notif_head) {
        // Queue full, drop notification
        return;
    }

    notif_queue[notif_tail].type = type;
    notif_queue[notif_tail].status = status;
    notif_queue[notif_tail].value = value;
    notif_tail = next_tail;

    if (!notif_showing) {
        show_next_notification();
    }
}

static void show_next_notification()
{
    if (notif_head == notif_tail) {
        notif_showing = false;
        return;  // Queue empty
    }

    notification_data_t *n = &notif_queue[notif_head];
    update_notifcation_label(n->type, n->status, n->value);

    lv_obj_clear_flag(ui_notification, LV_OBJ_FLAG_HIDDEN);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, ui_notification);
    lv_anim_set_values(&a, -297, -197);
    lv_anim_set_time(&a, 300);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_start(&a);

    notif_showing = true;

    if (notif_timer) {
        lv_timer_del(notif_timer);
    }
    notif_timer = lv_timer_create(hide_notification, NOTIFICATION_TIMEOUT_MS, NULL);

    app_speaker_play();
}


void update_notifcation_label(notification_type type, notification_status_t status, uint32_t value)
{
	if (status == NOTIF_FAIL)
	{
		/* Set image in notification label */
        lv_img_set_src(ui_notificationlblimg, &ui_img_incorrect_img_png);

		/* Set text in notification label */
		switch (type)
		{
			case NOTIFY_NETWORK_STATUS:
				switch((device_connection_state_t)value)
				{
				case DEV_ST_CLOUD_DISCONNECTED:
					lv_label_set_text(ui_notificationlabel, "Disconnected from Cloud.");
					break;

				case DEV_ST_WIFI_DISCONNECTED:
					lv_label_set_text(ui_notificationlabel, "Disconnected from WiFi network.");
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
			default:
				break;
		}
	}
	else if (status == NOTIF_SUCCESS)
	{
		/* Set image in notification label */
        lv_img_set_src(ui_notificationlblimg, &ui_img_correct_img_png);

		/* Set text in notification label */
		switch (type)
		{
			case NOTIFY_NETWORK_STATUS:
				switch((device_connection_state_t)value)
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
//				lv_label_set_text_fmt(ui_notificationlabel, "Temperature set to %lu°C.", value);
				lv_label_set_text_fmt(ui_notificationlabel, "Temperature set-point reached %lu°C.", value);
				break;
			case NOTIFY_MODE_UPDATE:
				lv_label_set_text_fmt(ui_notificationlabel, "Mode set to %lu.", value);  // You may map value to text
				break;
			case NOTIFY_FAN_MODE_UPDATE:
				lv_label_set_text_fmt(ui_notificationlabel, "Fan mode set to %lu.", value); // Same here
				break;
			default:
				break;
		}
	}
}


// Function to hide the label after 2 seconds
void hide_mappinfolabel(lv_timer_t * timer)
{
    lv_obj_add_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);
}

// Button tap event handler
void process_mappinfobutton_ex(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        // Show the label
        lv_obj_clear_flag(ui_mappinfolabel, LV_OBJ_FLAG_HIDDEN);

        // Start a 2-second (2000 ms) timer to hide the label
        lv_timer_create(hide_mappinfolabel, 2000, NULL);
    }
}

// Function to hide the label after 2 seconds
void hide_keyboardinfolabel(lv_timer_t * timer)
{
    lv_obj_add_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN);
}

// Button tap event handler
void process_keyboardinfobutton_ex(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
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
        wifi_ilabel_timer = xTimerCreate("WifiILabelTimer",
                                         pdMS_TO_TICKS(WIFI_ILABEL_TIMER_TIMEOUT_MS),
                                         pdFALSE,
                                         NULL,
                                         wifi_ilabel_timer_callback);
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
	if (!lv_obj_has_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN)) {
	    lv_obj_add_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN);
	}
}

void start_wifi_settings_kybd_label_timer_ex(void)
{
	if (lv_obj_has_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN)) {
	    lv_obj_clear_flag(ui_keyboardinfolabel, LV_OBJ_FLAG_HIDDEN);
	}
    if (wifi_kybd_label_timer == NULL)
    {
        // Create the timer (auto-reload = pdFALSE for one-shot)
    	wifi_kybd_label_timer = xTimerCreate("WifiKybdLabelTimer",
                                         pdMS_TO_TICKS(WIFI_ILABEL_TIMER_TIMEOUT_MS),
                                         pdFALSE,
                                         NULL,
										 wifi_kybd_label_timer_callback);
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
    // Update image based on current mode
    switch (mode)
    {
        case MODE_ECO:
            lv_img_set_src(ui_mode, &ui_img_eco_png);
            deg2sec = ECO_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_LOW);
            printf("Mode set to ECO\n");
            break;
        case MODE_RAPID:
            lv_img_set_src(ui_mode, &ui_img_mode_select_rapid_png);
            deg2sec = RAPID_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_HIGH);
            printf("Mode set to RAPID\n");
            break;
        case MODE_AUTO:
            lv_img_set_src(ui_mode, &ui_img_automode_png_png);
            deg2sec = AUTO_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_MED);
            printf("Mode set to AUTO\n");
        	update_device_temp(AUTO_MODE_TARGET_TEMP_DEFAULT);
            break;

        case MODE_OFF:
            lv_img_set_src(ui_mode, &ui_img_mode_select_fan_png);
            update_fan_mode(FAN_OFF);
            printf("Mode set to FAN\n");

            /* If temperature increaase decrease timer running, stop it */
    		if (temp_timer){
    			lv_timer_del(temp_timer);
    			temp_timer=NULL;
    		}

    		target_temp = current_temp;
    		temperature = current_temp;

			lv_obj_add_flag(ui_bluecontainer,LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(ui_redcontainer, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
			lv_arc_set_value(ui_temperaturearc, current_temp);
            break;

        default:
        	break;
    }

    update_mode_label(mode);
    update_device_mode_ipc(mode);
    update_thermostat_mode_timer();
}

void toggle_mode(lv_event_t * e)
{
    // Advance to next mode
	dev_current_mode = (dev_current_mode + 1) % MODE_MAX;
	set_thermostat_mode(dev_current_mode);
    lv_obj_set_style_opa(ui_mode, LV_OPA_COVER, 0);
}

void open_notifcaiton_ex(void)
{
	printf("Notification popup clicked.\n");

}
void delete_wifi_cred(lv_event_t * e)
{
	request_wifi_delete_ipc();
}

void display_qrcode(lv_event_t * e)
{
    qr_manager_init(&qr_manager, ui_qrcodecontainer, 120);
    qr_manager_update(&qr_manager, device_unique_id);
    lv_label_set_text(ui_qrcodelabel, "Scan this QR code using the mobile app.");
}

void hide_qr(lv_event_t * e)
{
    qr_manager_deinit(&qr_manager);
}


static void update_mode_label(thermostat_mode_t mode)
{
	switch (mode)
	{
	case MODE_ECO:
	     lv_label_set_text_fmt(ui_Mainroomtextactive, "ECO");
		break;

	case MODE_RAPID:
	     lv_label_set_text_fmt(ui_Mainroomtextactive, "RAPID");
		break;

	case MODE_AUTO:
	     lv_label_set_text_fmt(ui_Mainroomtextactive, "AUTO");
		break;

	case MODE_OFF:
	     lv_label_set_text_fmt(ui_Mainroomtextactive, "OFF");
		break;
    default:
    	break;
	}
}

void update_thermostat_mode(thermostat_mode_t mode)
{
    // Update image based on current mode
    switch (mode)
    {
        case MODE_ECO:
            lv_img_set_src(ui_mode, &ui_img_eco_png);
            deg2sec = ECO_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_LOW);
            printf("Mode set to ECO\n");
            break;
        case MODE_RAPID:
            lv_img_set_src(ui_mode, &ui_img_mode_select_rapid_png);
            deg2sec = RAPID_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_HIGH);
            printf("Mode set to RAPID\n");
            break;
        case MODE_AUTO:
            lv_img_set_src(ui_mode, &ui_img_mode_select_auto_png);
            deg2sec = AUTO_MODE_TEMP_TIMER_TIMEOUT;
            update_fan_mode(FAN_MED);
            printf("Mode set to AUTO\n");
        	update_device_temp(AUTO_MODE_TARGET_TEMP_DEFAULT);
            break;

        case MODE_OFF:
            lv_img_set_src(ui_mode, &ui_img_mode_select_fan_png);
            update_fan_mode(FAN_OFF);
            printf("Mode set to FAN\n");
            break;

        default:
        	break;
    }

    update_mode_label(mode);
    dev_current_mode = mode;
}


void update_device_temp(uint8_t temp)
{
	if(dev_current_mode != MODE_OFF) {
	    target_temp = temp;
	    temperature = target_temp;
	    dev_info.environment.target_temp = target_temp;

	    lv_label_set_text_fmt(ui_currenttemp,"%d°c",temperature);
	    lv_label_set_text_fmt(ui_MainTemptextLP, "%d°c",temperature);

	   if (temp_timer){
	     lv_timer_del(temp_timer);
	     temp_timer=NULL;
	   }

	   if(target_temp > current_temp)
	   {
		   lv_obj_set_style_text_color(ui_currenttemp, lv_color_hex(0xF44336), LV_PART_MAIN | LV_STATE_DEFAULT);
		    temp_timer=lv_timer_create(increase_temp_step,deg2sec,NULL);
		    lv_obj_add_flag(ui_bluecontainer,LV_OBJ_FLAG_HIDDEN);
		    lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
		    lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
		    _ui_flag_modify(ui_redcontainer, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
		    lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
		    red1anim_Animation(ui_red1, 1500);
		    red2anim_Animation(ui_red2, 1000);
		    red3anim_Animation(ui_red3, 500);
		    red4anim_Animation(ui_red4, 200);
		    current_state = STATE_HEATING;
			   lv_label_set_text_fmt(ui_Mainroomtextactive, ". . . Heating . . .");

			   generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
			   lv_label_set_text(ui_homescreensubmsg, subinfo);
			   lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
	   }
	   else if(target_temp < current_temp)
	   {
			lv_obj_set_style_text_color(ui_currenttemp, lv_color_hex(0xC6FFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_add_flag(ui_redcontainer,LV_OBJ_FLAG_HIDDEN);
			lv_obj_remove_flag(ui_settolbl, LV_OBJ_FLAG_HIDDEN);
			lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
			_ui_flag_modify(ui_bluecontainer, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
			lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
			blue1anim_Animation(ui_blue1, 300);
			blue2anim_Animation(ui_blue2, 500);
			blue3anim_Animation(ui_blue3, 800);
			blue4anim_Animation(ui_blue4, 1300);
		    current_state = STATE_COOLING;
		   temp_timer=lv_timer_create(decrease_temp_step,deg2sec,NULL);

			 lv_label_set_text_fmt(ui_Mainroomtextactive, ". . . Cooling . . .");

			   generate_thermostat_time_str(dev_current_mode, current_temp, target_temp, subinfo, sizeof(subinfo));
			   lv_label_set_text(ui_homescreensubmsg, subinfo);
			   lv_obj_clear_flag(ui_homescreensubmsg, LV_OBJ_FLAG_HIDDEN);
	   }
	}
	else
	{
		printf("Increase temperature error. Device in Off mode.\n");
	}

    update_temperature_data_ipc(&dev_info);
}

void display_ble_pairing_window(bool hide, char *code)
{
	if(hide)
	{
		lv_obj_add_flag(ui_popupoverlay,LV_OBJ_FLAG_HIDDEN);
	}
	else
	{
    	lv_obj_add_flag(ui_commissionMapp,LV_OBJ_FLAG_HIDDEN);
    	lv_obj_add_flag(ui_commissionKeyboard,LV_OBJ_FLAG_HIDDEN);
    	lv_obj_add_flag(ui_devconnstatecontianer,LV_OBJ_FLAG_HIDDEN);
    	lv_obj_add_flag(ui_blepairingcode,LV_OBJ_FLAG_HIDDEN);
    	lv_obj_add_flag(ui_qrcodecontainer,LV_OBJ_FLAG_HIDDEN);
    	lv_obj_add_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
    	lv_obj_add_flag(ui_devconnstatecontianer, LV_OBJ_FLAG_HIDDEN);

    	lv_obj_clear_flag(ui_popupoverlay, LV_OBJ_FLAG_HIDDEN);
    	lv_obj_clear_flag(ui_blepairingcode, LV_OBJ_FLAG_HIDDEN);

		lv_label_set_text_fmt(ui_pinlabel, code);
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
	switch(mode)
	{
	case MODE_ECO:
	case MODE_RAPID:
	case MODE_AUTO:
	case MODE_OFF:
		update_thermostat_mode(mode);
		lv_arc_set_value(ui_temperaturearc, current_temp);
		target_temp = current_temp;
		temperature = current_temp;
		break;

	default:
		break;
	}

	dev_current_mode = mode;
	dev_info.environment.current_temp 			= dev_info.environment.target_temp = temperature;
	dev_info.environment.current_co2_level 		= 315;
	dev_info.environment.current_humidity 		= 51;
	dev_info.thermostat_settings.fan_speed 		= dev_fan_mode;
	dev_info.thermostat_settings.mode 			= dev_current_mode;
	dev_info.thermostat_settings.time_remains	= 55;
	dev_info.preferences.display_brightness		= brightness_level = lv_slider_get_value(ui_Slider2);
	dev_info.preferences.audio_level			= audio_level = AUDIO_MED;

	send_device_config(dev_info);

	/* Set default audio config */
	update_thermostat_volume(AUDIO_MED);
}

void change_idle_timeout(lv_event_t * e)
{
	// Your code here
}

void update_temperature(lv_event_t * e)
{
	int temperature = lv_arc_get_value(ui_temperaturearc);
	printf("Temperature arc value: %d°C\n", temperature);

	update_device_temp((uint8_t)temperature);
}

void update_thermostat_mode_timer(void)
{
	if(dev_current_mode != MODE_OFF)
	{
		if(current_temp != target_temp) {
			if (temp_timer){
				lv_timer_del(temp_timer);
				temp_timer=NULL;
			}

			if (current_temp > target_temp){
				temp_timer=lv_timer_create(decrease_temp_step,deg2sec,NULL);
			} else {
				temp_timer=lv_timer_create(increase_temp_step,deg2sec,NULL);
			}
		}
	}
}

static void fw_update_spinner_done_cb(lv_timer_t * timer)
{
    // Unhide the "Latest firmware" label
    lv_obj_clear_flag(ui_Fwupdatelatestlbl, LV_OBJ_FLAG_HIDDEN);

    // Hide the spinner and its label
    lv_obj_add_flag(ui_FWUpdatespinner, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Fwupdatespinrlabel, LV_OBJ_FLAG_HIDDEN);

    // Optional: Delete timer after use
    lv_timer_del(timer);
}

void fw_update_check_ui(lv_event_t * e)
{
    // Unhide the "Latest firmware" label
	lv_obj_add_flag(ui_Fwupdatelatestlbl, LV_OBJ_FLAG_HIDDEN);

    // Hide the spinner and its label
    lv_obj_clear_flag(ui_FWUpdatespinner, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Fwupdatespinrlabel, LV_OBJ_FLAG_HIDDEN);

    // Create a one-shot timer to update UI after delay (e.g. 1.5 seconds)
    lv_timer_create(fw_update_spinner_done_cb, 5000, NULL);
}

static void set_volume(audio_level_t level)
{
    switch(level) {
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
}

void change_volume(lv_event_t * e){
	audio_level_t level = lv_dropdown_get_selected(ui_audoleveldropdown);
    printf("Selected Volume:%d\n", level);
    set_volume(level);
    update_audio_level_ipc(level);
	dev_info.preferences.audio_level = level;
}

void update_thermostat_volume(audio_level_t level)
{
	set_volume(level);
	lv_dropdown_set_selected(ui_audoleveldropdown, (uint16_t)level);
	dev_info.preferences.audio_level = level;
}

