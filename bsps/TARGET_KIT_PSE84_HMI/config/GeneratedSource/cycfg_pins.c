<<<<<<< HEAD
/*******************************************************************************
 * File Name: cycfg_pins.c
 *
 * Description:
 * Pin configuration
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

#include "cycfg_pins.h"

const cy_stc_gpio_pin_config_t CYBSP_RS_SPI_SS_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_RS_SPI_SS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_RS_SPI_SS_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_RS_SPI_SS_PORT_NUM,
    .channel_num = CYBSP_RS_SPI_SS_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_SS_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_OSPI_RAM_SS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_SS_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_SS_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_SS_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_BT_UART_RX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_BT_UART_RX_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_BT_UART_RX_PORT_NUM,
    .channel_num = CYBSP_BT_UART_RX_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_BT_UART_TX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_BT_UART_TX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_BT_UART_TX_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_BT_UART_TX_PORT_NUM,
    .channel_num = CYBSP_BT_UART_TX_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_BT_UART_CTS_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_BT_UART_CTS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_BT_UART_CTS_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_BT_UART_CTS_PORT_NUM,
    .channel_num = CYBSP_BT_UART_CTS_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RTS_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_BT_UART_RTS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_BT_UART_RTS_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_BT_UART_RTS_PORT_NUM,
    .channel_num = CYBSP_BT_UART_RTS_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_BT_HOST_WAKE_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_BT_HOST_WAKE_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_BT_HOST_WAKE_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_BT_HOST_WAKE_PORT_NUM,
    .channel_num = CYBSP_BT_HOST_WAKE_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_INT_CO2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_INT_CO2_HSIOM,
    .intEdge = CY_GPIO_INTR_FALLING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_INT_CO2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_INT_CO2_PORT_NUM,
    .channel_num = CYBSP_INT_CO2_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_BT_DEVICE_WAKE_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_BT_DEVICE_WAKE_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_BT_DEVICE_WAKE_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_BT_DEVICE_WAKE_PORT_NUM,
    .channel_num = CYBSP_BT_DEVICE_WAKE_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_DISP_TP_INT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_DISP_TP_INT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_DISP_TP_INT_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_DISP_TP_INT_PORT_NUM,
    .channel_num = CYBSP_DISP_TP_INT_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WIFI_SDIO_CLK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WIFI_SDIO_CLK_PORT_NUM,
    .channel_num = CYBSP_WIFI_SDIO_CLK_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_D0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WIFI_SDIO_D0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WIFI_SDIO_D0_PORT_NUM,
    .channel_num = CYBSP_WIFI_SDIO_D0_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_D1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WIFI_SDIO_D1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WIFI_SDIO_D1_PORT_NUM,
    .channel_num = CYBSP_WIFI_SDIO_D1_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_I2S_TX_FSYNC_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_I2S_TX_FSYNC_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2S_TX_FSYNC_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_I2S_TX_FSYNC_PORT_NUM,
    .channel_num = CYBSP_I2S_TX_FSYNC_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_D2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WIFI_SDIO_D2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WIFI_SDIO_D2_PORT_NUM,
    .channel_num = CYBSP_WIFI_SDIO_D2_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_D3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WIFI_SDIO_D3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WIFI_SDIO_D3_PORT_NUM,
    .channel_num = CYBSP_WIFI_SDIO_D3_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WIFI_HOST_WAKE_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_WIFI_HOST_WAKE_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WIFI_HOST_WAKE_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WIFI_HOST_WAKE_PORT_NUM,
    .channel_num = CYBSP_WIFI_HOST_WAKE_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SDHC_DETECT_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_SDHC_DETECT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SDHC_DETECT_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SDHC_DETECT_PORT_NUM,
    .channel_num = CYBSP_SDHC_DETECT_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_RS_RST_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_RS_RST_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_RS_RST_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_RS_RST_PORT_NUM,
    .channel_num = CYBSP_RS_RST_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_AMIC_1_INP_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_1_INP_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_AMIC_1_INP_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_AMIC_1_INP_PORT_NUM,
    .channel_num = CYBSP_AMIC_1_INP_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_AMIC_1_INN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_1_INN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_AMIC_1_INN_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_AMIC_1_INN_PORT_NUM,
    .channel_num = CYBSP_AMIC_1_INN_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_AMIC_1_OUT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_1_OUT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_AMIC_1_OUT_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_AMIC_1_OUT_PORT_NUM,
    .channel_num = CYBSP_AMIC_1_OUT_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_AMIC_2_OUT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_2_OUT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_AMIC_2_OUT_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_AMIC_2_OUT_PORT_NUM,
    .channel_num = CYBSP_AMIC_2_OUT_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_AMIC_2_INP_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_2_INP_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_AMIC_2_INP_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_AMIC_2_INP_PORT_NUM,
    .channel_num = CYBSP_AMIC_2_INP_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_AMIC_2_INN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_2_INN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_AMIC_2_INN_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_AMIC_2_INN_PORT_NUM,
    .channel_num = CYBSP_AMIC_2_INN_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_RS_INT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_RS_INT_HSIOM,
    .intEdge = CY_GPIO_INTR_FALLING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_RS_INT_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_RS_INT_PORT_NUM,
    .channel_num = CYBSP_RS_INT_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SPI_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_SPI_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SPI_CLK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SPI_CLK_PORT_NUM,
    .channel_num = CYBSP_SPI_CLK_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SPI_MOSI_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_SPI_MOSI_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SPI_MOSI_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SPI_MOSI_PORT_NUM,
    .channel_num = CYBSP_SPI_MOSI_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SPI_MISO_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_SPI_MISO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SPI_MISO_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SPI_MISO_PORT_NUM,
    .channel_num = CYBSP_SPI_MISO_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SPI_SS0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_SPI_SS0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SPI_SS0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SPI_SS0_PORT_NUM,
    .channel_num = CYBSP_SPI_SS0_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SPI_SS1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_SPI_SS1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SPI_SS1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SPI_SS1_PORT_NUM,
    .channel_num = CYBSP_SPI_SS1_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_DISP_BACKLIGHT_PWM_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_DISP_BACKLIGHT_PWM_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_DISP_BACKLIGHT_PWM_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_DISP_BACKLIGHT_PWM_PORT_NUM,
    .channel_num = CYBSP_DISP_BACKLIGHT_PWM_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_DISP_RST_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_CFGOUT3_STRONG_PULLUP_HIGHZ,
    .hsiom = CYBSP_DISP_RST_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_DISP_RST_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_DISP_RST_PORT_NUM,
    .channel_num = CYBSP_DISP_RST_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_I2C_SCL_3V3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_I2C_SCL_3V3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2C_SCL_3V3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_I2C_SCL_3V3_PORT_NUM,
    .channel_num = CYBSP_I2C_SCL_3V3_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_I2C_SDA_3V3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_I2C_SDA_3V3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2C_SDA_3V3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_I2C_SDA_3V3_PORT_NUM,
    .channel_num = CYBSP_I2C_SDA_3V3_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_ECO_IN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_ECO_IN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_ECO_IN_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_ECO_IN_PORT_NUM,
    .channel_num = CYBSP_ECO_IN_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_ECO_OUT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_ECO_OUT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_ECO_OUT_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_ECO_OUT_PORT_NUM,
    .channel_num = CYBSP_ECO_OUT_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_D0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_D0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_D0_PORT_NUM,
    .channel_num = CYBSP_OSPI_D0_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_D1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_D1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_D1_PORT_NUM,
    .channel_num = CYBSP_OSPI_D1_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_D2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_D2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_D2_PORT_NUM,
    .channel_num = CYBSP_OSPI_D2_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_D3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_D3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_D3_PORT_NUM,
    .channel_num = CYBSP_OSPI_D3_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_USER_LED1_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_USER_LED1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_USER_LED1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_USER_LED1_PORT_NUM,
    .channel_num = CYBSP_USER_LED1_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_USER_LED2_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_USER_LED2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_USER_LED2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_USER_LED2_PORT_NUM,
    .channel_num = CYBSP_USER_LED2_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_USER_LED3_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_USER_LED3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_USER_LED3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_USER_LED3_PORT_NUM,
    .channel_num = CYBSP_USER_LED3_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_STATUS_LED1_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_STATUS_LED1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_STATUS_LED1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_STATUS_LED1_PORT_NUM,
    .channel_num = CYBSP_STATUS_LED1_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_STATUS_LED2_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_STATUS_LED2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_STATUS_LED2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_STATUS_LED2_PORT_NUM,
    .channel_num = CYBSP_STATUS_LED2_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_CMD_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_CMD_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WIFI_SDIO_CMD_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WIFI_SDIO_CMD_PORT_NUM,
    .channel_num = CYBSP_WIFI_SDIO_CMD_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_I2S_TX_SD_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_I2S_TX_SD_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2S_TX_SD_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_I2S_TX_SD_PORT_NUM,
    .channel_num = CYBSP_I2S_TX_SD_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_I2S_TX_SCK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_I2S_TX_SCK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2S_TX_SCK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_I2S_TX_SCK_PORT_NUM,
    .channel_num = CYBSP_I2S_TX_SCK_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_I2S_TX_MCK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_I2S_TX_MCK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2S_TX_MCK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_I2S_TX_MCK_PORT_NUM,
    .channel_num = CYBSP_I2S_TX_MCK_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_RS_SPI_MISO_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_RS_SPI_MISO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_RS_SPI_MISO_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_RS_SPI_MISO_PORT_NUM,
    .channel_num = CYBSP_RS_SPI_MISO_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_RS_SPI_MOSI_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_RS_SPI_MOSI_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_RS_SPI_MOSI_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_RS_SPI_MOSI_PORT_NUM,
    .channel_num = CYBSP_RS_SPI_MOSI_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_RS_SPI_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_RS_SPI_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_RS_SPI_CLK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_RS_SPI_CLK_PORT_NUM,
    .channel_num = CYBSP_RS_SPI_CLK_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_CO2_5V_EN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_CO2_5V_EN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_CO2_5V_EN_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_CO2_5V_EN_PORT_NUM,
    .channel_num = CYBSP_CO2_5V_EN_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_QSPI_SS_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_QSPI_SS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_QSPI_SS_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_QSPI_SS_PORT_NUM,
    .channel_num = CYBSP_QSPI_SS_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_D0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_D0_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_D0_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_D1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_D1_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_D1_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_D2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_D2_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_D2_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_D3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_D3_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_D3_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D4_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D4_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_D4_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_D4_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_D4_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D5_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D5_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_D5_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_D5_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_D5_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D6_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D6_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_D6_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_D6_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_D6_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D7_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D7_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_OSPI_RAM_D7_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_OSPI_RAM_D7_PORT_NUM,
    .channel_num = CYBSP_OSPI_RAM_D7_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WIFI_WL_REG_ON_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_WIFI_WL_REG_ON_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WIFI_WL_REG_ON_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WIFI_WL_REG_ON_PORT_NUM,
    .channel_num = CYBSP_WIFI_WL_REG_ON_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_RX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_DEBUG_UART_RX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_DEBUG_UART_RX_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_DEBUG_UART_RX_PORT_NUM,
    .channel_num = CYBSP_DEBUG_UART_RX_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_WL_BT_DEV_WAKE_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WL_BT_DEV_WAKE_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_WL_BT_DEV_WAKE_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_WL_BT_DEV_WAKE_PORT_NUM,
    .channel_num = CYBSP_WL_BT_DEV_WAKE_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_TX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_DEBUG_UART_TX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_DEBUG_UART_TX_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_DEBUG_UART_TX_PORT_NUM,
    .channel_num = CYBSP_DEBUG_UART_TX_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SDHC_CMD_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_CMD_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SDHC_CMD_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SDHC_CMD_PORT_NUM,
    .channel_num = CYBSP_SDHC_CMD_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SDHC_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SDHC_CLK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SDHC_CLK_PORT_NUM,
    .channel_num = CYBSP_SDHC_CLK_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_USER_BTN3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_USER_BTN3_HSIOM,
    .intEdge = CY_GPIO_INTR_FALLING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_USER_BTN3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_USER_BTN3_PORT_NUM,
    .channel_num = CYBSP_USER_BTN3_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SDHC_IO0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_IO0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SDHC_IO0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SDHC_IO0_PORT_NUM,
    .channel_num = CYBSP_SDHC_IO0_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_EXT_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_EXT_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_EXT_CLK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_EXT_CLK_PORT_NUM,
    .channel_num = CYBSP_EXT_CLK_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SDHC_IO1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_IO1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SDHC_IO1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SDHC_IO1_PORT_NUM,
    .channel_num = CYBSP_SDHC_IO1_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SDHC_IO2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_IO2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SDHC_IO2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SDHC_IO2_PORT_NUM,
    .channel_num = CYBSP_SDHC_IO2_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_SDHC_IO3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_IO3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SDHC_IO3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SDHC_IO3_PORT_NUM,
    .channel_num = CYBSP_SDHC_IO3_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_I2C_SCL_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_I2C_SCL_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2C_SCL_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_I2C_SCL_PORT_NUM,
    .channel_num = CYBSP_I2C_SCL_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_I2C_SDA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_I2C_SDA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2C_SDA_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_I2C_SDA_PORT_NUM,
    .channel_num = CYBSP_I2C_SDA_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_BT_POWER_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_BT_POWER_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_BT_POWER_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_BT_POWER_PORT_NUM,
    .channel_num = CYBSP_BT_POWER_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_USER_BTN1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_USER_BTN1_HSIOM,
    .intEdge = CY_GPIO_INTR_FALLING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_USER_BTN1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_USER_BTN1_PORT_NUM,
    .channel_num = CYBSP_USER_BTN1_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_INT_IMU_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_INT_IMU_HSIOM,
    .intEdge = CY_GPIO_INTR_FALLING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_INT_IMU_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_INT_IMU_PORT_NUM,
    .channel_num = CYBSP_INT_IMU_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_PDM_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_PDM_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_PDM_CLK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_PDM_CLK_PORT_NUM,
    .channel_num = CYBSP_PDM_CLK_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_PDM_DATA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_PDM_DATA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_PDM_DATA_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_PDM_DATA_PORT_NUM,
    .channel_num = CYBSP_PDM_DATA_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

const cy_stc_gpio_pin_config_t CYBSP_USER_BTN2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_USER_BTN2_HSIOM,
    .intEdge = CY_GPIO_INTR_FALLING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_USER_BTN2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_USER_BTN2_PORT_NUM,
    .channel_num = CYBSP_USER_BTN2_PIN,
};
#endif /* defined (CY_USING_HAL) || (CY_USING_HAL_LITE) */

void init_cycfg_pins(void)
{
    Cy_GPIO_Pin_Init(CYBSP_RS_SPI_SS_PORT, CYBSP_RS_SPI_SS_PIN, &CYBSP_RS_SPI_SS_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_SS_PORT, CYBSP_OSPI_RAM_SS_PIN, &CYBSP_OSPI_RAM_SS_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_UART_RX_PORT, CYBSP_BT_UART_RX_PIN, &CYBSP_BT_UART_RX_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_UART_TX_PORT, CYBSP_BT_UART_TX_PIN, &CYBSP_BT_UART_TX_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_UART_CTS_PORT, CYBSP_BT_UART_CTS_PIN, &CYBSP_BT_UART_CTS_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_UART_RTS_PORT, CYBSP_BT_UART_RTS_PIN, &CYBSP_BT_UART_RTS_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_HOST_WAKE_PORT, CYBSP_BT_HOST_WAKE_PIN, &CYBSP_BT_HOST_WAKE_config);
    Cy_GPIO_Pin_Init(CYBSP_INT_CO2_PORT, CYBSP_INT_CO2_PIN, &CYBSP_INT_CO2_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_DEVICE_WAKE_PORT, CYBSP_BT_DEVICE_WAKE_PIN, &CYBSP_BT_DEVICE_WAKE_config);
    Cy_GPIO_Pin_Init(CYBSP_DISP_TP_INT_PORT, CYBSP_DISP_TP_INT_PIN, &CYBSP_DISP_TP_INT_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_CLK_PORT, CYBSP_WIFI_SDIO_CLK_PIN, &CYBSP_WIFI_SDIO_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_D0_PORT, CYBSP_WIFI_SDIO_D0_PIN, &CYBSP_WIFI_SDIO_D0_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_D1_PORT, CYBSP_WIFI_SDIO_D1_PIN, &CYBSP_WIFI_SDIO_D1_config);
    Cy_GPIO_Pin_Init(CYBSP_I2S_TX_FSYNC_PORT, CYBSP_I2S_TX_FSYNC_PIN, &CYBSP_I2S_TX_FSYNC_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_D2_PORT, CYBSP_WIFI_SDIO_D2_PIN, &CYBSP_WIFI_SDIO_D2_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_D3_PORT, CYBSP_WIFI_SDIO_D3_PIN, &CYBSP_WIFI_SDIO_D3_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_HOST_WAKE_PORT, CYBSP_WIFI_HOST_WAKE_PIN, &CYBSP_WIFI_HOST_WAKE_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_DETECT_PORT, CYBSP_SDHC_DETECT_PIN, &CYBSP_SDHC_DETECT_config);
    Cy_GPIO_Pin_Init(CYBSP_RS_RST_PORT, CYBSP_RS_RST_PIN, &CYBSP_RS_RST_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_1_INP_PORT, CYBSP_AMIC_1_INP_PIN, &CYBSP_AMIC_1_INP_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_1_INN_PORT, CYBSP_AMIC_1_INN_PIN, &CYBSP_AMIC_1_INN_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_1_OUT_PORT, CYBSP_AMIC_1_OUT_PIN, &CYBSP_AMIC_1_OUT_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_2_OUT_PORT, CYBSP_AMIC_2_OUT_PIN, &CYBSP_AMIC_2_OUT_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_2_INP_PORT, CYBSP_AMIC_2_INP_PIN, &CYBSP_AMIC_2_INP_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_2_INN_PORT, CYBSP_AMIC_2_INN_PIN, &CYBSP_AMIC_2_INN_config);
    Cy_GPIO_Pin_Init(CYBSP_RS_INT_PORT, CYBSP_RS_INT_PIN, &CYBSP_RS_INT_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_CLK_PORT, CYBSP_SPI_CLK_PIN, &CYBSP_SPI_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_MOSI_PORT, CYBSP_SPI_MOSI_PIN, &CYBSP_SPI_MOSI_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_MISO_PORT, CYBSP_SPI_MISO_PIN, &CYBSP_SPI_MISO_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_SS0_PORT, CYBSP_SPI_SS0_PIN, &CYBSP_SPI_SS0_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_SS1_PORT, CYBSP_SPI_SS1_PIN, &CYBSP_SPI_SS1_config);
    Cy_GPIO_Pin_Init(CYBSP_DISP_BACKLIGHT_PWM_PORT, CYBSP_DISP_BACKLIGHT_PWM_PIN, &CYBSP_DISP_BACKLIGHT_PWM_config);
    Cy_GPIO_Pin_Init(CYBSP_DISP_RST_PORT, CYBSP_DISP_RST_PIN, &CYBSP_DISP_RST_config);
    Cy_GPIO_Pin_Init(CYBSP_I2C_SCL_3V3_PORT, CYBSP_I2C_SCL_3V3_PIN, &CYBSP_I2C_SCL_3V3_config);
    Cy_GPIO_Pin_Init(CYBSP_I2C_SDA_3V3_PORT, CYBSP_I2C_SDA_3V3_PIN, &CYBSP_I2C_SDA_3V3_config);
    Cy_GPIO_Pin_Init(CYBSP_ECO_IN_PORT, CYBSP_ECO_IN_PIN, &CYBSP_ECO_IN_config);
    Cy_GPIO_Pin_Init(CYBSP_ECO_OUT_PORT, CYBSP_ECO_OUT_PIN, &CYBSP_ECO_OUT_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D0_PORT, CYBSP_OSPI_D0_PIN, &CYBSP_OSPI_D0_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D1_PORT, CYBSP_OSPI_D1_PIN, &CYBSP_OSPI_D1_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D2_PORT, CYBSP_OSPI_D2_PIN, &CYBSP_OSPI_D2_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D3_PORT, CYBSP_OSPI_D3_PIN, &CYBSP_OSPI_D3_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN, &CYBSP_USER_LED1_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, &CYBSP_USER_LED2_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_LED3_PORT, CYBSP_USER_LED3_PIN, &CYBSP_USER_LED3_config);
    Cy_GPIO_Pin_Init(CYBSP_STATUS_LED1_PORT, CYBSP_STATUS_LED1_PIN, &CYBSP_STATUS_LED1_config);
    Cy_GPIO_Pin_Init(CYBSP_STATUS_LED2_PORT, CYBSP_STATUS_LED2_PIN, &CYBSP_STATUS_LED2_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_CMD_PORT, CYBSP_WIFI_SDIO_CMD_PIN, &CYBSP_WIFI_SDIO_CMD_config);
    Cy_GPIO_Pin_Init(CYBSP_I2S_TX_SD_PORT, CYBSP_I2S_TX_SD_PIN, &CYBSP_I2S_TX_SD_config);
    Cy_GPIO_Pin_Init(CYBSP_I2S_TX_SCK_PORT, CYBSP_I2S_TX_SCK_PIN, &CYBSP_I2S_TX_SCK_config);
    Cy_GPIO_Pin_Init(CYBSP_I2S_TX_MCK_PORT, CYBSP_I2S_TX_MCK_PIN, &CYBSP_I2S_TX_MCK_config);
    Cy_GPIO_Pin_Init(CYBSP_RS_SPI_MISO_PORT, CYBSP_RS_SPI_MISO_PIN, &CYBSP_RS_SPI_MISO_config);
    Cy_GPIO_Pin_Init(CYBSP_RS_SPI_MOSI_PORT, CYBSP_RS_SPI_MOSI_PIN, &CYBSP_RS_SPI_MOSI_config);
    Cy_GPIO_Pin_Init(CYBSP_RS_SPI_CLK_PORT, CYBSP_RS_SPI_CLK_PIN, &CYBSP_RS_SPI_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_CO2_5V_EN_PORT, CYBSP_CO2_5V_EN_PIN, &CYBSP_CO2_5V_EN_config);
    Cy_GPIO_Pin_Init(CYBSP_QSPI_SS_PORT, CYBSP_QSPI_SS_PIN, &CYBSP_QSPI_SS_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D0_PORT, CYBSP_OSPI_RAM_D0_PIN, &CYBSP_OSPI_RAM_D0_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D1_PORT, CYBSP_OSPI_RAM_D1_PIN, &CYBSP_OSPI_RAM_D1_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D2_PORT, CYBSP_OSPI_RAM_D2_PIN, &CYBSP_OSPI_RAM_D2_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D3_PORT, CYBSP_OSPI_RAM_D3_PIN, &CYBSP_OSPI_RAM_D3_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D4_PORT, CYBSP_OSPI_RAM_D4_PIN, &CYBSP_OSPI_RAM_D4_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D5_PORT, CYBSP_OSPI_RAM_D5_PIN, &CYBSP_OSPI_RAM_D5_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D6_PORT, CYBSP_OSPI_RAM_D6_PIN, &CYBSP_OSPI_RAM_D6_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D7_PORT, CYBSP_OSPI_RAM_D7_PIN, &CYBSP_OSPI_RAM_D7_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_WL_REG_ON_PORT, CYBSP_WIFI_WL_REG_ON_PIN, &CYBSP_WIFI_WL_REG_ON_config);
    Cy_GPIO_Pin_Init(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_PIN, &CYBSP_DEBUG_UART_RX_config);
    Cy_GPIO_Pin_Init(CYBSP_WL_BT_DEV_WAKE_PORT, CYBSP_WL_BT_DEV_WAKE_PIN, &CYBSP_WL_BT_DEV_WAKE_config);
    Cy_GPIO_Pin_Init(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_PIN, &CYBSP_DEBUG_UART_TX_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_CMD_PORT, CYBSP_SDHC_CMD_PIN, &CYBSP_SDHC_CMD_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_CLK_PORT, CYBSP_SDHC_CLK_PIN, &CYBSP_SDHC_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_BTN3_PORT, CYBSP_USER_BTN3_PIN, &CYBSP_USER_BTN3_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_IO0_PORT, CYBSP_SDHC_IO0_PIN, &CYBSP_SDHC_IO0_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_IO1_PORT, CYBSP_SDHC_IO1_PIN, &CYBSP_SDHC_IO1_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_IO2_PORT, CYBSP_SDHC_IO2_PIN, &CYBSP_SDHC_IO2_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_IO3_PORT, CYBSP_SDHC_IO3_PIN, &CYBSP_SDHC_IO3_config);
    Cy_GPIO_Pin_Init(CYBSP_I2C_SCL_PORT, CYBSP_I2C_SCL_PIN, &CYBSP_I2C_SCL_config);
    Cy_GPIO_Pin_Init(CYBSP_I2C_SDA_PORT, CYBSP_I2C_SDA_PIN, &CYBSP_I2C_SDA_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_POWER_PORT, CYBSP_BT_POWER_PIN, &CYBSP_BT_POWER_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_BTN1_PORT, CYBSP_USER_BTN1_PIN, &CYBSP_USER_BTN1_config);
    Cy_GPIO_Pin_Init(CYBSP_INT_IMU_PORT, CYBSP_INT_IMU_PIN, &CYBSP_INT_IMU_config);
    Cy_GPIO_Pin_Init(CYBSP_PDM_CLK_PORT, CYBSP_PDM_CLK_PIN, &CYBSP_PDM_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_PDM_DATA_PORT, CYBSP_PDM_DATA_PIN, &CYBSP_PDM_DATA_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_BTN2_PORT, CYBSP_USER_BTN2_PIN, &CYBSP_USER_BTN2_config);
}
void reserve_cycfg_pins(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&CYBSP_RS_SPI_SS_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_SS_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_UART_RX_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_UART_TX_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_UART_CTS_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_UART_RTS_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_HOST_WAKE_obj);
    cyhal_hwmgr_reserve(&CYBSP_INT_CO2_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_DEVICE_WAKE_obj);
    cyhal_hwmgr_reserve(&CYBSP_DISP_TP_INT_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_SDIO_CLK_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_SDIO_D0_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_SDIO_D1_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2S_TX_FSYNC_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_SDIO_D2_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_SDIO_D3_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_HOST_WAKE_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_DETECT_obj);
    cyhal_hwmgr_reserve(&CYBSP_RS_RST_obj);
    cyhal_hwmgr_reserve(&CYBSP_AMIC_1_INP_obj);
    cyhal_hwmgr_reserve(&CYBSP_AMIC_1_INN_obj);
    cyhal_hwmgr_reserve(&CYBSP_AMIC_1_OUT_obj);
    cyhal_hwmgr_reserve(&CYBSP_AMIC_2_OUT_obj);
    cyhal_hwmgr_reserve(&CYBSP_AMIC_2_INP_obj);
    cyhal_hwmgr_reserve(&CYBSP_AMIC_2_INN_obj);
    cyhal_hwmgr_reserve(&CYBSP_RS_INT_obj);
    cyhal_hwmgr_reserve(&CYBSP_SPI_CLK_obj);
    cyhal_hwmgr_reserve(&CYBSP_SPI_MOSI_obj);
    cyhal_hwmgr_reserve(&CYBSP_SPI_MISO_obj);
    cyhal_hwmgr_reserve(&CYBSP_SPI_SS0_obj);
    cyhal_hwmgr_reserve(&CYBSP_SPI_SS1_obj);
    cyhal_hwmgr_reserve(&CYBSP_DISP_BACKLIGHT_PWM_obj);
    cyhal_hwmgr_reserve(&CYBSP_DISP_RST_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2C_SCL_3V3_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2C_SDA_3V3_obj);
    cyhal_hwmgr_reserve(&CYBSP_ECO_IN_obj);
    cyhal_hwmgr_reserve(&CYBSP_ECO_OUT_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_D0_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_D1_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_D2_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_D3_obj);
    cyhal_hwmgr_reserve(&CYBSP_USER_LED1_obj);
    cyhal_hwmgr_reserve(&CYBSP_USER_LED2_obj);
    cyhal_hwmgr_reserve(&CYBSP_USER_LED3_obj);
    cyhal_hwmgr_reserve(&CYBSP_STATUS_LED1_obj);
    cyhal_hwmgr_reserve(&CYBSP_STATUS_LED2_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_SDIO_CMD_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2S_TX_SD_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2S_TX_SCK_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2S_TX_MCK_obj);
    cyhal_hwmgr_reserve(&CYBSP_RS_SPI_MISO_obj);
    cyhal_hwmgr_reserve(&CYBSP_RS_SPI_MOSI_obj);
    cyhal_hwmgr_reserve(&CYBSP_RS_SPI_CLK_obj);
    cyhal_hwmgr_reserve(&CYBSP_CO2_5V_EN_obj);
    cyhal_hwmgr_reserve(&CYBSP_QSPI_SS_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_D0_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_D1_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_D2_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_D3_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_D4_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_D5_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_D6_obj);
    cyhal_hwmgr_reserve(&CYBSP_OSPI_RAM_D7_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_WL_REG_ON_obj);
    cyhal_hwmgr_reserve(&CYBSP_DEBUG_UART_RX_obj);
    cyhal_hwmgr_reserve(&CYBSP_WL_BT_DEV_WAKE_obj);
    cyhal_hwmgr_reserve(&CYBSP_DEBUG_UART_TX_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_CMD_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_CLK_obj);
    cyhal_hwmgr_reserve(&CYBSP_USER_BTN3_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_IO0_obj);
    cyhal_hwmgr_reserve(&CYBSP_EXT_CLK_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_IO1_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_IO2_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_IO3_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2C_SCL_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2C_SDA_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_POWER_obj);
    cyhal_hwmgr_reserve(&CYBSP_USER_BTN1_obj);
    cyhal_hwmgr_reserve(&CYBSP_INT_IMU_obj);
    cyhal_hwmgr_reserve(&CYBSP_PDM_CLK_obj);
    cyhal_hwmgr_reserve(&CYBSP_PDM_DATA_obj);
    cyhal_hwmgr_reserve(&CYBSP_USER_BTN2_obj);
#endif /* defined (CY_USING_HAL) */
}
=======
/*******************************************************************************
 * File Name: cycfg_pins.c
 *
 * Description:
 * Pin configuration
 * This file was automatically generated and should not be modified.
 * Configurator Backend 3.60.0
 * device-db 4.29.0.9102
 * mtb-dsl-pse8xxgp 1.0.0.744
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

#include "cycfg_pins.h"

const cy_stc_gpio_pin_config_t CYBSP_RSPI_CS_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_RSPI_CS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_SS_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_OSPI_RAM_SS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_D0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_D1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_D2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_D3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_D4_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D4_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_D5_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D5_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_D6_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D6_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_D7_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_D7_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D4_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D4_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D5_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D5_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D6_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D6_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_RAM_D7_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_OSPI_RAM_D7_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_OSPI_FLASH_SS_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_OSPI_FLASH_SS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WIFI_WL_REG_ON_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_WIFI_WL_REG_ON_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_RX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_DEBUG_UART_RX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WL_BT_DEV_WAKE_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WL_BT_DEV_WAKE_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_TX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_DEBUG_UART_TX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SDHC_CMD_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_CMD_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SDHC_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SDHC_IO0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_IO0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_EXT_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_EXT_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SDHC_IO1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_IO1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SDHC_IO2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_IO2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SDHC_IO3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SDHC_IO3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_I2C_SCL_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_I2C_SCL_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_I2C_SDA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_I2C_SDA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_PDM_CLK2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_PDM_CLK2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SW2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_SW2_HSIOM,
    .intEdge = CY_GPIO_INTR_FALLING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_PDM_DATA2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_PDM_DATA2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t C_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = C_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_PDM_DATA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_PDM_DATA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SW4_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_SW4_HSIOM,
    .intEdge = CY_GPIO_INTR_FALLING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_BT_UART_RX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_BT_UART_TX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_BT_UART_TX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_BT_UART_CTS_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_BT_UART_CTS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RTS_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_BT_UART_RTS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_BT_HOST_WAKE_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_BT_HOST_WAKE_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_CO2_5V_EN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_CO2_5V_EN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_BT_DEVICE_WAKE_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_BT_DEVICE_WAKE_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_D0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_D1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_I2S_TX_FSYNC_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_I2S_TX_FSYNC_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_D2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_D3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WIFI_HOST_WAKE_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_WIFI_HOST_WAKE_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SDHC_DETECT_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_SDHC_DETECT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_BT_POWER_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = CYBSP_BT_POWER_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_AMIC_1_INP_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_1_INP_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_AMIC_1_INN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_1_INN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_AMIC_1_OUT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_1_OUT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_AMIC_2_OUT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_2_OUT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_AMIC_2_INP_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_2_INP_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_AMIC_2_INN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_AMIC_2_INN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SPI_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_SPI_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SPI_MOSI_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_SPI_MOSI_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SPI_MISO_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_SPI_MISO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SPI_SS0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_SPI_SS0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_SPI_SS1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_SPI_SS1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_DISP_BACKLIGHT_PWM_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_DISP_BACKLIGHT_PWM_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_DISP_RST_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_CFGOUT3_STRONG_PULLUP_HIGHZ,
    .hsiom = CYBSP_DISP_RST_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_I2C_SCL_3V3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_I2C_SCL_3V3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_I2C_SDA_3V3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = CYBSP_I2C_SDA_3V3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_WCO_OUT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_WCO_IN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_ECO_IN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_ECO_IN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_ECO_OUT_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_ECO_OUT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_USER_LED1_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_USER_LED1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_USER_LED2_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_USER_LED2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_USER_LED3_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_USER_LED3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_RADAR_RESET_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_RADAR_RESET_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_CMD_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_WIFI_SDIO_CMD_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_I2S_TX_SD_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_I2S_TX_SD_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_I2S_TX_SCK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_I2S_TX_SCK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_I2S_TX_MCK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_I2S_TX_MCK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_RSPI_MISO_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CYBSP_RSPI_MISO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_RSPI_MOSI_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_RSPI_MOSI_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_RSPI_CLK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_RSPI_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
const cy_stc_gpio_pin_config_t CYBSP_RADAR_INT_config =
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_PULLDOWN,
    .hsiom = CYBSP_RADAR_INT_HSIOM,
    .intEdge = CY_GPIO_INTR_RISING,
    .intMask = 1UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};

void init_cycfg_pins(void)
{
    Cy_GPIO_Pin_Init(CYBSP_RSPI_CS_PORT, CYBSP_RSPI_CS_PIN, &CYBSP_RSPI_CS_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_SS_PORT, CYBSP_OSPI_RAM_SS_PIN, &CYBSP_OSPI_RAM_SS_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D0_PORT, CYBSP_OSPI_D0_PIN, &CYBSP_OSPI_D0_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D1_PORT, CYBSP_OSPI_D1_PIN, &CYBSP_OSPI_D1_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D2_PORT, CYBSP_OSPI_D2_PIN, &CYBSP_OSPI_D2_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D3_PORT, CYBSP_OSPI_D3_PIN, &CYBSP_OSPI_D3_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D4_PORT, CYBSP_OSPI_D4_PIN, &CYBSP_OSPI_D4_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D5_PORT, CYBSP_OSPI_D5_PIN, &CYBSP_OSPI_D5_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D6_PORT, CYBSP_OSPI_D6_PIN, &CYBSP_OSPI_D6_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_D7_PORT, CYBSP_OSPI_D7_PIN, &CYBSP_OSPI_D7_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D0_PORT, CYBSP_OSPI_RAM_D0_PIN, &CYBSP_OSPI_RAM_D0_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D1_PORT, CYBSP_OSPI_RAM_D1_PIN, &CYBSP_OSPI_RAM_D1_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D2_PORT, CYBSP_OSPI_RAM_D2_PIN, &CYBSP_OSPI_RAM_D2_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D3_PORT, CYBSP_OSPI_RAM_D3_PIN, &CYBSP_OSPI_RAM_D3_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D4_PORT, CYBSP_OSPI_RAM_D4_PIN, &CYBSP_OSPI_RAM_D4_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D5_PORT, CYBSP_OSPI_RAM_D5_PIN, &CYBSP_OSPI_RAM_D5_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D6_PORT, CYBSP_OSPI_RAM_D6_PIN, &CYBSP_OSPI_RAM_D6_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_RAM_D7_PORT, CYBSP_OSPI_RAM_D7_PIN, &CYBSP_OSPI_RAM_D7_config);
    Cy_GPIO_Pin_Init(CYBSP_OSPI_FLASH_SS_PORT, CYBSP_OSPI_FLASH_SS_PIN, &CYBSP_OSPI_FLASH_SS_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_WL_REG_ON_PORT, CYBSP_WIFI_WL_REG_ON_PIN, &CYBSP_WIFI_WL_REG_ON_config);
    Cy_GPIO_Pin_Init(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_PIN, &CYBSP_DEBUG_UART_RX_config);
    Cy_GPIO_Pin_Init(CYBSP_WL_BT_DEV_WAKE_PORT, CYBSP_WL_BT_DEV_WAKE_PIN, &CYBSP_WL_BT_DEV_WAKE_config);
    Cy_GPIO_Pin_Init(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_PIN, &CYBSP_DEBUG_UART_TX_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_CMD_PORT, CYBSP_SDHC_CMD_PIN, &CYBSP_SDHC_CMD_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_CLK_PORT, CYBSP_SDHC_CLK_PIN, &CYBSP_SDHC_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_IO0_PORT, CYBSP_SDHC_IO0_PIN, &CYBSP_SDHC_IO0_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_IO1_PORT, CYBSP_SDHC_IO1_PIN, &CYBSP_SDHC_IO1_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_IO2_PORT, CYBSP_SDHC_IO2_PIN, &CYBSP_SDHC_IO2_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_IO3_PORT, CYBSP_SDHC_IO3_PIN, &CYBSP_SDHC_IO3_config);
    Cy_GPIO_Pin_Init(CYBSP_I2C_SCL_PORT, CYBSP_I2C_SCL_PIN, &CYBSP_I2C_SCL_config);
    Cy_GPIO_Pin_Init(CYBSP_I2C_SDA_PORT, CYBSP_I2C_SDA_PIN, &CYBSP_I2C_SDA_config);
    Cy_GPIO_Pin_Init(CYBSP_PDM_CLK2_PORT, CYBSP_PDM_CLK2_PIN, &CYBSP_PDM_CLK2_config);
    Cy_GPIO_Pin_Init(CYBSP_SW2_PORT, CYBSP_SW2_PIN, &CYBSP_SW2_config);
    Cy_GPIO_Pin_Init(CYBSP_PDM_DATA2_PORT, CYBSP_PDM_DATA2_PIN, &CYBSP_PDM_DATA2_config);
    Cy_GPIO_Pin_Init(C_PORT, C_PIN, &C_config);
    Cy_GPIO_Pin_Init(CYBSP_PDM_DATA_PORT, CYBSP_PDM_DATA_PIN, &CYBSP_PDM_DATA_config);
    Cy_GPIO_Pin_Init(CYBSP_SW4_PORT, CYBSP_SW4_PIN, &CYBSP_SW4_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_UART_RX_PORT, CYBSP_BT_UART_RX_PIN, &CYBSP_BT_UART_RX_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_UART_TX_PORT, CYBSP_BT_UART_TX_PIN, &CYBSP_BT_UART_TX_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_UART_CTS_PORT, CYBSP_BT_UART_CTS_PIN, &CYBSP_BT_UART_CTS_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_UART_RTS_PORT, CYBSP_BT_UART_RTS_PIN, &CYBSP_BT_UART_RTS_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_HOST_WAKE_PORT, CYBSP_BT_HOST_WAKE_PIN, &CYBSP_BT_HOST_WAKE_config);
    Cy_GPIO_Pin_Init(CYBSP_CO2_5V_EN_PORT, CYBSP_CO2_5V_EN_PIN, &CYBSP_CO2_5V_EN_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_DEVICE_WAKE_PORT, CYBSP_BT_DEVICE_WAKE_PIN, &CYBSP_BT_DEVICE_WAKE_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_CLK_PORT, CYBSP_WIFI_SDIO_CLK_PIN, &CYBSP_WIFI_SDIO_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_D0_PORT, CYBSP_WIFI_SDIO_D0_PIN, &CYBSP_WIFI_SDIO_D0_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_D1_PORT, CYBSP_WIFI_SDIO_D1_PIN, &CYBSP_WIFI_SDIO_D1_config);
    Cy_GPIO_Pin_Init(CYBSP_I2S_TX_FSYNC_PORT, CYBSP_I2S_TX_FSYNC_PIN, &CYBSP_I2S_TX_FSYNC_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_D2_PORT, CYBSP_WIFI_SDIO_D2_PIN, &CYBSP_WIFI_SDIO_D2_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_D3_PORT, CYBSP_WIFI_SDIO_D3_PIN, &CYBSP_WIFI_SDIO_D3_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_HOST_WAKE_PORT, CYBSP_WIFI_HOST_WAKE_PIN, &CYBSP_WIFI_HOST_WAKE_config);
    Cy_GPIO_Pin_Init(CYBSP_SDHC_DETECT_PORT, CYBSP_SDHC_DETECT_PIN, &CYBSP_SDHC_DETECT_config);
    Cy_GPIO_Pin_Init(CYBSP_BT_POWER_PORT, CYBSP_BT_POWER_PIN, &CYBSP_BT_POWER_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_1_INP_PORT, CYBSP_AMIC_1_INP_PIN, &CYBSP_AMIC_1_INP_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_1_INN_PORT, CYBSP_AMIC_1_INN_PIN, &CYBSP_AMIC_1_INN_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_1_OUT_PORT, CYBSP_AMIC_1_OUT_PIN, &CYBSP_AMIC_1_OUT_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_2_OUT_PORT, CYBSP_AMIC_2_OUT_PIN, &CYBSP_AMIC_2_OUT_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_2_INP_PORT, CYBSP_AMIC_2_INP_PIN, &CYBSP_AMIC_2_INP_config);
    Cy_GPIO_Pin_Init(CYBSP_AMIC_2_INN_PORT, CYBSP_AMIC_2_INN_PIN, &CYBSP_AMIC_2_INN_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_CLK_PORT, CYBSP_SPI_CLK_PIN, &CYBSP_SPI_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_MOSI_PORT, CYBSP_SPI_MOSI_PIN, &CYBSP_SPI_MOSI_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_MISO_PORT, CYBSP_SPI_MISO_PIN, &CYBSP_SPI_MISO_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_SS0_PORT, CYBSP_SPI_SS0_PIN, &CYBSP_SPI_SS0_config);
    Cy_GPIO_Pin_Init(CYBSP_SPI_SS1_PORT, CYBSP_SPI_SS1_PIN, &CYBSP_SPI_SS1_config);
    Cy_GPIO_Pin_Init(CYBSP_DISP_BACKLIGHT_PWM_PORT, CYBSP_DISP_BACKLIGHT_PWM_PIN, &CYBSP_DISP_BACKLIGHT_PWM_config);
    Cy_GPIO_Pin_Init(CYBSP_DISP_RST_PORT, CYBSP_DISP_RST_PIN, &CYBSP_DISP_RST_config);
    Cy_GPIO_Pin_Init(CYBSP_I2C_SCL_3V3_PORT, CYBSP_I2C_SCL_3V3_PIN, &CYBSP_I2C_SCL_3V3_config);
    Cy_GPIO_Pin_Init(CYBSP_I2C_SDA_3V3_PORT, CYBSP_I2C_SDA_3V3_PIN, &CYBSP_I2C_SDA_3V3_config);
    Cy_GPIO_Pin_Init(CYBSP_WCO_OUT_PORT, CYBSP_WCO_OUT_PIN, &CYBSP_WCO_OUT_config);
    Cy_GPIO_Pin_Init(CYBSP_WCO_IN_PORT, CYBSP_WCO_IN_PIN, &CYBSP_WCO_IN_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN, &CYBSP_USER_LED1_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, &CYBSP_USER_LED2_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_LED3_PORT, CYBSP_USER_LED3_PIN, &CYBSP_USER_LED3_config);
    Cy_GPIO_Pin_Init(CYBSP_RADAR_RESET_PORT, CYBSP_RADAR_RESET_PIN, &CYBSP_RADAR_RESET_config);
    Cy_GPIO_Pin_Init(CYBSP_WIFI_SDIO_CMD_PORT, CYBSP_WIFI_SDIO_CMD_PIN, &CYBSP_WIFI_SDIO_CMD_config);
    Cy_GPIO_Pin_Init(CYBSP_I2S_TX_SD_PORT, CYBSP_I2S_TX_SD_PIN, &CYBSP_I2S_TX_SD_config);
    Cy_GPIO_Pin_Init(CYBSP_I2S_TX_SCK_PORT, CYBSP_I2S_TX_SCK_PIN, &CYBSP_I2S_TX_SCK_config);
    Cy_GPIO_Pin_Init(CYBSP_I2S_TX_MCK_PORT, CYBSP_I2S_TX_MCK_PIN, &CYBSP_I2S_TX_MCK_config);
    Cy_GPIO_Pin_Init(CYBSP_RSPI_MISO_PORT, CYBSP_RSPI_MISO_PIN, &CYBSP_RSPI_MISO_config);
    Cy_GPIO_Pin_Init(CYBSP_RSPI_MOSI_PORT, CYBSP_RSPI_MOSI_PIN, &CYBSP_RSPI_MOSI_config);
    Cy_GPIO_Pin_Init(CYBSP_RSPI_CLK_PORT, CYBSP_RSPI_CLK_PIN, &CYBSP_RSPI_CLK_config);
    Cy_GPIO_Pin_Init(CYBSP_RADAR_INT_PORT, CYBSP_RADAR_INT_PIN, &CYBSP_RADAR_INT_config);
}
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
