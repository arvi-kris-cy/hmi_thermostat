#ifndef APP_VA_HMI_THERMOSTAT_h
#define APP_VA_HMI_THERMOSTAT_h

#include "VA_HMI_Thermostat.h"

#define  VA_HMI_THERMOSTAT_INTENT_SETTEMPERATURE                    0x0
#define  VA_HMI_THERMOSTAT_INTENT_HEATINGOFFMODE                    0x1
#define  VA_HMI_THERMOSTAT_INTENT_SCREENOFF                         0x2
#define  VA_HMI_THERMOSTAT_INTENT_HEATINGONMODE                     0x3
#define  VA_HMI_THERMOSTAT_INTENT_SCREENON                          0x4
#define  VA_HMI_THERMOSTAT_INTENT_COOLINGMODE                       0x5
#define  VA_HMI_THERMOSTAT_INTENT_FANENABLEMODE                     0x6
#define  VA_HMI_THERMOSTAT_INTENT_FANDISABLEMODE                    0x7
#define  VA_HMI_THERMOSTAT_INTENT_SYSTEMSTATUS                      0x8
#define  VA_HMI_THERMOSTAT_INTENT_CONNECTTOWIFI                     0x9
#define  VA_HMI_THERMOSTAT_INTENT_DECREASESCREENBRIGHTNESS          0xa
#define  VA_HMI_THERMOSTAT_INTENT_DECREASETEMPERATURE               0xb
#define  VA_HMI_THERMOSTAT_INTENT_INCREASESCREENBRIGHTNESS          0xc
#define  VA_HMI_THERMOSTAT_INTENT_INCREASETEMPERATURE               0xd
#define  VA_HMI_THERMOSTAT_INTENT_UNMUTEVOLUME                      0xe
#define  VA_HMI_THERMOSTAT_INTENT_MUTEVOLUME                        0xf
#define  VA_HMI_THERMOSTAT_INTENT_SLEEPMODE                         0x10
#define  VA_HMI_THERMOSTAT_INTENT_SETTINGMODE                       0x11
#define  VA_HMI_THERMOSTAT_INTENT_TEMPERATURESTATUS                 0x12
#define  VA_HMI_THERMOSTAT_INTENT_WIFISTATUS                        0x13

#define  VA_HMI_THERMOSTAT_VARIABLE_DegreeTemperature_          0x0
#define  VA_HMI_THERMOSTAT_VARIABLE_DecreTemperature_           0x1
#define  VA_HMI_THERMOSTAT_VARIABLE_IncreTemperature_           0x2

#define  VA_HMI_THERMOSTAT_UNIT_degree                            0x0
#define  VA_HMI_THERMOSTAT_UNIT_degrees                           0x1
#define  VA_HMI_THERMOSTAT_UNIT_percent                           0x2
#define  VA_HMI_THERMOSTAT_UNIT_level                             0x3
#define  VA_HMI_THERMOSTAT_UNIT_levels                            0x4
#define  VA_HMI_THERMOSTAT_UNIT_hour                              0x5
#define  VA_HMI_THERMOSTAT_UNIT_hours                             0x6
#define  VA_HMI_THERMOSTAT_UNIT_minute                            0x7
#define  VA_HMI_THERMOSTAT_UNIT_minutes                           0x8
#define  VA_HMI_THERMOSTAT_UNIT_second                            0x9
#define  VA_HMI_THERMOSTAT_UNIT_seconds                           0xa
#define  VA_HMI_THERMOSTAT_UNIT_day                               0xb
#define  VA_HMI_THERMOSTAT_UNIT_days                              0xc
#define  VA_HMI_THERMOSTAT_UNIT_                                  0xd
#define  VA_HMI_THERMOSTAT_UNIT_AM                                0xe
#define  VA_HMI_THERMOSTAT_UNIT_PM                                0xf

#endif // APP_VA_HMI_THERMOSTAT_h
