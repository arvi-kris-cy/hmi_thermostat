/*******************************************************************************
 * File Name:  app_rtc.c
 *
 * Description: Real-time clock (RTC) control module implementation.
 *
 * This file implements the RTC control functions, including initialization,
 * time synchronization using a FreeRTOS timer, and manual time updates.
 *
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

/*******************************************************************************
 *                              INCLUDES
 ******************************************************************************/
#include "app_rtc.h"
#include "timers.h"

/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/
#define READ_RTC_DATA_MS        1000U   /* Read RTC time update interval */

/*******************************************************************************
 *                              GLOBAL VARIABLES
 ******************************************************************************/
volatile bool update_timestamp = false;
cy_stc_rtc_config_t ui_current_time;
const cy_stc_sysint_t rtc_intr_config = { .intrSrc = srss_interrupt_rtc_IRQn, .intrPriority = 3, };

/*******************************************************************************
 *                              STATIC VARIABLES
 ******************************************************************************/
static TimerHandle_t minute_sync_timer = NULL;
static bool rtc_init_state = false;

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/

/*******************************************************************************
 * Function Name: minute_sync_freertos_timer_cb
 ********************************************************************************
 * Summary:
 * FreeRTOS timer callback for minute synchronization. Notifies the graphics
 * task to update the UI and resets the timer for the next minute.
 *
 * Parameter:
 * xTimer: A handle to the FreeRTOS timer.
 *
 * Return :
 * void
 *******************************************************************************/
static void minute_sync_freertos_timer_cb(TimerHandle_t xTimer);

/*******************************************************************************
 * Function Name: start_minute_sync_timer
 ********************************************************************************
 * Summary:
 * Initializes and starts the minute synchronization timer.
 *
 * Parameter:
 * void
 *
 * Return :
 * void
 *******************************************************************************/
void start_minute_sync_timer(void);

/*******************************************************************************
 * Function Name: stop_minute_sync_timer
 ********************************************************************************
 * Summary:
 * Stops the minute synchronization timer.
 *
 * Parameter:
 * void
 *
 * Return :
 * void
 *******************************************************************************/
void stop_minute_sync_timer(void);

/*******************************************************************************
 * Function Name: validate_date_time
 ********************************************************************************
 * Summary:
 * This function validates date and time value.
 *
 * Parameters:
 * int sec     : The second valid range is [0-59].
 * int min     : The minute valid range is [0-59].
 * int hour    : The hour valid range is [0-23].
 * int mday    : The day of month valid range is [1-31], if the month of February
 * is selected as the Month parameter, then the valid range
 * is [0-29].
 * int month   : The month valid range is [1-12].
 * int year    : The year valid range is [> 0].
 *
 * Return:
 * false - invalid ; true - valid
 *
 *******************************************************************************/
static bool validate_date_time(int sec, int min, int hour, int mday, int month, int year);

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

static void minute_sync_freertos_timer_cb(TimerHandle_t xTimer)
{
    /* Notify the graphics task to update the UI. */
    update_timestamp = true;

    /* Read current date and time */
    Cy_RTC_GetDateAndTime(&ui_current_time);

    /* After the initial one-shot, we set the timer to fire again in 1 seconds.
     This creates a continuous, per-minute update loop. */
    xTimerChangePeriod(xTimer, pdMS_TO_TICKS(READ_RTC_DATA_MS), 0);
}

void start_minute_sync_timer(void)
{
    /* The timer is set to be a one-shot initially (pdFALSE). */
    minute_sync_timer = xTimerCreate("MinuteSyncTimer", pdMS_TO_TICKS(READ_RTC_DATA_MS), pdFALSE, (void*) 0,
            minute_sync_freertos_timer_cb);

    /* Start the timer */
    if (minute_sync_timer != NULL)
    {
        xTimerStart(minute_sync_timer, 0);
    }
}

void stop_minute_sync_timer(void)
{
    if (minute_sync_timer != NULL)
    {
        /* Stop and delete the timer. */
        xTimerStop(minute_sync_timer, 100);
        xTimerDelete(minute_sync_timer, 100);
        minute_sync_timer = NULL;
    }
}

static bool validate_date_time(int sec, int min, int hour, int mday, int month, int year)
{
    static const uint8_t days_in_month_table[MONTHS_PER_YEAR] = {
        DAYS_IN_JANUARY,
        DAYS_IN_FEBRUARY,
        DAYS_IN_MARCH,
        DAYS_IN_APRIL,
        DAYS_IN_MAY,
        DAYS_IN_JUNE,
        DAYS_IN_JULY,
        DAYS_IN_AUGUST,
        DAYS_IN_SEPTEMBER,
        DAYS_IN_OCTOBER,
        DAYS_IN_NOVEMBER,
        DAYS_IN_DECEMBER,
    };

    uint8_t days_in_month;

    bool rslt = IS_SEC_VALID(sec) & IS_MIN_VALID(min) &
    IS_HOUR_VALID(hour) & IS_MONTH_VALID(month) &
    IS_YEAR_VALID(year);

    if (rslt)
    {
        days_in_month = days_in_month_table[month - 1];

        if (IS_LEAP_YEAR(year) && (month == 2))
        {
            days_in_month++;
        }

        rslt &= (mday > 0U) && (mday <= days_in_month);
    }

    return rslt;
}

void app_rtc_init(void)
{
    Cy_RTC_Init(&CYBSP_RTC_config);

    cy_stc_rtc_config_t current_time = { .sec = 0, .min = 30, .hour = 13, .hrFormat = CY_RTC_24_HOURS, .dayOfWeek =
            CY_RTC_MONDAY, .date = 1, .month = 9, .year = 25 };

    /* Set current time */
    Cy_RTC_SetDateAndTime(&current_time);

    /* Read current datetime info */
    Cy_RTC_GetDateAndTime(&ui_current_time);

    /* Start minute sync timer */
    start_minute_sync_timer();

    /* Update state */
    rtc_init_state = true;
    update_timestamp = true;
}

void update_time_on_ui(void)
{
    if (true == rtc_init_state)
    {
        /* Read current date time info */
        Cy_RTC_GetDateAndTime(&ui_current_time);
    }
}

void set_date_time_rtc(DateTime *info)
{
    cy_stc_rtc_config_t new_rtc_config;

    /* Read the current RTC configuration to preserve settings like hrFormat. */
    Cy_RTC_GetDateAndTime(&new_rtc_config);

    new_rtc_config.year = (info->year - RTC_CENTURY);
    new_rtc_config.month = info->month;
    new_rtc_config.date = info->day;
    new_rtc_config.hour = info->hour;
    new_rtc_config.min = info->minute;
    new_rtc_config.sec = info->second;

    /* Calculate the day of the week from the date */
    struct tm time_info = {
        .tm_sec = new_rtc_config.sec,
        .tm_min = new_rtc_config.min,
        .tm_hour = new_rtc_config.hour,
        .tm_mday = new_rtc_config.date,
        .tm_mon = new_rtc_config.month - 1,
        .tm_year = ((new_rtc_config.year + RTC_CENTURY) - TM_YEAR_BASE)
    };

    // Use mktime to populate the tm_wday field
    if (mktime(&time_info) != -1)
    {
        new_rtc_config.dayOfWeek = time_info.tm_wday + 1;
        printf("Calculated day of the week: %d\r\n", new_rtc_config.dayOfWeek);
    }
    else
    {
        printf("Error: Could not calculate day of the week.\r\n");
    }

    /* Now set the new date and time on the RTC. */
    Cy_RTC_SetDateAndTime(&new_rtc_config);

    /* Read values from the RTC to update the global UI timestamp */
    Cy_RTC_GetDateAndTime(&ui_current_time);

    /* Restart UI update timer */
    stop_minute_sync_timer();
    start_minute_sync_timer();
    update_timestamp = true;
}

/* [] END OF FILE */
