/*******************************************************************************
 * File Name:   app_sensor.h
 *
 * Description:  Public interface for sensor control.
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

#ifndef APP_SENSOR_H_
#define APP_SENSOR_H_

/*******************************************************************************
 *                                INCLUDES
 *******************************************************************************/
#include "cybsp.h"
#include "mtb_sht4x.h"
#include "retarget_io_init.h"
#include "xensiv_pasco2_mtb.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cyabs_rtos.h"
#include "cyabs_rtos_impl.h"
#include "semphr.h"

/*******************************************************************************
 *                                MACROS
 *******************************************************************************/
#define SENSOR_TASK_NAME                ("SensorTask")
#define SENSOR_TASK_STACK_SIZE          (256U)
#define SENSOR_TASK_PRIORITY            (2U)

#define WAIT_SENSOR_RDY_MS              (1000)

#define DEFAULT_PRESSURE_REF_HPA        (0x3F7)

#define SENSOR_SAMPLING_INTERVAL_ACTIVE 2000U   /* Active state interval for sampling (2s) */
#define SENSOR_SAMPLING_INTERVAL_IDLE   10000U   /* Idle state interval for sampling (10s) */

/*******************************************************************************
 *                                CONSTANTS
 *******************************************************************************/

/*******************************************************************************
 *                                DATA TYPES
 *******************************************************************************/

/*******************************************************************************
 *                                GLOBAL VARIABLES
 *******************************************************************************/
extern mtb_hal_i2c_t CYBSP_I2C_CONTROLLER_2_hal_obj;
extern volatile bool sensor_data_available;
extern uint16_t read_ppm;
extern int32_t read_temperature, read_humidity;
extern SemaphoreHandle_t i2c_mutex;
extern TimerHandle_t restart_sensor_timer;

/*******************************************************************************
 *                                FUNCTION PROTOTYPES
 *******************************************************************************/
/**
 * To power and stabilize the co2 sensor
 */
void power_co2_sensor(void);

/**
 * @brief CO2 sensor initialization
 * 
 * @return cy_rslt_t 
 */
cy_rslt_t pasco2_sensor_init(void);

/**
 * @brief Temperature and Humidity sensor initialization
 * 
 * @return cy_rslt_t 
 */
cy_rslt_t sht_sensor_init(void);

/**
 * @brief All sensor initialization
 * 
 * @return cy_rslt_t 
 */
cy_rslt_t sensor_init(void);

/**
 * @brief SENSOR_TASK
 * 
 * @param arg 
 */
void sensor_task(void *arg);

/**
 * @brief Set sensor sampling interval
 *
 * @param interval_ms   Sampling interval in milliseconds
 */
void set_sensor_sampling_interval(uint32_t interval_ms);

/**
 * @brief Initializes the Sensor Task and its retry mechanism.
 *
 * This function creates the Sensor Task, which is responsible for
 * initializing and managing sensor operations.
 *
 * If the Sensor Task creation fails, an error message is printed.
 * If successful, the task begins execution immediately.
 *
 * @note The retry timer (5 seconds) is used in conjunction with the
 *       `restart_sensor_task_timer_cb()` callback to recreate the task after failure.
 *
 * @retval None
 */
void app_sensor_task_init(void);


#endif /* APP_SENSOR_H_ */

/* [] END OF FILE */
