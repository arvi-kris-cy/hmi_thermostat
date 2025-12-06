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
#define RTC_ADDRESS             (0x6F)

#define REG_RTCSEC              (0x00)  // Time Second (bit7 = ST)
#define REG_RTCMIN              (0x01)  // Time Minute
#define REG_RTCHOUR             (0x02)  // Time Hour (bit6 = 12/24 hour)
#define REG_RTCWKDAY            (0x03)  // Day-of-Week (bit3 = VBATEN, bit4 = PWRFAIL)
#define REG_RTCDATE             (0x04)  // Day
#define REG_RTCMTH              (0x05)  // Month
#define REG_RTCYEAR             (0x06)  // Year (00..99)
#define REG_CONTROL             (0x07)  // Control
#define REG_OSCTRIM             (0x08)  // Oscillator digital trim
#define REG_PWRDNMIN            (0x18)  // Power Failure Time Minute
#define REG_PWRDNHR             (0x19)  // Power Failure Time Hour
#define REG_PWRUPMIN            (0x1C)  // Power Restore Time Minute
#define REG_PWRUPHR             (0x1D)  // Power Restore Time Hour

#define I2C_TIMEOUT_MS          (50)
#define RTC_SRAM_MAGIC_ADDR  0x20    // start of battery-backed SRAM
#define RTC_SRAM_MAGIC_LEN   4
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

extern mtb_hal_i2c_t CYBSP_I2C_CONTROLLER_hal_obj;
// extern cy_stc_scb_i2c_context_t CYBSP_I2C_CONTROLLER_context;

static const uint8_t rtc_magic[RTC_SRAM_MAGIC_LEN] = { 'I','F','X','R' };

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

    cy_stc_rtc_config_t current_time = { .sec = 0, .min = 30, .hour = 17, .hrFormat = CY_RTC_24_HOURS, .dayOfWeek =
            CY_RTC_MONDAY, .date = 27, .month = 10, .year = 25 };

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

/********************************************************************
 * @brief RTC Reg
 * 
 * @param reg 
 * @param value 
 * @return cy_rslt_t 
 ********************************************************************/
cy_rslt_t rtc_init(void)
{
    cy_rslt_t rslt;

    // Reset control (optional, safe)
    rslt = rtc_write_register(REG_CONTROL, 0x00);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    // Optional: set digital trim (match your design/calibration)
    rslt = rtc_write_register(REG_OSCTRIM, 0x45);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    // 24-hour mode: clear bit6 in RTCHOUR
    rslt = rtc_configure_bit(REG_RTCHOUR, 6, 0x00);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    // Enable battery backup (VBATEN bit3). Safe to call every boot.
    rslt = rtc_configure_bit(REG_RTCWKDAY, 3, 0x01);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    // If oscillator is already running, do nothing more
    if (rtc_is_running()) {
        return CY_RSLT_SUCCESS;
    }

    // If oscillator is not running, just start it without overwriting the time registers
    // (time will continue from whatever is in the registers; see note in comments).
    rslt = rtc_configure_bit(REG_RTCSEC, 7, 0x01);
    return rslt;
}

// BCD helpers
static uint8_t to_bcd(uint8_t dec)   { return ((dec / 10) << 4) | (dec % 10); }
static uint8_t from_bcd(uint8_t bcd) { return ((bcd >> 4) * 10) + (bcd & 0x0F); }

cy_rslt_t rtc_configure_bit(uint8_t reg, uint8_t positions, uint8_t value)
{
    uint8_t regval = 0;
    cy_rslt_t rslt = rtc_read_register(reg, &regval);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    if (value) regval |=  (1u << positions);
    else       regval &= ~(1u << positions);

    return rtc_write_register(reg, regval);
}

cy_rslt_t rtc_write_register(uint8_t reg, uint8_t value)
{
    return rtc_write_registers(reg, &value, 1);
}
cy_rslt_t rtc_read_register(uint8_t reg, uint8_t* value)
{
    return rtc_read_registers(reg, value, 1);
}

// Write a buffer to a register (first byte is register address, followed by data)
cy_rslt_t rtc_write_registers(uint8_t start_reg, const uint8_t* data, size_t len)
{
    // Prepare a tx buffer: [reg][data...]
    uint8_t temp[1 + 16]; // adjust if you need larger writes
    if (len > 16) return CY_RSLT_TYPE_ERROR;
    temp[0] = start_reg;
    for (size_t i = 0; i < len; ++i) temp[1 + i] = data[i];

    // Write with a STOP
    return mtb_hal_i2c_controller_write(&CYBSP_I2C_CONTROLLER_hal_obj,
                                    RTC_ADDRESS,
                                    temp, (1 + len),
                                    I2C_TIMEOUT_MS, true);
}

// Read N bytes starting at a register address (write reg, repeated-start, read data)
cy_rslt_t rtc_read_registers(uint8_t start_reg, uint8_t* data, size_t len)
{
    cy_rslt_t rslt;

    // Write the register pointer without STOP to allow repeated-start
    rslt = mtb_hal_i2c_controller_write(&CYBSP_I2C_CONTROLLER_hal_obj,
                                    RTC_ADDRESS,
                                    &start_reg, 1,
                                    I2C_TIMEOUT_MS, false);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    // Now read data with STOP
    rslt = mtb_hal_i2c_controller_read(&CYBSP_I2C_CONTROLLER_hal_obj,
                                   RTC_ADDRESS,
                                   data, len,
                                   I2C_TIMEOUT_MS, true);
    return rslt;
}

// Set the RTC time (24-hour mode). Recommended sequence: write all time/date with ST=0, then set ST=1.
cy_rslt_t rtc_set_time(rtc_time_t* t)
{
    if (!t) return CY_RSLT_TYPE_ERROR;
    cy_rslt_t rslt;
    uint8_t buf[7];

    buf[0] = to_bcd(t->seconds) & 0x7F; // ST=0 while writing
    buf[1] = to_bcd(t->minutes) & 0x7F;
    buf[2] = to_bcd(t->hours)   & 0x3F; // 24-hour mode
    buf[3] = to_bcd(t->dow)     & 0x07; // VBATEN will be re-set below
    buf[4] = to_bcd(t->day)     & 0x3F;
    buf[5] = to_bcd(t->month)   & 0x1F;
    buf[6] = to_bcd(t->year);

    rslt = rtc_write_registers(REG_RTCSEC, buf, 7);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    // Re-enable VBATEN (WKDAY bit3) because writing WKDAY clobbers control bits
    rslt = rtc_configure_bit(REG_RTCWKDAY, 3, 0x01);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    // Start oscillator
    rslt = rtc_configure_bit(REG_RTCSEC, 7, 0x01);
    return rslt;
}

cy_rslt_t rtc_set_time_safe(const rtc_time_t* t)
{
    if (!t) return CY_RSLT_TYPE_ERROR;
    cy_rslt_t r;

    // 1) Clear ST while loading seconds (write seconds with bit7=0)
    uint8_t sec = to_bcd(t->seconds) & 0x7F;
    r = rtc_write_register(REG_RTCSEC, sec);
    if (r != CY_RSLT_SUCCESS) return r;

    // 2) Write other fields individually (do NOT touch WKDAY here)
    r = rtc_write_register(REG_RTCMIN,  to_bcd(t->minutes) & 0x7F); if (r != CY_RSLT_SUCCESS) return r;
    r = rtc_write_register(REG_RTCHOUR, to_bcd(t->hours)   & 0x3F); if (r != CY_RSLT_SUCCESS) return r; // 24-hour mode
    r = rtc_write_register(REG_RTCDATE, to_bcd(t->day)     & 0x3F); if (r != CY_RSLT_SUCCESS) return r;
    r = rtc_write_register(REG_RTCMTH,  to_bcd(t->month)   & 0x1F); if (r != CY_RSLT_SUCCESS) return r;
    r = rtc_write_register(REG_RTCYEAR, to_bcd(t->year));           if (r != CY_RSLT_SUCCESS) return r;

    // 3) Update WKDAY with the new DOW, preserving VBATEN (bit3) and letting PWRFAIL clear as per spec
    uint8_t wkday_old = 0;
    r = rtc_read_register(REG_RTCWKDAY, &wkday_old);
    if (r != CY_RSLT_SUCCESS) return r;

    uint8_t new_wkday = (wkday_old & 0xF8) | (to_bcd(t->dow) & 0x07); // keep bit3 (VBATEN), clear bit4 by write, preserve bit5 status
    r = rtc_write_register(REG_RTCWKDAY, new_wkday);
    if (r != CY_RSLT_SUCCESS) return r;

    // 4) Start oscillator by setting ST bit
    r = rtc_configure_bit(REG_RTCSEC, 7, 0x01);
    return r;
}

// Read the current RTC time
cy_rslt_t rtc_get_time(rtc_time_t* t)
{
    if (!t) return CY_RSLT_TYPE_ERROR;
    uint8_t buf[7];
    cy_rslt_t rslt = rtc_read_registers(REG_RTCSEC, buf, 7);
    if (rslt != CY_RSLT_SUCCESS) return rslt;

    t->seconds = from_bcd(buf[0] & 0x7F);
    t->minutes = from_bcd(buf[1] & 0x7F);
    t->hours   = from_bcd(buf[2] & 0x3F);
    t->dow     = from_bcd(buf[3] & 0x07);
    t->day     = from_bcd(buf[4] & 0x3F);
    t->month   = from_bcd(buf[5] & 0x1F);
    t->year    = from_bcd(buf[6]);

    return CY_RSLT_SUCCESS;
}

// Clear the power fail flag (WKDAY bit4)
cy_rslt_t rtc_clear_power_fail_flag(void)
{
    return rtc_configure_bit(REG_RTCWKDAY, 4, 0x00);
}

// Status helpers
bool rtc_is_running(void)
{
    uint8_t sec = 0;
    if (rtc_read_register(REG_RTCSEC, &sec) != CY_RSLT_SUCCESS) return false;
    return (sec & 0x80) != 0; // ST bit
}

cy_rslt_t rtc_start_oscillator(void)
{
    return rtc_configure_bit(REG_RTCSEC, 7, 0x01);
}

bool rtc_time_is_valid(const rtc_time_t* t)
{
    if (!t) return false;

    // Basic range checks for 24-hour mode and BCD-decoded fields
    bool ranges_ok =
        (t->seconds <= 59) &&
        (t->minutes <= 59) &&
        (t->hours   <= 23) &&
        (t->dow     >= 1 && t->dow <= 7) &&
        (t->day     >= 1 && t->day <= 31) &&
        (t->month   >= 1 && t->month <= 12) &&
        (t->year    <= 99);

    // Consider all-zero time/date as invalid
    bool all_zero = (t->seconds == 0) && (t->minutes == 0) && (t->hours == 0) &&
                    (t->dow == 1) && (t->day == 1) && (t->month == 1) && (t->year == 1);

    return ranges_ok && !all_zero;
}

void rtc_debug_check(void)
{
    uint8_t wkday = 0;
    if (rtc_read_register(REG_RTCWKDAY, &wkday) == CY_RSLT_SUCCESS) {
        bool vbat_en   = (wkday & 0x08) != 0; // bit3
        bool pwr_fail  = (wkday & 0x10) != 0; // bit4
        printf("WKDAY=0x%02X  VBATEN=%u  PWRFAIL=%u\r\n", wkday, vbat_en, pwr_fail);

        if (pwr_fail) {
            uint8_t pwrDn[2] = {0}, pwrUp[2] = {0};
            if (rtc_read_registers(REG_PWRDNMIN, pwrDn, 2) == CY_RSLT_SUCCESS &&
                rtc_read_registers(REG_PWRUPMIN, pwrUp, 2) == CY_RSLT_SUCCESS) {
                uint8_t dn_min = ((pwrDn[0] >> 4) * 10) + (pwrDn[0] & 0x0F);
                uint8_t dn_hr  = ((pwrDn[1] >> 4) * 10) + (pwrDn[1] & 0x0F);
                uint8_t up_min = ((pwrUp[0] >> 4) * 10) + (pwrUp[0] & 0x0F);
                uint8_t up_hr  = ((pwrUp[1] >> 4) * 10) + (pwrUp[1] & 0x0F);
                printf("PWRDN %02u:%02u  PWRUP %02u:%02u\r\n", dn_hr, dn_min, up_hr, up_min);
            } else {
                printf("Failed to read PWRDN/PWRUP registers\r\n");
            }
        }
    } else {
        printf("Failed to read WKDAY\r\n");
    }

    // Also dump raw time/date registers to see what is actually stored at boot
    uint8_t sec=0, min=0, hr=0, dt=0, mth=0, yr=0;
    rtc_read_register(REG_RTCSEC,  &sec);
    rtc_read_register(REG_RTCMIN,  &min);
    rtc_read_register(REG_RTCHOUR, &hr);
    rtc_read_register(REG_RTCDATE, &dt);
    rtc_read_register(REG_RTCMTH,  &mth);
    rtc_read_register(REG_RTCYEAR, &yr);
    printf("Raw: SEC=0x%02X MIN=0x%02X HR=0x%02X DATE=0x%02X MTH=0x%02X YEAR=0x%02X\r\n",
           sec, min, hr, dt, mth, yr);
}

cy_rslt_t rtc_service_power_fail(void)
{
    cy_rslt_t r;
    uint8_t wkday = 0;
    r = rtc_read_register(REG_RTCWKDAY, &wkday);
    if (r != CY_RSLT_SUCCESS) return r;

    if ((wkday & 0x10) != 0) {
        // Read power-down/up timestamps while the flag is still set
        uint8_t pwrDn[2] = {0}, pwrUp[2] = {0};
        r = rtc_read_registers(REG_PWRDNMIN, pwrDn, 2);
        if (r != CY_RSLT_SUCCESS) return r;
        r = rtc_read_registers(REG_PWRUPMIN, pwrUp, 2);
        if (r != CY_RSLT_SUCCESS) return r;

        uint8_t dn_min = ((pwrDn[0] >> 4) * 10) + (pwrDn[0] & 0x0F);
        uint8_t dn_hr  = ((pwrDn[1] >> 4) * 10) + (pwrDn[1] & 0x0F);
        uint8_t up_min = ((pwrUp[0] >> 4) * 10) + (pwrUp[0] & 0x0F);
        uint8_t up_hr  = ((pwrUp[1] >> 4) * 10) + (pwrUp[1] & 0x0F);

        printf("Power Failure Detected: PWRDN %02u:%02u, PWRUP %02u:%02u\r\n", dn_hr, dn_min, up_hr, up_min);

        // Clear PWRFAIL so the next event will be logged
        r = rtc_clear_power_fail_flag();
        if (r != CY_RSLT_SUCCESS) return r;
    }
    return CY_RSLT_SUCCESS;
}

cy_rslt_t rtc_sram_write_magic(void)
{
    return rtc_write_registers(RTC_SRAM_MAGIC_ADDR, rtc_magic, RTC_SRAM_MAGIC_LEN);
}

cy_rslt_t rtc_sram_read_magic(bool* present)
{
    if (!present) return CY_RSLT_TYPE_ERROR;
    uint8_t buf[RTC_SRAM_MAGIC_LEN] = {0};
    cy_rslt_t r = rtc_read_registers(RTC_SRAM_MAGIC_ADDR, buf, RTC_SRAM_MAGIC_LEN);
    if (r != CY_RSLT_SUCCESS) return r;
    *present = (buf[0] == rtc_magic[0] &&
                buf[1] == rtc_magic[1] &&
                buf[2] == rtc_magic[2] &&
                buf[3] == rtc_magic[3]);
    return CY_RSLT_SUCCESS;
}

// Compute DOW in range 1..7 (user-defined; here 1=Monday, 7=Sunday)
// You can change mapping if you prefer 1=Sunday.
static uint8_t dow_from_date(uint16_t year, uint8_t month, uint8_t day)
{
    // Zeller’s congruence (Gregorian) adapted to produce 1..7
    // Map: 1=Monday, 7=Sunday
    int y = (month < 3) ? (year - 1) : year;
    int m = (month < 3) ? (month + 12) : month;
    int K = y % 100;
    int J = y / 100;
    int h = (day + (13*(m + 1))/5 + K + (K/4) + (J/4) + (5*J)) % 7; // 0=Saturday, 1=Sunday, 2=Monday...
    int d = ((h + 5) % 7) + 1; // 1=Monday ... 7=Sunday
    return (uint8_t)d;
}

// Use this instead of set_date_time_rtc (UI-only). This writes the hardware RTC safely.
cy_rslt_t rtc_set_time_safe_from_http(uint16_t year, uint8_t month, uint8_t day,
                                             uint8_t hour, uint8_t minute, uint8_t second)
{
    cy_rslt_t r;

    // 1) Stop oscillator while setting seconds (write seconds with ST=0)
    r = rtc_write_register(REG_RTCSEC, (to_bcd(second) & 0x7F));
    if (r != CY_RSLT_SUCCESS) return r;

    // 2) Write other time/date fields individually (do NOT write WKDAY yet)
    r = rtc_write_register(REG_RTCMIN,  (to_bcd(minute) & 0x7F)); if (r != CY_RSLT_SUCCESS) return r;
    r = rtc_write_register(REG_RTCHOUR, (to_bcd(hour)   & 0x3F)); if (r != CY_RSLT_SUCCESS) return r; // 24h mode
    r = rtc_write_register(REG_RTCDATE, (to_bcd(day)    & 0x3F)); if (r != CY_RSLT_SUCCESS) return r;
    r = rtc_write_register(REG_RTCMTH,  (to_bcd(month)  & 0x1F)); if (r != CY_RSLT_SUCCESS) return r;
    r = rtc_write_register(REG_RTCYEAR,  to_bcd((uint8_t)(year % 100))); if (r != CY_RSLT_SUCCESS) return r;

    // 3) Compose WKDAY: new DOW, preserve VBATEN (bit3), writing WKDAY clears PWRFAIL by design
    uint8_t wkday_old = 0;
    r = rtc_read_register(REG_RTCWKDAY, &wkday_old);
    if (r != CY_RSLT_SUCCESS) return r;

    uint8_t dow = dow_from_date(year, month, day); // 1..7
    uint8_t new_wkday = (wkday_old & 0xF8) | (to_bcd(dow) & 0x07); // keep VBATEN (bit3), OSCRUN (bit5)
    new_wkday |= 0x08; // ensure VBATEN stays set

    r = rtc_write_register(REG_RTCWKDAY, new_wkday);
    if (r != CY_RSLT_SUCCESS) return r;

    // 4) Start oscillator: set ST bit in seconds register
    r = rtc_configure_bit(REG_RTCSEC, 7, 0x01);
    return r;
}

void rtc_boot_once(void)
{
    cy_rslt_t r = rtc_init();
    if (r != CY_RSLT_SUCCESS) {
        printf("RTC init failed: 0x%08lx\r\n", (unsigned long)r);
        return;
    }

    // Service any power-fail record BEFORE any WKDAY write or time block writes
    r = rtc_service_power_fail();
    if (r != CY_RSLT_SUCCESS) {
        printf("Power-fail service failed: 0x%08lx\r\n", (unsigned long)r);
    }

    // Check if RTC was previously initialized (SRAM signature)
    bool initialized = false;
    r = rtc_sram_read_magic(&initialized);
    if (r != CY_RSLT_SUCCESS) {
        printf("SRAM read magic failed: 0x%08lx\r\n", (unsigned long)r);
    }

    // Read current time
    rtc_time_t now;
    r = rtc_get_time(&now);
    if (r != CY_RSLT_SUCCESS) {
        printf("RTC read failed: 0x%08lx\r\n", (unsigned long)r);
        return;
    }

    // Decide whether to seed
    if (!initialized || !rtc_time_is_valid(&now)) {
        rtc_time_t init_time = { .seconds=0, .minutes=52, .hours=10, .dow=7, .day=4, .month=4, .year=25 };
        r = rtc_set_time_safe(&init_time);
        if (r == CY_RSLT_SUCCESS) {
            // Write the signature to indicate the RTC was initialized
            (void)rtc_sram_write_magic();
            printf("RTC seeded once and oscillator started.\r\n");
        } else {
            printf("Set time failed: 0x%08lx\r\n", (unsigned long)r);
        }
    } else {
        // Ensure oscillator is running; don’t overwrite time
        if (!rtc_is_running()) {
            r = rtc_start_oscillator();
            if (r != CY_RSLT_SUCCESS) {
                printf("Oscillator start failed: 0x%08lx\r\n", (unsigned long)r);
            }
        }
    }

    // Final read and print
    r = rtc_get_time(&now);
    if (r == CY_RSLT_SUCCESS) {
        printf("Current Time: %02u:%02u:%02u  DOW:%u  %02u/%02u/%02u\r\n",
               now.hours, now.minutes, now.seconds, now.dow,
               now.day, now.month, now.year);
    }
}

/* [] END OF FILE */
