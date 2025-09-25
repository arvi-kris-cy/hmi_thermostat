/*******************************************************************************
 * File Name:  app_rtc.h
 *
 * Description: Public interface for the real-time clock (RTC) control module.
 *
 *******************************************************************************
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

#ifndef APP_RTC_H_
#define APP_RTC_H_

/*******************************************************************************
 *                                INCLUDES
 *******************************************************************************/
#include "cybsp.h"
#include "retarget_io_init.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cyabs_rtos.h"
#include "cyabs_rtos_impl.h"
#include "app_common.h"

/*******************************************************************************
 *                                CONSTANTS
 *******************************************************************************/
/* Structure tm stores years since 1900 */
#define TM_YEAR_BASE            (1900U)

#define RTC_CENTURY             (2000U)

/* Maximum value of seconds and minutes */
#define MAX_SEC_OR_MIN          (60U)

/* Maximum value of hours definition */
#define MAX_HOURS_24H           (23U)

/* Month per year definition */
#define MONTHS_PER_YEAR         (12U)

/* Days per week definition */
#define DAYS_PER_WEEK           (7U)

/* Days in month */
#define DAYS_IN_JANUARY         (31U)  /* Number of days in January */
#define DAYS_IN_FEBRUARY        (28U)  /* Number of days in February */
#define DAYS_IN_MARCH           (31U)  /* Number of days in March */
#define DAYS_IN_APRIL           (30U)  /* Number of days in April */
#define DAYS_IN_MAY             (31U)  /* Number of days in May */
#define DAYS_IN_JUNE            (30U)  /* Number of days in June */
#define DAYS_IN_JULY            (31U)  /* Number of days in July */
#define DAYS_IN_AUGUST          (31U)  /* Number of days in August */
#define DAYS_IN_SEPTEMBER       (30U)  /* Number of days in September */
#define DAYS_IN_OCTOBER         (31U)  /* Number of days in October */
#define DAYS_IN_NOVEMBER        (30U)  /* Number of days in November */
#define DAYS_IN_DECEMBER        (31U)  /* Number of days in December */


/* Macro to validate seconds parameter */
#define IS_SEC_VALID(sec) ((sec) <= (MAX_SEC_OR_MIN -1))

/* Macro to validate minutes parameters */
#define IS_MIN_VALID(min) ((min) <= (MAX_SEC_OR_MIN -1))

/* Macro to validate hour parameter */
#define IS_HOUR_VALID(hour) ((hour) <= MAX_HOURS_24H)

/* Macro to validate month parameter */
#define IS_MONTH_VALID(month) (((month) > 0U) && ((month) <= MONTHS_PER_YEAR))

/* Macro to validate the year value */
#define IS_YEAR_VALID(year) ((year) > 0U)

/* Checks whether the year passed through the parameter is leap or not */
#define IS_LEAP_YEAR(year) \
(((0U == (year % 4UL)) && (0U != (year % 100UL))) || (0U == (year % 400UL)))

/*******************************************************************************
 *                               GLOBAL VARIABLES
 *******************************************************************************/
extern const cy_stc_sysint_t rtc_intr_config;
extern volatile bool update_timestamp;
extern cy_stc_rtc_config_t ui_current_time;

/*******************************************************************************
 *                              FUNCTION PROTOTYPES
 *******************************************************************************/
/**
 * @brief Initializes the RTC module and starts the minute synchronization timer.
 *
 */
void app_rtc_init(void);

/**
 * @brief Updates the LVGL UI with the current time from the RTC.
 *
 */
void update_time_on_ui(void);

void set_date_time_rtc(DateTime *info);
void start_minute_sync_timer(void);
void stop_minute_sync_timer(void);

#endif /* APP_RTC_H_ */

/* [] END OF FILE */
