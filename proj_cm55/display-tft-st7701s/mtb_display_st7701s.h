/*******************************************************************************
* \file mtb_display_st7701s.h
*
* \brief
* Provides constants, parameter values, and API prototypes for the ST7701S TFT
* Display DSI driver library.
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

#ifndef MTB_DISPLAY_ST7701S_H
#define MTB_DISPLAY_ST7701S_H

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_graphics.h"
#include "cy_pdl.h"


/*******************************************************************************
* Macros
*******************************************************************************/

/*******************************************************************************
* Data Structures
*******************************************************************************/

/* st7701s display pin configuration structure */
typedef struct
{
    GPIO_PRT_Type* reset_port;
    uint32_t reset_pin;
} mtb_display_st7701s_pin_config_t;

typedef struct
{
    const uint8_t *data;     // Buffer to data for the command
    uint8_t bytes;           // Size of the data buffer for the command
    unsigned short delay_ms; // Delay in milliseconds after the command
} lcd_init_cmd_t;

/* Display backlight configuration structure */
typedef struct
{
    GPIO_PRT_Type* bl_port;
    uint32_t bl_pin;
    TCPWM_Type* pwm_hw;
    uint32_t pwm_num;
    cy_stc_tcpwm_pwm_config_t* pwm_config;
} mtb_display_st7701s_backlight_config_t;


/*******************************************************************************
* Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
// TODO: GPIO config structure should be pass through init APIs
cy_en_mipidsi_status_t mtb_display_st7701s_init ( GFXSS_MIPIDSI_Type *mipi_dsi_base , mtb_display_st7701s_pin_config_t* disp_st7701s_pin_config );
cy_en_mipidsi_status_t mtb_display_st7701s_deinit(GFXSS_MIPIDSI_Type* mipi_dsi_base);
cy_en_tcpwm_status_t mtb_display_st7701s_backlight_init(mtb_display_st7701s_backlight_config_t* disp_st7701s_backlight_config);
void mtb_display_st7701s_set_brightness(uint8_t brightness_percent);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* MTB_DISPLAY_ST7701S_H */

/* [] END OF FILE */
