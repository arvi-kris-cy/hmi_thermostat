/*******************************************************************************
* \file mtb_ctp_ft5446.h
*
* \brief
* Provides constants, parameter values, and API prototypes for the FT5446
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

#ifndef MTB_CTP_FT5446_H
#define MTB_CTP_FT5446_H

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
/* FT5446 Device Mode Register. */
#define MTB_FT5446_REG_DEVICE_MODE          (0x0)

/* FT5446 Gesture ID Register. */
#define MTB_FT5446_REG_GEST_ID              (0x1)

/* FT5446 Status Register. */
#define MTB_FT5446_REG_TD_STATUS            (0x2)

/* FT5446 Touch1 X-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH1_XH            (0x3)

/* FT5446 Touch1 X-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH1_XL            (0x4)

/* FT5446 Touch1 Y-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH1_YH            (0x5)

/* FT5446 Touch1 Y-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH1_YL            (0x6)

/* FT5446 Touch2 X-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH2_XH            (0x9)

/* FT5446 Touch2 X-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH2_XL            (0xA)

/* FT5446 Touch2 Y-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH2_YH            (0xB)

/* FT5446 Touch2 Y-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH2_YL            (0xC)

/* FT5446 Touch3 X-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH3_XH            (0xF)

/* FT5446 Touch3 X-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH3_XL            (0x10)

/* FT5446 Touch3 Y-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH3_YH            (0x11)

/* FT5446 Touch3 Y-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH3_YL            (0x12)

/* FT5446 Touch4 X-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH4_XH            (0x15)

/* FT5446 Touch4 X-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH4_XL            (0x16)

/* FT5446 Touch4 Y-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH4_YH            (0x17)

/* FT5446 Touch4 Y-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH4_YL            (0x18)

/* FT5446 Touch5 X-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH5_XH            (0x1B)

/* FT5446 Touch5 X-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH5_XL            (0x1C)

/* FT5446 Touch5 Y-coord MSB Register. */
#define MTB_FT5446_REG_TOUCH5_YH            (0x1D)

/* FT5446 Touch5 Y-coord LSB Register. */
#define MTB_FT5446_REG_TOUCH5_YL            (0x1E)

/* FT5446 Touch Detection Threshold Register. */
#define MTB_FT5446_REG_TOUCH_DET_TH         (0x80)

/* FT5446 Touch Peak Detection Threshold Register. */
#define MTB_FT5446_REG_TOUCH_PEAK_DET_TH    (0x81)

/* FT5446 Touch Threshold Calibration Register. */
#define MTB_FT5446_REG_TOUCH_TH_CAL         (0x82)

/* FT5446 Touch Threshold Water Register. */
#define MTB_FT5446_REG_TOUCH_TH_WATER       (0x83)

/* FT5446 Touch Threshold Temperature Compensation Register. */
#define MTB_FT5446_REG_TOUCH_TH_TEMP_COMP   (0x84)

/* FT5446 Power Control Register. */
#define MTB_FT5446_REG_POWER_CTRL_MODE      (0x86)

/* FT5446 Timer Status Monitor Register. */
#define MTB_FT5446_REG_MONITOR_STATUS_TMR   (0x87)

/* FT5446 Actual Period Monitor Register. */
#define MTB_FT5446_REG_MONITOR_ACT_PERIOD   (0x88)

/* FT5446 Enter Idle Timer Register.  */
#define MTB_FT5446_REG_ENTER_IDLE_TIMER     (0x89)

/* FT5446 Auto Calibration Register.  */
#define MTB_FT5446_REG_AUTO_CALIB_MODE      (0xA0)

/* FT5446 Version MSB Register. */
#define MTB_FT5446_REG_LIB_VERSION_H        (0xA1)

/* FT5446 Version LSB Register.  */
#define MTB_FT5446_REG_LIB_VERSION_L        (0xA2)

/* FT5446 Chip Vendor ID Register.  */
#define MTB_FT5446_REG_LIB_CHIP_VENDOR_ID   (0xA3)

/* FT5446 IVT To Host Status Register. */
#define MTB_FT5446_REG_IVT_TO_HOST_STATUS   (0xA4)

/* FT5446 Power Consume Register.  */
#define MTB_FT5446_REG_POWER_CONSUME_MODE   (0xA5)

/* FT5446 FW ID Register. */
#define MTB_FT5446_REG_FW_ID                (0xA6)

/* FT5446 Running State Register. */
#define MTB_FT5446_REG_RUNNING_STATE        (0xA7)

/* FT5446 CTPM Vendor ID Register. */
#define MTB_FT5446_REG_CTPM_VENDOR_ID       (0xA8)

/* FT5446 Error ID Register. */
#define MTB_FT5446_REG_ERROR_ID             (0xA9)

/* FT5446 Calibration Mode Configuration Register. */
#define MTB_FT5446_REG_CONFIGURE_CAL_MODE   (0xAA)

/* FT5446 Big Area Threshold Register.  */
#define MTB_FT5446_REG_BIG_AREA_TH          (0xAE)

/* FT5446 Interrupt Polling Mode Setting. */
#define MTB_FT5446_INT_MODE_POLLING         (0)

/* FT5446 Interrupt Trigger Mode Setting. */
#define MTB_FT5446_INT_MODE_TRIGGER         (1)

/* FT5446 Slave Address Setting.  */
#define MTB_FT5446_I2C_ADDR                 (0x38)

/* FT5446 Data Transfer Limits Setting. */
#define MTB_FT5446_N_DATA_TRANSFER_MIN      (1)
#define MTB_FT5446_N_DATA_TRANSFER_MAX      (256)

/* FT5446 Gesture Items Limit Setting. */
#define MTB_FT5446_GESTURE_ITEMS_MAX        (7)

/* FT5446 Touch Pressure Event. */
#define MTB_FT5446_EVENT_PRESS_DET          (0x80)

/* FT5446 Touch Coordinates Mask. */
#define MTB_FT5446_MASK_PRESS_COORD         (0xFFF)

/* FT5446 Touch Event Mask. */
#define MTB_FT5446_MASK_PRESS_EVENT         (0xC0)

/* FT5446 Touch Detection Mask. */
#define MTB_FT5446_MASK_PRESS_DET           (0xC0)

/* FT5446 TP Number Mask. */
#define MTB_FT5446_MASK_TP_NUM              (0xF)

/* FT5446 Touch Event Offset. */
#define MTB_FT5446_OFFSET_PRESS_EVENT       (6)

/* FT5446 Touch ID Offset. */
#define MTB_FT5446_OFFSET_PRESS_ID          (4)

/* FT5446 Device Mode Offset. */
#define MTB_FT5446_OFFSET_DEV_MODE          (4)

/* FT5446 Touch Reading Offset. */
#define MTB_FT5446_OFFSET_TOUCH_READING     (6)

/* FT5446 Vendor ID. */
#define MTB_FT5446_VENDOR_ID                (0x54)

/* Error code definition */
#define CY_RSLT_FT5446_ERR_BASE             (0x0253)

/* No touch detected by the driver */
#define CY_RSLT_FT5446_NO_TOUCH                           \
                CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_FT5446_ERR_BASE, 1)

/* FT5446 device error */
#define CY_RSLT_FT5446_DEV_ERR                            \
               CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_FT5446_ERR_BASE, 2)

#define MTB_CTP_TOUCH_RESOLUTION_X       (480U)
#define MTB_CTP_TOUCH_RESOLUTION_Y       (480U)

// TODO: Confirm with Focaltech
#define TP_MAX_VAL_X                     (480U)

// TODO: Confirm with Focaltech
#define TP_MAX_VAL_Y                     (800U)

/* Supports 0, 90, 180, 270 */
#define ORIENTATION                      (0)

/* CTP IRQ priority */
#define MTB_CTP_FT5446_IRQ_PRIORITY      (3U)



/*******************************************************************************
* Data Structures
*******************************************************************************/
/* FT5446 touch controller configuration structure */
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
} mtb_ctp_ft5446_config_t;


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void mtb_ctp_ft5446_irq_handler(void);
cy_rslt_t mtb_ctp_ft5446_init(mtb_ctp_ft5446_config_t* ctp_ft5446_config);
cy_rslt_t mtb_ctp_ft5446_get_single_touch(int* touch_x, int* touch_y);
void mtb_ctp_ft5446_deinit(void);


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* MTB_CTP_FT5446_H */

/* [] END OF FILE */
