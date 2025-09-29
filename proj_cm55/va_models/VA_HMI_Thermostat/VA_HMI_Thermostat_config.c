/****************************************************************************
* File Name        : VA_HMI_Thermostat_config.c
*
* Description      : This source file contains the configuration object for WWD and NLU
*
* Related Document : See README.md
*
*****************************************************************************
* Copyright 2023-2024, Cypress Semiconductor Corporation (an Infineon company) or
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
*****************************************************************************/

#include "VA_HMI_Thermostat_config.h"

#include "mtb_ml.h"
#include "mtb_ml_model_16x8.h"
#include "AM_LSTM_tflm_model_int16x8.h"

#include "ifx_va_prms.h"
#include "ifx_sp_common_priv.h"

#include "VA_HMI_Thermostat_U55_WWmodel.h"
#include "VA_HMI_Thermostat_U55_CMDmodel.h"
#include "U55_NMBmodel.h"

#include "VA_HMI_Thermostat.h"

/* Following am_tensor_arena has been counted as part of persistent memory total size */
/* Tensor_arena buffer must be in SOCMEM and aligned by 16 which are required by U55 */
static uint8_t am_tensor_arena[AM_LSTM_ARENA_SIZE] __attribute__((aligned(16)))
                                          __attribute__((section(".cy_socmem_data")));


static int16_t data_feed_int[N_SEQ * FEATURE_BUF_SZ] __attribute__((aligned(16)));
static float mtb_ml_input_buffer[N_SEQ * FEATURE_BUF_SZ];

static float xIn[FRAME_SIZE_16K] __attribute__((section(".wwd_nlu_data3")));
static float features[FEATURE_BUF_SZ] __attribute__((section(".wwd_nlu_data4")));
static float output_scores[(N_PHONEMES + 1) * (1 + AM_LOOKBACK)] __attribute__((section(".wwd_nlu_data5")));

//common buffers
static mtb_wwd_nlu_buff_t wwd_nlu_buff =
{
    .am_model_bin = { MTB_ML_MODEL_BIN_DATA(AM_LSTM) },
    .am_model_buffer = {
        .tensor_arena = am_tensor_arena,
        .tensor_arena_size = AM_LSTM_ARENA_SIZE
    },
    .data_feed_int = data_feed_int,
    .mtb_ml_input_buffer = mtb_ml_input_buffer,
    .output_scores = output_scores,
    .xIn = xIn,
    .features = features
};

// NLU setup array
static mtb_nlu_setup_array_t nlu_setup_array =
{
    .intent_name_list = VA_HMI_Thermostat_intent_name_list,
    .variable_name_list = VA_HMI_Thermostat_variable_name_list,
    .variable_phrase_list = VA_HMI_Thermostat_variable_phrase_list,
    .unit_phrase_list = VA_HMI_Thermostat_unit_phrase_list,
    .intent_map_array = VA_HMI_Thermostat_intent_map_array,
    .intent_map_array_sizes = VA_HMI_Thermostat_intent_map_array_sizes,
    .variable_phrase_sizes = VA_HMI_Thermostat_variable_phrase_sizes,
    .unit_phrase_map_array = VA_HMI_Thermostat_unit_phrase_map_array,
    .unit_phrase_map_array_sizes = VA_HMI_Thermostat_unit_phrase_map_array_sizes,
    .NUM_UNIT_PHRASES = sizeof(VA_HMI_Thermostat_unit_phrase_list),
};

// WW config
static mtb_wwd_conf_t ww_conf = {
    .callback.cb_for_event = CY_EVENT_SOD,
    .callback.cb_function = VA_HMI_Thermostat_wake_word_callback
};

// NLU config
static mtb_nlu_config_t nlu_conf = {
    .nlu_pre_silence_timeout = 2000,
    .nlu_command_timeout = 5000,
};

static mtb_wwd_nlu_config_t ww_1_conf = {
    .ww_model_ptr = VA_HMI_Thermostat_WWmodeldata,
    .cmd_model_ptr = VA_HMI_Thermostat_CMDmodeldata,
    .nmb_model_ptr = NMBmodeldata,
    .wwd_nlu_buff_data = &wwd_nlu_buff,
    .ww_conf = &ww_conf,
    .nlu_conf.nlu_config = &nlu_conf,
    .nlu_conf.nlu_variable_data = &nlu_setup_array,
};

mtb_wwd_nlu_config_t *VA_HMI_Thermostat_ww_nlu_configs[VA_HMI_THERMOSTAT_NO_OF_WAKE_WORD] = {&ww_1_conf
};

char *VA_HMI_Thermostat_ww_str[VA_HMI_THERMOSTAT_NO_OF_WAKE_WORD] = {"OK Thermostat"};

const char* VA_HMI_Thermostat_intent_name_list[VA_HMI_THERMOSTAT_NUM_INTENTS] = {
    "SETTEMPERATURE",
    "HEATINGOFFMODE",
    "SCREENOFF",
    "HEATINGONMODE",
    "SCREENON",
    "COOLINGMODE",
    "FANENABLEMODE",
    "FANDISABLEMODE",
    "SYSTEMSTATUS",
    "CONNECTTOWIFI",
    "DECREASESCREENBRIGHTNESS",
    "DECREASETEMPERATURE",
    "INCREASESCREENBRIGHTNESS",
    "INCREASETEMPERATURE",
    "UNMUTEVOLUME",
    "MUTEVOLUME",
    "SLEEPMODE",
    "SETTINGMODE",
    "TEMPERATURESTATUS",
    "WIFISTATUS",
};

const char* VA_HMI_Thermostat_variable_name_list[VA_HMI_THERMOSTAT_NUM_VARIABLES] = {
    "DegreeTemperature",
    "DecreTemperature",
    "IncreTemperature",
};

const char* VA_HMI_Thermostat_variable_phrase_list[VA_HMI_THERMOSTAT_NUM_VARIABLE_PHRASES] = {
    "", // DegreeTemperature
    "", // DecreTemperature
    "", // IncreTemperature
};

const char* VA_HMI_Thermostat_unit_phrase_list[VA_HMI_THERMOSTAT_NUM_UNIT_PHRASES] = {
    "degree", "degrees", 
    "percent", 
    "level", "levels", 
    "hour", "hours", 
    "minute", "minutes", 
    "second", "seconds", 
    "day", "days", 
    "", 
    "AM", 
    "PM", 
};

const int VA_HMI_Thermostat_intent_map_array[VA_HMI_THERMOSTAT_INTENT_MAP_ARRAY_TOTAL_SIZE] = {
    0, 1, 0, -1, // Set temperature <numbers30> {degree,degrees}
    0, 1, 0, -1, // Set temperature to <numbers30> {degree,degrees}
    1, 0, // Switch off heating
    2, 0, // Switch off screen
    2, 0, // Switch off the screen
    3, 0, // Switch on heating
    4, 0, // Switch on screen
    4, 0, // Switch on the screen
    5, 0, // Switch cooling
    5, 0, // Switch to cooling
    6, 0, // Enable fan mode
    7, 0, // Disable fan mode
    8, 0, // Is system on
    8, 0, // Is the system on
    9, 0, // Connect wifi
    9, 0, // Connect to wifi
    10, 0, // Decrease brightness
    10, 0, // Decrease screen brightness
    11, 1, 1, -1, // Decrease temperature <numbers10> {degree,degrees}
    11, 1, 1, -1, // Decrease temperature by <numbers10> {degree,degrees}
    12, 0, // Increase brightness
    12, 0, // Increase screen brightness
    13, 1, 2, -1, // Increase temperature <numbers10> {degree,degrees}
    13, 1, 2, -1, // Increase temperature by <numbers10> {degree,degrees}
    14, 0, // Unmute Volume
    15, 0, // Mute Volume
    16, 0, // Go sleep
    16, 0, // Go to sleep
    17, 0, // Go setting
    17, 0, // Go to setting
    18, 0, // What is current temperature
    18, 0, // What is the current temperature
    19, 0, // What is wifi status
    19, 0, // What is the wifi status
};

const int VA_HMI_Thermostat_intent_map_array_sizes[VA_HMI_THERMOSTAT_NUM_COMMANDS] = {
    4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 4, 4, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 
};

const int VA_HMI_Thermostat_variable_phrase_sizes[VA_HMI_THERMOSTAT_NUM_VARIABLES] = {
    0, // DegreeTemperature: None
    0, // DecreTemperature: None
    0, // IncreTemperature: None
};

const int VA_HMI_Thermostat_unit_phrase_map_array[VA_HMI_THERMOSTAT_UNIT_PHRASE_MAP_ARRAY_TOTAL_SIZE] = {
    1, 2, 0, 1, // {degree,degrees}
    1, 2, 0, 1, // {degree,degrees}
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    1, 2, 0, 1, // {degree,degrees}
    1, 2, 0, 1, // {degree,degrees}
    0, // None
    0, // None
    1, 2, 0, 1, // {degree,degrees}
    1, 2, 0, 1, // {degree,degrees}
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
    0, // None
};

const int VA_HMI_Thermostat_unit_phrase_map_array_sizes[VA_HMI_THERMOSTAT_NUM_COMMANDS] = {
    4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 
};



__attribute__((weak)) void VA_HMI_Thermostat_wake_word_callback(mtb_wwd_nlu_events_t event)
{

}
