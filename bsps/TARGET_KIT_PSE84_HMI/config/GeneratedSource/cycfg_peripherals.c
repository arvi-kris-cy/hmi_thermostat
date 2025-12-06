/*******************************************************************************
 * File Name: cycfg_peripherals.c
 *
 * Description:
<<<<<<< HEAD
 * Peripheral Hardware Block configuration
 * This file was automatically generated and should not be modified.
 * Configurator Backend 3.50.0
 * device-db 4.5.30.8214
 * mtb-device-support-pse8xxgp 1.0.200.195
=======
 * Analog configuration
 * This file was automatically generated and should not be modified.
 * Configurator Backend 3.60.0
 * device-db 4.29.0.9102
 * mtb-dsl-pse8xxgp 1.0.0.744
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
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

#include "cycfg_peripherals.h"

<<<<<<< HEAD
#define CYBSP_TCPWM_0_GRP_0_COUNTER_0_INPUT_DISABLED 0x7U
#define emUSB_OS_Timer_INPUT_DISABLED 0x7U
#define CYBSP_TCPWM_0_GRP_1_PWM_5_INPUT_DISABLED 0x7U

cy_stc_gfx_layer_config_t GFXSS_graphics_layer =
{
    .layer_type = GFX_LAYER_GRAPHICS,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .uv_buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 512,
    .height = 480,
    .zorder = 0,
    .layer_enable = true,
};
cy_stc_gfx_layer_config_t GFXSS_overlay0_layer =
{
    .layer_type = GFX_LAYER_OVERLAY0,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .uv_buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 1,
    .height = 1,
    .zorder = 0,
    .layer_enable = false,
};
cy_stc_gfx_layer_config_t GFXSS_overlay1_layer =
{
    .layer_type = GFX_LAYER_OVERLAY1,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .uv_buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 1,
    .height = 1,
    .zorder = 0,
    .layer_enable = false,
};
cy_stc_gfx_dc_config_t GFXSS_dc_config =
{
    .gfx_layer_config = &GFXSS_graphics_layer,
    .ovl0_layer_config = &GFXSS_overlay0_layer,
    .ovl1_layer_config = &GFXSS_overlay1_layer,
    .display_type = GFX_DISP_TYPE_DSI_DPI,
    .display_format = vivD24,
    .display_size = vivDISPLAY_CUSTOMIZED,
    .display_width = 512,
    .display_height = 480,
};
cy_stc_gfx_gpu_cfg_t GFXSS_gpu_config =
{
    .enable = true,
};
cy_stc_mipidsi_display_params_t GFXSS_mipidsi_display_params =
{
    .pixel_clock = 16964,
    .hdisplay = 512,
    .hsync_width = 10,
    .hfp = 12,
    .hbp = 15,
    .vdisplay = 480,
    .vsync_width = 8,
    .vfp = 12,
    .vbp = 15,
    .polarity_flags = 0,
};
cy_stc_mipidsi_config_t GFXSS_mipi_dsi_config =
{
    .virtual_ch = 0,
    .num_of_lanes = 2,
    .per_lane_mbps = 500,
    .dpi_fmt = CY_MIPIDSI_FMT_RGB888,
    .dsi_mode = DSI_VIDEO_MODE,
    .max_phy_clk = 2500000000,
    .mode_flags = VID_MODE_TYPE_BURST | ENABLE_LOW_POWER_CMD | ENABLE_LOW_POWER,
    .display_params = &GFXSS_mipidsi_display_params,
};
cy_stc_gfx_config_t GFXSS_config =
{
    .dc_cfg = &GFXSS_dc_config,
    .gpu_cfg = &GFXSS_gpu_config,
    .mipi_dsi_cfg = &GFXSS_mipi_dsi_config,
    .display_update_type = GFX_DOUBLE_BUFFER,
    .clockHz = 399999999U,
};
=======
#define CYBSP_GENERAL_PURPOSE_TIMER_INPUT_DISABLED 0x7U
#define emUSB_OS_Timer_INPUT_DISABLED 0x7U
#define CYBSP_PWM_DISP_BACKLIGHT_INPUT_DISABLED 0x7U

>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
cy_stc_autanalog_cfg_t autonomous_analog_cfg =
{
    .prb = NULL,
    .ac = &CYBSP_AUTONOMOUS_CONTROLLER_cfg,
    .ctb =
    {
        &CYBSP_CTB_0_cfg, 
        NULL, 
    },
    .ptcomp =
    {
        NULL, 
    },
    .dac =
    {
        NULL, 
        NULL, 
    },
    .sar =
    {
        &CYBSP_SAR_ADC_cfg, 
    },
};
cy_stc_autanalog_stt_t autonomous_analog_stt[] =
{
    {
        .ac = &CYBSP_AUTONOMOUS_CONTROLLER_stt[0U],
        .prb = NULL,
        .ctb =
        {
            &CYBSP_CTB_0_stt[0U], 
            NULL, 
        },
        .ptcomp =
        {
            NULL, 
        },
        .dac =
        {
            NULL, 
            NULL, 
        },
        .sar =
        {
            &CYBSP_SAR_ADC_stt[0U], 
        },
    },
    {
        .ac = &CYBSP_AUTONOMOUS_CONTROLLER_stt[1U],
        .prb = NULL,
        .ctb =
        {
            &CYBSP_CTB_0_stt[1U], 
            NULL, 
        },
        .ptcomp =
        {
            NULL, 
        },
        .dac =
        {
            NULL, 
            NULL, 
        },
        .sar =
        {
            &CYBSP_SAR_ADC_stt[1U], 
        },
    },
    {
        .ac = &CYBSP_AUTONOMOUS_CONTROLLER_stt[2U],
        .prb = NULL,
        .ctb =
        {
            &CYBSP_CTB_0_stt[2U], 
            NULL, 
        },
        .ptcomp =
        {
            NULL, 
        },
        .dac =
        {
            NULL, 
            NULL, 
        },
        .sar =
        {
            &CYBSP_SAR_ADC_stt[2U], 
        },
    },
    {
        .ac = &CYBSP_AUTONOMOUS_CONTROLLER_stt[3U],
        .prb = NULL,
        .ctb =
        {
            &CYBSP_CTB_0_stt[3U], 
            NULL, 
        },
        .ptcomp =
        {
            NULL, 
        },
        .dac =
        {
            NULL, 
            NULL, 
        },
        .sar =
        {
            &CYBSP_SAR_ADC_stt[3U], 
        },
    },
    {
        .ac = &CYBSP_AUTONOMOUS_CONTROLLER_stt[4U],
        .prb = NULL,
        .ctb =
        {
            &CYBSP_CTB_0_stt[4U], 
            NULL, 
        },
        .ptcomp =
        {
            NULL, 
        },
        .dac =
        {
            NULL, 
            NULL, 
        },
        .sar =
        {
            &CYBSP_SAR_ADC_stt[4U], 
        },
    },
    {
        .ac = &CYBSP_AUTONOMOUS_CONTROLLER_stt[5U],
        .prb = NULL,
        .ctb =
        {
            &CYBSP_CTB_0_stt[5U], 
            NULL, 
        },
        .ptcomp =
        {
            NULL, 
        },
        .dac =
        {
            NULL, 
            NULL, 
        },
        .sar =
        {
            &CYBSP_SAR_ADC_stt[5U], 
        },
    },
};
cy_stc_autanalog_t autonomous_analog_init =
{
    .configuration = &autonomous_analog_cfg,
    .numSttEntries = sizeof(autonomous_analog_stt)/sizeof(autonomous_analog_stt[0U]),
    .stateTransitionTable = &autonomous_analog_stt[0U],
};
cy_en_autanalog_ac_out_trigger_mask_t CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[] =
{
    CY_AUTANALOG_AC_OUT_TRIG_MASK_EMPTY, 
    CY_AUTANALOG_AC_OUT_TRIG_MASK_EMPTY, 
    CY_AUTANALOG_AC_OUT_TRIG_MASK_EMPTY, 
    CY_AUTANALOG_AC_OUT_TRIG_MASK_EMPTY, 
    CY_AUTANALOG_AC_OUT_TRIG_MASK_EMPTY, 
    CY_AUTANALOG_AC_OUT_TRIG_MASK_EMPTY, 
    CY_AUTANALOG_AC_OUT_TRIG_MASK_EMPTY, 
    CY_AUTANALOG_AC_OUT_TRIG_MASK_EMPTY, 
};
cy_stc_autanalog_ac_t CYBSP_AUTONOMOUS_CONTROLLER_cfg =
{
    .gpioOutEn =   CY_AUTANALOG_STT_AC_GPIO_OUT_DISABLED,
    .mask =
    {
        &CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[0U], 
        &CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[1U], 
        &CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[2U], 
        &CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[3U], 
        &CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[4U], 
        &CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[5U], 
        &CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[6U], 
        &CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[7U], 
    },
    .timer =
    {
        .enable = true,
        .clkSrc = CY_AUTANALOG_TIMER_CLK_LP,
        .period = 255U,
    },
};
cy_stc_autanalog_stt_ac_t CYBSP_AUTONOMOUS_CONTROLLER_stt[] =
{
    {
        .unlock = true,
        .lpMode = true,
        .condition = CY_AUTANALOG_STT_AC_CONDITION_BLOCK_READY,
        .action = CY_AUTANALOG_STT_AC_ACTION_WAIT_FOR,
        .branchState = 1U,
        .trigInt = false,
        .count = 0U,
        .unlockGpioOut = false,
        .gpioOut = CY_AUTANALOG_STT_AC_GPIO_OUT_DISABLED,
    },
    {
        .unlock = false,
        .lpMode = false,
        .condition = CY_AUTANALOG_STT_AC_CONDITION_TR_AUTANALOG_IN0,
        .action = CY_AUTANALOG_STT_AC_ACTION_WAIT_FOR,
        .branchState = 0U,
        .trigInt = false,
        .count = 0U,
        .unlockGpioOut = false,
        .gpioOut = CY_AUTANALOG_STT_AC_GPIO_OUT_DISABLED,
    },
    {
        .unlock = false,
        .lpMode = false,
        .condition = CY_AUTANALOG_STT_AC_CONDITION_TIMER_DONE_WAKE,
        .action = CY_AUTANALOG_STT_AC_ACTION_WAIT_FOR,
        .branchState = 0U,
        .trigInt = false,
        .count = 0U,
        .unlockGpioOut = false,
        .gpioOut = CY_AUTANALOG_STT_AC_GPIO_OUT_DISABLED,
    },
    {
        .unlock = false,
        .lpMode = false,
        .condition = CY_AUTANALOG_STT_AC_CONDITION_BLOCK_READY,
        .action = CY_AUTANALOG_STT_AC_ACTION_WAIT_FOR,
        .branchState = 0U,
        .trigInt = false,
        .count = 0U,
        .unlockGpioOut = false,
        .gpioOut = CY_AUTANALOG_STT_AC_GPIO_OUT_DISABLED,
    },
    {
        .unlock = false,
        .lpMode = false,
        .condition = CY_AUTANALOG_STT_AC_CONDITION_SAR_FIFO_DONE,
        .action = CY_AUTANALOG_STT_AC_ACTION_WAIT_FOR,
        .branchState = 0U,
        .trigInt = false,
        .count = 0U,
        .unlockGpioOut = false,
        .gpioOut = CY_AUTANALOG_STT_AC_GPIO_OUT_DISABLED,
    },
    {
        .unlock = false,
        .lpMode = false,
        .condition = CY_AUTANALOG_STT_AC_CONDITION_TRUE,
        .action = CY_AUTANALOG_STT_AC_ACTION_BRANCH_IF_TRUE,
        .branchState = 2U,
        .trigInt = false,
        .count = 0U,
        .unlockGpioOut = false,
        .gpioOut = CY_AUTANALOG_STT_AC_GPIO_OUT_DISABLED,
    },
};
cy_stc_autanalog_ctb_sta_t CYBSP_CTB_0_sta_cfg =
{
    .pwrOpamp0 = CY_AUTANALOG_CTB_OA_PWR_ULTRA_LOW,
    .pwrOpamp1 = CY_AUTANALOG_CTB_OA_PWR_ULTRA_LOW,
    .topologyOpamp0 = CY_AUTANALOG_CTB_OA_TOPO_OPEN_LOOP_OPAMP,
    .topologyOpamp1 = CY_AUTANALOG_CTB_OA_TOPO_OPEN_LOOP_OPAMP,
    .intComp0 = CY_AUTANALOG_CTB_COMP_INT_EDGE_DISABLED,
    .intComp1 = CY_AUTANALOG_CTB_COMP_INT_EDGE_DISABLED,
    .capCcOpamp0 = CY_AUTANALOG_CTB_OA_CC_CAP_DISABLED,
    .capCcOpamp1 = CY_AUTANALOG_CTB_OA_CC_CAP_2_1_pF,
    .capFbOpamp0 = CY_AUTANALOG_CTB_OA_FB_CAP_0_pF,
    .capFbOpamp1 = CY_AUTANALOG_CTB_OA_FB_CAP_0_pF,
};
cy_stc_autanalog_ctb_dyn_t CYBSP_CTB_0_dyn_cfg[] =
{
    {
        .ninvInpPin = CY_AUTANALOG_CTB_OA_NINV_PIN_OA0_P0_OA1_P5,
        .ninvInpRef = CY_AUTANALOG_CTB_OA_NINV_REF_DISCONNECT,
        .invInpPin = CY_AUTANALOG_CTB_OA_INV_PIN_OA0_P1_OA1_P4,
        .resInpPin = CY_AUTANALOG_CTB_OA_RES_PIN_DISCONNECT,
        .resInpRef = CY_AUTANALOG_CTB_OA_RES_REF_DISCONNECT,
        .sharedMuxIn = CY_AUTANALOG_CTB_OA_MUX_IN_DISCONNECT,
        .sharedMuxOut = CY_AUTANALOG_CTB_OA_MUX_OUT_DISCONNECT,
        .outToPin = true,
    },
    {
        .ninvInpPin = CY_AUTANALOG_CTB_OA_NINV_PIN_OA0_P1_OA1_P4,
        .ninvInpRef = CY_AUTANALOG_CTB_OA_NINV_REF_DISCONNECT,
        .invInpPin = CY_AUTANALOG_CTB_OA_INV_PIN_OA0_P0_OA1_P5,
        .resInpPin = CY_AUTANALOG_CTB_OA_RES_PIN_DISCONNECT,
        .resInpRef = CY_AUTANALOG_CTB_OA_RES_REF_DISCONNECT,
        .sharedMuxIn = CY_AUTANALOG_CTB_OA_MUX_IN_DISCONNECT,
        .sharedMuxOut = CY_AUTANALOG_CTB_OA_MUX_OUT_DISCONNECT,
        .outToPin = true,
    },
};
cy_stc_autanalog_ctb_t CYBSP_CTB_0_cfg =
{
    .ctbStaCfg = &CYBSP_CTB_0_sta_cfg,
    .ctbDynCfgNum = 2U,
    .ctbDynCfgArr = CYBSP_CTB_0_dyn_cfg,
};
cy_stc_autanalog_stt_ctb_t CYBSP_CTB_0_stt[] =
{
    {
        .unlock = true,
        .enableOpamp0 = true,
        .cfgOpamp0 = 0U,
        .gainOpamp0 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
        .enableOpamp1 = true,
        .cfgOpamp1 = 1U,
        .gainOpamp1 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
    },
    {
        .unlock = false,
        .enableOpamp0 = false,
        .cfgOpamp0 = 0U,
        .gainOpamp0 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
        .enableOpamp1 = false,
        .cfgOpamp1 = 0U,
        .gainOpamp1 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
    },
    {
        .unlock = false,
        .enableOpamp0 = false,
        .cfgOpamp0 = 0U,
        .gainOpamp0 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
        .enableOpamp1 = false,
        .cfgOpamp1 = 0U,
        .gainOpamp1 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
    },
    {
        .unlock = false,
        .enableOpamp0 = false,
        .cfgOpamp0 = 0U,
        .gainOpamp0 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
        .enableOpamp1 = false,
        .cfgOpamp1 = 0U,
        .gainOpamp1 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
    },
    {
        .unlock = false,
        .enableOpamp0 = false,
        .cfgOpamp0 = 0U,
        .gainOpamp0 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
        .enableOpamp1 = false,
        .cfgOpamp1 = 0U,
        .gainOpamp1 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
    },
    {
        .unlock = false,
        .enableOpamp0 = false,
        .cfgOpamp0 = 0U,
        .gainOpamp0 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
        .enableOpamp1 = false,
        .cfgOpamp1 = 0U,
        .gainOpamp1 = CY_AUTANALOG_STT_CTB_OA_GAIN_1_00,
    },
};
cy_stc_autanalog_sar_mux_chan_t CYBSP_SAR_ADC_mux_ch_cfg[] =
{
    {
        .posPin = CY_AUTANALOG_SAR_PIN_MUX_CTB0_OA0_OUT,
        .sign = true,
        .posCoeff = CY_AUTANALOG_SAR_CH_COEFF_DISABLED,
        .negPin = CY_AUTANALOG_SAR_PIN_MUX_CTB0_PIN1,
        .buffBypass = false,
        .accShift = false,
        .negCoeff = CY_AUTANALOG_SAR_CH_COEFF_DISABLED,
        .muxLimit = CY_AUTANALOG_SAR_LIMIT_STATUS_DISABLED,
        .fifoSel = CY_AUTANALOG_FIFO_SELECT0,
    },
    {
        .posPin = CY_AUTANALOG_SAR_PIN_MUX_CTB0_OA1_OUT,
        .sign = true,
        .posCoeff = CY_AUTANALOG_SAR_CH_COEFF_DISABLED,
        .negPin = CY_AUTANALOG_SAR_PIN_MUX_CTB0_PIN1,
        .buffBypass = false,
        .accShift = false,
        .negCoeff = CY_AUTANALOG_SAR_CH_COEFF_DISABLED,
        .muxLimit = CY_AUTANALOG_SAR_LIMIT_STATUS_DISABLED,
        .fifoSel = CY_AUTANALOG_FIFO_SELECT1,
    },
};
cy_stc_autanalog_sar_sta_lp_t CYBSP_SAR_ADC_sta_lp_cfg =
{
    .lpDiffEn = false,
    .lpVref = CY_AUTANALOG_SAR_VREF_VDDA_BY_2,
    .lpSampleTime =
    {
        5U, 
        5U, 
        5U, 
        5U, 
    },
};
cy_stc_autanalog_sar_sta_t CYBSP_SAR_ADC_sta_cfg =
{
    .lpStaCfg = &CYBSP_SAR_ADC_sta_lp_cfg,
    .hsStaCfg = NULL,
    .posBufPwr = CY_AUTANALOG_SAR_BUF_PWR_LOW_RAIL,
    .negBufPwr = CY_AUTANALOG_SAR_BUF_PWR_OFF,
    .accMode = CY_AUTANALOG_SAR_ACC_DISABLED,
    .startupCal = CY_AUTANALOG_SAR_CAL_DISABLED,
    .chanID = false,
    .shiftMode = false,
    .intMuxChan =
    {
        &CYBSP_SAR_ADC_mux_ch_cfg[0U], 
        &CYBSP_SAR_ADC_mux_ch_cfg[1U], 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
    },
    .limitCond =
    {
        NULL, 
        NULL, 
        NULL, 
        NULL, 
    },
    .muxResultMask =   CY_AUTANALOG_SAR_CHAN_MASK_MUX0 |   CY_AUTANALOG_SAR_CHAN_MASK_MUX1,
    .firResultMask =   CY_AUTANALOG_SAR_MASK_FIR_DISABLED,
};
cy_stc_autanalog_sar_seq_tab_lp_t CYBSP_SAR_ADC_seq_lp_cfg[] =
{
    {
        .chanEn = true,
        .mux0Sel = CY_AUTANALOG_SAR_CHAN_CFG_MUX0,
        .sampleTimeEn = true,
        .sampleTime = CY_AUTANALOG_SAR_SAMPLE_TIME0,
        .accEn = false,
        .accCount = CY_AUTANALOG_SAR_ACC_CNT2,
        .calReq = CY_AUTANALOG_SAR_CAL_DISABLED,
        .nextAction = CY_AUTANALOG_SAR_NEXT_ACTION_GO_TO_NEXT,
    },
    {
        .chanEn = true,
        .mux0Sel = CY_AUTANALOG_SAR_CHAN_CFG_MUX1,
        .sampleTimeEn = true,
        .sampleTime = CY_AUTANALOG_SAR_SAMPLE_TIME0,
        .accEn = false,
        .accCount = CY_AUTANALOG_SAR_ACC_CNT2,
        .calReq = CY_AUTANALOG_SAR_CAL_DISABLED,
        .nextAction = CY_AUTANALOG_SAR_NEXT_ACTION_STATE_STOP,
    },
};
cy_stc_autanalog_fifo_cfg_t CYBSP_SAR_ADC_fifo_cfg =
{
    .split = CY_AUTANALOG_FIFO_SPLIT2,
    .level =
    {
        159U, 
        159U, 
        0U, 
        0U, 
        0U, 
        0U, 
        0U, 
        0U, 
    },
};
cy_stc_autanalog_sar_t CYBSP_SAR_ADC_cfg =
{
    .sarStaCfg = &CYBSP_SAR_ADC_sta_cfg,
    .hsSeqTabNum = 0U,
    .hsSeqTabArr = NULL,
    .lpSeqTabNum = sizeof(CYBSP_SAR_ADC_seq_lp_cfg)/sizeof(CYBSP_SAR_ADC_seq_lp_cfg[0U]),
    .lpSeqTabArr = &CYBSP_SAR_ADC_seq_lp_cfg[0U],
    .firNum = 0U,
    .firCfg = NULL,
    .fifoCfg = &CYBSP_SAR_ADC_fifo_cfg,
};
cy_stc_autanalog_stt_sar_t CYBSP_SAR_ADC_stt[] =
{
    {
        .unlock = false,
        .enable = false,
        .trigger = false,
        .entryState = 0U,
    },
    {
        .unlock = false,
        .enable = false,
        .trigger = false,
        .entryState = 0U,
    },
    {
        .unlock = false,
        .enable = false,
        .trigger = false,
        .entryState = 0U,
    },
    {
<<<<<<< HEAD
        .unlock = true,
        .enable = true,
=======
        .unlock = false,
        .enable = false,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
        .trigger = false,
        .entryState = 0U,
    },
    {
<<<<<<< HEAD
        .unlock = true,
        .enable = true,
        .trigger = true,
        .entryState = 0U,
    },
    {
        .unlock = true,
=======
        .unlock = false,
        .enable = false,
        .trigger = false,
        .entryState = 0U,
    },
    {
        .unlock = false,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
        .enable = false,
        .trigger = false,
        .entryState = 0U,
    },
};

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_ADC)
mtb_hal_adc_configurator_t CYBSP_SAR_ADC_hal_config =
{
    .config = &CYBSP_SAR_ADC_cfg,
    .num_channels = 0U,
    .clock = NULL,
    .adc_index = 0U,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_ADC) */

<<<<<<< HEAD
=======
cy_stc_gfx_layer_config_t GFXSS_graphics_layer =
{
    .layer_type = GFX_LAYER_GRAPHICS,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .uv_buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 512,
    .height = 480,
    .zorder = 0,
    .layer_enable = true,
    .visibility = true,
};
cy_stc_gfx_layer_config_t GFXSS_overlay0_layer =
{
    .layer_type = GFX_LAYER_OVERLAY0,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .uv_buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 1,
    .height = 1,
    .zorder = 0,
    .layer_enable = false,
    .visibility = false,
};
cy_stc_gfx_layer_config_t GFXSS_overlay1_layer =
{
    .layer_type = GFX_LAYER_OVERLAY1,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .uv_buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 1,
    .height = 1,
    .zorder = 0,
    .layer_enable = false,
    .visibility = false,
};
cy_stc_gfx_dc_config_t GFXSS_dc_config =
{
    .gfx_layer_config = &GFXSS_graphics_layer,
    .ovl0_layer_config = &GFXSS_overlay0_layer,
    .ovl1_layer_config = &GFXSS_overlay1_layer,
    .display_type = GFX_DISP_TYPE_DSI_DPI,
    .display_format = vivD24,
    .display_size = vivDISPLAY_CUSTOMIZED,
    .display_width = 512,
    .display_height = 480,
};
cy_stc_gfx_gpu_cfg_t GFXSS_gpu_config =
{
    .enable = true,
};
cy_stc_mipidsi_display_params_t GFXSS_mipidsi_display_params =
{
    .pixel_clock = 11268,
    .hdisplay = 512,
    .hsync_width = 8,
    .hfp = 12,
    .hbp = 15,
    .vdisplay = 480,
    .vsync_width = 8,
    .vfp = 12,
    .vbp = 15,
    .polarity_flags = 0,
};
cy_stc_mipidsi_config_t GFXSS_mipi_dsi_config =
{
    .virtual_ch = 0,
    .num_of_lanes = 2,
    .per_lane_mbps = 500,
    .dpi_fmt = CY_MIPIDSI_FMT_RGB888,
    .dsi_mode = DSI_VIDEO_MODE,
    .max_phy_clk = 2500000000,
    .mode_flags = VID_MODE_TYPE_BURST | ENABLE_LOW_POWER_CMD | ENABLE_LOW_POWER,
    .display_params = &GFXSS_mipidsi_display_params,
};
cy_stc_gfx_config_t GFXSS_config =
{
    .dc_cfg = &GFXSS_dc_config,
    .gpu_cfg = &GFXSS_gpu_config,
    .mipi_dsi_cfg = &GFXSS_mipi_dsi_config,
    .display_update_type = GFX_DOUBLE_BUFFER,
    .clockHz = 399999999U,
};
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
const cy_stc_pdm_pcm_config_v2_t CYBSP_PDM_config =
{
    .clkDiv = 7,
    .clksel = CY_PDM_PCM_SEL_SRSS_CLOCK,
    .halverate = CY_PDM_PCM_RATE_FULL,
    .route = 4,
    .fir0_coeff_user_value = false,
    .fir1_coeff_user_value = false,
};
const cy_stc_pdm_pcm_channel_config_t channel_2_config =
{
    .sampledelay = 1,
    .wordSize = CY_PDM_PCM_WSIZE_16_BIT,
    .signExtension = true,
    .rxFifoTriggerLevel = 31,
    .fir0_enable = false,
    .cic_decim_code = CY_PDM_PCM_CHAN_CIC_DECIM_32,
    .fir0_decim_code = CY_PDM_PCM_CHAN_FIR0_DECIM_1,
    .fir0_scale = 0,
    .fir1_decim_code = CY_PDM_PCM_CHAN_FIR1_DECIM_3,
    .fir1_scale = 10,
    .dc_block_disable = false,
    .dc_block_code = CY_PDM_PCM_CHAN_DCBLOCK_CODE_16,
};
const cy_stc_pdm_pcm_channel_config_t channel_3_config =
{
    .sampledelay = 5,
    .wordSize = CY_PDM_PCM_WSIZE_16_BIT,
    .signExtension = true,
    .rxFifoTriggerLevel = 31,
    .fir0_enable = false,
    .cic_decim_code = CY_PDM_PCM_CHAN_CIC_DECIM_32,
    .fir0_decim_code = CY_PDM_PCM_CHAN_FIR0_DECIM_1,
    .fir0_scale = 0,
    .fir1_decim_code = CY_PDM_PCM_CHAN_FIR1_DECIM_3,
    .fir1_scale = 10,
    .dc_block_disable = false,
    .dc_block_code = CY_PDM_PCM_CHAN_DCBLOCK_CODE_16,
};
<<<<<<< HEAD

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_PDM_channel_2_obj =
{
    .type = CYHAL_RSC_PDM,
    .block_num = 0U,
    .channel_num = 2U,
};
const cyhal_resource_inst_t CYBSP_PDM_channel_3_obj =
{
    .type = CYHAL_RSC_PDM,
    .block_num = 0U,
    .channel_num = 3U,
};
#endif /* defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) && !defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK)
const cyhal_clock_t CYBSP_PDM_clock =
{
    .block = CYHAL_CLOCK_BLOCK1_PERIPHERAL1_16_5BIT,
    .channel = 1,
    .reserved = false,
    .funcs = NULL,
};
#endif /* defined (CY_USING_HAL) && !defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK) */

#if defined (CY_USING_HAL) && defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK)
cyhal_clock_t CYBSP_PDM_clock;
#endif /* defined (CY_USING_HAL) && defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK) */

#if defined (CY_USING_HAL)
const cyhal_pdm_pcm_configurator_t CYBSP_PDM_channel_2_hal_config =
{
    .resource = &CYBSP_PDM_channel_2_obj,
    .config = &CYBSP_PDM_config,
    .chan_config = &channel_2_config,
    .clock = &CYBSP_PDM_clock,
};
const cyhal_pdm_pcm_configurator_t CYBSP_PDM_channel_3_hal_config =
{
    .resource = &CYBSP_PDM_channel_3_obj,
    .config = &CYBSP_PDM_config,
    .chan_config = &channel_3_config,
    .clock = &CYBSP_PDM_clock,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_scb_i2c_config_t CYBSP_I2C_CONTROLLER_0_config =
=======
const cy_stc_scb_i2c_config_t CYBSP_I2C_CONTROLLER_config =
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
{
    .i2cMode = CY_SCB_I2C_MASTER,
    .useRxFifo = true,
    .useTxFifo = true,
    .slaveAddress = 0U,
    .slaveAddressMask = 0U,
    .acceptAddrInFifo = false,
    .ackGeneralAddr = false,
    .enableWakeFromSleep = false,
    .enableDigitalFilter = false,
    .lowPhaseDutyCycle = 16,
    .highPhaseDutyCycle = 9,
};

<<<<<<< HEAD
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2C_CONTROLLER_0_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 0U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
const cyhal_clock_t CYBSP_I2C_CONTROLLER_0_clock =
{
    .block = CYHAL_CLOCK_BLOCK0_PERIPHERAL1_16BIT,
    .channel = 0,
#if defined (CY_USING_HAL)
    .reserved = false,
    .funcs = NULL,
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_i2c_configurator_t CYBSP_I2C_CONTROLLER_0_hal_config =
{
    .resource = &CYBSP_I2C_CONTROLLER_0_obj,
    .config = &CYBSP_I2C_CONTROLLER_0_config,
    .clock = &CYBSP_I2C_CONTROLLER_0_clock,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_I2C_CONTROLLER_0_clock_ref =
{
    .clk_dst = (en_clk_dst_t)CYBSP_I2C_CONTROLLER_0_CLK_DIV_GRP_NUM,
    .div_type = CYBSP_I2C_CONTROLLER_0_CLK_DIV_HW,
    .div_num = CYBSP_I2C_CONTROLLER_0_CLK_DIV_NUM,
};
const mtb_hal_clock_t CYBSP_I2C_CONTROLLER_0_hal_clock =
{
    .clock_ref = &CYBSP_I2C_CONTROLLER_0_clock_ref,
=======
#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_I2C_CONTROLLER_clock_ref =
{
    .clk_dst = (en_clk_dst_t)CYBSP_I2C_CONTROLLER_CLK_DIV_GRP_NUM,
    .div_type = CYBSP_I2C_CONTROLLER_CLK_DIV_HW,
    .div_num = CYBSP_I2C_CONTROLLER_CLK_DIV_NUM,
};
const mtb_hal_clock_t CYBSP_I2C_CONTROLLER_hal_clock =
{
    .clock_ref = &CYBSP_I2C_CONTROLLER_clock_ref,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C)
<<<<<<< HEAD
const mtb_hal_i2c_configurator_t CYBSP_I2C_CONTROLLER_0_hal_config =
{
    .base = CYBSP_I2C_CONTROLLER_0_HW,
    .config = &CYBSP_I2C_CONTROLLER_0_config,
    .clock = &CYBSP_I2C_CONTROLLER_0_hal_clock,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C) */

const cy_stc_scb_spi_config_t CYBSP_SPI_CONTROLLER_10_config =
{
    .spiMode = CY_SCB_SPI_MASTER,
    .subMode = CY_SCB_SPI_MOTOROLA,
    .sclkMode = CY_SCB_SPI_CPHA0_CPOL0,
    .parity = CY_SCB_SPI_PARITY_NONE,
    .dropOnParityError = false,
    .oversample = 16,
    .rxDataWidth = 8UL,
    .txDataWidth = 8UL,
    .enableMsbFirst = true,
    .enableInputFilter = false,
    .enableFreeRunSclk = false,
    .enableMisoLateSample = true,
    .enableTransferSeperation = false,
    .ssPolarity = ((CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT0) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT1) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT2) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT3)),
    .ssSetupDelay = false,
    .ssHoldDelay = false,
    .ssInterFrameDelay = false,
    .enableWakeFromSleep = false,
    .rxFifoTriggerLevel = 63UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 63UL,
    .txFifoIntEnableMask = 0UL,
    .masterSlaveIntEnableMask = 0UL,
};

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SPI_CONTROLLER_10_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 10U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
const cyhal_clock_t CYBSP_SPI_CONTROLLER_10_clock =
{
    .block = CYHAL_CLOCK_BLOCK0_PERIPHERAL1_8BIT,
    .channel = 1,
#if defined (CY_USING_HAL)
    .reserved = false,
    .funcs = NULL,
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_spi_configurator_t CYBSP_SPI_CONTROLLER_10_hal_config =
{
    .resource = &CYBSP_SPI_CONTROLLER_10_obj,
    .config = &CYBSP_SPI_CONTROLLER_10_config,
    .clock = &CYBSP_SPI_CONTROLLER_10_clock,
    .gpios = {.sclk = P16_0, .ssel = {P16_3, P16_4, NC, NC}, .mosi = P16_1, .miso = P16_2},
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_SPI_CONTROLLER_10_clock_ref =
{
    .clk_dst = (en_clk_dst_t)PCLK_SCB10_CLOCK_SCB_EN,
    .div_type = CY_SYSCLK_DIV_8_BIT,
    .div_num = 1,
};
const mtb_hal_clock_t CYBSP_SPI_CONTROLLER_10_hal_clock =
{
    .clock_ref = &CYBSP_SPI_CONTROLLER_10_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI)
const mtb_hal_spi_configurator_t CYBSP_SPI_CONTROLLER_10_hal_config =
{
    .base = CYBSP_SPI_CONTROLLER_10_HW,
    .clock = &CYBSP_SPI_CONTROLLER_10_hal_clock,
    .config = &CYBSP_SPI_CONTROLLER_10_config,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI) */

const cy_stc_scb_i2c_config_t CYBSP_I2C_CONTROLLER_11_config =
{
    .i2cMode = CY_SCB_I2C_MASTER,
    .useRxFifo = true,
    .useTxFifo = true,
    .slaveAddress = 0U,
    .slaveAddressMask = 0U,
    .acceptAddrInFifo = false,
    .ackGeneralAddr = false,
    .enableWakeFromSleep = false,
    .enableDigitalFilter = false,
    .lowPhaseDutyCycle = 16,
    .highPhaseDutyCycle = 9,
};

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_I2C_CONTROLLER_11_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 11U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
const cyhal_clock_t CYBSP_I2C_CONTROLLER_11_clock =
{
    .block = CYHAL_CLOCK_BLOCK0_PERIPHERAL1_8BIT,
    .channel = 4,
#if defined (CY_USING_HAL)
    .reserved = false,
    .funcs = NULL,
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_i2c_configurator_t CYBSP_I2C_CONTROLLER_11_hal_config =
{
    .resource = &CYBSP_I2C_CONTROLLER_11_obj,
    .config = &CYBSP_I2C_CONTROLLER_11_config,
    .clock = &CYBSP_I2C_CONTROLLER_11_clock,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_I2C_CONTROLLER_11_clock_ref =
{
    .clk_dst = (en_clk_dst_t)CYBSP_I2C_3V3_CLK_DIV_GRP_NUM,
    .div_type = CYBSP_I2C_3V3_CLK_DIV_HW,
    .div_num = CYBSP_I2C_3V3_CLK_DIV_NUM,
};
const mtb_hal_clock_t CYBSP_I2C_CONTROLLER_11_hal_clock =
{
    .clock_ref = &CYBSP_I2C_CONTROLLER_11_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C)
const mtb_hal_i2c_configurator_t CYBSP_I2C_CONTROLLER_11_hal_config =
{
    .base = CYBSP_I2C_CONTROLLER_11_HW,
    .config = &CYBSP_I2C_CONTROLLER_11_config,
    .clock = &CYBSP_I2C_CONTROLLER_11_hal_clock,
=======
const mtb_hal_i2c_configurator_t CYBSP_I2C_CONTROLLER_hal_config =
{
    .base = CYBSP_I2C_CONTROLLER_HW,
    .config = &CYBSP_I2C_CONTROLLER_config,
    .clock = &CYBSP_I2C_CONTROLLER_hal_clock,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C) */

const cy_stc_scb_uart_config_t CYBSP_DEBUG_UART_config =
{
    .uartMode = CY_SCB_UART_STANDARD,
<<<<<<< HEAD
    .enableMutliProcessorMode = false,
=======
    .enableMultiProcessorMode = false,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .smartCardRetryOnNack = false,
    .irdaInvertRx = false,
    .irdaEnableLowPowerReceiver = false,
    .oversample = 10,
    .enableMsbFirst = false,
    .dataWidth = 8UL,
    .parity = CY_SCB_UART_PARITY_NONE,
    .stopBits = CY_SCB_UART_STOP_BITS_1,
    .enableInputFilter = false,
    .breakWidth = 11UL,
    .dropOnFrameError = false,
    .dropOnParityError = false,
    .breaklevel = false,
    .receiverAddress = 0x0UL,
    .receiverAddressMask = 0x0UL,
    .acceptAddrInFifo = false,
    .enableCts = false,
    .ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
    .rtsRxFifoLevel = 0UL,
    .rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
    .rxFifoTriggerLevel = 63UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 63UL,
    .txFifoIntEnableMask = 0UL,
};

<<<<<<< HEAD
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_DEBUG_UART_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 2U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
const cyhal_clock_t CYBSP_DEBUG_UART_clock =
{
    .block = CYHAL_CLOCK_BLOCK0_PERIPHERAL1_16BIT,
    .channel = 1,
#if defined (CY_USING_HAL)
    .reserved = false,
    .funcs = NULL,
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_uart_configurator_t CYBSP_DEBUG_UART_hal_config =
{
    .resource = &CYBSP_DEBUG_UART_obj,
    .config = &CYBSP_DEBUG_UART_config,
    .clock = &CYBSP_DEBUG_UART_clock,
#if defined (CY_USING_HAL)
    .gpios = {.pin_tx = P6_7, .pin_rts = NC, .pin_cts = NC},
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_DEBUG_UART_clock_ref =
{
    .clk_dst = (en_clk_dst_t)CYBSP_DEBUG_UART_CLK_DIV_GRP_NUM,
    .div_type = CYBSP_DEBUG_UART_CLK_DIV_HW,
    .div_num = CYBSP_DEBUG_UART_CLK_DIV_NUM,
};
const mtb_hal_clock_t CYBSP_DEBUG_UART_hal_clock =
{
    .clock_ref = &CYBSP_DEBUG_UART_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_UART)
const mtb_hal_uart_configurator_t CYBSP_DEBUG_UART_hal_config =
{
    .base = CYBSP_DEBUG_UART_HW,
    .clock = &CYBSP_DEBUG_UART_hal_clock,
    .tx_pin = 7,
#if defined (COMPONENT_MW_ASYNC_TRANSFER)
    .rts_pin = 0xFF,
#endif /* defined (COMPONENT_MW_ASYNC_TRANSFER) */
    .tx_port = 6,
#if defined (COMPONENT_MW_ASYNC_TRANSFER)
    .rts_port = 0xFF,
    .rts_enable = 0UL,
#endif /* defined (COMPONENT_MW_ASYNC_TRANSFER) */
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_UART) */

<<<<<<< HEAD
const cy_stc_scb_spi_config_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_config =
=======
const cy_stc_scb_spi_config_t CYBSP_SPI_CONTROLLER_2_config =
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
{
    .spiMode = CY_SCB_SPI_MASTER,
    .subMode = CY_SCB_SPI_MOTOROLA,
    .sclkMode = CY_SCB_SPI_CPHA0_CPOL0,
    .parity = CY_SCB_SPI_PARITY_NONE,
    .dropOnParityError = false,
    .oversample = 16,
    .rxDataWidth = 8UL,
    .txDataWidth = 8UL,
    .enableMsbFirst = true,
    .enableInputFilter = false,
    .enableFreeRunSclk = false,
    .enableMisoLateSample = true,
<<<<<<< HEAD
    .enableTransferSeperation = false,
=======
    .enableTransferSeparation = false,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .ssPolarity = ((CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT0) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT1) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT2) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT3)),
    .ssSetupDelay = false,
    .ssHoldDelay = false,
    .ssInterFrameDelay = false,
    .enableWakeFromSleep = false,
    .rxFifoTriggerLevel = 63UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 63UL,
    .txFifoIntEnableMask = 0UL,
    .masterSlaveIntEnableMask = 0UL,
};

<<<<<<< HEAD
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 3U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
const cyhal_clock_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_clock =
{
    .block = CYHAL_CLOCK_BLOCK0_PERIPHERAL1_8BIT,
    .channel = 0,
#if defined (CY_USING_HAL)
    .reserved = false,
    .funcs = NULL,
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_spi_configurator_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_hal_config =
{
    .resource = &CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_obj,
    .config = &CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_config,
    .clock = &CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_clock,
    .gpios = {.sclk = P21_6, .ssel = {P0_0, NC, NC, NC}, .mosi = P21_5, .miso = P21_4},
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_clock_ref =
=======
#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_SPI_CONTROLLER_2_clock_ref =
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
{
    .clk_dst = (en_clk_dst_t)PCLK_SCB3_CLOCK_SCB_EN,
    .div_type = CY_SYSCLK_DIV_8_BIT,
    .div_num = 0,
};
<<<<<<< HEAD
const mtb_hal_clock_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_hal_clock =
{
    .clock_ref = &CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_clock_ref,
=======
const mtb_hal_clock_t CYBSP_SPI_CONTROLLER_2_hal_clock =
{
    .clock_ref = &CYBSP_SPI_CONTROLLER_2_clock_ref,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI)
<<<<<<< HEAD
const mtb_hal_spi_configurator_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_hal_config =
{
    .base = CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_HW,
    .clock = &CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_hal_clock,
    .config = &CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_config,
=======
const mtb_hal_spi_configurator_t CYBSP_SPI_CONTROLLER_2_hal_config =
{
    .base = CYBSP_SPI_CONTROLLER_2_HW,
    .clock = &CYBSP_SPI_CONTROLLER_2_hal_clock,
    .config = &CYBSP_SPI_CONTROLLER_2_config,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI) */

const cy_stc_scb_uart_config_t CYBSP_BT_UART_config =
{
    .uartMode = CY_SCB_UART_STANDARD,
<<<<<<< HEAD
    .enableMutliProcessorMode = false,
=======
    .enableMultiProcessorMode = false,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .smartCardRetryOnNack = false,
    .irdaInvertRx = false,
    .irdaEnableLowPowerReceiver = false,
    .oversample = 8,
    .enableMsbFirst = false,
    .dataWidth = 8UL,
    .parity = CY_SCB_UART_PARITY_NONE,
    .stopBits = CY_SCB_UART_STOP_BITS_1,
    .enableInputFilter = false,
    .breakWidth = 11UL,
    .dropOnFrameError = false,
    .dropOnParityError = false,
    .breaklevel = false,
    .receiverAddress = 0x0UL,
    .receiverAddressMask = 0x0UL,
    .acceptAddrInFifo = false,
    .enableCts = true,
    .ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
    .rtsRxFifoLevel = 63,
    .rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
    .rxFifoTriggerLevel = 63UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 63UL,
    .txFifoIntEnableMask = 0UL,
};

<<<<<<< HEAD
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_BT_UART_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 4U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
const cyhal_clock_t CYBSP_BT_UART_clock =
{
    .block = CYHAL_CLOCK_BLOCK0_PERIPHERAL1_16_5BIT,
    .channel = 0,
#if defined (CY_USING_HAL)
    .reserved = false,
    .funcs = NULL,
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_uart_configurator_t CYBSP_BT_UART_hal_config =
{
    .resource = &CYBSP_BT_UART_obj,
    .config = &CYBSP_BT_UART_config,
    .clock = &CYBSP_BT_UART_clock,
#if defined (CY_USING_HAL)
    .gpios = {.pin_tx = P10_1, .pin_rts = P10_3, .pin_cts = P10_2},
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_BT_UART_clock_ref =
{
    .clk_dst = (en_clk_dst_t)CYBSP_BT_UART_CLK_DIV_GRP_NUM,
    .div_type = CYBSP_BT_UART_CLK_DIV_HW,
    .div_num = CYBSP_BT_UART_CLK_DIV_NUM,
};
const mtb_hal_clock_t CYBSP_BT_UART_hal_clock =
{
    .clock_ref = &CYBSP_BT_UART_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_UART)
const mtb_hal_uart_configurator_t CYBSP_BT_UART_hal_config =
{
    .base = CYBSP_BT_UART_HW,
    .clock = &CYBSP_BT_UART_hal_clock,
    .tx_pin = 1,
#if defined (COMPONENT_MW_ASYNC_TRANSFER)
    .rts_pin = 3,
#endif /* defined (COMPONENT_MW_ASYNC_TRANSFER) */
    .tx_port = 10,
#if defined (COMPONENT_MW_ASYNC_TRANSFER)
    .rts_port = 10,
    .rts_enable = 1UL,
#endif /* defined (COMPONENT_MW_ASYNC_TRANSFER) */
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_UART) */

<<<<<<< HEAD
=======
const cy_stc_scb_spi_config_t CYBSP_SPI_CONTROLLER_config =
{
    .spiMode = CY_SCB_SPI_MASTER,
    .subMode = CY_SCB_SPI_MOTOROLA,
    .sclkMode = CY_SCB_SPI_CPHA0_CPOL0,
    .parity = CY_SCB_SPI_PARITY_NONE,
    .dropOnParityError = false,
    .oversample = 16,
    .rxDataWidth = 8UL,
    .txDataWidth = 8UL,
    .enableMsbFirst = true,
    .enableInputFilter = false,
    .enableFreeRunSclk = false,
    .enableMisoLateSample = true,
    .enableTransferSeparation = false,
    .ssPolarity = ((CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT0) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT1) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT2) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT3)),
    .ssSetupDelay = false,
    .ssHoldDelay = false,
    .ssInterFrameDelay = false,
    .enableWakeFromSleep = false,
    .rxFifoTriggerLevel = 63UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 63UL,
    .txFifoIntEnableMask = 0UL,
    .masterSlaveIntEnableMask = 0UL,
};

#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_SPI_CONTROLLER_clock_ref =
{
    .clk_dst = (en_clk_dst_t)PCLK_SCB10_CLOCK_SCB_EN,
    .div_type = CY_SYSCLK_DIV_8_BIT,
    .div_num = 1,
};
const mtb_hal_clock_t CYBSP_SPI_CONTROLLER_hal_clock =
{
    .clock_ref = &CYBSP_SPI_CONTROLLER_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI)
const mtb_hal_spi_configurator_t CYBSP_SPI_CONTROLLER_hal_config =
{
    .base = CYBSP_SPI_CONTROLLER_HW,
    .clock = &CYBSP_SPI_CONTROLLER_hal_clock,
    .config = &CYBSP_SPI_CONTROLLER_config,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI) */

const cy_stc_scb_i2c_config_t CYBSP_I2C_CONTROLLER_2_config =
{
    .i2cMode = CY_SCB_I2C_MASTER,
    .useRxFifo = true,
    .useTxFifo = true,
    .slaveAddress = 0U,
    .slaveAddressMask = 0U,
    .acceptAddrInFifo = false,
    .ackGeneralAddr = false,
    .enableWakeFromSleep = false,
    .enableDigitalFilter = false,
    .lowPhaseDutyCycle = 16,
    .highPhaseDutyCycle = 16,
};

#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_I2C_CONTROLLER_2_clock_ref =
{
    .clk_dst = (en_clk_dst_t)CYBSP_I2C_3V3_CLK_DIV_GRP_NUM,
    .div_type = CYBSP_I2C_3V3_CLK_DIV_HW,
    .div_num = CYBSP_I2C_3V3_CLK_DIV_NUM,
};
const mtb_hal_clock_t CYBSP_I2C_CONTROLLER_2_hal_clock =
{
    .clock_ref = &CYBSP_I2C_CONTROLLER_2_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C)
const mtb_hal_i2c_configurator_t CYBSP_I2C_CONTROLLER_2_hal_config =
{
    .base = CYBSP_I2C_CONTROLLER_2_HW,
    .config = &CYBSP_I2C_CONTROLLER_2_config,
    .clock = &CYBSP_I2C_CONTROLLER_2_hal_clock,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C) */

>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
cy_en_sd_host_card_capacity_t CYBSP_WIFI_SDIO_cardCapacity = CY_SD_HOST_SDSC;
cy_en_sd_host_card_type_t CYBSP_WIFI_SDIO_cardType = CY_SD_HOST_NOT_EMMC;
uint32_t CYBSP_WIFI_SDIO_rca = 0u;
const cy_stc_sd_host_init_config_t CYBSP_WIFI_SDIO_config =
{
    .emmc = false,
    .dmaType = CY_SD_HOST_DMA_ADMA2,
    .enableLedControl = false,
};
cy_stc_sd_host_sd_card_config_t CYBSP_WIFI_SDIO_card_cfg =
{
    .lowVoltageSignaling = false,
    .busWidth = CY_SD_HOST_BUS_WIDTH_4_BIT,
    .cardType = &CYBSP_WIFI_SDIO_cardType,
    .rca = &CYBSP_WIFI_SDIO_rca,
    .cardCapacity = &CYBSP_WIFI_SDIO_cardCapacity,
};

<<<<<<< HEAD
#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_WIFI_SDIO_obj =
{
    .type = CYHAL_RSC_SDHC,
    .block_num = 0U,
    .channel_num = 0U,
};
const cyhal_clock_t CYBSP_WIFI_SDIO_clock =
{
    .block = CYHAL_CLOCK_BLOCK1_PERIPHERAL2_8BIT,
    .channel = 0,
    .reserved = false,
    .funcs = NULL,
};
const cyhal_sdhc_configurator_t CYBSP_WIFI_SDIO_sdhc_hal_config =
{
    .resource = &CYBSP_WIFI_SDIO_obj,
    .host_config = &CYBSP_WIFI_SDIO_config,
    .card_config = &CYBSP_WIFI_SDIO_card_cfg,
    .gpios = {.clk = P12_0, .cmd = P21_0, .data = {P12_1, P12_2, P12_4, P12_5, NC, NC, NC, NC}, .card_detect = NC, .card_mech_write_prot = NC, .card_pwr_en = NC, .io_volt_sel = NC, .led_ctrl = NC, .emmc_reset = NC},
};
const cyhal_sdio_configurator_t CYBSP_WIFI_SDIO_sdio_hal_config =
{
    .resource = &CYBSP_WIFI_SDIO_obj,
    .host_config = &CYBSP_WIFI_SDIO_config,
    .card_config = &CYBSP_WIFI_SDIO_card_cfg,
    .gpios = {.clk = P12_0, .cmd = P21_0,  .data = {P12_1, P12_2, P12_4, P12_5}},
};
#endif /* defined (CY_USING_HAL) */

=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDHC)
const mtb_hal_peri_div_t CYBSP_WIFI_SDIO_clock_ref =
{
    .clk_dst = (en_clk_dst_t)CYBSP_SDHC_0_CLK_DIV_GRP_NUM,
    .div_type = CYBSP_SDHC_0_CLK_DIV_HW,
    .div_num = CYBSP_SDHC_0_CLK_DIV_NUM,
};
const mtb_hal_clock_t CYBSP_WIFI_SDIO_hal_clock =
{
    .clock_ref = &CYBSP_WIFI_SDIO_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
const mtb_hal_sdhc_configurator_t CYBSP_WIFI_SDIO_sdhc_hal_config =
{
    .base = CYBSP_WIFI_SDIO_HW,
    .host_config = &CYBSP_WIFI_SDIO_config,
    .card_config = &CYBSP_WIFI_SDIO_card_cfg,
    .clock = &CYBSP_WIFI_SDIO_hal_clock,
    .gpios = {.clk = { .pin_num = 0, .port_num = 12}, .cmd = { .pin_num = 0, .port_num = 21}, .data = {{ .pin_num = 1, .port_num = 12}, { .pin_num = 2, .port_num = 12}, { .pin_num = 4, .port_num = 12}, { .pin_num = 5, .port_num = 12}, { .pin_num = NC, .port_num = NC}, { .pin_num = NC, .port_num = NC}, { .pin_num = NC, .port_num = NC}, { .pin_num = NC, .port_num = NC}}},
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDHC) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDIO)
const mtb_hal_sdio_configurator_t CYBSP_WIFI_SDIO_sdio_hal_config =
{
    .base = CYBSP_WIFI_SDIO_HW,
    .host_config = &CYBSP_WIFI_SDIO_config,
    .clock = &CYBSP_WIFI_SDIO_hal_clock,
    .gpios = {.clk = { .pin_num = 0, .port_num = 12}, .cmd = { .pin_num = 0, .port_num = 21}, .data = {{ .pin_num = 1, .port_num = 12}, { .pin_num = 2, .port_num = 12}, { .pin_num = 4, .port_num = 12}, { .pin_num = 5, .port_num = 12}}},
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDIO) */

cy_en_sd_host_card_capacity_t CYBSP_SDHC_1_cardCapacity = CY_SD_HOST_SDSC;
cy_en_sd_host_card_type_t CYBSP_SDHC_1_cardType = CY_SD_HOST_NOT_EMMC;
uint32_t CYBSP_SDHC_1_rca = 0u;
const cy_stc_sd_host_init_config_t CYBSP_SDHC_1_config =
{
    .emmc = false,
    .dmaType = CY_SD_HOST_DMA_ADMA2,
    .enableLedControl = false,
};
cy_stc_sd_host_sd_card_config_t CYBSP_SDHC_1_card_cfg =
{
    .lowVoltageSignaling = false,
    .busWidth = CY_SD_HOST_BUS_WIDTH_4_BIT,
    .cardType = &CYBSP_SDHC_1_cardType,
    .rca = &CYBSP_SDHC_1_rca,
    .cardCapacity = &CYBSP_SDHC_1_cardCapacity,
};

<<<<<<< HEAD
#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_SDHC_1_obj =
{
    .type = CYHAL_RSC_SDHC,
    .block_num = 1U,
    .channel_num = 0U,
};
const cyhal_clock_t CYBSP_SDHC_1_clock =
{
    .block = CYHAL_CLOCK_BLOCK1_PERIPHERAL3_8BIT,
    .channel = 0,
    .reserved = false,
    .funcs = NULL,
};
const cyhal_sdhc_configurator_t CYBSP_SDHC_1_sdhc_hal_config =
{
    .resource = &CYBSP_SDHC_1_obj,
    .host_config = &CYBSP_SDHC_1_config,
    .card_config = &CYBSP_SDHC_1_card_cfg,
    .gpios = {.clk = P7_1, .cmd = P7_0, .data = {P7_3, P7_5, P7_6, P7_7, NC, NC, NC, NC}, .card_detect = NC, .card_mech_write_prot = NC, .card_pwr_en = NC, .io_volt_sel = NC, .led_ctrl = NC, .emmc_reset = NC},
};
const cyhal_sdio_configurator_t CYBSP_SDHC_1_sdio_hal_config =
{
    .resource = &CYBSP_SDHC_1_obj,
    .host_config = &CYBSP_SDHC_1_config,
    .card_config = &CYBSP_SDHC_1_card_cfg,
    .gpios = {.clk = P7_1, .cmd = P7_0,  .data = {P7_3, P7_5, P7_6, P7_7}},
};
#endif /* defined (CY_USING_HAL) */

=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDHC)
const mtb_hal_peri_div_t CYBSP_SDHC_1_clock_ref =
{
    .clk_dst = (en_clk_dst_t)CYBSP_SDHC_1_CLK_DIV_GRP_NUM,
    .div_type = CYBSP_SDHC_1_CLK_DIV_HW,
    .div_num = CYBSP_SDHC_1_CLK_DIV_NUM,
};
const mtb_hal_clock_t CYBSP_SDHC_1_hal_clock =
{
    .clock_ref = &CYBSP_SDHC_1_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
const mtb_hal_sdhc_configurator_t CYBSP_SDHC_1_sdhc_hal_config =
{
    .base = CYBSP_SDHC_1_HW,
    .host_config = &CYBSP_SDHC_1_config,
    .card_config = &CYBSP_SDHC_1_card_cfg,
    .clock = &CYBSP_SDHC_1_hal_clock,
    .gpios = {.clk = { .pin_num = 1, .port_num = 7}, .cmd = { .pin_num = 0, .port_num = 7}, .data = {{ .pin_num = 3, .port_num = 7}, { .pin_num = 5, .port_num = 7}, { .pin_num = 6, .port_num = 7}, { .pin_num = 7, .port_num = 7}, { .pin_num = NC, .port_num = NC}, { .pin_num = NC, .port_num = NC}, { .pin_num = NC, .port_num = NC}, { .pin_num = NC, .port_num = NC}}},
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDHC) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDIO)
const mtb_hal_sdio_configurator_t CYBSP_SDHC_1_sdio_hal_config =
{
    .base = CYBSP_SDHC_1_HW,
    .host_config = &CYBSP_SDHC_1_config,
    .clock = &CYBSP_SDHC_1_hal_clock,
    .gpios = {.clk = { .pin_num = 1, .port_num = 7}, .cmd = { .pin_num = 0, .port_num = 7}, .data = {{ .pin_num = 3, .port_num = 7}, { .pin_num = 5, .port_num = 7}, { .pin_num = 6, .port_num = 7}, { .pin_num = 7, .port_num = 7}}},
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDIO) */

const cy_stc_smif_config_t CYBSP_SMIF_CORE_0_XSPI_FLASH_config =
{
    .mode = (uint32_t)CY_SMIF_NORMAL,
    .deselectDelay = CYBSP_SMIF_CORE_0_XSPI_FLASH_DESELECT_DELAY,
    .blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
    .inputFrequencyMHz = 200,
<<<<<<< HEAD
    .enable_internal_dll = false,
    .dll_divider_value = CY_SMIF_DLL_DIVIDE_BY_2,
    .rx_capture_mode = CY_SMIF_SEL_NORMAL_SPI,
=======
    .enable_internal_dll = true,
    .dll_divider_value = CY_SMIF_DLL_DIVIDE_BY_2,
    .rx_capture_mode = CY_SMIF_SEL_XSPI_HYPERBUS_WITH_DQS,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .mdl_tap = CY_SMIF_MDL_8_TAP_DELAY,
    .device0_sdl_tap = CY_SMIF_SDL_8_TAP_DELAY,
    .device1_sdl_tap = CY_SMIF_SDL_8_TAP_DELAY,
    .device2_sdl_tap = CY_SMIF_SDL_8_TAP_DELAY,
    .device3_sdl_tap = CY_SMIF_SDL_8_TAP_DELAY,
    .tx_sdr_extra = CY_SMIF_TX_TWO_PERIOD_AHEAD,
};

<<<<<<< HEAD
#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_SMIF_CORE_0_XSPI_FLASH_obj =
{
    .type = CYHAL_RSC_SMIF,
    .block_num = 0U,
    .channel_num = 0U,
};
const cyhal_qspi_configurator_t CYBSP_SMIF_CORE_0_XSPI_FLASH_hal_config =
{
    .resource = &CYBSP_SMIF_CORE_0_XSPI_FLASH_obj,
    .config = &CYBSP_SMIF_CORE_0_XSPI_FLASH_config,
    .clock = &CYHAL_CLOCK_HF[3],
    .gpios = {.sclk = NC, .ssel = {NC, P2_0, NC, NC}, .io = {P1_0, P1_1, P1_2, P1_3, NC, NC, NC, NC},},
    .irqs = CYBSP_SMIF_CORE_0_XSPI_FLASH_MEMORY_MODE_ALIGMENT_ERROR << 5 | CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_DATA_FIFO_UNDERFLOW << 4 | CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_COMMAND_FIFO_OVERFLOW << 3 | CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_DATA_FIFO_OVERFLOW << 2 | (uint8_t)(CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_FIFO_TRIGGER_LEVEL != 0) << 1 | (uint8_t)(CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_FIFO_TRIGGER_LEVEL != 0) << 0,
    .dmas = CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_DMA_TRIGGER_OUT_USED << 1 | CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_DMA_TRIGGER_OUT_USED << 0,
};
#endif /* defined (CY_USING_HAL) */

=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (COMPONENT_MTB_HAL)
const mtb_hal_hf_clock_t CYBSP_SMIF_CORE_0_XSPI_FLASH_clock_ref =
{
    .inst_num = 3U,
};
const mtb_hal_clock_t CYBSP_SMIF_CORE_0_XSPI_FLASH_hal_clock =
{
    .clock_ref = &CYBSP_SMIF_CORE_0_XSPI_FLASH_clock_ref,
    .interface = &mtb_hal_clock_hf_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI)
const mtb_hal_memoryspi_configurator_t CYBSP_SMIF_CORE_0_XSPI_FLASH_hal_config =
{
    .base = SMIF0_CORE,
    .clock = &CYBSP_SMIF_CORE_0_XSPI_FLASH_hal_clock,
    .config = &CYBSP_SMIF_CORE_0_XSPI_FLASH_config,
<<<<<<< HEAD
    .csel = {false, true, false, false},
=======
    .csel = {true, false, false, false},
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .irqs = CYBSP_SMIF_CORE_0_XSPI_FLASH_MEMORY_MODE_ALIGMENT_ERROR << 5 | CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_DATA_FIFO_UNDERFLOW << 4 | CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_COMMAND_FIFO_OVERFLOW << 3 | CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_DATA_FIFO_OVERFLOW << 2 | (uint8_t)(CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_FIFO_TRIGGER_LEVEL != 0) << 1 | (uint8_t)(CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_FIFO_TRIGGER_LEVEL != 0) << 0,
    .dmas = CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_DMA_TRIGGER_OUT_USED << 1 | CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_DMA_TRIGGER_OUT_USED << 0,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI) */

const cy_stc_smif_config_t CYBSP_SMIF_CORE_1_PSRAM_config =
{
    .mode = (uint32_t)CY_SMIF_NORMAL,
    .deselectDelay = CYBSP_SMIF_CORE_1_PSRAM_DESELECT_DELAY,
    .blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
    .inputFrequencyMHz = 399,
    .enable_internal_dll = false,
    .dll_divider_value = CY_SMIF_DLL_DIVIDE_BY_2,
    .rx_capture_mode = CY_SMIF_SEL_NORMAL_SPI,
    .mdl_tap = CY_SMIF_MDL_8_TAP_DELAY,
    .device0_sdl_tap = CY_SMIF_SDL_8_TAP_DELAY,
    .device1_sdl_tap = CY_SMIF_SDL_8_TAP_DELAY,
    .device2_sdl_tap = CY_SMIF_SDL_8_TAP_DELAY,
    .device3_sdl_tap = CY_SMIF_SDL_8_TAP_DELAY,
    .tx_sdr_extra = CY_SMIF_TX_TWO_PERIOD_AHEAD,
};

<<<<<<< HEAD
#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_SMIF_CORE_1_PSRAM_obj =
{
    .type = CYHAL_RSC_SMIF,
    .block_num = 0U,
    .channel_num = 0U,
};
const cyhal_qspi_configurator_t CYBSP_SMIF_CORE_1_PSRAM_hal_config =
{
    .resource = &CYBSP_SMIF_CORE_1_PSRAM_obj,
    .config = &CYBSP_SMIF_CORE_1_PSRAM_config,
    .clock = &CYHAL_CLOCK_HF[4],
    .gpios = {.sclk = NC, .ssel = {NC, NC, P0_1, NC}, .io = {P4_0, P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7},},
    .irqs = CYBSP_SMIF_CORE_1_PSRAM_MEMORY_MODE_ALIGMENT_ERROR << 5 | CYBSP_SMIF_CORE_1_PSRAM_RX_DATA_FIFO_UNDERFLOW << 4 | CYBSP_SMIF_CORE_1_PSRAM_TX_COMMAND_FIFO_OVERFLOW << 3 | CYBSP_SMIF_CORE_1_PSRAM_TX_DATA_FIFO_OVERFLOW << 2 | (uint8_t)(CYBSP_SMIF_CORE_1_PSRAM_RX_FIFO_TRIGGER_LEVEL != 0) << 1 | (uint8_t)(CYBSP_SMIF_CORE_1_PSRAM_TX_FIFO_TRIGGER_LEVEL != 0) << 0,
    .dmas = CYBSP_SMIF_CORE_1_PSRAM_RX_DMA_TRIGGER_OUT_USED << 1 | CYBSP_SMIF_CORE_1_PSRAM_TX_DMA_TRIGGER_OUT_USED << 0,
};
#endif /* defined (CY_USING_HAL) */

=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (COMPONENT_MTB_HAL)
const mtb_hal_hf_clock_t CYBSP_SMIF_CORE_1_PSRAM_clock_ref =
{
    .inst_num = 4U,
};
const mtb_hal_clock_t CYBSP_SMIF_CORE_1_PSRAM_hal_clock =
{
    .clock_ref = &CYBSP_SMIF_CORE_1_PSRAM_clock_ref,
    .interface = &mtb_hal_clock_hf_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI)
const mtb_hal_memoryspi_configurator_t CYBSP_SMIF_CORE_1_PSRAM_hal_config =
{
    .base = SMIF1_CORE,
    .clock = &CYBSP_SMIF_CORE_1_PSRAM_hal_clock,
    .config = &CYBSP_SMIF_CORE_1_PSRAM_config,
    .csel = {false, false, true, false},
    .irqs = CYBSP_SMIF_CORE_1_PSRAM_MEMORY_MODE_ALIGMENT_ERROR << 5 | CYBSP_SMIF_CORE_1_PSRAM_RX_DATA_FIFO_UNDERFLOW << 4 | CYBSP_SMIF_CORE_1_PSRAM_TX_COMMAND_FIFO_OVERFLOW << 3 | CYBSP_SMIF_CORE_1_PSRAM_TX_DATA_FIFO_OVERFLOW << 2 | (uint8_t)(CYBSP_SMIF_CORE_1_PSRAM_RX_FIFO_TRIGGER_LEVEL != 0) << 1 | (uint8_t)(CYBSP_SMIF_CORE_1_PSRAM_TX_FIFO_TRIGGER_LEVEL != 0) << 0,
    .dmas = CYBSP_SMIF_CORE_1_PSRAM_RX_DMA_TRIGGER_OUT_USED << 1 | CYBSP_SMIF_CORE_1_PSRAM_TX_DMA_TRIGGER_OUT_USED << 0,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI) */

const cy_stc_mcwdt_config_t CYBSP_CM33_LPTIMER_0_config =
{
    .c0Match = 32768U,
    .c1Match = 32768U,
    .c0Mode = CY_MCWDT_MODE_INT,
    .c1Mode = CY_MCWDT_MODE_INT,
    .c2ToggleBit = 0U,
    .c2Mode = CY_MCWDT_MODE_NONE,
    .c0ClearOnMatch = false,
    .c1ClearOnMatch = false,
    .c0c1Cascade = true,
    .c1c2Cascade = false,
    .c0LowerLimitMode = CY_MCWDT_LOWER_LIMIT_MODE_NOTHING,
    .c0LowerLimit = 0U,
    .c1LowerLimitMode = CY_MCWDT_LOWER_LIMIT_MODE_NOTHING,
    .c1LowerLimit = 0U,
};

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER)
const mtb_hal_lptimer_configurator_t CYBSP_CM33_LPTIMER_0_hal_config =
{
    .base = CYBSP_CM33_LPTIMER_0_HW,
<<<<<<< HEAD
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER) */

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_CM33_LPTIMER_0_obj =
{
    .type = CYHAL_RSC_LPTIMER,
    .block_num = 0U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

=======
    .lfclk_freqhz = 32768,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER) */

>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
const cy_stc_mcwdt_config_t CYBSP_CM55_LPTIMER_1_config =
{
    .c0Match = 32768U,
    .c1Match = 32768U,
    .c0Mode = CY_MCWDT_MODE_INT,
    .c1Mode = CY_MCWDT_MODE_INT,
    .c2ToggleBit = 0U,
    .c2Mode = CY_MCWDT_MODE_NONE,
    .c0ClearOnMatch = false,
    .c1ClearOnMatch = false,
    .c0c1Cascade = true,
    .c1c2Cascade = false,
    .c0LowerLimitMode = CY_MCWDT_LOWER_LIMIT_MODE_NOTHING,
    .c0LowerLimit = 0U,
    .c1LowerLimitMode = CY_MCWDT_LOWER_LIMIT_MODE_NOTHING,
    .c1LowerLimit = 0U,
};

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER)
const mtb_hal_lptimer_configurator_t CYBSP_CM55_LPTIMER_1_hal_config =
{
    .base = CYBSP_CM55_LPTIMER_1_HW,
<<<<<<< HEAD
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER) */

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_CM55_LPTIMER_1_obj =
{
    .type = CYHAL_RSC_LPTIMER,
    .block_num = 1U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_rtc_config_t CYBSP_RTC_config =
{
    .sec = 0U,
    .min = 0U,
    .hour = 12U,
    .amPm = CY_RTC_AM,
    .hrFormat = CY_RTC_24_HOURS,
    .dayOfWeek = CY_RTC_MONDAY,
    .date = 1U,
    .month = CY_RTC_JANUARY,
    .year = 24U,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_RTC_obj =
{
    .type = CYHAL_RSC_RTC,
    .block_num = 0U,
    .channel_num = 0U,
};
const cyhal_rtc_configurator_t CYBSP_RTC_hal_config =
{
    .resource = &CYBSP_RTC_obj,
    .config = &CYBSP_RTC_config,
    .dst_config = NULL,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_counter_config_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_config =
{
    .period = 9999,
=======
    .lfclk_freqhz = 32768,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER) */

cy_stc_tdm_config_tx_t CYBSP_TDM_CONTROLLER_0_tx_config =
{
    .enable = true,
    .masterMode = CY_TDM_DEVICE_MASTER,
    .wordSize = CY_TDM_SIZE_16,
    .format = CY_TDM_LEFT_DELAYED,
    .clkDiv = 4,
    .clkSel = CY_TDM_SEL_SRSS_CLK0,
    .sckPolarity = CY_TDM_CLK,
    .fsyncPolarity = CY_TDM_SIGN_INVERTED,
    .fsyncFormat = CY_TDM_CH_PERIOD,
    .channelNum = 2,
    .channelSize = 16,
    .fifoTriggerLevel = 64,
    .chEn = 0x3,
    .signalInput = 0,
    .i2sMode = true,
};
cy_stc_tdm_config_rx_t CYBSP_TDM_CONTROLLER_0_rx_config =
{
    .enable = false,
    .masterMode = CY_TDM_DEVICE_SLAVE,
    .wordSize = CY_TDM_SIZE_16,
    .signExtend = CY_ZERO_EXTEND,
    .format = CY_TDM_LEFT_DELAYED,
    .clkDiv = 16,
    .clkSel = CY_TDM_SEL_SRSS_CLK0,
    .sckPolarity = CY_TDM_CLK,
    .fsyncPolarity = CY_TDM_SIGN,
    .lateSample = false,
    .fsyncFormat = CY_TDM_BIT_PERIOD,
    .channelNum = 2,
    .channelSize = 16,
    .fifoTriggerLevel = 0,
    .chEn = 0x3,
    .signalInput = 0,
    .i2sMode = true,
};
const cy_stc_tdm_config_t CYBSP_TDM_CONTROLLER_0_config =
{
    .tx_config = &CYBSP_TDM_CONTROLLER_0_tx_config,
    .rx_config = &CYBSP_TDM_CONTROLLER_0_rx_config,
};
const cy_stc_tcpwm_counter_config_t CYBSP_GENERAL_PURPOSE_TIMER_config =
{
    .period = 863999999,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_COMPARE,
<<<<<<< HEAD
    .compare0 = 9999,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = (CY_TCPWM_INT_ON_TC ) | (CY_TCPWM_INT_ON_CC0 & 0U) | (CY_TCPWM_INT_ON_CC1 & 0U),
    .captureInputMode = CYBSP_TCPWM_0_GRP_0_COUNTER_0_INPUT_DISABLED & 0x3U,
    .captureInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = CYBSP_TCPWM_0_GRP_0_COUNTER_0_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = CYBSP_TCPWM_0_GRP_0_COUNTER_0_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = CYBSP_TCPWM_0_GRP_0_COUNTER_0_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = CYBSP_TCPWM_0_GRP_0_COUNTER_0_INPUT_DISABLED & 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
    .capture1InputMode = CYBSP_TCPWM_0_GRP_0_COUNTER_0_INPUT_DISABLED & 0x3U,
=======
    .compare0 = 863999999,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = (CY_TCPWM_INT_ON_TC ) | (CY_TCPWM_INT_ON_CC0 & 0U) | (CY_TCPWM_INT_ON_CC1 & 0U),
    .captureInputMode = CYBSP_GENERAL_PURPOSE_TIMER_INPUT_DISABLED & 0x3U,
    .captureInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = CYBSP_GENERAL_PURPOSE_TIMER_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = CYBSP_GENERAL_PURPOSE_TIMER_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = CYBSP_GENERAL_PURPOSE_TIMER_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = CYBSP_GENERAL_PURPOSE_TIMER_INPUT_DISABLED & 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
    .capture1InputMode = CYBSP_GENERAL_PURPOSE_TIMER_INPUT_DISABLED & 0x3U,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .capture1Input = CY_TCPWM_INPUT_0,
    .compare2 = CY_TCPWM_GRP_CNT_CC0_DEFAULT,
    .compare3 = CY_TCPWM_GRP_CNT_CC0_BUFF_DEFAULT,
    .enableCompare1Swap = false,
    .trigger0Event = CY_TCPWM_CNT_TRIGGER_ON_DISABLED,
    .trigger1Event = CY_TCPWM_CNT_TRIGGER_ON_DISABLED,
#if defined (CY_IP_MXS40TCPWM)
    .buffer_swap_enable = false,
    .direction_mode = CY_TCPWM_COUNTER_DIRECTION_DISABLE,
    .glitch_filter_enable = false,
    .gf_depth = CY_GLITCH_FILTER_DEPTH_SUPPORT_VALUE_0,
#endif /* defined (CY_IP_MXS40TCPWM) */
};

<<<<<<< HEAD
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
const cyhal_clock_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_clock =
{
    .block = CYHAL_CLOCK_BLOCK0_PERIPHERAL1_16BIT,
    .channel = 2,
#if defined (CY_USING_HAL)
    .reserved = false,
    .funcs = NULL,
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_timer_configurator_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_hal_config =
{
    .resource = &CYBSP_TCPWM_0_GRP_0_COUNTER_0_obj,
    .config = &CYBSP_TCPWM_0_GRP_0_COUNTER_0_config,
    .clock = &CYBSP_TCPWM_0_GRP_0_COUNTER_0_clock,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_clock_ref =
=======
#if defined (COMPONENT_MTB_HAL)
const mtb_hal_peri_div_t CYBSP_GENERAL_PURPOSE_TIMER_clock_ref =
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
{
    .clk_dst = (en_clk_dst_t)PCLK_TCPWM0_CLOCK_COUNTER_EN0,
    .div_type = CY_SYSCLK_DIV_16_BIT,
    .div_num = 2,
};
<<<<<<< HEAD
const mtb_hal_clock_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_hal_clock =
{
    .clock_ref = &CYBSP_TCPWM_0_GRP_0_COUNTER_0_clock_ref,
=======
const mtb_hal_clock_t CYBSP_GENERAL_PURPOSE_TIMER_hal_clock =
{
    .clock_ref = &CYBSP_GENERAL_PURPOSE_TIMER_clock_ref,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_TIMER)
<<<<<<< HEAD
const mtb_hal_timer_configurator_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_hal_config =
{
    .tcpwm_base = CYBSP_TCPWM_0_GRP_0_COUNTER_0_HW,
    .clock = &CYBSP_TCPWM_0_GRP_0_COUNTER_0_hal_clock,
=======
const mtb_hal_timer_configurator_t CYBSP_GENERAL_PURPOSE_TIMER_hal_config =
{
    .tcpwm_base = CYBSP_GENERAL_PURPOSE_TIMER_HW,
    .clock = &CYBSP_GENERAL_PURPOSE_TIMER_hal_clock,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .tcpwm_cntnum = 0U,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_TIMER) */

cy_stc_tcpwm_counter_config_t emUSB_OS_Timer_config =
{
    .period = 99999,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384U,
    .compare1 = 16384U,
    .enableCompareSwap = false,
    .interruptSources = (CY_TCPWM_INT_ON_TC & 0U) | (CY_TCPWM_INT_ON_CC0) | (CY_TCPWM_INT_ON_CC1 & 0U),
    .captureInputMode = emUSB_OS_Timer_INPUT_DISABLED & 0x3U,
    .captureInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = emUSB_OS_Timer_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = emUSB_OS_Timer_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = emUSB_OS_Timer_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = emUSB_OS_Timer_INPUT_DISABLED & 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
    .capture1InputMode = emUSB_OS_Timer_INPUT_DISABLED & 0x3U,
    .capture1Input = CY_TCPWM_INPUT_0,
    .compare2 = CY_TCPWM_GRP_CNT_CC0_DEFAULT,
    .compare3 = CY_TCPWM_GRP_CNT_CC0_BUFF_DEFAULT,
    .enableCompare1Swap = false,
    .trigger0Event = CY_TCPWM_CNT_TRIGGER_ON_DISABLED,
    .trigger1Event = CY_TCPWM_CNT_TRIGGER_ON_DISABLED,
};

#if defined (COMPONENT_MTB_HAL)
mtb_hal_peri_div_t emUSB_OS_Timer_clock_ref =
{
    .clk_dst = (en_clk_dst_t)PCLK_TCPWM0_CLOCK_COUNTER_EN1,
    .div_type = CY_SYSCLK_DIV_8_BIT,
    .div_num = 2,
};
mtb_hal_clock_t emUSB_OS_Timer_hal_clock =
{
    .clock_ref = &emUSB_OS_Timer_clock_ref,
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_TIMER)
mtb_hal_timer_configurator_t emUSB_OS_Timer_hal_config =
{
    .tcpwm_base = emUSB_OS_Timer_HW,
    .clock = &emUSB_OS_Timer_hal_clock,
    .tcpwm_cntnum = 1U,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_TIMER) */

<<<<<<< HEAD
cy_stc_tcpwm_pwm_config_t CYBSP_TCPWM_0_GRP_1_PWM_5_config =
=======
cy_stc_tcpwm_pwm_config_t CYBSP_PWM_DISP_BACKLIGHT_config =
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
{
    .pwmMode = CY_TCPWM_PWM_MODE_PWM,
    .clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
    .pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
    .deadTimeClocks = 0,
    .runMode = CY_TCPWM_PWM_CONTINUOUS,
    .period0 = 2499,
    .period1 = 32768,
    .enablePeriodSwap = false,
    .compare0 = 1249,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = (CY_TCPWM_INT_ON_TC & 0U) | (CY_TCPWM_INT_ON_CC0 & 0U) | (CY_TCPWM_INT_ON_CC1 & 0U),
    .invertPWMOut = CY_TCPWM_PWM_INVERT_ENABLE,
    .invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
    .killMode = CY_TCPWM_PWM_STOP_ON_KILL,
<<<<<<< HEAD
    .swapInputMode = CYBSP_TCPWM_0_GRP_1_PWM_5_INPUT_DISABLED & 0x3U,
    .swapInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = CYBSP_TCPWM_0_GRP_1_PWM_5_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = CYBSP_TCPWM_0_GRP_1_PWM_5_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .killInputMode = CYBSP_TCPWM_0_GRP_1_PWM_5_INPUT_DISABLED & 0x3U,
    .killInput = CY_TCPWM_INPUT_0,
    .countInputMode = CYBSP_TCPWM_0_GRP_1_PWM_5_INPUT_DISABLED & 0x3U,
=======
    .swapInputMode = CYBSP_PWM_DISP_BACKLIGHT_INPUT_DISABLED & 0x3U,
    .swapInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = CYBSP_PWM_DISP_BACKLIGHT_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = CYBSP_PWM_DISP_BACKLIGHT_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .killInputMode = CYBSP_PWM_DISP_BACKLIGHT_INPUT_DISABLED & 0x3U,
    .killInput = CY_TCPWM_INPUT_0,
    .countInputMode = CYBSP_PWM_DISP_BACKLIGHT_INPUT_DISABLED & 0x3U,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .countInput = CY_TCPWM_INPUT_1,
    .swapOverflowUnderflow = false,
    .immediateKill = false,
    .tapsEnabled = 45,
    .compare2 = 0,
    .compare3 = 16384,
    .enableCompare1Swap = false,
    .compare0MatchUp = true,
    .compare0MatchDown = false,
    .compare1MatchUp = true,
    .compare1MatchDown = false,
<<<<<<< HEAD
    .kill1InputMode = CYBSP_TCPWM_0_GRP_1_PWM_5_INPUT_DISABLED & 0x3U,
=======
    .kill1InputMode = CYBSP_PWM_DISP_BACKLIGHT_INPUT_DISABLED & 0x3U,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .kill1Input = CY_TCPWM_INPUT_0,
    .pwmOnDisable = CY_TCPWM_PWM_OUTPUT_HIGHZ,
    .trigger0Event = CY_TCPWM_CNT_TRIGGER_ON_DISABLED,
    .trigger1Event = CY_TCPWM_CNT_TRIGGER_ON_DISABLED,
    .reloadLineSelect = false,
    .line_out_sel = CY_TCPWM_OUTPUT_PWM_SIGNAL,
    .linecompl_out_sel = CY_TCPWM_OUTPUT_INVERTED_PWM_SIGNAL,
    .line_out_sel_buff = CY_TCPWM_OUTPUT_PWM_SIGNAL,
    .linecompl_out_sel_buff = CY_TCPWM_OUTPUT_INVERTED_PWM_SIGNAL,
    .deadTimeClocks_linecompl_out = 0,
#if defined (CY_IP_MXS40TCPWM)
    .hrpwm_enable = false,
    .hrpwm_input_freq = CY_TCPWM_HRPWM_FREQ_80MHZ_OR_100MHZ,
    .kill_line_polarity = CY_TCPWM_LINEOUT_AND_LINECMPOUT_IS_LOW,
    .deadTimeClocksBuff = 0,
    .deadTimeClocksBuff_linecompl_out = 0,
    .buffer_swap_enable = false,
    .glitch_filter_enable = false,
    .gf_depth = CY_GLITCH_FILTER_DEPTH_SUPPORT_VALUE_0,
    .dithering_mode = CY_TCPWM_DITHERING_DISABLE,
    .period_dithering_value = 128,
    .duty_dithering_value = 128,
    .limiter = CY_TCPWM_DITHERING_LIMITER_7,
    .pwm_tc_sync_kill_dt = false,
    .pwm_sync_kill_dt = false,
#endif /* defined (CY_IP_MXS40TCPWM) */
};

#if defined (COMPONENT_MTB_HAL)
<<<<<<< HEAD
mtb_hal_peri_div_t CYBSP_TCPWM_0_GRP_1_PWM_5_clock_ref =
=======
mtb_hal_peri_div_t CYBSP_PWM_DISP_BACKLIGHT_clock_ref =
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
{
    .clk_dst = (en_clk_dst_t)PCLK_TCPWM0_CLOCK_COUNTER_EN261,
    .div_type = CY_SYSCLK_DIV_16_BIT,
    .div_num = 3,
};
<<<<<<< HEAD
mtb_hal_clock_t CYBSP_TCPWM_0_GRP_1_PWM_5_hal_clock =
{
    .clock_ref = &CYBSP_TCPWM_0_GRP_1_PWM_5_clock_ref,
=======
mtb_hal_clock_t CYBSP_PWM_DISP_BACKLIGHT_hal_clock =
{
    .clock_ref = &CYBSP_PWM_DISP_BACKLIGHT_clock_ref,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .interface = &mtb_hal_clock_peri_interface,
};
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_PWM)
<<<<<<< HEAD
mtb_hal_pwm_configurator_t CYBSP_TCPWM_0_GRP_1_PWM_5_hal_config =
{
    .base = CYBSP_TCPWM_0_GRP_1_PWM_5_HW,
    .clock = &CYBSP_TCPWM_0_GRP_1_PWM_5_hal_clock,
=======
mtb_hal_pwm_configurator_t CYBSP_PWM_DISP_BACKLIGHT_hal_config =
{
    .base = CYBSP_PWM_DISP_BACKLIGHT_HW,
    .clock = &CYBSP_PWM_DISP_BACKLIGHT_hal_clock,
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    .group = 1UL,
    .cntnum = 261UL,
    .max_count = 2499,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_PWM) */

<<<<<<< HEAD
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_TCPWM_0_GRP_1_PWM_5_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 1U,
    .channel_num = 5U,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
cyhal_clock_t CYBSP_TCPWM_0_GRP_1_PWM_5_clock =
{
    .block = CYHAL_CLOCK_BLOCK0_PERIPHERAL1_16BIT,
    .channel = 3,
#if defined (CY_USING_HAL)
    .reserved = false,
    .funcs = NULL,
#endif /* defined (CY_USING_HAL) */
};
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
cyhal_pwm_configurator_t CYBSP_TCPWM_0_GRP_1_PWM_5_hal_config =
{
    .resource = &CYBSP_TCPWM_0_GRP_1_PWM_5_obj,
    .config = &CYBSP_TCPWM_0_GRP_1_PWM_5_config,
    .clock = &CYBSP_TCPWM_0_GRP_1_PWM_5_clock,
};
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

cy_stc_tdm_config_tx_t CYBSP_TDM_CONTROLLER_0_tx_config =
{
    .enable = true,
    .masterMode = CY_TDM_DEVICE_MASTER,
    .wordSize = CY_TDM_SIZE_16,
    .format = CY_TDM_LEFT_DELAYED,
    .clkDiv = 4,
    .clkSel = CY_TDM_SEL_SRSS_CLK0,
    .sckPolarity = CY_TDM_CLK,
    .fsyncPolarity = CY_TDM_SIGN_INVERTED,
    .fsyncFormat = CY_TDM_CH_PERIOD,
    .channelNum = 2,
    .channelSize = 16,
    .fifoTriggerLevel = 64,
    .chEn = 0x3,
    .signalInput = 0,
    .i2sMode = true,
};
cy_stc_tdm_config_rx_t CYBSP_TDM_CONTROLLER_0_rx_config =
{
    .enable = false,
    .masterMode = CY_TDM_DEVICE_SLAVE,
    .wordSize = CY_TDM_SIZE_16,
    .signExtend = CY_ZERO_EXTEND,
    .format = CY_TDM_LEFT_DELAYED,
    .clkDiv = 16,
    .clkSel = CY_TDM_SEL_SRSS_CLK0,
    .sckPolarity = CY_TDM_CLK,
    .fsyncPolarity = CY_TDM_SIGN,
    .lateSample = false,
    .fsyncFormat = CY_TDM_BIT_PERIOD,
    .channelNum = 2,
    .channelSize = 16,
    .fifoTriggerLevel = 0,
    .chEn = 0x3,
    .signalInput = 0,
    .i2sMode = true,
};
const cy_stc_tdm_config_t CYBSP_TDM_CONTROLLER_0_config =
{
    .tx_config = &CYBSP_TDM_CONTROLLER_0_tx_config,
    .rx_config = &CYBSP_TDM_CONTROLLER_0_rx_config,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_TDM_CONTROLLER_0_obj =
{
    .type = CYHAL_RSC_TDM,
    .block_num = 0U,
    .channel_num = 0U,
};
const cyhal_clock_t CYBSP_TDM_CONTROLLER_0_clock =
{
    .block = CYHAL_CLOCK_BLOCK1_PERIPHERAL1_16_5BIT,
    .channel = 0,
    .reserved = false,
    .funcs = NULL,
};
const cyhal_tdm_configurator_t CYBSP_TDM_CONTROLLER_0_hal_config =
{
    .resource = &CYBSP_TDM_CONTROLLER_0_obj,
    .config = &CYBSP_TDM_CONTROLLER_0_config,
    .clock = &CYBSP_TDM_CONTROLLER_0_clock,
    .mclk_hz_rx = 0u,
    .mclk_hz_tx = 0u,
};
#endif /* defined (CY_USING_HAL) */

void init_cycfg_peripherals(void)
{
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_GPU_PERI_NR, CY_MMIO_GFXSS_GPU_GROUP_NR, CY_MMIO_GFXSS_GPU_SLAVE_NR, CY_MMIO_GFXSS_GPU_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_DC_PERI_NR, CY_MMIO_GFXSS_DC_GROUP_NR, CY_MMIO_GFXSS_DC_SLAVE_NR, CY_MMIO_GFXSS_DC_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_MIPIDSI_PERI_NR, CY_MMIO_GFXSS_MIPIDSI_GROUP_NR, CY_MMIO_GFXSS_MIPIDSI_SLAVE_NR, CY_MMIO_GFXSS_MIPIDSI_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_PASS_PERI_NR, CY_MMIO_PASS_GROUP_NR, CY_MMIO_PASS_SLAVE_NR, CY_MMIO_PASS_CLK_HF_NR);
#if defined (CY_USING_HAL) && defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK)
    _cyhal_clock_get_pclk(CYHAL_CLOCK_BLOCK1_PERIPHERAL1_16_5BIT, 1, &CYBSP_PDM_clock);
#endif /* defined (CY_USING_HAL) && defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK) */
=======
void init_cycfg_peripherals(void)
{
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_PASS_PERI_NR, CY_MMIO_PASS_GROUP_NR, CY_MMIO_PASS_SLAVE_NR, CY_MMIO_PASS_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_GPU_PERI_NR, CY_MMIO_GFXSS_GPU_GROUP_NR, CY_MMIO_GFXSS_GPU_SLAVE_NR, CY_MMIO_GFXSS_GPU_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_DC_PERI_NR, CY_MMIO_GFXSS_DC_GROUP_NR, CY_MMIO_GFXSS_DC_SLAVE_NR, CY_MMIO_GFXSS_DC_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_MIPIDSI_PERI_NR, CY_MMIO_GFXSS_MIPIDSI_GROUP_NR, CY_MMIO_GFXSS_MIPIDSI_SLAVE_NR, CY_MMIO_GFXSS_MIPIDSI_CLK_HF_NR);
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_PDM0_PERI_NR, CY_MMIO_PDM0_GROUP_NR, CY_MMIO_PDM0_SLAVE_NR, CY_MMIO_PDM0_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) */
    Cy_SysClk_PeriPclkAssignDivider(PCLK_PDM0_CLK_IF_SRSS, CY_SYSCLK_DIV_16_5_BIT, 1U);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SCB0_PERI_NR, CY_MMIO_SCB0_GROUP_NR, CY_MMIO_SCB0_SLAVE_NR, CY_MMIO_SCB0_CLK_HF_NR);
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SCB0_CLOCK_SCB_EN, CY_SYSCLK_DIV_16_BIT, 0U);
<<<<<<< HEAD
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SCB10_PERI_NR, CY_MMIO_SCB10_GROUP_NR, CY_MMIO_SCB10_SLAVE_NR, CY_MMIO_SCB10_CLK_HF_NR);
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SCB10_CLOCK_SCB_EN, CY_SYSCLK_DIV_8_BIT, 1U);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SCB11_PERI_NR, CY_MMIO_SCB11_GROUP_NR, CY_MMIO_SCB11_SLAVE_NR, CY_MMIO_SCB11_CLK_HF_NR);
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SCB11_CLOCK_SCB_EN, CY_SYSCLK_DIV_8_BIT, 4U);
=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SCB2_PERI_NR, CY_MMIO_SCB2_GROUP_NR, CY_MMIO_SCB2_SLAVE_NR, CY_MMIO_SCB2_CLK_HF_NR);
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SCB2_CLOCK_SCB_EN, CY_SYSCLK_DIV_16_BIT, 1U);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SCB3_PERI_NR, CY_MMIO_SCB3_GROUP_NR, CY_MMIO_SCB3_SLAVE_NR, CY_MMIO_SCB3_CLK_HF_NR);
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SCB3_CLOCK_SCB_EN, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SCB4_PERI_NR, CY_MMIO_SCB4_GROUP_NR, CY_MMIO_SCB4_SLAVE_NR, CY_MMIO_SCB4_CLK_HF_NR);
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SCB4_CLOCK_SCB_EN, CY_SYSCLK_DIV_16_5_BIT, 0U);
<<<<<<< HEAD
=======
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SCB10_PERI_NR, CY_MMIO_SCB10_GROUP_NR, CY_MMIO_SCB10_SLAVE_NR, CY_MMIO_SCB10_CLK_HF_NR);
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SCB10_CLOCK_SCB_EN, CY_SYSCLK_DIV_8_BIT, 1U);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SCB11_PERI_NR, CY_MMIO_SCB11_GROUP_NR, CY_MMIO_SCB11_SLAVE_NR, CY_MMIO_SCB11_CLK_HF_NR);
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SCB11_CLOCK_SCB_EN, CY_SYSCLK_DIV_8_BIT, 4U);
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SDHC0_PERI_NR, CY_MMIO_SDHC0_GROUP_NR, CY_MMIO_SDHC0_SLAVE_NR, CY_MMIO_SDHC0_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) */
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SDHC0_CLK_HF, CY_SYSCLK_DIV_8_BIT, 0U);
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SDHC1_PERI_NR, CY_MMIO_SDHC1_GROUP_NR, CY_MMIO_SDHC1_SLAVE_NR, CY_MMIO_SDHC1_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) */
    Cy_SysClk_PeriPclkAssignDivider(PCLK_SDHC1_CLK_HF, CY_SYSCLK_DIV_8_BIT, 0U);
<<<<<<< HEAD
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) && defined (CY_IP_MXS22SRSS)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SMIF0_PERI_NR, CY_MMIO_SMIF0_GROUP_NR, CY_MMIO_SMIF0_SLAVE_NR, CY_MMIO_SMIF0_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SMIF01_PERI_NR, CY_MMIO_SMIF01_GROUP_NR, CY_MMIO_SMIF01_SLAVE_NR, CY_MMIO_SMIF01_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) && defined (CY_IP_MXS22SRSS) */
=======
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_USBHS_PERI_NR, CY_MMIO_USBHS_GROUP_NR, CY_MMIO_USBHS_SLAVE_NR, CY_MMIO_USBHS_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SMIF0_PERI_NR, CY_MMIO_SMIF0_GROUP_NR, CY_MMIO_SMIF0_SLAVE_NR, CY_MMIO_SMIF0_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SMIF01_PERI_NR, CY_MMIO_SMIF01_GROUP_NR, CY_MMIO_SMIF01_SLAVE_NR, CY_MMIO_SMIF01_CLK_HF_NR);
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_TDM0_PERI_NR, CY_MMIO_TDM0_GROUP_NR, CY_MMIO_TDM0_SLAVE_NR, CY_MMIO_TDM0_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) */
    Cy_SysClk_PeriPclkAssignDivider(PCLK_TDM0_CLK_IF_SRSS0, CY_SYSCLK_DIV_16_5_BIT, 0U);
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_TCPWM0_PERI_NR, CY_MMIO_TCPWM0_GROUP_NR, CY_MMIO_TCPWM0_SLAVE_NR, CY_MMIO_TCPWM0_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) */
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM0_CLOCK_COUNTER_EN0, CY_SYSCLK_DIV_16_BIT, 2U);
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_TCPWM0_PERI_NR, CY_MMIO_TCPWM0_GROUP_NR, CY_MMIO_TCPWM0_SLAVE_NR, CY_MMIO_TCPWM0_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) */
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM0_CLOCK_COUNTER_EN1, CY_SYSCLK_DIV_8_BIT, 2U);
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_TCPWM0_PERI_NR, CY_MMIO_TCPWM0_GROUP_NR, CY_MMIO_TCPWM0_SLAVE_NR, CY_MMIO_TCPWM0_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) */
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM0_CLOCK_COUNTER_EN261, CY_SYSCLK_DIV_16_BIT, 3U);
<<<<<<< HEAD
#if defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE)
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_TDM0_PERI_NR, CY_MMIO_TDM0_GROUP_NR, CY_MMIO_TDM0_SLAVE_NR, CY_MMIO_TDM0_CLK_HF_NR);
#endif /* defined (CY_DEVICE_CONFIGURATOR_IP_ENABLE_FEATURE) */
    Cy_SysClk_PeriPclkAssignDivider(PCLK_TDM0_CLK_IF_SRSS0, CY_SYSCLK_DIV_16_5_BIT, 0U);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_USBHS_PERI_NR, CY_MMIO_USBHS_GROUP_NR, CY_MMIO_USBHS_SLAVE_NR, CY_MMIO_USBHS_CLK_HF_NR);
}
void reserve_cycfg_peripherals(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&CYBSP_I2C_CONTROLLER_0_obj);
    cyhal_hwmgr_reserve(&CYBSP_SPI_CONTROLLER_10_obj);
    cyhal_hwmgr_reserve(&CYBSP_I2C_CONTROLLER_11_obj);
    cyhal_hwmgr_reserve(&CYBSP_DEBUG_UART_obj);
    cyhal_hwmgr_reserve(&CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_obj);
    cyhal_hwmgr_reserve(&CYBSP_BT_UART_obj);
    cyhal_hwmgr_reserve(&CYBSP_WIFI_SDIO_obj);
    cyhal_hwmgr_reserve(&CYBSP_SDHC_1_obj);
    cyhal_hwmgr_reserve(&CYBSP_SMIF_CORE_0_XSPI_FLASH_obj);
    cyhal_hwmgr_reserve(&CYBSP_SMIF_CORE_1_PSRAM_obj);
    cyhal_hwmgr_reserve(&CYBSP_CM33_LPTIMER_0_obj);
    cyhal_hwmgr_reserve(&CYBSP_CM55_LPTIMER_1_obj);
    cyhal_hwmgr_reserve(&CYBSP_RTC_obj);
    cyhal_hwmgr_reserve(&CYBSP_TCPWM_0_GRP_0_COUNTER_0_obj);
    cyhal_hwmgr_reserve(&CYBSP_TCPWM_0_GRP_1_PWM_5_obj);
#endif /* defined (CY_USING_HAL) */
=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
}
