/******************************************************************************
* File Name : app_voice_control.c
*
* Description :
* Code for voice control using DEEPCRAFT voice assistant
********************************************************************************
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
* Header Files
*******************************************************************************/

#include "stdlib.h"

#include "cy_pdl.h"
#include "cycfg.h"

#include "retarget_io_init.h"

#include "app_voice_control.h"
#include "voice_assistant.h"

/* Peripherals related includes */
#include "pdm_mic_interface.h"
#include "audio_input_configuration.h"
#include "audio_usb_send_utils.h"
#include "audio_conv_utils.h"

#ifdef USE_AUDIO_ENHANCEMENT
#include "audio_enhancement.h"
#endif

#ifdef PROFILER_ENABLE
#include "cy_afe_profiler.h"
#include "cy_profiler.h"
#endif /* PROFILER_ENABLE */

#include "thermostat_events.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* Debounce counter for button presses (multiply by 10 ms) */
#define BUTTON_DEBOUNCE_COUNT                   (2U)

/*******************************************************************************
* Global Variables
*******************************************************************************/

#ifdef ENABLE_STEREO_INPUT_FEED
int16_t non_interleaved_audio[2*PDM_MIC_SAMPLES_COUNT] = {0};
#endif

extern bool handle_ww_for_ui;
extern bool handle_command_for_ui;

extern int intent_value;
extern uint8_t brightness_level;

/*******************************************************************************
* Function Definitions
*******************************************************************************/

void go_to_sleepmode()
{
    _ui_screen_change(&ui_LPScreen, LV_SCR_LOAD_ANIM_FADE_ON, 230, 0, &ui_LPScreen_screen_init);
}

void decrease_screen_brightness()
{
    brightness_level  = lv_slider_get_value(ui_BrightnessSlider);
    
    if(brightness_level >= (lv_slider_get_min_value(ui_BrightnessSlider) + 20))
    {
        brightness_level -= 20;
    }
    else 
    {
        brightness_level = lv_slider_get_min_value(ui_BrightnessSlider);
    }

    if(brightness_level%20 !=0)
    {
        brightness_level = ((brightness_level) - (brightness_level%20));
    }

    if(brightness_level >= lv_slider_get_min_value(ui_BrightnessSlider) && brightness_level <= lv_slider_get_max_value(ui_BrightnessSlider))
    {
        lv_slider_set_value(ui_BrightnessSlider, brightness_level, LV_ANIM_OFF);
        mtb_display_st7701s_set_brightness(brightness_level);

        printf("Brightness level %d. \n", brightness_level);
    }
}

void increase_screen_brightness()
{
    brightness_level  = lv_slider_get_value(ui_BrightnessSlider);
  
    if((brightness_level + 20) <= lv_slider_get_max_value(ui_BrightnessSlider))
    {
        brightness_level += 20;
    }
    else 
    {
        brightness_level = lv_slider_get_max_value(ui_BrightnessSlider);
    }

    if(brightness_level%20 !=0)
    {
        brightness_level = ((brightness_level) - (brightness_level%20));
    }
    
    if(brightness_level >= lv_slider_get_min_value(ui_BrightnessSlider) && brightness_level <= lv_slider_get_max_value(ui_BrightnessSlider))
    {
        lv_slider_set_value(ui_BrightnessSlider, brightness_level, LV_ANIM_OFF);
        mtb_display_st7701s_set_brightness(brightness_level);

        printf("Brightness level %d. \n", brightness_level);
    }
}

void go_to_setting()
{
    _ui_screen_change(&ui_SettingsScreen, LV_SCR_LOAD_ANIM_FADE_ON, 230, 0, &ui_SettingsScreen_screen_init);
}

void disable_fanmode()
{
    // lv_obj_remove_state(ui_Switch1, LV_STATE_CHECKED);
}

void enable_fanmode()
{
    // lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
}

void set_temperature(int *value)
{
    // Check if the value pointer is NULL
    if (value == NULL) {
        printf("Error: NULL temperature value provided.\n");
        return;
    }

    // Create a buffer to hold the formatted string
    char temp_with_unit[32];

    // Format the integer value with the "°C" unit
    snprintf(temp_with_unit, sizeof(temp_with_unit), "%d°c", *value);

    // Print for debugging (replace this with your LVGL label update)
    printf("Setting temperature: %s\n", temp_with_unit);

    // Update the LVGL label (example function call)
    lv_label_set_text(ui_TemperatureCurrValueLbl, temp_with_unit);
}

/*******************************************************************************
 * Function Name: Intent to ui changes
 *******************************************************************************/
#if 0
va_rslt_t intent_to_ui(const char *command)
{
    if(handle_command_for_ui)
    {
        handle_command_for_ui = false;

        if (NULL == command)
        {
            // printf("No command received.\n");
            return VA_RSLT_INVALID_ARGUMENT;  // Return immediately for invalid input
        }

        /** Switch to Active screen */
        switch_to_active_screen();

        // Map the command string to an enum value
        va_detect_cmd_t cmd = va_command_to_id(command);

        // Handle the command using a switch statement
        switch (cmd) 
        {
            case SLEEPMODE:
            {
                go_to_sleepmode();
                printf("Handling SLEEPMODE command.\n");
                break;
            }
            case DECREASESCREENBRIGHTNESS:
            {
                decrease_screen_brightness();
                printf("Handling DECREASESCREENBRIGHTNESS command.\n");
                break;
            }
            case INCREASESCREENBRIGHTNESS:
            {
                increase_screen_brightness();
                printf("Handling INCREASESCREENBRIGHTNESS command.\n");
                break;
            }
            case DECREASETEMPERATURE:
            {
            	if(intent_value)
            	{
            		update_device_temp((uint8_t)(get_current_temperature() - (uint8_t)(intent_value)));
            	}
            	else
            	{
            		update_device_temp((uint8_t)(get_current_temperature() - 1));
            	}
                printf("Handling DECREASETEMPERATURE command.\n");
                break;
            }
            case INCREASETEMPERATURE:
            {
            	if(intent_value)
            	{
            		update_device_temp((uint8_t)(get_current_temperature() + (uint8_t)(intent_value)));
            	}
            	else
            	{
            		update_device_temp((uint8_t)(get_current_temperature() + 1));
            	}
                printf("Handling INCREASETEMPERATURE command.\n");
                break;
            }
            case SETTEMPERATURE:
            {
            	if(intent_value)
            	{
            		update_device_temp((uint8_t)(intent_value));
            		printf("Handling SETTEMPERATURE command.\n");
            	}
            	else
            	{
            		printf("Temperature not defined.\n");
            	}
                break;
            }
            case COOLINGMODE:
            {
                update_thermostat_mode(MODE_RAPID);
                current_settings.thermostat_setting.mode = MODE_RAPID;
                current_settings.thermostat_setting.fan_mode = get_current_fan_mode();
                update_thermostat_mode_timer();
                update_current_device_setting();
                printf("Handling COOLINGMODE command.\n");
                break;
            }
            case HEATINGOFFMODE:
            {
                update_thermostat_mode(MODE_ECO);
                current_settings.thermostat_setting.mode = MODE_ECO;
                current_settings.thermostat_setting.fan_mode = get_current_fan_mode();
                update_thermostat_mode_timer();
                update_current_device_setting();
                printf("Handling HEATINGOFFMODE command.\n");
                break;
            }
            case HEATINGONMODE:
            {
                update_thermostat_mode(MODE_OFF);
                current_settings.thermostat_setting.mode = MODE_OFF;
                current_settings.thermostat_setting.fan_mode = get_current_fan_mode();
                update_thermostat_mode_timer();
                update_current_device_setting();
                printf("Handling HEATINGONMODE command.\n");
                break;
            }
            case SCREENOFF:
            {
            	go_to_sleepmode();
                printf("Handling SCREENOFF command.\n");
                break;
            }
            case SCREENON:
            {
                _ui_screen_change(&ui_ActiveScreen, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_ActiveScreen_screen_init);
                printf("Handling SCREENON command.\n");
                break;
            }
            case FANENABLEMODE:
            {
                update_fan_mode(FAN_MED);
                current_settings.thermostat_setting.fan_mode = FAN_MED;
                update_current_device_setting();
                printf("Handling FANENABLEMODE command.\n");
                break;
            }
            case FANDISABLEMODE:
            {
                update_fan_mode(FAN_OFF);
                current_settings.thermostat_setting.fan_mode = FAN_OFF;
                update_current_device_setting();
                printf("Handling FANDISABLEMODE command.\n");
                break;
            }
            case TEMPERATURESTATUS:
            {
                _ui_screen_change(&ui_ActiveScreen, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_ActiveScreen_screen_init);
                printf("Handling TEMPERATURESTATUS command.\n");
                break;
            }
            case WIFISTATUS:
            {
                printf("Handling WIFISTATUS command.\n");
                break;
            }
            case SYSTEMSTATUS:
            {
                _ui_screen_change(&ui_ActiveScreen, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_ActiveScreen_screen_init);
                printf("Handling SYSTEMSTATUS command.\n");
                break;
            }
            case CONNECTTOWIFI:
            {
                update_switch_wifi_ipc();
                printf("Handling CONNECTTOWIFI command.\n");
                break;
            }
            case UNMUTEVOLUME:
            {
                update_thermostat_volume(AUDIO_MED);
                printf("Handling UNMUTEVOLUME command.\n");
                break;
            }
            case MUTEVOLUME:
            {
                update_thermostat_volume(AUDIO_OFF);
                printf("Handling MUTEVOLUME command.\n");
                break;
            }
            case SETTINGMODE:
            {
                go_to_setting();
                printf("Handling SETTINGMODE command.\n");
                break;
            }

            default:
            {
                // Handle unknown commands
                if (command && command[0] != '\0')
                {
                    printf("Unknown command: %s\n", command);
                }
                else
                {
                    printf("Unknown or empty command received.\n");
                }

                return VA_RSLT_INVALID_ARGUMENT;  // Return an error for unknown commands
            }
        }
    }

    // Successfully handled the command
    return VA_RSLT_SUCCESS;
}
#endif

va_rslt_t intent_to_ui(const char *command)
{
    if(handle_command_for_ui)
    {
        handle_command_for_ui = false;

        if (NULL == command)
        {
            // printf("No command received.\n");
            return VA_RSLT_INVALID_ARGUMENT;  // Return immediately for invalid input
        }

        /** Switch to Active screen */
        switch_to_active_screen(); 
        lv_obj_add_flag(ui_voicecmdcontainer, LV_OBJ_FLAG_HIDDEN);
        display_fan_anim(); 
        // Map the command string to an enum value
        va_detect_cmd_t cmd = va_command_to_id(command);

        // Handle the command using a switch statement
        switch (cmd) 
        {
            case SETTEMPERATURE:
            {
            	if(intent_value)
            	{
            		update_device_temp((uint8_t)(intent_value));
            		printf("Handling SETTEMPERATURE command.\n");
            	}
            	else
            	{
            		printf("Temperature not defined.\n");
            	}
                break;
            }
            case INCREASETEMPERATURE:
            {
            	if(intent_value)
            	{
            		update_device_temp((uint8_t)(get_current_temperature() + (uint8_t)(intent_value)));
            	}
            	else
            	{
            		update_device_temp((uint8_t)(get_current_temperature() + 1));
            	}
                printf("Handling INCREASETEMPERATURE command.\n");
                break;
            }
            case DECREASETEMPERATURE:
            {
            	if(intent_value)
            	{
            		update_device_temp((uint8_t)(get_current_temperature() - (uint8_t)(intent_value)));
            	}
            	else
            	{
            		update_device_temp((uint8_t)(get_current_temperature() - 1));
            	}
                printf("Handling DECREASETEMPERATURE command.\n");
                break;
            }
            case INCREASESCREENBRIGHTNESS:
            {
                increase_screen_brightness();
                printf("Handling INCREASESCREENBRIGHTNESS command.\n");
                break;
            }
            case DECREASESCREENBRIGHTNESS:
            {
                decrease_screen_brightness();
                printf("Handling DECREASESCREENBRIGHTNESS command.\n");
                break;
            }
            case INCREASEFANSPEED:
            {
                fan_speed_t fan_mode = get_current_fan_mode();

                if(fan_mode != FAN_HIGH)
                {
                    if (fan_mode == FAN_OFF)
                    {
                        fan_mode = FAN_LOW;  // Start cycle at LOW

                        update_thermostat_mode(MODE_ECO);
                    }
                    else if (fan_mode == FAN_LOW)
                    {
                        fan_mode = FAN_MED;
                    }
                    else if (fan_mode == FAN_MED)
                    {
                        fan_mode = FAN_HIGH;
                    }

                    current_settings.thermostat_setting.fan_mode = fan_mode;

                    update_fan_mode(fan_mode);
                    update_current_device_setting();
                }

                printf("Handling INCREASEFANSPEED command.\n");
                break;
            }
            case DECREASEFANSPEED:
            {
                fan_speed_t fan_mode = get_current_fan_mode();

                if((fan_mode != FAN_OFF) && (fan_mode != FAN_LOW))
                {
                    if (fan_mode == FAN_HIGH)
                    {
                        fan_mode = FAN_MED;  // Start cycle at LOW
                    }
                    else if (fan_mode == FAN_MED)
                    {
                        fan_mode = FAN_LOW;
                    }
                    //else if (fan_mode == FAN_LOW)
                    //{
                    //    fan_mode = FAN_OFF;
                    //}

                    current_settings.thermostat_setting.fan_mode = fan_mode;

                    update_fan_mode(fan_mode);
                    update_current_device_setting();
                }

                printf("Handling DECREASEFANSPEED command.\n");
                break;
            }
            case TURNONCMD:
            {
                thermostat_mode_t current_mode = get_current_device_mode();
				
                if(intent_value)
                {	
                    if(current_mode == MODE_OFF)
                    {
                        update_thermostat_mode(MODE_ECO);

                        current_settings.thermostat_setting.fan_mode = FAN_LOW;

                        update_fan_mode(FAN_LOW);
                        update_thermostat_mode_timer();
                        update_current_device_setting();
                    }
                }

                printf("Handling TURNONCMD command.\n");
                break;
            }
            case TURNOFFCMD:
            {
                thermostat_mode_t current_mode = get_current_device_mode();

                if(intent_value)
                {
                    if(current_mode != MODE_OFF)
                    {
                        update_thermostat_mode(MODE_OFF);

                        current_settings.thermostat_setting.fan_mode = FAN_OFF;

                        update_fan_mode(FAN_OFF);
                        update_thermostat_mode_timer();
                        update_current_device_setting();
                    }
                }

                printf("Handling TURNOFFCMD command.\n");
                break;
            }
            case SETTINGMODE:
            {
                go_to_setting();
                printf("Handling SETTINGMODE command.\n");
                break;
            }
            case THERMOSTATMODE:
            {
                thermostat_mode_t current_mode = get_current_device_mode();

                if(intent_value)
                {
					const char* command = MTB_NLU_VARIABLE_PHRASE_LIST(PROJECT_PREFIX)[intent_value];
					
					if (strcmp(command, "eco") == 0)
					{	
                    	current_mode = MODE_ECO;
					}
					else if (strcmp(command, "rapid") == 0)
					{
						current_mode = MODE_RAPID;
					}
					else if (strcmp(command, "auto") == 0)
					{
						current_mode = MODE_AUTO;
					}
                }
                else
                {
                    if(current_mode == MODE_OFF)
                    {
                        current_mode = MODE_ECO;
                    }
                    else if(current_mode == MODE_ECO)
                    {
                        current_mode = MODE_RAPID;
                    }
                    else if(current_mode == MODE_RAPID)
                    {
                        current_mode = MODE_AUTO;
                    }
                    else if(current_mode == MODE_AUTO)
                    {
                        current_mode = MODE_ECO;
                    }
                }

                update_thermostat_mode(current_mode);
                current_settings.thermostat_setting.mode = current_mode;
                current_settings.thermostat_setting.fan_mode = get_current_fan_mode();
                update_thermostat_mode_timer();
                update_current_device_setting();

                printf("Handling THERMOSTATMODE command.\n");
                break;
            }
            case WIFISTATUS:
            {
                printf("Handling WIFISTATUS command.\n");
                break;
            }
            case MUTEVOLUME:
            {
                update_thermostat_volume(AUDIO_OFF);
                printf("Handling MUTEVOLUME command.\n");
                break;
            }
            case UNMUTEVOLUME:
            {
                update_thermostat_volume(AUDIO_MED);
                printf("Handling UNMUTEVOLUME command.\n");
                break;
            }

            default:
            {
                // Handle unknown commands
                if (command && command[0] != '\0')
                {
                    printf("Unknown command: %s\n", command);
                }
                else
                {
                    printf("Unknown or empty command received.\n");
                }

                return VA_RSLT_INVALID_ARGUMENT;  // Return an error for unknown commands
            }
        }
    }

    // Successfully handled the command
    return VA_RSLT_SUCCESS;
}

void ww_to_ui()
{
    if(handle_ww_for_ui)
    {
        handle_ww_for_ui = false;

        // Check if the current screen is ui_LPScreen
        //if (lv_screen_active() == ui_LPScreen)
        //{
        //    voice_assistant_change_state(VA_RUN_CMD);
       // }
       // else
       // {
       //     // printf("Current screen is not ui_LPScreen. Screen change skipped.\n");
       // }

        /** Switch to Active screen  */
        switch_to_active_screen();
        display_fan_anim();
    }
}

/*******************************************************************************
 * Function Name: check_button_pressed
 *******************************************************************************
 * Summary:
 * Check if the user button is pressed.
 *
 * Parameters:
 *
 * Return:
 *  Returns true if the button is pressed, false otherwise
 *
 *******************************************************************************/
bool check_button_pressed(void)
{
    static uint32_t btn_count = 0;

    if (0 == Cy_GPIO_Read(CYBSP_USER_BTN1_PORT, CYBSP_USER_BTN1_NUM))
    {
        btn_count++;
    }
    else
    {
        btn_count = 0;
    }

    /* If the button is pressed for more than BUTTON_DEBOUNCE_COUNT, return true */
    if (btn_count > BUTTON_DEBOUNCE_COUNT)
    {
        btn_count = 0;
        return true;
    }
    return false;
}

/*******************************************************************************
 * Function Name: audio_enhancement_process_output
 *******************************************************************************
 * Summary:
 * Use case API
 * Sends back AE data/tuning data back to PC via USB Audio Class
 *
 * Parameters:
 *  output_buffer: pointer to the output audio data buffer.
 *
 * Return:
 *  void
 *
 *******************************************************************************/
 
void audio_enhancement_process_output(ae_buffer_info_t *ae_output_buffer)
{

#ifdef AE_TUNING_MODE
    int16_t *output_dgb1 = (int16_t *)ae_output_buffer->dbg_output1;
    int16_t *output_dgb2 = (int16_t *)ae_output_buffer->dbg_output2;
    int16_t *output_dgb3 = (int16_t *)ae_output_buffer->dbg_output3;
    int16_t *output_dgb4 = (int16_t *)ae_output_buffer->dbg_output4;
#endif /* AE_TUNING_MODE */

#if AE_APP_PROFILE
    cy_afe_profile(AFE_PROFILE_CMD_PRINT_STATS_1SEC, NULL);
    cy_afe_profile(AFE_PROFILE_CMD_RESET, NULL);
#endif /* AE_APP_PROFILE */

#ifdef AE_TUNING_MODE
    usb_send_out_dbg_put(USB_CHANNEL_1,(int16_t *)output_dgb1);
    usb_send_out_dbg_put(USB_CHANNEL_2,(int16_t *)output_dgb2);
    usb_send_out_dbg_put(USB_CHANNEL_3,(int16_t *)output_dgb3);
    usb_send_out_dbg_put(USB_CHANNEL_4,(int16_t *)output_dgb4);
#endif /* AE_TUNING_MODE */

    /* Use the output data from the audio enhancement with the voice-assistant */
    run_voice_assistant_process(ae_output_buffer->output_buf);
}

/*******************************************************************************
 * Function Name: voice_assistant_task
 *******************************************************************************
 * Summary:
 * This is the FreeRTOS task to execute the DEEPCRAFT voice assistant routines.
 *
 * Parameters:
 *  void * arg
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void voice_assistant_task(void * arg)
{
    va_rslt_t va_result;
    int16_t *audio_frame;
    int16_t *audio_feed_input;
#ifdef USE_AUDIO_ENHANCEMENT
    ae_rslt_t ae_result;
    uint8_t ae_license_error = 0;
#endif /* USE_AUDIO_ENHANCEMENT */    

    // Initialize the profiler to print MCPS
#if INFERENCING_PROFILE
    cy_profiler_init();
#endif /* INFERENCING_PROFILE */ 

   /* If AFE is used, initialize the audio enhancement */
#ifdef USE_AUDIO_ENHANCEMENT
#if AE_APP_PROFILE
    cy_profiler_init();
    cy_afe_profile(AFE_PROFILE_CMD_ENABLE,NULL);
#endif /* AE_APP_PROFILE */ 

    ae_result = audio_enhancement_init(AFE_INPUT_NUMBER_CHANNELS);
    if (ae_result != AE_RSLT_SUCCESS)
    {
        printf("Error initializing the audio enhancement. Error code=%d\r\n", ae_result);
        handle_app_error();
    }
    else
    {
        printf("Audio Enhancement initialized!\r\n");
    }

#ifdef AE_TUNING_MODE
    /* Enable USB interface*/
    app_log_print("Initializing USB interface \r\n");
    usb_audio_interface_init();
    usb_send_out_dbg_init_channels();
#endif
#endif /* USE_AUDIO_ENHANCEMENT */

    /* Initialize the PDM microphone */
    pdm_mic_init(); 

    /* Initialize the voice assistant */
    va_result = voice_assistant_init(RUNNING_MODE);

    if (va_result != VA_RSLT_SUCCESS)
    {
        printf("Error initializing the voice assistant. Error code=%d\r\n", va_result);
        handle_app_error();
    }
    else
    {
        printf("Voice Assistant initialized!\r\n\r\n");
    }
    
    for (;;)
    {
        /* Get audio data */
        pdm_mic_get_data(&audio_frame);

//#ifdef ENABLE_STEREO_INPUT_FEED
#if 0    
        convert_interleaved_to_stereo_non_interleaved((uint16_t *)audio_frame, (uint16_t *)non_interleaved_audio);
    
        audio_feed_input = (int16_t*)non_interleaved_audio;
#else
	    audio_feed_input = audio_frame;
#endif /* ENABLE_STEREO_INPUT_FEED */

#ifdef USE_AUDIO_ENHANCEMENT
        /* Apply the audio enhancement if AFE is enabled */
        ae_result = audio_enhancement_feed_input(audio_feed_input, NULL);
        if (ae_result == AE_RSLT_LICENSE_ERROR)
        {
			if(ae_license_error == 0)
			{
				ae_license_error = 1;
           		printf("ERROR! Audio Enhancement license expired!\r\n");
			}
            //handle_app_error();
        }
#else   

        /* If Audio Enhancement is not used, run voice-assistant 
         * directly with the microphone data */
        run_voice_assistant_process(audio_feed_input);

#endif  /* USE_AUDIO_ENHANCEMENT */       

    }  
}
