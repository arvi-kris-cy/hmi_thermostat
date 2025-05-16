/*******************************************************************************
* \file mtb_ctp_p4100tp.h
*
* \brief
* Provides constants, parameter values, and API prototypes for the P4100TP
* touch driver library.
*
********************************************************************************
* \copyright
* Copyright 2024, Cypress Semiconductor Corporation (an Infineon company)
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

#ifndef MTB_CTP_P4100TP_H
#define MTB_CTP_P4100TP_H

#define COMPONENT_MTB_CTP_P4100TP

#if defined(COMPONENT_MTB_CTP_P4100TP)

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "cy_result.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"


/*******************************************************************************
* Macros
*******************************************************************************/

/*!
 * @addtogroup middlewaregroup Middleware
 * @brief This section includes the mikroSDK API Reference for Middleware Layer.
 * @{
 */

/*!
 * @addtogroup p4100tp P4100TP Touch Controllers Driver
 * @brief P4100TP Touch Controller Driver API Reference.
 * @{
 */

/**
 * @defgroup p4100tp_registers P4100TP Registers
 * @brief P4100TP Registers List.
 * @details P4100TP registers description with respective addresses.
 */

/**
 * @addtogroup p4100tp_registers
 * @{
 */

/**
 * @brief P4100TP Slave Address Setting.
 * @details Slave address setting for P4100TP series touch controllers.
 */
#define P4100TP_I2C_ADDR                                    0x08


/* Error code definition */
#define CY_RSLT_P4100TP_ERR_BASE                          (0x0253)

/* No touch detected by the driver */
#define CY_RSLT_P4100TP_NO_TOUCH                           \
                CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_P4100TP_ERR_BASE, 1)

/* P4100TP device error */
#define CY_RSLT_P4100TP_DEV_ERR                            \
               CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_P4100TP_ERR_BASE, 2)

#define MTB_CTP_TOUCH_RESOLUTION_X                        (480U)
#define MTB_CTP_TOUCH_RESOLUTION_Y                        (480U)

#define ORIENTATION                                         0       // 0� also supported 90�, 180�, 270�

/* CTP IRQ priority */
#define MTB_CTP_P4100TP_IRQ_PRIORITY                      (3U)


/*******************************************************************************
* Data Structures
*******************************************************************************/
/* P4100TP touch controller configuration structure */
typedef struct
{
    CySCB_Type* scb_inst;
    cy_stc_scb_i2c_context_t* i2c_context;
    GPIO_PRT_Type* rst_port;
    uint32_t rst_pin;
    GPIO_PRT_Type* irq_port;
    uint32_t irq_pin;
    IRQn_Type irq_num;
    volatile bool touch_event;
} mtb_ctp_p4100tp_config_t;


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void mtb_ctp_p4100tp_irq_handler(void);
cy_rslt_t mtb_ctp_p4100tp_init(mtb_ctp_p4100tp_config_t* ctp_p4100tp_config);
cy_rslt_t mtb_ctp_p4100tp_get_single_touch(int* touch_x, int* touch_y);
void mtb_ctp_p4100tp_deinit(void);


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* COMPONENT_MTB_CTP_P4100TP */

#endif /* MTB_CTP_P4100TP_H */

/* [] END OF FILE */
