/*******************************************************************************
* \file mtb_display_st7701s.c
* \version 0.5.0
*
* \brief
* Provides implementation of the st7701s TFT DSI display driver library.
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

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "mtb_display_st7701s.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* Display controller command commit mask */
#define DC_CMD_COMMIT_MASK                               (0x00000001U)

/*******************************************************************************
* Global Variables
*******************************************************************************/
#define HSIOM_SEL_TCPWM0_NUM                             (10U)
#define GPIO_LOW                                         (0U)
#define GPIO_HIGH                                        (1U)
#define ONE_MS_DELAY                                     (120U)
#define PIN_HIGH_DELAY_MS                                (120U)
static mtb_display_st7701s_backlight_config_t* backlight_config = NULL;
static uint32_t pwm_period = 0U;
#define MAX_BRIGHTNESS_PERCENT                           (100U)
/* Macro to convert brightness percentage into PWM counter value */
#define BRIGHTNESS_PERCENT_TO_PWM_COUNT(percentage, period) \
    ((uint32_t)(((percentage) * (period)) / 100))

static mtb_display_st7701s_pin_config_t* disp_pin_config = NULL;

static const lcd_init_cmd_t panel_init_sequence[] = {
    {(uint8_t[]){0x01}, 1, 120},
    {(uint8_t[]){0xFF, 0x77, 0x01, 0x00, 0x00, 0x10}, 6, 0},
    {(uint8_t[]){0xC0, 0x3B, 0x00}, 3, 0 },
    {(uint8_t[]){0xC1, 0x0D, 0x02}, 3, 0 },
    {(uint8_t[]){0xC2, 0x21, 0x08}, 3, 0 },
    {(uint8_t[]){0xB0, 0x00, 0x11, 0x18, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22, 0x04, 0x12, 0x0F, 0xAA, 0x31, 0x18}, 17, 0},
    {(uint8_t[]){0xB1, 0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x08, 0x08, 0x08, 0x22, 0x04, 0x11, 0x11, 0xA9, 0x32, 0x18}, 17, 0},
    {(uint8_t[]){0xB1, 0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x08, 0x08, 0x08, 0x22, 0x04, 0x11, 0x11, 0xA9, 0x32, 0x18}, 17, 0},
    {(uint8_t[]){0xFF, 0x77, 0x01, 0x00, 0x00, 0x11}, 6, 0},
    {(uint8_t[]){0xB0, 0x60}, 2, 0},
    {(uint8_t[]){0xB1, 0x30}, 2, 0},
    {(uint8_t[]){0xB2, 0x87}, 2, 0},
    {(uint8_t[]){0xB3, 0x80}, 2, 0},
    {(uint8_t[]){0xB5, 0x49}, 2, 0},
    {(uint8_t[]){0xB7, 0x85}, 2, 0},
    {(uint8_t[]){0xB8, 0x21}, 2, 0},
    {(uint8_t[]){0xC1, 0x78}, 2, 0},
    {(uint8_t[]){0xC2, 0x78}, 2, 0},
    {(uint8_t[]){0xE0, 0x00, 0x1B, 0x02}, 4, 0},
    {(uint8_t[]){0xE1, 0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44, 0x44}, 12, 0},
    {(uint8_t[]){0xE2, 0x11, 0x11, 0x44, 0x44, 0xED, 0xA0, 0x00, 0x00, 0xEC, 0xA0, 0x00, 0x00}, 13, 0},
    {(uint8_t[]){0xE3, 0x00, 0x00, 0x11, 0x11}, 5, 0},
    {(uint8_t[]){0xE4, 0x44, 0x44}, 3, 0},
    {(uint8_t[]){0xE5, 0x0A, 0xE9, 0xD8, 0xA0, 0x0C, 0xEB, 0xD8, 0xA0, 0x0E, 0xED, 0xD8, 0xA0, 0x10, 0xEF, 0xD8, 0xA0}, 17, 0},
    {(uint8_t[]){0xE6, 0x00, 0x00, 0x11, 0x11}, 5, 0},
    {(uint8_t[]){0xE7, 0x44, 0x44}, 3, 0},
    {(uint8_t[]){0xE8, 0x09, 0xE8, 0xD8, 0xA0, 0x0B, 0xEA, 0xD8, 0xA0, 0x0D, 0xEC, 0xD8, 0xA0, 0x0F, 0xEE, 0xD8, 0xA0}, 17, 0},
    {(uint8_t[]){0xEB, 0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40}, 8, 0},
    {(uint8_t[]){0xEC, 0x3C, 0x00}, 3, 0},
    {(uint8_t[]){0xED, 0xAB, 0x89, 0x76, 0x54, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x45, 0x67, 0x98, 0xBA}, 17, 0},
    {(uint8_t[]){0xFF, 0x77, 0x01, 0x00, 0x00, 0x00}, 6, 0},
//    {(uint8_t[]){0xFF, 0x77, 0x01, 0x00, 0x00, 0x12}, 6, 0},
//    {(uint8_t[]){0xd1, 0x81}, 2, 0},
//    {(uint8_t[]){0xd2, 0x08}, 2, 0},
    {(uint8_t[]){0x11}, 1, 120},
    {(uint8_t[]){0x29}, 1, 0},
};

/*******************************************************************************
* Function Name: mtb_display_st7701s_init
********************************************************************************
*
* Performs st7701s TFT driver initialization using MIPI DSI interface.
*
* \param mipi_dsi_base
* Pointer to the MIPI DSI register base address.
*
* \param disp_st7701s_pin_config
* Pointer to the st7701s display pin configuration structure.
*
* \return cy_en_mipidsi_status_t
* Initialization status.
*
* \funcusage
* \snippet snippet/main.c snippet_mtb_display_st7701s_init
*
*******************************************************************************/
uint16_t size_arry;
cy_en_mipidsi_status_t mtb_display_st7701s_init ( GFXSS_MIPIDSI_Type *mipi_dsi_base , mtb_display_st7701s_pin_config_t* disp_st7701s_pin_config )
{
    cy_en_mipidsi_status_t status = CY_MIPIDSI_BAD_PARAM;

    disp_pin_config = disp_st7701s_pin_config;

    // TODO: Reseting lcd does not work
#if 0
	/* Display pin initialization sequence */
	/* Initialize display RESET GPIO pin with initial value as HIGH */
	Cy_GPIO_Pin_FastInit(disp_pin_config->reset_port, disp_pin_config->reset_pin,
			CY_GPIO_DM_STRONG, GPIO_HIGH, HSIOM_SEL_GPIO);
	Cy_SysLib_Delay(200);

	/* Perform reset */
	/* Pull the display RESET GPIO pin to LOW */
	Cy_GPIO_Write(disp_pin_config->reset_port, disp_pin_config->reset_pin, GPIO_LOW);
	Cy_SysLib_Delay(400);

	/* Pull the display RESET GPIO pin to HIGH */
	Cy_GPIO_Write(disp_pin_config->reset_port, disp_pin_config->reset_pin, GPIO_HIGH);
	Cy_SysLib_Delay(200);

	// debug gpio
	Cy_GPIO_Write(GPIO_PRT11, 1, 1);
#endif

    status = Cy_MIPIDSI_ExitSleep(mipi_dsi_base);
    if ( CY_MIPIDSI_SUCCESS == status )
    {
        Cy_SysLib_Delay(1);
        status = Cy_MIPIDSI_DisplayON(mipi_dsi_base);

        if ( CY_MIPIDSI_SUCCESS == status )
        {
            Cy_SysLib_Delay(1);

            viv_set_commit(DC_CMD_COMMIT_MASK);
        }
    }

    size_arry = (sizeof(panel_init_sequence) / sizeof(lcd_init_cmd_t));
    /* Set the LCM init settings */
    for ( uint16_t i = 0; i < (sizeof(panel_init_sequence) / sizeof(lcd_init_cmd_t)); i++ )
    {
        if (panel_init_sequence[i].data[0] == 0xB0 || panel_init_sequence[i].data[0] == 0xB1) {
            GFXSS_GFXSS_MIPIDSI->DWCMIPIDSI.VID_MODE_CFG |= ENABLE_LOW_POWER_CMD;
        }
        else GFXSS_GFXSS_MIPIDSI->DWCMIPIDSI.VID_MODE_CFG &= ~ ENABLE_LOW_POWER_CMD;
        status = Cy_MIPIDSI_WritePacket(mipi_dsi_base, panel_init_sequence[i].data, panel_init_sequence[i].bytes);
        Cy_SysLib_Delay(panel_init_sequence[i].delay_ms);
        if ( CY_MIPIDSI_SUCCESS != status )
        {
            break;
        }
    }

    Cy_SysLib_Delay(10);

    // mipi_dsi_base->DWCMIPIDSI.VID_MODE_CFG = VID_MODE_TYPE_BURST | ENABLE_LOW_POWER_CMD;

    // size_arry = (sizeof(panel_init_sequence) / sizeof(lcd_init_cmd_t));
    //     /* Set the LCM init settings */
	// for ( uint16_t i = 0; i < (sizeof(panel_init_sequence) / sizeof(lcd_init_cmd_t)); i++ )
	// {
	// 	status = Cy_MIPIDSI_WritePacket(mipi_dsi_base, panel_init_sequence[i].data, panel_init_sequence[i].bytes);
	// 	Cy_SysLib_Delay(panel_init_sequence[i].delay_ms);
	// 	if ( CY_MIPIDSI_SUCCESS != status )
	// 	{
	// 		break;
	// 	}
	// }

    return status;
}


/*******************************************************************************
 * Function Name: mtb_display_st7701s_deinit
 ********************************************************************************
 *
 * Performs de-initialization of the st7701s TFT driver using MIPI DSI interface.
 * It also de-initializes PWM output on display backlight pin.
 *
 * \param mipi_dsi_base
 * Pointer to the MIPI DSI register base address.
 *
 * \return cy_en_mipidsi_status_t
 * De-initialization status.
 *
 *******************************************************************************/
cy_en_mipidsi_status_t mtb_display_st7701s_deinit ( GFXSS_MIPIDSI_Type *mipi_dsi_base )
{
    cy_en_mipidsi_status_t status = CY_MIPIDSI_BAD_PARAM;

    CY_ASSERT(NULL != mipi_dsi_base);
//    CY_ASSERT(NULL != disp_pin_config);
//    CY_ASSERT(NULL != backlight_config);

    status = Cy_MIPIDSI_EnterSleep(mipi_dsi_base);
    if ( CY_MIPIDSI_SUCCESS == status )
    {
        // TODO: Add Deinit Code
//        /* Set display RESET GPIO pin to LOW */
//        Cy_GPIO_Write(disp_pin_config->reset_port, disp_pin_config->reset_pin, GPIO_LOW);
//
//        /* Stop and de-initialize PWM on display backlight pin */
//        Cy_TCPWM_TriggerStopOrKill_Single(backlight_config->pwm_hw, backlight_config->pwm_num);
//        Cy_TCPWM_PWM_Disable(backlight_config->pwm_hw, backlight_config->pwm_num);
//        Cy_TCPWM_PWM_DeInit(backlight_config->pwm_hw, backlight_config->pwm_num,
//                            backlight_config->pwm_config);
    }

    return status;
}

/*******************************************************************************
* Function Name: mtb_display_st7701s_backlight_init
********************************************************************************
*
* Configures display backlight GPIO pin and enables PWM output on it.
*
* \param disp_st7701s_backlight_config
* Pointer to the display backlight configuration structure.
*
* \return cy_en_tcpwm_status_t
* Display backlight PWM initialization status.
*
* \funcusage
* \snippet snippet/main.c snippet_mtb_display_st7701s_backlight_init
*
*******************************************************************************/
cy_en_tcpwm_status_t mtb_display_st7701s_backlight_init(
    mtb_display_st7701s_backlight_config_t* disp_st7701s_backlight_config)
{
    cy_en_tcpwm_status_t status = CY_TCPWM_BAD_PARAM;

    CY_ASSERT(NULL != disp_st7701s_backlight_config);

    backlight_config = disp_st7701s_backlight_config;

//    /* Display backlight GPIO pin initialization in PWM mode */
//    Cy_GPIO_Pin_FastInit(backlight_config->bl_port, backlight_config->bl_pin,
//                         CY_GPIO_DM_STRONG_IN_OFF, GPIO_LOW, HSIOM_SEL_TCPWM0_NUM);
    Cy_TCPWM_TriggerStopOrKill_Single(backlight_config->pwm_hw, backlight_config->pwm_num);
    /* Initialize the TCPWM block for backlight pin */
    status = Cy_TCPWM_PWM_Init(backlight_config->pwm_hw, backlight_config->pwm_num,
                               backlight_config->pwm_config);

    if (CY_TCPWM_SUCCESS == status)
    {
        /* Enable the TCPWM block for backlight pin */
        Cy_TCPWM_PWM_Enable(backlight_config->pwm_hw, backlight_config->pwm_num);

        /* Fetch the initial values of period register */
        pwm_period =
            Cy_TCPWM_PWM_GetPeriod0(backlight_config->pwm_hw, backlight_config->pwm_num);

        /* Trigger a software start on the selected TCPWM */
        Cy_TCPWM_TriggerStart_Single(backlight_config->pwm_hw, backlight_config->pwm_num);
    }

    return status;
}

/*******************************************************************************
* Function Name: mtb_display_st7701s_set_brightness
********************************************************************************
*
* Sets the brightness of the display panel to the desired percentage.
*
* \param brightness_percent
* Brightness value in percentage (0 to 100).
*
* \return void
*
*******************************************************************************/
void mtb_display_st7701s_set_brightness(uint8_t brightness_percent)
{
    uint32_t compare0_value = 0;
    uint32_t compare1_value = 0;
    uint32_t counter_val    = 0;

    CY_ASSERT(MAX_BRIGHTNESS_PERCENT >= brightness_percent);

    counter_val = BRIGHTNESS_PERCENT_TO_PWM_COUNT(brightness_percent, pwm_period);
    compare0_value = compare1_value = (counter_val > pwm_period) ?
                                      pwm_period : counter_val;

   /* Set new values for CC0/1 compare buffers */
    Cy_TCPWM_PWM_SetCompare0Val(backlight_config->pwm_hw, backlight_config->pwm_num,
                                   compare0_value);

    /* Trigger compare swap with its buffer values */
    Cy_TCPWM_TriggerCaptureOrSwap_Single(backlight_config->pwm_hw, backlight_config->pwm_num);

}



/* [] END OF FILE */
