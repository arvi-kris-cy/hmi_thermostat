/***************************************************************************//**
* \file cybsp_external_rtc.c
*
* \brief
* The device common system-source file.
*
********************************************************************************
* \copyright
* (c) (2016-2025), Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
*
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
#include "cybsp.h"
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(CYBSP_POST_CFG_INIT)

/* External RTC I2C address */
#define EXT_RTC_ADDRESS            (0x6F)

/*****************************************************************************
* Function Name: cybsp_post_cfg_init
******************************************************************************
* Summary:
* Initializes and configures the external RTC over I2C and enables the WCO as
* the backup domain clock source.
*
* Parameters:
* None
*
* Return:
* cy_rslt_t
*****************************************************************************/
cy_rslt_t cybsp_post_cfg_init(void)
{
    cy_en_sysclk_status_t wco_status= CY_SYSCLK_SUCCESS;

    #if !defined(COMPONENT_SECURE_DEVICE) && defined(COMPONENT_NON_SECURE_DEVICE)
    cy_en_scb_i2c_status_t status = CY_SCB_I2C_SUCCESS;
    cy_stc_scb_i2c_context_t CYBSP_I2C_CONTROLLER_context;

    /* Initialize the WCO input pin */
    (void)Cy_GPIO_Pin_FastInit(GPIO_PRT18, 1U, 0x00U, 0x00U, HSIOM_SEL_GPIO);

    /* Initialize the I2C master interface for external RTC */
    status = Cy_SCB_I2C_Init(CYBSP_I2C_CONTROLLER_HW, &CYBSP_I2C_CONTROLLER_config, &CYBSP_I2C_CONTROLLER_context);
    if(CY_SCB_I2C_SUCCESS != status)
    {
        return (cy_rslt_t)status;
    }

    /* Enable I2C */
    Cy_SCB_I2C_Enable(CYBSP_I2C_CONTROLLER_HW);

    /* External RTC memory addresses and data */
    const uint8_t mem_addr[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    const uint8_t data[8] = {0x80, 0x25, 0x49, 0x2B, 0x01, 0x07, 0x025, 0xC3};
    for (int i = 0; i < 8; i++)
    {
        status = Cy_SCB_I2C_MasterSendStart(CYBSP_I2C_CONTROLLER_HW, EXT_RTC_ADDRESS, CY_SCB_I2C_WRITE_XFER, 0, &CYBSP_I2C_CONTROLLER_context);
        if (CY_SCB_I2C_SUCCESS == status)
        {
            status = Cy_SCB_I2C_MasterWriteByte(CYBSP_I2C_CONTROLLER_HW, mem_addr[i], 0, &CYBSP_I2C_CONTROLLER_context);
            if (CY_SCB_I2C_SUCCESS == status)
            {
                status = Cy_SCB_I2C_MasterWriteByte(CYBSP_I2C_CONTROLLER_HW, data[i], 0, &CYBSP_I2C_CONTROLLER_context);
                if(CY_SCB_I2C_SUCCESS != status)
                {
                    break;
                }
            }
            Cy_SCB_I2C_MasterSendStop(CYBSP_I2C_CONTROLLER_HW, 0, &CYBSP_I2C_CONTROLLER_context);
           }
    }

    /* De-initialize I2C */
    Cy_SCB_I2C_DeInit(CYBSP_I2C_CONTROLLER_HW);

       /* Early return on I2C failure */
    if (status != CY_SCB_I2C_SUCCESS)
    {
        return (cy_rslt_t)status;
    }

    /* Configure and enable WCO */
    Cy_SysClk_WcoBypass(CY_SYSCLK_WCO_BYPASSED);
    wco_status = Cy_SysClk_WcoEnable(1000000UL);

    /* Set WCO as the source for the backup domain clock */
    Cy_SysClk_ClkBakSetSource(CY_SYSCLK_BAK_IN_WCO);
    #endif

    return (cy_rslt_t)wco_status;
}

#endif

#if defined(__cplusplus)
}
#endif