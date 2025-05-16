/*******************************************************************************
* \file mtb_ctp_ft5xx6.h
*
* \brief
* Provides constants, parameter values, and API prototypes for the FT5XX6
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

#ifndef MTB_CTP_FT5XX6_H
#define MTB_CTP_FT5XX6_H

#define COMPONENT_MTB_CTP_FT5XX6

#if defined(COMPONENT_MTB_CTP_FT5XX6)

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "cy_result.h"
#include <stdbool.h>


/*******************************************************************************
* Macros
*******************************************************************************/

/*!
 * @addtogroup middlewaregroup Middleware
 * @brief This section includes the mikroSDK API Reference for Middleware Layer.
 * @{
 */

/*!
 * @addtogroup ft5xx6 FT5xx6 Touch Controllers Driver
 * @brief FT5xx6 Touch Controller Driver API Reference.
 * @{
 */

/**
 * @defgroup ft5xx6_registers FT5xx6 Registers
 * @brief FT5xx6 Registers List.
 * @details FT5xx6 registers description with respective addresses.
 */

/**
 * @addtogroup ft5xx6_registers
 * @{
 */

/**
 * @brief FT5xx6 Device Mode Register.
 * @details Register address specified for device mode of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_DEVICE_MODE          0x0

/**
 * @brief FT5xx6 Gesture ID Register.
 * @details Register address specified for gesture ID of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_GEST_ID              0x1

/**
 * @brief FT5xx6 Status Register.
 * @details Register address specified for status of FT5xx6 touch controller.
 */
#define FT5XX6_REG_TD_STATUS            0x2

/**
 * @brief FT5xx6 Touch1 X-coord MSB Register.
 * @details Register address specified for touch1 x coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH1_XH            0x3

/**
 * @brief FT5xx6 Touch1 X-coord LSB Register.
 * @details Register address specified for touch1 x coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH1_XL            0x4

/**
 * @brief FT5xx6 Touch1 Y-coord MSB Register.
 * @details Register address specified for touch1 y coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH1_YH            0x5

/**
 * @brief FT5xx6 Touch1 Y-coord LSB Register.
 * @details Register address specified for touch1 y coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH1_YL            0x6

/**
 * @brief FT5xx6 Touch2 X-coord MSB Register.
 * @details Register address specified for touch2 x coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH2_XH            0x9

/**
 * @brief FT5xx6 Touch2 X-coord LSB Register.
 * @details Register address specified for touch2 x coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH2_XL            0xA

/**
 * @brief FT5xx6 Touch2 Y-coord MSB Register.
 * @details Register address specified for touch2 y coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH2_YH            0xB

/**
 * @brief FT5xx6 Touch2 Y-coord LSB Register.
 * @details Register address specified for touch2 y coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH2_YL            0xC

/**
 * @brief FT5xx6 Touch3 X-coord MSB Register.
 * @details Register address specified for touch3 x coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH3_XH            0xF

/**
 * @brief FT5xx6 Touch3 X-coord LSB Register.
 * @details Register address specified for touch3 x coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH3_XL            0x10

/**
 * @brief FT5xx6 Touch3 Y-coord MSB Register.
 * @details Register address specified for touch3 y coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH3_YH            0x11

/**
 * @brief FT5xx6 Touch3 Y-coord LSB Register.
 * @details Register address specified for touch3 y coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH3_YL            0x12

/**
 * @brief FT5xx6 Touch4 X-coord MSB Register.
 * @details Register address specified for touch4 x coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH4_XH            0x15

/**
 * @brief FT5xx6 Touch4 X-coord LSB Register.
 * @details Register address specified for touch4 x coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH4_XL            0x16

/**
 * @brief FT5xx6 Touch4 Y-coord MSB Register.
 * @details Register address specified for touch4 y coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH4_YH            0x17

/**
 * @brief FT5xx6 Touch4 Y-coord LSB Register.
 * @details Register address specified for touch4 y coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH4_YL            0x18

/**
 * @brief FT5xx6 Touch5 X-coord MSB Register.
 * @details Register address specified for touch5 x coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH5_XH            0x1B

/**
 * @brief FT5xx6 Touch5 X-coord LSB Register.
 * @details Register address specified for touch5 x coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH5_XL            0x1C

/**
 * @brief FT5xx6 Touch5 Y-coord MSB Register.
 * @details Register address specified for touch5 y coordinate higher byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH5_YH            0x1D

/**
 * @brief FT5xx6 Touch5 Y-coord LSB Register.
 * @details Register address specified for touch5 y coordinate lower byte of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH5_YL            0x1E

/**
 * @brief FT5xx6 Touch Detection Threshold Register.
 * @details Register address specified for touch detection threshold of FT5xx6
 * touch controller.
 */
#define FT5XX6_REG_TOUCH_DET_TH         0x80

/**
 * @brief FT5xx6 Touch Peak Detection Threshold Register.
 * @details Register address specified for touch peak detection threshold of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH_PEAK_DET_TH    0x81

/**
 * @brief FT5xx6 Touch Threshold Calibration Register.
 * @details Register address specified for touch threshold calibration of FT5xx6
 * touch controller.
 */
#define FT5XX6_REG_TOUCH_TH_CAL         0x82

/**
 * @brief FT5xx6 Touch Threshold Water Register.
 * @details Register address specified for touch threshold water of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_TOUCH_TH_WATER       0x83

/**
 * @brief FT5xx6 Touch Threshold Temperature Compensation Register.
 * @details Register address specified for touch threshold temperature
 * compensation of FT5xx6 touch controller.
 */
#define FT5XX6_REG_TOUCH_TH_TEMP_COMP   0x84

/**
 * @brief FT5xx6 Power Control Register.
 * @details Register address specified for power control of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_POWER_CTRL_MODE      0x86

/**
 * @brief FT5xx6 Timer Status Monitor Register.
 * @details Register address specified for timer status monitor of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_MONITOR_STATUS_TMR   0x87

/**
 * @brief FT5xx6 Actual Period Monitor Register.
 * @details Register address specified for actual period monitor of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_MONITOR_ACT_PERIOD   0x88

/**
 * @brief FT5xx6 Enter Idle Timer Register.
 * @details Register address specified for enter idle timer of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_ENTER_IDLE_TIMER     0x89

/**
 * @brief FT5xx6 Auto Calibration Register.
 * @details Register address specified for auto calibration of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_AUTO_CALIB_MODE      0xA0

/**
 * @brief FT5xx6 Version MSB Register.
 * @details Register address specified for version higher byte of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_LIB_VERSION_H        0xA1

/**
 * @brief FT5xx6 Version LSB Register.
 * @details Register address specified for version lower byte of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_LIB_VERSION_L        0xA2

/**
 * @brief FT5xx6 Chip Vendor ID Register.
 * @details Register address specified for chip vendor ID of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_LIB_CHIP_VENDOR_ID   0xA3

/**
 * @brief FT5xx6 IVT To Host Status Register.
 * @details Register address specified for IVT to host status of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_IVT_TO_HOST_STATUS   0xA4

/**
 * @brief FT5xx6 Power Consume Register.
 * @details Register address specified for power consume of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_POWER_CONSUME_MODE   0xA5

/**
 * @brief FT5xx6 FW ID Register.
 * @details Register address specified for firmware ID of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_FW_ID                0xA6

/**
 * @brief FT5xx6 Running State Register.
 * @details Register address specified for running state of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_RUNNING_STATE        0xA7

/**
 * @brief FT5xx6 CTPM Vendor ID Register.
 * @details Register address specified for CTPM vendor ID of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_CTPM_VENDOR_ID       0xA8

/**
 * @brief FT5xx6 Error ID Register.
 * @details Register address specified for error ID of FT5xx6 touch controller.
 */
#define FT5XX6_REG_ERROR_ID             0xA9

/**
 * @brief FT5xx6 Calibration Mode Configuration Register.
 * @details Register address specified for calibration mode configuration of
 * FT5xx6 touch controller.
 */
#define FT5XX6_REG_CONFIGURE_CAL_MODE   0xAA

/**
 * @brief FT5xx6 Big Area Threshold Register.
 * @details Register address specified for big area threshold of FT5xx6 touch
 * controller.
 */
#define FT5XX6_REG_BIG_AREA_TH          0xAE

/*! @} */ // ft5xx6_registers

/**
 * @defgroup ft5xx6_settings FT5xx6 Settings
 * @brief FT5xx6 Settings List.
 * @details FT5xx6 settings description with respective values.
 */

/**
 * @addtogroup ft5xx6_settings
 * @{
 */

/**
 * @brief FT5xx6 Interrupt Polling Mode Setting.
 * @details INT pin polling mode setting for FT5xx6 series touch controllers.
 */
#define FT5XX6_INT_MODE_POLLING         0

/**
 * @brief FT5xx6 Interrupt Trigger Mode Setting.
 * @details INT pin trigger mode setting for FT5xx6 series touch controllers.
 */
#define FT5XX6_INT_MODE_TRIGGER         1

/**
 * @brief FT5xx6 Slave Address Setting.
 * @details Slave address setting for FT5xx6 series touch controllers.
 */
#define FT5XX6_I2C_ADDR                 0x38

/**
 * @brief FT5xx6 Data Transfer Limits Setting.
 * @details Data transfer limits for FT5xx6 series touch controllers.
 */
#define FT5XX6_N_DATA_TRANSFER_MIN      1
#define FT5XX6_N_DATA_TRANSFER_MAX      256

/**
 * @brief FT5xx6 Gesture Items Limit Setting.
 * @details Gesture items limit for FT5xx6 series touch controllers.
 */
#define FT5XX6_GESTURE_ITEMS_MAX        7

/**
 * @brief FT5xx6 Touch Pressure Event.
 * @details Touch pressure event for FT5xx6 series touch controllers.
 */
#define FT5XX6_EVENT_PRESS_DET          0x80

/**
 * @defgroup ft5xx6_masks FT5xx6 Masks
 * @brief FT5xx6 Masks List.
 * @details FT5xx6 masks description with respective values.
 */

/**
 * @addtogroup ft5xx6_masks
 * @{
 */

/**
 * @brief FT5xx6 Touch Coordinates Mask.
 * @details Touch pressure coordinates mask for FT5xx6 series touch controllers.
 */
#define FT5XX6_MASK_PRESS_COORD         0xFFF

/**
 * @brief FT5xx6 Touch Event Mask.
 * @details Touch pressure event mask for FT5xx6 series touch controllers.
 */
#define FT5XX6_MASK_PRESS_EVENT         0xC0

/**
 * @brief FT5xx6 Touch Detection Mask.
 * @details Touch pressure detection mask for FT5xx6 series touch controllers.
 */
#define FT5XX6_MASK_PRESS_DET           0xC0

/**
 * @brief FT5xx6 TP Number Mask.
 * @details Touch point number mask for FT5xx6 series touch controllers.
 */
#define FT5XX6_MASK_TP_NUM              0xF

/*! @} */ // ft5xx6_masks

/**
 * @defgroup ft5xx6_offsets FT5xx6 Offsets
 * @brief FT5xx6 Offsets List.
 * @details FT5xx6 offsets description with respective values.
 */

/**
 * @addtogroup ft5xx6_offsets
 * @{
 */

/**
 * @brief FT5xx6 Touch Event Offset.
 * @details Touch event offset for FT5xx6 series touch controllers.
 */
#define FT5XX6_OFFSET_PRESS_EVENT       6

/**
 * @brief FT5xx6 Touch ID Offset.
 * @details Touch ID offset for FT5xx6 series touch controllers.
 */
#define FT5XX6_OFFSET_PRESS_ID          4

/**
 * @brief FT5xx6 Device Mode Offset.
 * @details Device mode offset for FT5xx6 series touch controllers.
 */
#define FT5XX6_OFFSET_DEV_MODE          4

/**
 * @brief FT5xx6 Touch Reading Offset.
 * @details Reading of touches offset for FT5xx6 series touch controllers.
 */
#define FT5XX6_OFFSET_TOUCH_READING     6


#define FT5446_VENDOR_ID                0x54

/* Error code definition */
#define CY_RSLT_FT5XX6_ERR_BASE         (0x0253)

/* No touch detected by the driver */
#define CY_RSLT_FT5XX6_NO_TOUCH                           \
                CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_FT5XX6_ERR_BASE, 1)

/* FT5XX6 device error */
#define CY_RSLT_FT5XX6_DEV_ERR                            \
               CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_FT5XX6_ERR_BASE, 2)

#define MTB_CTP_TOUCH_RESOLUTION_X       (480U)
#define MTB_CTP_TOUCH_RESOLUTION_Y       (480U)

// TODO: Confirm with Focaltech
#define TP_MAX_VAL_X                      (480U)

// TODO: Confirm with Focaltech
#define TP_MAX_VAL_Y                      (800U)

#define ORIENTATION                       0       // 0 also supported 90, 180, 270

/* CTP IRQ priority */
#define MTB_CTP_FT5XX6_IRQ_PRIORITY       (3U)



/*******************************************************************************
* Data Structures
*******************************************************************************/
/* FT5XX6 touch controller configuration structure */
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
} mtb_ctp_ft5xx6_config_t;


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void mtb_ctp_ft5xx6_irq_handler(void);
cy_rslt_t mtb_ctp_ft5xx6_init(mtb_ctp_ft5xx6_config_t* ctp_ft5xx6_config);
cy_rslt_t mtb_ctp_ft5xx6_get_single_touch(int* touch_x, int* touch_y);
void mtb_ctp_ft5xx6_deinit(void);


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* COMPONENT_MTB_CTP_FT5XX6 */

#endif /* MTB_CTP_FT5XX6_H */

/* [] END OF FILE */
