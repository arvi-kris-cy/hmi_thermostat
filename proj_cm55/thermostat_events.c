#include "ui/ui.h"
#include "ui/ui_events.h"
#include "thermostat_events.h"

static int temperature = 24;
static int current_temp = 24;
static int target_temp;
lv_timer_t *temp_timer = NULL;

void increase_temp(lv_event_t * e)
{
    target_temp = ++temperature;
	lv_label_set_text_fmt(ui_MainTempactive, "%d°c",temperature);
    lv_label_set_text_fmt(ui_MainTemptextLP, "%d°c",temperature);
    lv_obj_add_flag(ui_bluecontainer,LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
    _ui_flag_modify(ui_redcontainer, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
    red1anim_Animation(ui_red1, 1500);
    red2anim_Animation(ui_red2, 1000);
    red3anim_Animation(ui_red3, 500);
    red4anim_Animation(ui_red4, 200);
    fanspeed_Animation(ui_fanactive, 0);
    if(!is_fan_dragged_by_temp && !is_fan_dragged_by_button){
        fandrag_Animation(ui_fanbutton,0);
        is_fan_dragged_by_temp = true;
        is_fan_dragged =true;
    }
    current_state = STATE_HEATING;
  
   if (temp_timer){
     lv_timer_del(temp_timer);
     temp_timer=NULL;
   }
   temp_timer=lv_timer_create(increase_temp_step,2500,NULL);
}

 void decrease_temp(lv_event_t * e)
{
    target_temp = --temperature;
	lv_label_set_text_fmt(ui_MainTempactive, "%d°c",temperature);
    lv_label_set_text_fmt(ui_MainTemptextLP, "%d°c",temperature);
    lv_obj_add_flag(ui_redcontainer,LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
    _ui_flag_modify(ui_bluecontainer, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
    blue1anim_Animation(ui_blue1, 300);
    blue2anim_Animation(ui_blue2, 500);
    blue3anim_Animation(ui_blue3, 800);
    blue4anim_Animation(ui_blue4, 1300);
    fanspeed_Animation(ui_fanactive, 0);
    if(!is_fan_dragged_by_temp && !is_fan_dragged_by_button){
        fandrag_Animation(ui_fanbutton,0);
        is_fan_dragged_by_temp = true;
        is_fan_dragged =true;
    }
    current_state = STATE_COOLING;
    if (temp_timer){
        lv_timer_del(temp_timer);
        temp_timer=NULL;
      }
     temp_timer=lv_timer_create(decrease_temp_step,2500,NULL);
       
}

void increase_temp_step (lv_timer_t * timer){
     
    if (current_temp < target_temp){
        current_temp++;
        lv_label_set_text_fmt(ui_currenttemp,"%d°c",current_temp);
    }
    if (current_temp == target_temp){
        lv_timer_del(timer);
        temp_timer=NULL;
        lv_label_set_text_fmt(ui_currenttemp,"%d°c",current_temp); 
        lv_obj_add_flag(ui_redcontainer,LV_OBJ_FLAG_HIDDEN);
        if(is_fan_dragged){
            fandragup_Animation(ui_fanbutton,0);
            is_fan_dragged_by_temp = false;
            is_fan_dragged_by_button =false;
            is_fan_dragged = false;
            lv_obj_add_flag(ui_redcontainer,LV_OBJ_FLAG_HIDDEN);
        }
        
        lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
       
    }
}

void decrease_temp_step (lv_timer_t * timer){
    lv_obj_remove_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN); 
    if (current_temp > target_temp){
        current_temp--;
        lv_label_set_text_fmt(ui_currenttemp,"%d°c",current_temp);
    }
    if (current_temp == target_temp){
        lv_timer_del(timer);
        temp_timer=NULL;
        lv_label_set_text_fmt(ui_currenttemp,"%d°c",current_temp); 
        lv_obj_add_flag(ui_bluecontainer,LV_OBJ_FLAG_HIDDEN);
        if(is_fan_dragged){
            fandragup_Animation(ui_fanbutton,0);
            is_fan_dragged_by_temp = false;
            is_fan_dragged_by_button =false;
            is_fan_dragged = false;
            lv_obj_add_flag(ui_bluecontainer,LV_OBJ_FLAG_HIDDEN);
        }
        lv_obj_add_flag(ui_currenttemp, LV_OBJ_FLAG_HIDDEN);
       
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

static int weather_temp = 28;

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

void change_fan_speed(lv_event_t * e){
    if(temp_timer){
        lv_timer_del(temp_timer);
        temp_timer=NULL;
    }
    fanspeed_Animation(ui_fanactive, 0);
    if(!is_fan_dragged_by_temp && !is_fan_dragged){
    fandrag_Animation(ui_fanbutton, 0);
    _ui_flag_modify(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
     is_fan_dragged = true;
     is_fan_dragged_by_button = true;
}
}

void fan_high(lv_event_t * e){
    current_fan_speed = FAN_HIGH;
    fanspeed_Animation(ui_fanactive, 0);
    if(is_fan_dragged){
        fandragup_Animation(ui_fanbutton,0);
        is_fan_dragged= false;
        is_fan_dragged_by_button =false;
        lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
    }
}
void fan_med(lv_event_t * e){
    current_fan_speed = FAN_MED;
    fanspeed_Animation(ui_fanactive, 0);
    if(is_fan_dragged){
        fandragup_Animation(ui_fanbutton,0);
        is_fan_dragged= false;
        is_fan_dragged_by_button =false;
        lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
    }
}
void fan_low(lv_event_t * e){
    current_fan_speed = FAN_LOW;
    fanspeed_Animation(ui_fanactive, 0);
    if(is_fan_dragged){
        fandragup_Animation(ui_fanbutton,0);
        is_fan_dragged= false;
        is_fan_dragged_by_button =false;
        lv_obj_add_flag(ui_fanspeedcontainer, LV_OBJ_FLAG_HIDDEN);
    }
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

void change_brightness(lv_event_t * e){
     brightness_level  = lv_slider_get_value(ui_Slider1);
     mtb_display_st7701s_set_brightness(brightness_level);
}