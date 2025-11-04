/*******************************************************************************
 * File Name:  app_sensor.c
 *
 * Description: Sensor control module implementation.
 *
 * This file contains the implementation for environmental sensor interfacing
 * and handling.
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
 *                                INCLUDES
 ******************************************************************************/
#include "app_sensor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "lv_timer.h"
#include "app_common.h"
/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/

/*******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/
mtb_hal_i2c_t CYBSP_I2C_CONTROLLER_2_hal_obj;
extern cy_stc_scb_i2c_context_t disp_touch_i2c_controller_context;
volatile bool sensor_data_available = false;
uint16_t read_ppm = 0;
int32_t read_temperature, read_humidity = 0;
extern TaskHandle_t rtos_cm55_sensor_task_handle;
TimerHandle_t restart_sensor_timer = NULL;
/*******************************************************************************
 *                             STATIC VARIABLES
 ******************************************************************************/
static xensiv_pasco2_t xensiv_pasco2;
static xensiv_pasco2_id_t chipID;
static TimerHandle_t sensor_read_hdlr;
static uint32_t sensor_sampling_intv = SENSOR_SAMPLING_INTERVAL_ACTIVE;

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/


/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

void power_co2_sensor(void)
{
    Cy_GPIO_Write(CYBSP_CO2_5V_EN_PORT, CYBSP_CO2_5V_EN_PIN, 0u);
    vTaskDelay(100);
    Cy_GPIO_Write(CYBSP_CO2_5V_EN_PORT, CYBSP_CO2_5V_EN_PIN, 1u);
    vTaskDelay(WAIT_SENSOR_RDY_MS);
}

cy_rslt_t pasco2_sensor_init(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    LOG_INFO(CYLF_DEF, "Initializing CO2 sensor...\r\n");

    /* Initialize PAS CO2 sensor */
    result = xensiv_pasco2_mtb_init_i2c(&xensiv_pasco2, &CYBSP_I2C_CONTROLLER_2_hal_obj);
    if (result != CY_RSLT_SUCCESS)
    {
        LOG_ERROR(CYLF_DEF, "PAS CO2 sensor initialization failed!\r\n");
        return result;
    }

    /* Retrieve chip ID */
    result = xensiv_pasco2_get_id(&xensiv_pasco2, &chipID);
    if (result != CY_RSLT_SUCCESS)
    {
        LOG_ERROR(CYLF_DEF, "Failed to read CO2 sensor chip ID!\r\n");
        return result;
    }

    /* Configure sensor in single shot mode */
    result = xensiv_pasco2_start_single_mode(&xensiv_pasco2);
    if (result != CY_RSLT_SUCCESS)
    {
        LOG_ERROR(CYLF_DEF, "Failed to configure sensor in single shot mode!\r\n");
    }

    LOG_INFO(CYLF_DEF, "CO2 sensor initialized successfully \r\n");
    return CY_RSLT_SUCCESS;
}

cy_rslt_t sht_sensor_init(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    LOG_INFO(CYLF_DEF, "Initializing SHT40 sensor...\r\n");

    /* Initialize SHT40 sensor */
    result = mtb_sht4x_init(&CYBSP_I2C_CONTROLLER_2_hal_obj, MTB_SHT40_ADDRESS_DEFAULT);
    if (CY_RSLT_SUCCESS != result)
    {
        LOG_ERROR(CYLF_DEF, "SHT40 sensor initialization failed!\r\n");
        return result;
    }

    LOG_INFO(CYLF_DEF, "SHT40 sensor initialized successfully.\r\n");
    return CY_RSLT_SUCCESS;
}

cy_rslt_t sensor_init(void)
{
    cy_rslt_t result;
    int retry = 0;
    const int max_retries = 5;

    /* Initialize CO2 sensor with retry */
    for (retry = 0; retry < max_retries; retry++)
    {
        /* Wait for sensor task to acquire the i2c bus. */
        if (xSemaphoreTake(i2c_mutex, portMAX_DELAY) == pdTRUE)
        {
            result = pasco2_sensor_init();

            xSemaphoreGive(i2c_mutex);
        }
        
        if (result == CY_RSLT_SUCCESS)
        {
            break;
        }
        LOG_INFO(CYLF_DEF, "CO2 sensor init failed, retrying... (%d/%d)\r\n", retry + 1, max_retries);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    if (result != CY_RSLT_SUCCESS)
    {
        LOG_ERROR(CYLF_DEF, "CO2 sensor initialization failed after retries!\r\n");
        return result;
    }
#if 0
    /* Initialize SHT40 sensor with retry */
    for (retry = 0; retry < max_retries; retry++)
    {
        result = sht_sensor_init();
        if (result == CY_RSLT_SUCCESS)
        {
            break;
        }
        LOG_INFO(CYLF_DEF, "SHT40 sensor init failed, retrying... (%d/%d)\r\n", retry + 1, max_retries);
        vTaskDelay(pdMS_TO_TICKS(1000)); // small delay before retry
    }

    if (result != CY_RSLT_SUCCESS)
    {
        LOG_ERROR(CYLF_DEF, "SHT40 sensor initialization failed after retries!\r\n");
        return result;
    }
#endif
    LOG_INFO(CYLF_DEF, "All sensors initialized successfully.\r\n");
    return CY_RSLT_SUCCESS;
}

void sensor_task(void *arg)
{
    CY_UNUSED_PARAMETER(arg);
    cy_rslt_t result;

    LOG_INFO(CYLF_DEF, "Sensor task running.\n");

    /* Power pasco2 sensor */
    power_co2_sensor();
    
    /* Initialize all sensors */
    result = sensor_init();
    if (result != CY_RSLT_SUCCESS)
    {
        LOG_INFO(CYLF_DEF, "Sensor initialization failed! Halting task.\r\n");

        /* Start timer to recreate this task after 5 sec */
        // if (restart_sensor_timer != NULL)
        // {
        //     Cy_GPIO_Write(CYBSP_CO2_5V_EN_PORT, CYBSP_CO2_5V_EN_PIN, 0u);
        //     vTaskDelay(pdMS_TO_TICKS(2000));
        //     Cy_GPIO_Write(CYBSP_CO2_5V_EN_PORT, CYBSP_CO2_5V_EN_PIN, 1u);
        //     vTaskDelay(pdMS_TO_TICKS(2000));
        //     xTimerStart(restart_sensor_timer, 0);
        // }

        // vTaskSuspend(NULL);
        //CY_ASSERT(0);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    /* Initial delay to allow other tasks to start */
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    /* Wait for sensor task to acquire the i2c bus. */
    if (xSemaphoreTake(i2c_mutex, portMAX_DELAY) == pdTRUE)
    {
	    /* Configure sensor in single shot mode (Idle Mode -> Single shot mode) */
	    xensiv_pasco2_start_single_mode(&xensiv_pasco2);
	    
        // Release the mutex, allowing other tasks to use the bus.
        xSemaphoreGive(i2c_mutex);
    }

    for (;;)
    {
        if(0 == ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(sensor_sampling_intv)))
        {
			/* Read sensor only at sensor_sampling_intv period. Ignore when task notify update done for interval update during screen change */
#if 0
	        /* Wait for sensor task to acquire the i2c bus. */
	        if (xSemaphoreTake(i2c_mutex, portMAX_DELAY) == pdTRUE)
	        {
	            /* Read temperature and humidity from SHT40 */
	            result = mtb_sht4x_measure_low_precision(&CYBSP_I2C_CONTROLLER_2_hal_obj, &read_temperature,
	                    &read_humidity);
	            if (result == CY_RSLT_SUCCESS)
	            {
	                //            printf("Humidity    : %ld milli RH\r\n", (long)humidity);
	                //            printf("Temperature : %ld milli degC\r\n", (long)temperature);
	            }
	            else
	            {
	                LOG_ERROR(CYLF_DEF, "SHT40 measurement failed. Resetting sensor...\r\n");
	                sht4x_soft_reset();
	            }
	
	            // Release the mutex, allowing other tasks to use the bus.
	            xSemaphoreGive(i2c_mutex);
	            vTaskDelay(pdMS_TO_TICKS(70));
	        }
#endif
	        /* Wait for sensor task to acquire the i2c bus. */
	        if (xSemaphoreTake(i2c_mutex, portMAX_DELAY) == pdTRUE)
	        {
	            /* Read CO2 data */
	            result = xensiv_pasco2_mtb_read(&xensiv_pasco2, DEFAULT_PRESSURE_REF_HPA, &read_ppm);
	            if (result == CY_RSLT_SUCCESS)
	            {
	                //            printf("CO2 concentration: %d ppm\r\n", ppm);
	                sensor_data_available = true;
	            }
	            else if (result == XENSIV_PASCO2_RSLT_READ_NRDY)
	            {
	                LOG_ERROR(CYLF_DEF,  "CO2 sensor data not ready.\n");
	            }
	            else
	            {
	                LOG_ERROR(CYLF_DEF, "Error reading CO2 sensor data\n");
	            }
	
	            /* Configure sensor in single shot mode (Idle Mode -> Single shot mode) */
	            xensiv_pasco2_start_single_mode(&xensiv_pasco2);
	
	            // Release the mutex, allowing other tasks to use the bus.
	            xSemaphoreGive(i2c_mutex);
	        }
		}
    }
}

void set_sensor_sampling_interval(uint32_t interval_ms)
{
    sensor_sampling_intv = interval_ms;
    xTaskNotifyGive(rtos_cm55_sensor_task_handle);
}

void app_sensor_task_init(void)
{
    BaseType_t task_return = pdFAIL;

    /* Start sensor task */
    task_return = xTaskCreate(sensor_task, SENSOR_TASK_NAME,
                                           SENSOR_TASK_STACK_SIZE, NULL,
                                           SENSOR_TASK_PRIORITY,
                                           &rtos_cm55_sensor_task_handle);

    if(task_return == pdFAIL)
    {
        LOG_ERROR(CYLF_DEF, "App. sensor task create failed.\n");
        CY_ASSERT(0);
    }
    else
    {
        LOG_INFO(CYLF_DEF, "App. sensor task create Ok.\n");
    }
}
