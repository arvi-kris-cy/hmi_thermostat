/******************************************************************************
* File Name : voice_assistant.c
*
* Description :
* Code for DEEPCRAFT voice assistant
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

#include "voice_assistant.h"
#include "profiler.h"

#ifdef PROFILER_ENABLE
#include "cy_afe_profiler.h"
#include "cy_profiler.h"
#endif /* PROFILER_ENABLE */

/*******************************************************************************
* Macros
*******************************************************************************/

/* This is the maximum size of the command string that can be detected by the
 * voice assistant.
 */
#define COMMAND_STRING_SIZE                     (250U)

#define MODEL_PREFIX(var) (MODEL_DATA_BIN(PROJECT_PREFIX, _##var))

/* How often to print the MCPS (multiply by 10 ms) */
#define PRINT_MCPS_COUNT                        (100u)

#define INTENT_TEXT_SIZE                    (20)
/*******************************************************************************
* Global Variables
*******************************************************************************/
static mtb_wwd_t va_wwd_obj;
static mtb_nlu_t va_nlu_obj;
static va_mode_t va_mode = VA_MODE_WW_SINGLE_CMD;
static va_run_state_t va_state = VA_RUN_WWD;

bool cur_voice_active = false;
bool pre_voice_active = false;

uint8_t bf_coeffs[1];
uint32_t bf_coeffs_total_len;

#if INFERENCING_PROFILE
/* Variables used to print and calculate MCPS */
uint32_t show_count = 0;
uint32_t cpu_cycle_sum = 0;
#endif

extern bool handle_ww_for_ui;
extern bool handle_command_for_ui;

int intent_value;
extern char *intent_text;

/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
 * Function Name: voice_assistant_init
 *******************************************************************************
 * Summary:
 * Initializes the voice assistant with the specified mode.
 *
 * Parameters:
 *  mode: New mode to set.
 *
 * Return:
 *  Returns VA_RSLT_SUCCESS if successful, otherwise returns an error code.
 *
 *******************************************************************************/
va_rslt_t voice_assistant_init(va_mode_t mode)
{
    cy_rslt_t result;

    va_mode = mode;

    switch (mode)
    {
        case VA_MODE_WW_SINGLE_CMD:
        case VA_MODE_WW_MULTI_CMD:
            result = mtb_wwd_init(&va_wwd_obj, MTB_WWD_NLU_CONFIG_STRUCT(PROJECT_PREFIX)[0]);
            if (result != CY_WWD_RSLT_SUCCESS)
            {
                return VA_RSLT_FAIL;
            }
            result = mtb_nlu_init(&va_nlu_obj, MTB_WWD_NLU_CONFIG_STRUCT(PROJECT_PREFIX)[0]);
            if (result != CY_WWD_RSLT_SUCCESS)
            {
                return VA_RSLT_FAIL;
            }
            va_state = VA_RUN_WWD;
            break;

        case VA_MODE_WW_ONLY:
            result = mtb_wwd_init(&va_wwd_obj, MTB_WWD_NLU_CONFIG_STRUCT(PROJECT_PREFIX)[0]);
            if (result != CY_WWD_RSLT_SUCCESS)
            {
                return VA_RSLT_FAIL;
            }
            va_state = VA_RUN_WWD;
            break;

        case VA_MODE_CMD_ONLY:
            result = mtb_nlu_init(&va_nlu_obj, MTB_WWD_NLU_CONFIG_STRUCT(PROJECT_PREFIX)[0]);
            if (result != CY_NLU_RSLT_SUCCESS)
            {
                return VA_RSLT_FAIL;
            }
            va_state = VA_RUN_CMD;
            break;

        default:
            return VA_RSLT_INVALID_ARGUMENT;
    }

    return VA_RSLT_SUCCESS;
}

/*******************************************************************************
 * Function Name: voice_assistant_get_curr_state
 *******************************************************************************
 * Summary:
 * Get the current state of the voice assistant.
 *
 * Parameters:
 *  state: New state to set.
 *
 * Return:
 *  void
 *
 *******************************************************************************/
va_run_state_t voice_assistant_get_curr_state(void)
{
    return va_state;
}

/*******************************************************************************
 * Function Name: voice_assistant_change_state
 *******************************************************************************
 * Summary:
 * Changes the state of the voice assistant.
 *
 * Parameters:
 *  state: New state to set.
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void voice_assistant_change_state(va_run_state_t state)
{
    va_state = state;
}

/*******************************************************************************
 * Function Name: voice_assistant_process
 *******************************************************************************
 * Summary:
 * Processes the audio data and detects the wake word or command.
 *
 * Parameters:
 *  audio_frame: Pointer to the audio data frame.
 *  event: Pointer to the event detected.
 *  va_data: Pointer to the data detected.
 *
 * Return:
 *  Returns VA_RSLT_SUCCESS if successful, otherwise returns an error code.
 *
 *******************************************************************************/
va_rslt_t voice_assistant_process(int16_t *audio_frame, va_event_t *event, va_data_t *va_data)
{
    cy_rslt_t result;
    mtb_wwd_state_t wwd_state;
    mtb_nlu_state_t nlu_state;
    mtb_nlu_variable_t variable[VA_NLU_MAX_NUM_VARIABLES] = {0};
    
    if ((event == NULL) || (audio_frame == NULL))
    {
        return VA_RSLT_INVALID_ARGUMENT;
    }

    /* Check if the current VA state is WWD */
    if (va_state == VA_RUN_WWD)
    {
        /* Run the wake-word detection process */
        result = mtb_wwd_process(&va_wwd_obj, audio_frame, &wwd_state);

        if (result == CY_WWD_RSLT_LICENSE_ERROR)
        {
            return VA_RSLT_LICENSE_ERROR;
        } 
        else if (result != CY_WWD_RSLT_SUCCESS)
        {
            return VA_RSLT_FAIL;
        }

        /* Check if the wake-word was detected */
        if (wwd_state == CY_WWD_DETECTED)
        {
            *event = VA_EVENT_WW_DETECTED;

            /* Change state to detect command */
            if (va_mode != VA_MODE_WW_ONLY)
            {
                va_state = VA_RUN_CMD;
            }
        }
        else if (wwd_state == CY_WWD_NOT_DETECTED)
        {
            *event = VA_EVENT_WW_NOT_DETECTED;
        }
        else
        {
            *event = VA_NO_EVENT;
        }
    }
    /* Check if the current VA state is CMD */
    else if (va_state == VA_RUN_CMD)
    {
        if (va_data == NULL)
        {
            return VA_RSLT_INVALID_ARGUMENT;
        }

        /* Run the command detection process */
        result = mtb_nlu_process(&va_nlu_obj, audio_frame, &nlu_state, &va_data->intent_index, variable, &va_data->num_var);

        if (result == CY_NLU_RSLT_LICENSE_ERROR)
        {
            return VA_RSLT_LICENSE_ERROR;
        }

        /* Check if a command was detected */
        if (nlu_state == CY_NLU_DETECTED)
        {
            *event = VA_EVENT_CMD_DETECTED;

            if (va_mode == VA_MODE_WW_SINGLE_CMD)
            {
                va_state = VA_RUN_WWD;
            }
            
            for (int i = 0; i < va_data->num_var; i++)
            {
                va_data->variable[i].value = variable[i].value;
                va_data->variable[i].unit_idx = variable[i].unit_idx;
            }
        }
        else if (result == CY_NLU_RSLT_COMMAND_TIMEOUT)
        {
            *event = VA_EVENT_CMD_TIMEOUT;
            if (va_mode != VA_MODE_CMD_ONLY)
            {
                va_state = VA_RUN_WWD;
            }
        }
        else if (result == CY_NLU_RSLT_PRE_SILENCE_TIMEOUT)
        {
            *event = VA_EVENT_CMD_SILENCE_TIMEOUT;
            if ((va_mode != VA_MODE_CMD_ONLY) && (va_mode != VA_MODE_WW_MULTI_CMD))
            {
                va_state = VA_RUN_WWD;
            }
        }
        else
        {
            *event = VA_NO_EVENT;
        }
    }
    
    return VA_RSLT_SUCCESS;
}

/*******************************************************************************
 * Function Name: voice_assistant_get_command
 *******************************************************************************
 * Summary:
 * Returns the command string detected by the voice-assistant.
 *
 * Parameters:
 *  text: detected command string
 *
 * Return:
 *  Returns VA_RSLT_SUCCESS if successful, otherwise returns an error code.
 *
 *******************************************************************************/
va_rslt_t voice_assistant_get_command(char *text)
{
    cy_rslt_t result;

    if (text == NULL)
    {
        return VA_RSLT_INVALID_ARGUMENT;
    }

    result = mtb_nlu_get_command(&va_nlu_obj, text);

    return result;
}

#if 0
va_detect_cmd_t va_command_to_id(const char *command)
{
	printf("%s\r\n", command);

    if (strcmp(command, "SLEEPMODE") == 0)
    {
        return SLEEPMODE;
    } 
    else if (strcmp(command, "DECREASESCREENBRIGHTNESS") == 0)
    {
        return DECREASESCREENBRIGHTNESS;
    }
    else if (strcmp(command, "INCREASESCREENBRIGHTNESS") == 0)
    {
        return INCREASESCREENBRIGHTNESS;
    }
    else if (strcmp(command, "DECREASETEMPERATURE") == 0)
    {
        return DECREASETEMPERATURE;
    }
    else if (strcmp(command, "INCREASETEMPERATURE") == 0)
    {
        return INCREASETEMPERATURE;
    }
    else if (strcmp(command, "SETTEMPERATURE") == 0)
    {
        return SETTEMPERATURE;
    }
    else if (strcmp(command, "COOLINGMODE") == 0)
    {
        return COOLINGMODE;
    }
    else if (strcmp(command, "HEATINGOFFMODE") == 0)
    {
        return HEATINGOFFMODE;
    }
    else if (strcmp(command, "HEATINGONMODE") == 0)
    {
        return HEATINGONMODE;
    }
    else if (strcmp(command, "SCREENOFF") == 0)
    {
        return SCREENOFF;
    }
    else if (strcmp(command, "SCREENON") == 0)
    {
        return SCREENON;
    }
    else if (strcmp(command, "FANENABLEMODE") == 0)
    {
        return FANENABLEMODE;
    }
    else if (strcmp(command, "FANDISABLEMODE") == 0)
    {
        return FANDISABLEMODE;
    }
    else if (strcmp(command, "TEMPERATURESTATUS") == 0)
    {
        return TEMPERATURESTATUS;
    }
    else if (strcmp(command, "WIFISTATUS") == 0)
    {
        return WIFISTATUS;
    }
    else if (strcmp(command, "SYSTEMSTATUS") == 0)
    {
        return SYSTEMSTATUS;
    }
    else if (strcmp(command, "CONNECTTOWIFI") == 0)
    {
        return CONNECTTOWIFI;
    }
    else if (strcmp(command, "UNMUTEVOLUME") == 0)
    {
        return UNMUTEVOLUME;
    }
    else if (strcmp(command, "MUTEVOLUME") == 0)
    {
        return MUTEVOLUME;
    }
    else if (strcmp(command, "SETTINGMODE") == 0)
    {
        return SETTINGMODE; 
    }
    else
    {
        return -1;  // Unknown command
    }
}
#endif

va_detect_cmd_t va_command_to_id(const char *command)
{
	printf("%s\r\n", command);

    if (strcmp(command, "SETTEMPERATURE") == 0)
    {
        return SETTEMPERATURE;
    }
    else if (strcmp(command, "INCREASETEMPERATURE") == 0)
    {
        return INCREASETEMPERATURE;
    }
    else if (strcmp(command, "DECREASETEMPERATURE") == 0)
    {
        return DECREASETEMPERATURE;
    }
    else if (strcmp(command, "INCREASESCREENBRIGHTNESS") == 0)
    {
        return INCREASESCREENBRIGHTNESS;
    }
    else if (strcmp(command, "DECREASESCREENBRIGHTNESS") == 0)
    {
        return DECREASESCREENBRIGHTNESS;
    }
    else if (strcmp(command, "INCREASEFANSPEED") == 0)
    {
        return INCREASEFANSPEED;
    }
    else if (strcmp(command, "DECREASEFANSPEED") == 0)
    {
        return DECREASEFANSPEED;
    }
    else if (strcmp(command, "TURNONCMD") == 0)
    {
        return TURNONCMD;
    }
    else if (strcmp(command, "TURNOFFCMD") == 0)
    {
        return TURNOFFCMD;
    }
    else if (strcmp(command, "SETTINGMODE") == 0)
    {
        return SETTINGMODE;
    }
    else if (strcmp(command, "THERMOSTATMODE") == 0)
    {
        return THERMOSTATMODE;
    }
    else if (strcmp(command, "WIFISTATUS") == 0)
    {
        return WIFISTATUS;
    }
    else if (strcmp(command, "MUTEVOLUME") == 0)
    {
        return MUTEVOLUME;
    }
    else if (strcmp(command, "UNMUTEVOLUME") == 0)
    {
        return UNMUTEVOLUME;
    }
    else
    {
        return -1;  // Unknown command
    }
}

#if INFERENCING_PROFILE
/*******************************************************************************
 * Function Name: print_mcps
 *******************************************************************************
 * Summary:
 * Prints the number of CPU cycles in Mega cycles per second (MCPS)
 *
 * Parameters:
 *
 * Return:
 *  void
 *
 *******************************************************************************/
static void print_mcps(void)
{
    cpu_cycle_sum += cy_profiler_get_cycles();
    show_count++;
    if(show_count >= PRINT_MCPS_COUNT)
    {
        printf("cy_profiler: %u MCPS\r\n", cpu_cycle_sum/1000000);
        show_count = 0;
        cpu_cycle_sum = 0;
    }
}
#endif

/*******************************************************************************
 * Function Name: print_voice_assistant_status
 *******************************************************************************
 * Summary:
 * Prints an error message if wake-word detection result is not successful. 
 * Or a detection message if wake-word is detected.
 *
 * Parameters:
 *  result: result of the voice-assistant operation
 *  event: state of the voice-assistant operation
 *  va_data: data detected from the voice-assistant operation
 *
 * Return:
 *  void
 *
 *******************************************************************************/
static void print_voice_assistant_status(cy_rslt_t result, va_event_t event, va_data_t *va_data)
{
    if (result == VA_RSLT_LICENSE_ERROR)
    {
        printf("ERROR! Voice Assistant license expired!\r\n");
        //handle_app_error();
    }
    else if ( result != VA_RSLT_SUCCESS )
    {
        printf("Error! voice_assistant_process!! Error code=%d\r\n", result);
    }
    else
    {
        if ( event == VA_EVENT_WW_DETECTED )
        {
            printf("Wake-word detected!\r\n");
            Cy_GPIO_Set(CYBSP_LED_BLUE_PORT, CYBSP_LED_BLUE_NUM);
            cur_voice_active = true;
            handle_ww_for_ui = true;
        }
        else if ( event == VA_EVENT_WW_NOT_DETECTED )
        {
            printf("Wake-word rejected!\r\n");
            Cy_GPIO_Clr(CYBSP_LED_BLUE_PORT, CYBSP_LED_BLUE_NUM);
            cur_voice_active = false;
        }
        else if ( event == VA_EVENT_CMD_TIMEOUT )
        {
            printf("Command Timeout!\r\n");
            Cy_GPIO_Clr(CYBSP_LED_BLUE_PORT, CYBSP_LED_BLUE_NUM);
            cur_voice_active = false;
        }
        else if ( event == VA_EVENT_CMD_SILENCE_TIMEOUT )
        {
            if ((RUNNING_MODE != VA_MODE_WW_MULTI_CMD) && (RUNNING_MODE != VA_MODE_CMD_ONLY))
            {
                printf("Pre Silence Timeout!\r\n");
                Cy_GPIO_Clr(CYBSP_LED_BLUE_PORT, CYBSP_LED_BLUE_NUM);
                cur_voice_active = false;
            }
        }
        else if ( event == VA_EVENT_CMD_DETECTED )
        {
            char command_text[COMMAND_STRING_SIZE] = {0};
            
            printf("Command detected: ");
            if (CY_RSLT_SUCCESS == voice_assistant_get_command(command_text))
            {
                printf("%s\r\n\r\n", command_text);
            }

            if (va_data == NULL)
            {
                return;
            }

            intent_value = 0;
            intent_text = NULL;
            //printf("Intent name: %s\r\n", MTB_NLU_INTENT_NAME_LIST(PROJECT_PREFIX)[va_data->intent_index] );

            if (va_data->num_var != 0)
            {
                printf("Variable values: ");
                for (int i = 0; i < va_data->num_var; i++)
                {
                    if (va_data->variable[i].unit_idx < 0)
                    {
                        printf("%s ", MTB_NLU_VARIABLE_PHRASE_LIST(PROJECT_PREFIX)[va_data->variable[i].value]);
                    }
                    else
                    {
                        printf("%d ", va_data->variable[i].value);
                    }
                    intent_value =  va_data->variable[i].value;
                }
                printf("\n\rVariable units : ");
                for (int i = 0; i < va_data->num_var; i++)
                {
                    if (va_data->variable[i].unit_idx < 0)
                    {
                        printf("---");
                    }
                    else
                    {
                        printf("%s", MTB_NLU_UNIT_PHRASE_LIST(PROJECT_PREFIX)[va_data->variable[i].unit_idx]);
                    }
                }
                printf("\n\r");
            }
            printf("\n\r");

            intent_text = (char *)MTB_NLU_INTENT_NAME_LIST(PROJECT_PREFIX)[va_data->intent_index];
            handle_command_for_ui = true;

            Cy_GPIO_Clr(CYBSP_LED_BLUE_PORT, CYBSP_LED_BLUE_NUM);
        }
    }
}

/*******************************************************************************
 * Function Name: run_voice_assistant_process
 *******************************************************************************
 * Summary:
 * Run the voice assistant process and print any information in the terminal.
 *  
 * Parameters:
 *  audio_frame: pointer to the audio data frame
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void run_voice_assistant_process(int16_t *audio_frame)
{
    va_rslt_t va_result;
    va_data_t va_data;
    va_event_t va_event;

#if INFERENCING_PROFILE
    cy_profiler_start();
#endif    
    /* Process the audio data */
    va_result = voice_assistant_process(audio_frame, &va_event, &va_data);
#if INFERENCING_PROFILE
    cy_profiler_stop();
    print_mcps();
#endif

    /* Print the status of the voice assistant */
    print_voice_assistant_status(va_result, va_event, &va_data);
    
}

