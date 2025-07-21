/*******************************************************************************
 * File Name: cycfg_clocks.c
 *
 * Description:
 * Clock configuration
 * This file was automatically generated and should not be modified.
 * Configurator Backend 3.50.0
 * device-db 4.5.30.8214
 * mtb-device-support-pse8xxgp 1.0.200.195
 *
 *******************************************************************************
 * Copyright 2025 Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.
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
 ******************************************************************************/

#include "cycfg_clocks.h"

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_I2C_CONTROLLER_0_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_I2C_CONTROLLER_0_CLK_DIV_HW,
    .channel_num = CYBSP_I2C_CONTROLLER_0_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_DEBUG_UART_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_DEBUG_UART_CLK_DIV_HW,
    .channel_num = CYBSP_DEBUG_UART_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_TCPWM_0_GRP_0_COUNTER_0_CLK_DIV_HW,
    .channel_num = CYBSP_TCPWM_0_GRP_0_COUNTER_0_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_TCPWM_0_GRP_1_PWM_5_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_TCPWM_0_GRP_1_PWM_5_CLK_DIV_HW,
    .channel_num = CYBSP_TCPWM_0_GRP_1_PWM_5_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_BT_UART_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_BT_UART_CLK_DIV_HW,
    .channel_num = CYBSP_BT_UART_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_TCPWM_0_GRP_1_PWM_6_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_TCPWM_0_GRP_1_PWM_6_CLK_DIV_HW,
    .channel_num = CYBSP_TCPWM_0_GRP_1_PWM_6_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_RS_SPI_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_RS_SPI_CLK_DIV_HW,
    .channel_num = CYBSP_RS_SPI_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_SPI_CONTROLLER_10_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_SPI_CONTROLLER_10_CLK_DIV_HW,
    .channel_num = CYBSP_SPI_CONTROLLER_10_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_USB_OS_TIMER_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_USB_OS_TIMER_CLK_DIV_HW,
    .channel_num = CYBSP_USB_OS_TIMER_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_I2C_3V3_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_I2C_3V3_CLK_DIV_HW,
    .channel_num = CYBSP_I2C_3V3_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_TRACE_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_TRACE_CLK_DIV_HW,
    .channel_num = CYBSP_TRACE_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_TDM_CONTROLLER_0_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_TDM_CONTROLLER_0_CLK_DIV_HW,
    .channel_num = CYBSP_TDM_CONTROLLER_0_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_PDM_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_PDM_CLK_DIV_HW,
    .channel_num = CYBSP_PDM_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_SDHC_0_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_SDHC_0_CLK_DIV_HW,
    .channel_num = CYBSP_SDHC_0_CLK_DIV_NUM,
};
const cyhal_resource_inst_t CYBSP_SDHC_1_CLK_DIV_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CYBSP_SDHC_1_CLK_DIV_HW,
    .channel_num = CYBSP_SDHC_1_CLK_DIV_NUM,
};
#endif /* defined (CY_USING_HAL) */

void init_cycfg_clocks(void)
{
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_I2C_CONTROLLER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_I2C_CONTROLLER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 0U, 9U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_I2C_CONTROLLER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_DEBUG_UART_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 1U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_DEBUG_UART_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 1U, 86U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_DEBUG_UART_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 1U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_0_COUNTER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 2U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_0_COUNTER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 2U, 9999U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_0_COUNTER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 2U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_1_PWM_5_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 3U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_1_PWM_5_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 3U, 9U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_1_PWM_5_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_BIT, 3U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_BT_UART_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U);
    Cy_SysClk_PeriPclkSetFracDivider((en_clk_dst_t)CYBSP_BT_UART_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U, 107U, 16U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_BT_UART_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_1_PWM_6_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 1U);
    Cy_SysClk_PeriPclkSetFracDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_1_PWM_6_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 1U, 4999U, 0U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_TCPWM_0_GRP_1_PWM_6_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 1U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_RS_SPI_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_RS_SPI_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U, 5U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_RS_SPI_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_SPI_CONTROLLER_10_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 1U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_SPI_CONTROLLER_10_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 1U, 5U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_SPI_CONTROLLER_10_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 1U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_USB_OS_TIMER_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 2U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_USB_OS_TIMER_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 2U, 0U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_USB_OS_TIMER_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 2U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_I2C_3V3_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_I2C_3V3_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 4U, 9U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_I2C_3V3_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_TRACE_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U);
    Cy_SysClk_PeriPclkSetFracDivider((en_clk_dst_t)CYBSP_TRACE_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U, 0U, 0U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_TRACE_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U);
    Cy_SysClk_PeriGroupSetDivider((0 << 8) | 2, 1U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_TDM_CONTROLLER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U);
    Cy_SysClk_PeriPclkSetFracDivider((en_clk_dst_t)CYBSP_TDM_CONTROLLER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U, 23U, 0U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_TDM_CONTROLLER_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 0U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_PDM_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 1U);
    Cy_SysClk_PeriPclkSetFracDivider((en_clk_dst_t)CYBSP_PDM_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 1U, 3U, 0U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_PDM_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_16_5_BIT, 1U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_SDHC_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_SDHC_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U, 1U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_SDHC_0_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)CYBSP_SDHC_1_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)CYBSP_SDHC_1_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U, 1U);
    Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)CYBSP_SDHC_1_CLK_DIV_GRP_NUM, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriGroupSetDivider((1 << 8) | 1, 1U);
    Cy_SysClk_PeriGroupSetDivider((1 << 8) | 3, 3U);
}
void reserve_cycfg_clocks(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&CYBSP_I2C_CONTROLLER_0_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_DEBUG_UART_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_TCPWM_0_GRP_0_COUNTER_0_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_TCPWM_0_GRP_1_PWM_5_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_UART_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_TCPWM_0_GRP_1_PWM_6_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_RS_SPI_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_SPI_CONTROLLER_10_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_USB_OS_TIMER_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2C_3V3_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_TRACE_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_TDM_CONTROLLER_0_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_PDM_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_0_CLK_DIV_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_1_CLK_DIV_obj);
#endif /* defined (CY_USING_HAL) */
}
