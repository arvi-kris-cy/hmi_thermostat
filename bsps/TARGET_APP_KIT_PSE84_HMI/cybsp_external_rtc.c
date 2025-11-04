/***************************************************************************//**
* \file cybsp_external_rtc.c
*
* \brief
* The device common system-source file.
*
********************************************************************************
* \copyright
* (c) (2025), Cypress Semiconductor Corporation (an Infineon company) or
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

#define EXT_RTC_ADDRESS            (0x6F)
#define CONTROL_REGISTER_DATA      (0xC3)
#define SEC_REGISTER_DATA          (0x80)
#define I2C_TIMEOUT_MS             (0u)
#define OSC_TIMEOUT_MS             (1u)
#define WCO_ENABLE_TIMEOUT_MS      (1000000UL)

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
    cy_en_sysclk_status_t wco_status = CY_SYSCLK_SUCCESS;

    #if !defined(COMPONENT_SECURE_DEVICE) && defined(CY_SYSTEM_CPU_M33)
    cy_en_scb_i2c_status_t i2c_status = CY_SCB_I2C_SUCCESS;
    cy_stc_scb_i2c_context_t CYBSP_I2C_CONTROLLER_context;

    /* External RTC memory addresses and data */
    const uint8_t mem_addr[2] = {0x00, 0x07};
    uint8_t data;

    /* Initialize the I2C master interface for external RTC */
    i2c_status = Cy_SCB_I2C_Init(CYBSP_I2C_CONTROLLER_HW, &CYBSP_I2C_CONTROLLER_config, &CYBSP_I2C_CONTROLLER_context);
    if(CY_SCB_I2C_SUCCESS != i2c_status)
    {
        return (cy_rslt_t)i2c_status;
    }
    /* Enable I2C */
    Cy_SCB_I2C_Enable(CYBSP_I2C_CONTROLLER_HW);

    /* Write the RTC control register to provide the 32.768 kHz frequency on MFP pin */
    i2c_status = Cy_SCB_I2C_MasterSendStart(CYBSP_I2C_CONTROLLER_HW, EXT_RTC_ADDRESS, CY_SCB_I2C_WRITE_XFER, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
    if (CY_SCB_I2C_SUCCESS == i2c_status)
    {
        i2c_status = Cy_SCB_I2C_MasterWriteByte(CYBSP_I2C_CONTROLLER_HW, mem_addr[1], I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
        if (CY_SCB_I2C_SUCCESS == i2c_status)
        {
            i2c_status = Cy_SCB_I2C_MasterWriteByte(CYBSP_I2C_CONTROLLER_HW, CONTROL_REGISTER_DATA, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
            if(CY_SCB_I2C_SUCCESS != i2c_status)
            {
                /* De-initialize I2C */
                Cy_SCB_I2C_DeInit(CYBSP_I2C_CONTROLLER_HW);
                return (cy_rslt_t)i2c_status;
            }
        }
    }
    Cy_SCB_I2C_MasterSendStop(CYBSP_I2C_CONTROLLER_HW, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);

    /* Write the RTCSEC register to set the ST bit to enable the oscillator */
    i2c_status = Cy_SCB_I2C_MasterSendStart(CYBSP_I2C_CONTROLLER_HW, EXT_RTC_ADDRESS, CY_SCB_I2C_WRITE_XFER, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
    if (CY_SCB_I2C_SUCCESS == i2c_status)
    {
        i2c_status = Cy_SCB_I2C_MasterWriteByte(CYBSP_I2C_CONTROLLER_HW, mem_addr[0], I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
        if (CY_SCB_I2C_SUCCESS == i2c_status)
        {
            i2c_status = Cy_SCB_I2C_MasterWriteByte(CYBSP_I2C_CONTROLLER_HW, SEC_REGISTER_DATA, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
            if(CY_SCB_I2C_SUCCESS != i2c_status)
            {
                /* De-initialize I2C */
                Cy_SCB_I2C_DeInit(CYBSP_I2C_CONTROLLER_HW);
                return (cy_rslt_t)i2c_status;
            }
        }
    }
    Cy_SCB_I2C_MasterSendStop(CYBSP_I2C_CONTROLLER_HW, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);

    /* Poll until the oscillator starts */
    while (true)
    {
        i2c_status = Cy_SCB_I2C_MasterSendStart(CYBSP_I2C_CONTROLLER_HW, EXT_RTC_ADDRESS, CY_SCB_I2C_WRITE_XFER, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
        if (CY_SCB_I2C_SUCCESS == i2c_status)
        {
            i2c_status = Cy_SCB_I2C_MasterWriteByte(CYBSP_I2C_CONTROLLER_HW, mem_addr[0], I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
            if (CY_SCB_I2C_SUCCESS == i2c_status)
            {
                i2c_status = Cy_SCB_I2C_MasterSendReStart(CYBSP_I2C_CONTROLLER_HW, EXT_RTC_ADDRESS, CY_SCB_I2C_READ_XFER, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
                if (CY_SCB_I2C_SUCCESS == i2c_status)
                {
                    i2c_status = Cy_SCB_I2C_MasterReadByte(CYBSP_I2C_CONTROLLER_HW, CY_SCB_I2C_ACK, &data, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);
                    if(CY_SCB_I2C_SUCCESS != i2c_status)
                    {
                        /* De-initialize I2C */
                        Cy_SCB_I2C_DeInit(CYBSP_I2C_CONTROLLER_HW);
                        return (cy_rslt_t)i2c_status;
                    }
                }
            }
        }
        Cy_SCB_I2C_MasterSendStop(CYBSP_I2C_CONTROLLER_HW, I2C_TIMEOUT_MS, &CYBSP_I2C_CONTROLLER_context);

        /* Check if the ST bit is set and break the loop */
        if((data & SEC_REGISTER_DATA) == SEC_REGISTER_DATA)
        {
            break;
        }
        Cy_SysLib_Delay(OSC_TIMEOUT_MS);
    }

    /* De-initialize I2C */
    Cy_SCB_I2C_DeInit(CYBSP_I2C_CONTROLLER_HW);

    /* Initialize the WCO pins */
    Cy_GPIO_Pin_FastInit(GPIO_PRT18, 1U, 0x00U, 0x00U, HSIOM_SEL_GPIO);
    Cy_GPIO_Pin_FastInit(GPIO_PRT18, 0U, 0x00U, 0x00U, HSIOM_SEL_GPIO);

    /* Poll until the WCO enables */
    wco_status = CY_SYSCLK_INVALID_STATE;
    while (wco_status != CY_SYSCLK_SUCCESS)
    {
        /* Configure and enable WCO */
        Cy_SysClk_WcoBypass(CY_SYSCLK_WCO_BYPASSED);
        wco_status = Cy_SysClk_WcoEnable(WCO_ENABLE_TIMEOUT_MS);
        if (wco_status == CY_SYSCLK_SUCCESS)
        {
            /* Set WCO as the source for the backup domain clock */
            Cy_SysClk_ClkBakSetSource(CY_SYSCLK_BAK_IN_WCO);
        }
    }
    #endif

    return (cy_rslt_t)wco_status;
}

#endif

#if defined(__cplusplus)
}
#endif