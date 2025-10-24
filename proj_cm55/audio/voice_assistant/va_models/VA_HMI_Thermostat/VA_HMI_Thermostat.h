/*****************************************************************************
* \file VA_HMI_Thermostat.h
*****************************************************************************
* \copyright
* Copyright 2024, Infineon Technologies.
* All rights reserved.
*****************************************************************************/

#ifndef VA_HMI_THERMOSTAT_H
#define VA_HMI_THERMOSTAT_H

#include <stdint.h>

#define VA_HMI_THERMOSTAT_NUM_INTENTS 20
#define VA_HMI_THERMOSTAT_NUM_COMMANDS 34
#define VA_HMI_THERMOSTAT_NUM_VARIABLES 3
#define VA_HMI_THERMOSTAT_NUM_VARIABLE_PHRASES 3
#define VA_HMI_THERMOSTAT_NUM_UNIT_PHRASES 16
#define VA_HMI_THERMOSTAT_INTENT_MAP_ARRAY_TOTAL_SIZE 80
#define VA_HMI_THERMOSTAT_UNIT_PHRASE_MAP_ARRAY_TOTAL_SIZE 52

extern const char* VA_HMI_Thermostat_intent_name_list[VA_HMI_THERMOSTAT_NUM_INTENTS];

extern const char* VA_HMI_Thermostat_variable_name_list[VA_HMI_THERMOSTAT_NUM_VARIABLES];

extern const char* VA_HMI_Thermostat_variable_phrase_list[VA_HMI_THERMOSTAT_NUM_VARIABLE_PHRASES];

extern const char* VA_HMI_Thermostat_unit_phrase_list[VA_HMI_THERMOSTAT_NUM_UNIT_PHRASES];

extern const int VA_HMI_Thermostat_intent_map_array[VA_HMI_THERMOSTAT_INTENT_MAP_ARRAY_TOTAL_SIZE];

extern const int VA_HMI_Thermostat_intent_map_array_sizes[VA_HMI_THERMOSTAT_NUM_COMMANDS];

extern const int VA_HMI_Thermostat_variable_phrase_sizes[VA_HMI_THERMOSTAT_NUM_VARIABLES];

extern const int VA_HMI_Thermostat_unit_phrase_map_array[VA_HMI_THERMOSTAT_UNIT_PHRASE_MAP_ARRAY_TOTAL_SIZE];

extern const int VA_HMI_Thermostat_unit_phrase_map_array_sizes[VA_HMI_THERMOSTAT_NUM_COMMANDS];

#endif // VA_HMI_THERMOSTAT_H
