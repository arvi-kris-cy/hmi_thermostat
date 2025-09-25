/******************************************************************************
 * \file mtb_sht4x.c
 *
 * \brief
 *     This file contains the functions for interacting with the
 *     SHT4x humidity sensor.
 *
 ********************************************************************************
 * \copyright
 * Copyright 2025 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/


#include "mtb_sht4x.h"
#include "sensirion_i2c_hal.h"
#include "sensirion_common.h"
#include "mtb_hal.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/******************************************************************************
* Macros
******************************************************************************/
#define SHT4X_DELAY (100)

/******************************************************************************
* Global variables
******************************************************************************/
mtb_hal_i2c_t* sht4x_i2c = NULL;

/******************************************************************************
* mtb_sht4x_init
******************************************************************************/
cy_rslt_t mtb_sht4x_init(mtb_hal_i2c_t* i2c_instance, mtb_sht40_address_t i2c_address)
{
    cy_rslt_t result;

    CY_ASSERT(i2c_instance != NULL);
    sht4x_init(i2c_address);

    sht4x_i2c = i2c_instance;

    sensirion_i2c_hal_sleep_usec(SHT4X_DELAY);
    result = mtb_sht4x_soft_reset(sht4x_i2c);
    sensirion_i2c_hal_sleep_usec(SHT4X_DELAY);

    return result;
}


/******************************************************************************
* mtb_sht4x_measure_high_precision
******************************************************************************/
cy_rslt_t mtb_sht4x_measure_high_precision(mtb_hal_i2c_t* i2c_instance, int32_t* temperature, int32_t* humidity)
{
    CY_ASSERT(i2c_instance != NULL);
    sht4x_i2c = i2c_instance;

    return sht4x_measure_high_precision(temperature, humidity);
}


/******************************************************************************
* mtb_sht4x_measure_medium_precision
******************************************************************************/
cy_rslt_t mtb_sht4x_measure_medium_precision(mtb_hal_i2c_t* i2c_instance, int32_t* temperature, int32_t* humidity)
{
    CY_ASSERT(i2c_instance != NULL);
    sht4x_i2c = i2c_instance;

    return sht4x_measure_medium_precision(temperature, humidity);
}


/******************************************************************************
* mtb_sht4x_measure_lowest_precision
******************************************************************************/
cy_rslt_t mtb_sht4x_measure_low_precision(mtb_hal_i2c_t* i2c_instance, int32_t* temperature, int32_t* humidity)
{
    CY_ASSERT(i2c_instance != NULL);
    sht4x_i2c = i2c_instance;

    return sht4x_measure_lowest_precision(temperature, humidity);
}


/******************************************************************************
* mtb_sht4x_soft_reset
******************************************************************************/
cy_rslt_t mtb_sht4x_soft_reset(mtb_hal_i2c_t* i2c_instance)
{
    CY_ASSERT(i2c_instance != NULL);
    sht4x_i2c = i2c_instance;
    return sht4x_soft_reset();
}


/******************************************************************************
* mtb_sht4x_free
******************************************************************************/
void mtb_sht4x_free(mtb_hal_i2c_t* i2c_instance)
{
    CY_ASSERT(i2c_instance != NULL);
    sht4x_i2c = i2c_instance;
    sensirion_i2c_hal_free();
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */

