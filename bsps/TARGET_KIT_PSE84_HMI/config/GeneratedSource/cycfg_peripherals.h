/*******************************************************************************
 * File Name: cycfg_peripherals.h
 *
 * Description:
 * Peripheral Hardware Block configuration
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

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_graphics.h"
#include "cy_sysclk.h"
#include "cy_autanalog.h"
#include "cy_pdm_pcm_v2.h"
#include "cy_scb_i2c.h"
#include "cy_scb_spi.h"
#include "cy_scb_uart.h"
#include "cy_sd_host.h"
#include "cy_smif.h"
#include "cycfg_qspi_memslot.h"
#include "cy_mcwdt.h"
#include "cy_rtc.h"
#include "cy_tcpwm_counter.h"
#include "cycfg_routing.h"
#include "cy_tcpwm_pwm.h"
#include "cy_tdm.h"

#if defined (CY_USING_HAL)
#include "cyhal_hwmgr.h"
#include "cyhal.h"
#include "cycfg_clocks.h"
#endif /* defined (CY_USING_HAL) */

#if defined (CY_USING_HAL_LITE) || defined (CY_USING_HAL)
#include "cycfg_clocks.h"
#endif /* defined (CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (COMPONENT_MTB_HAL)
#include "mtb_hal.h"
#include "cycfg_clocks.h"
#include "mtb_hal_hw_types.h"
#include "mtb_hal_clock.h"
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (CY_USING_HAL_LITE)
#include "cyhal_hw_types.h"
#endif /* defined (CY_USING_HAL_LITE) */

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define gfxss_0_ENABLED 1U
#define GFXSS_HW GFXSS
#define GFXSS_GPU_IRQ gfxss_interrupt_gpu_IRQn
#define GFXSS_DC_IRQ gfxss_interrupt_dc_IRQn
#define GFXSS_MIPIDSI_IRQ gfxss_interrupt_mipidsi_IRQn
#define CYBSP_AUTONOMOUS_ANALOG_ENABLED 1U
#define ANALOG_MIC_INTERFACE_ENABLED CYBSP_AUTONOMOUS_ANALOG_ENABLED
#define CYBSP_AUTONOMOUS_ANALOG_lppass_IRQ pass_interrupt_lppass_IRQn
#define CYBSP_AUTONOMOUS_ANALOG_fifo_IRQ pass_interrupt_fifo_IRQn
#define CYBSP_AUTONOMOUS_CONTROLLER_ENABLED 1U
#define ANALOG_MIC_INTERFACE_STATES_ENABLED CYBSP_AUTONOMOUS_CONTROLLER_ENABLED
#define CYBSP_AUTONOMOUS_CONTROLLER_STATE_0_ENABLED 1U
#define ENABLE_AMPS_AAD_WAIT_ENABLED CYBSP_AUTONOMOUS_CONTROLLER_STATE_0_ENABLED
#define CYBSP_AUTONOMOUS_CONTROLLER_STATE_1_ENABLED 1U
#define WAIT_FOR_FW_TRIGGER0_ENABLED CYBSP_AUTONOMOUS_CONTROLLER_STATE_1_ENABLED
#define CYBSP_AUTONOMOUS_CONTROLLER_STATE_2_ENABLED 1U
#define SLEEP_UNTIL_TIMER_TRIGGER_ENABLED CYBSP_AUTONOMOUS_CONTROLLER_STATE_2_ENABLED
#define CYBSP_AUTONOMOUS_CONTROLLER_STATE_3_ENABLED 1U
#define ENABLE_ADC_WAIT_ENABLED CYBSP_AUTONOMOUS_CONTROLLER_STATE_3_ENABLED
#define CYBSP_AUTONOMOUS_CONTROLLER_STATE_4_ENABLED 1U
#define TRIGGER_ADC_WAIT_FIFO_ENABLED CYBSP_AUTONOMOUS_CONTROLLER_STATE_4_ENABLED
#define CYBSP_AUTONOMOUS_CONTROLLER_STATE_5_ENABLED 1U
#define DISABLE_ADC_GOTO_STATE2_ENABLED CYBSP_AUTONOMOUS_CONTROLLER_STATE_5_ENABLED
#define CYBSP_CTB_0_ENABLED 1U
#define MIC_PREAMPS_AND_FILTERS_ENABLED CYBSP_CTB_0_ENABLED
#define CYBSP_OPAMP_0_ENABLED 1U
#define MIC_LEFT_AMP_AND_FILTER_ENABLED CYBSP_OPAMP_0_ENABLED
#define CYBSP_OPAMP_0_DYNAMIC_CONFIGURATION_0_ENABLED 1U
#define CONNECT_TO_P0_1_2_ENABLED CYBSP_OPAMP_0_DYNAMIC_CONFIGURATION_0_ENABLED
#define CYBSP_OPAMP_1_ENABLED 1U
#define MIC_RIGHT_AMP_AND_FILTER_ENABLED CYBSP_OPAMP_1_ENABLED
#define CYBSP_OPAMP_1_DYNAMIC_CONFIGURATION_0_ENABLED 1U
#define CONNECT_TO_P4_5_3_ENABLED CYBSP_OPAMP_1_DYNAMIC_CONFIGURATION_0_ENABLED
#define CYBSP_SAR_ADC_ENABLED 1U
#define CYBSP_SAR_ADC_FIFO_ENABLED 1U
#define CYBSP_SAR_ADC_SCAN_GRP_0_ENABLED 1U
#define DUAL_MIC_SCAN_ENABLED CYBSP_SAR_ADC_SCAN_GRP_0_ENABLED
#define CYBSP_SAR_ADC_SCAN_GRP_0_SCAN_0_ENABLED 1U
#define SCAN_LEFT_MIC_ENABLED CYBSP_SAR_ADC_SCAN_GRP_0_SCAN_0_ENABLED
#define CYBSP_SAR_ADC_SCAN_GRP_0_SCAN_1_ENABLED 1U
#define SCAN_RIGHT_MIC_ENABLED CYBSP_SAR_ADC_SCAN_GRP_0_SCAN_1_ENABLED
#define CYBSP_SAR_ADC_MUX_CH_0_ENABLED 1U
#define MIC_LEFT_AMP_OUT_ENABLED CYBSP_SAR_ADC_MUX_CH_0_ENABLED
#define CYBSP_SAR_ADC_MUX_CH_1_ENABLED 1U
#define MIC_RIGHT_AMP_OUT_ENABLED CYBSP_SAR_ADC_MUX_CH_1_ENABLED
#define CYBSP_PDM_ENABLED 1U
#define CYBSP_PDM_HW PDM0
#define CYBSP_PDM_CHANNEL_2_IRQ pdm_0_interrupts_2_IRQn
#define CYBSP_PDM_CHANNEL_3_IRQ pdm_0_interrupts_3_IRQn
#define CYBSP_I2C_CONTROLLER_0_ENABLED 1U
#define CYBSP_I2C_1V8_ENABLED CYBSP_I2C_CONTROLLER_0_ENABLED
#define CYBSP_I2C_CONTROLLER_0_HW SCB0
#define CYBSP_I2C_1V8_HW CYBSP_I2C_CONTROLLER_0_HW
#define CYBSP_I2C_CONTROLLER_0_IRQ scb_0_interrupt_IRQn
#define CYBSP_I2C_1V8_IRQ CYBSP_I2C_CONTROLLER_0_IRQ
#define CYBSP_SPI_CONTROLLER_10_ENABLED 1U
#define CYBSP_SPI_CONTROLLER_10_HW SCB10
#define CYBSP_SPI_CONTROLLER_10_IRQ scb_10_interrupt_IRQn
#define CYBSP_I2C_CONTROLLER_11_ENABLED 1U
#define CYBSP_I2C_3V3_ENABLED CYBSP_I2C_CONTROLLER_11_ENABLED
#define CYBSP_I2C_CONTROLLER_11_HW SCB11
#define CYBSP_I2C_3V3_HW CYBSP_I2C_CONTROLLER_11_HW
#define CYBSP_I2C_CONTROLLER_11_IRQ scb_11_interrupt_IRQn
#define CYBSP_I2C_3V3_IRQ CYBSP_I2C_CONTROLLER_11_IRQ
#define CYBSP_DEBUG_UART_ENABLED 1U
#define CYBSP_DEBUG_UART_HW SCB2
#define CYBSP_DEBUG_UART_IRQ scb_2_interrupt_IRQn
#define CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_ENABLED 1U
#define CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_HW SCB3
#define CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_IRQ scb_3_interrupt_IRQn
#define CYBSP_BT_UART_ENABLED 1U
#define CYBSP_BT_UART_HW SCB4
#define CYBSP_BT_UART_IRQ scb_4_interrupt_IRQn
#define CYBSP_WIFI_SDIO_ENABLED 1U
#define CYBSP_WIFI_SDIO_HW SDHC0
#define CYBSP_WIFI_SDIO_IRQ sdhc_0_interrupt_general_IRQn
#define CYBSP_SDHC_1_ENABLED 1U
#define CYBSP_SDHC_1_HW SDHC1
#define CYBSP_SDHC_1_IRQ sdhc_1_interrupt_general_IRQn
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_ENABLED 1U
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_HW SMIF0_CORE
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_IRQ smif_0_smif0_interrupt_nsec_IRQn
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_MEMORY_MODE_ALIGMENT_ERROR (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_DATA_FIFO_UNDERFLOW (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_COMMAND_FIFO_OVERFLOW (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_DATA_FIFO_OVERFLOW (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_DMA_TRIGGER_OUT_USED (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_DMA_TRIGGER_OUT_USED (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_RX_FIFO_TRIGGER_LEVEL (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_TX_FIFO_TRIGGER_LEVEL (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_DATALINES0_1 (1UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_DATALINES2_3 (1UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_DATALINES4_5 (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_DATALINES6_7 (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_SS0 (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_SS1 (1UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_SS2 (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_SS3 (0UL)
#define CYBSP_SMIF_CORE_0_XSPI_FLASH_DESELECT_DELAY 7
#define CYBSP_SMIF_CORE_1_PSRAM_ENABLED 1U
#define CYBSP_SMIF_CORE_1_PSRAM_HW SMIF1_CORE
#define CYBSP_SMIF_CORE_1_PSRAM_IRQ smif_1_smif0_interrupt_nsec_IRQn
#define CYBSP_SMIF_CORE_1_PSRAM_MEMORY_MODE_ALIGMENT_ERROR (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_RX_DATA_FIFO_UNDERFLOW (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_TX_COMMAND_FIFO_OVERFLOW (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_TX_DATA_FIFO_OVERFLOW (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_RX_DMA_TRIGGER_OUT_USED (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_TX_DMA_TRIGGER_OUT_USED (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_RX_FIFO_TRIGGER_LEVEL (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_TX_FIFO_TRIGGER_LEVEL (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_DATALINES0_1 (1UL)
#define CYBSP_SMIF_CORE_1_PSRAM_DATALINES2_3 (1UL)
#define CYBSP_SMIF_CORE_1_PSRAM_DATALINES4_5 (1UL)
#define CYBSP_SMIF_CORE_1_PSRAM_DATALINES6_7 (1UL)
#define CYBSP_SMIF_CORE_1_PSRAM_SS0 (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_SS1 (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_SS2 (1UL)
#define CYBSP_SMIF_CORE_1_PSRAM_SS3 (0UL)
#define CYBSP_SMIF_CORE_1_PSRAM_DESELECT_DELAY 7
#define CYBSP_CM33_LPTIMER_0_ENABLED 1U
#define CYBSP_CM33_LPTIMER_0_HW MCWDT_STRUCT0
#define CYBSP_CM33_LPTIMER_0_IRQ srss_interrupt_mcwdt_0_IRQn
#define CYBSP_CM55_LPTIMER_1_ENABLED 1U
#define CYBSP_CM55_LPTIMER_1_HW MCWDT_STRUCT1
#define CYBSP_CM55_LPTIMER_1_IRQ srss_interrupt_mcwdt_1_IRQn
#define CYBSP_RTC_ENABLED 1U
#define CYBSP_RTC_10_MONTH_OFFSET (28U)
#define CYBSP_RTC_MONTH_OFFSET (24U)
#define CYBSP_RTC_10_DAY_OFFSET (20U)
#define CYBSP_RTC_DAY_OFFSET (16U)
#define CYBSP_RTC_1000_YEAR_OFFSET (12U)
#define CYBSP_RTC_100_YEAR_OFFSET (8U)
#define CYBSP_RTC_10_YEAR_OFFSET (4U)
#define CYBSP_RTC_YEAR_OFFSET (0U)
#define CYBSP_TCPWM_0_GRP_0_COUNTER_0_ENABLED 1U
#define CYBSP_TCPWM_0_GRP_0_COUNTER_0_HW TCPWM0
#define CYBSP_TCPWM_0_GRP_0_COUNTER_0_NUM 0UL
#define CYBSP_TCPWM_0_GRP_0_COUNTER_0_IRQ tcpwm_0_interrupts_0_IRQn
#define CYBSP_USB_OS_TIMER_COUNTER_1_ENABLED 1U
#define emUSB_OS_Timer_HW TCPWM0
#define emUSB_OS_Timer_NUM 1UL
#define emUSB_OS_Timer_IRQ tcpwm_0_interrupts_1_IRQn
#define CYBSP_TCPWM_0_GRP_1_PWM_5_ENABLED 1U
#define CYBSP_TCPWM_0_GRP_1_PWM_5_HW TCPWM0
#define CYBSP_TCPWM_0_GRP_1_PWM_5_NUM 261UL
#define CYBSP_TDM_CONTROLLER_0_ENABLED 1U
#define CYBSP_TDM_CONTROLLER_0_HW TDM_STRUCT0
#define CYBSP_TDM_CONTROLLER_0_TX_HW TDM_STRUCT0_TX
#define CYBSP_TDM_CONTROLLER_0_RX_HW TDM_STRUCT0_RX
#define CYBSP_TDM_CONTROLLER_0_TX_IRQ tdm_0_interrupts_tx_0_IRQn
#define CYBSP_TDM_CONTROLLER_0_RX_IRQ tdm_0_interrupts_rx_0_IRQn
#define CYBSP_USB_DEVICE_0_ENABLED 1U

extern cy_stc_gfx_layer_config_t GFXSS_graphics_layer;
extern cy_stc_gfx_layer_config_t GFXSS_overlay0_layer;
extern cy_stc_gfx_layer_config_t GFXSS_overlay1_layer;
extern cy_stc_gfx_dc_config_t GFXSS_dc_config;
extern cy_stc_gfx_gpu_cfg_t GFXSS_gpu_config;
extern cy_stc_mipidsi_display_params_t GFXSS_mipidsi_display_params;
extern cy_stc_mipidsi_config_t GFXSS_mipi_dsi_config;
extern cy_stc_gfx_config_t GFXSS_config;
extern cy_stc_autanalog_cfg_t autonomous_analog_cfg;
extern cy_stc_autanalog_stt_t autonomous_analog_stt[];
extern cy_stc_autanalog_t autonomous_analog_init;
extern cy_en_autanalog_ac_out_trigger_mask_t CYBSP_AUTONOMOUS_CONTROLLER_out_trig_mask[];
extern cy_stc_autanalog_ac_t CYBSP_AUTONOMOUS_CONTROLLER_cfg;
extern cy_stc_autanalog_stt_ac_t CYBSP_AUTONOMOUS_CONTROLLER_stt[];
extern cy_stc_autanalog_ctb_sta_t CYBSP_CTB_0_sta_cfg;
extern cy_stc_autanalog_ctb_dyn_t CYBSP_CTB_0_dyn_cfg[];
extern cy_stc_autanalog_ctb_t CYBSP_CTB_0_cfg;
extern cy_stc_autanalog_stt_ctb_t CYBSP_CTB_0_stt[];
extern cy_stc_autanalog_sar_mux_chan_t CYBSP_SAR_ADC_mux_ch_cfg[];
extern cy_stc_autanalog_sar_sta_lp_t CYBSP_SAR_ADC_sta_lp_cfg;
extern cy_stc_autanalog_sar_sta_t CYBSP_SAR_ADC_sta_cfg;
extern cy_stc_autanalog_sar_seq_tab_lp_t CYBSP_SAR_ADC_seq_lp_cfg[];
extern cy_stc_autanalog_fifo_cfg_t CYBSP_SAR_ADC_fifo_cfg;
extern cy_stc_autanalog_sar_t CYBSP_SAR_ADC_cfg;
extern cy_stc_autanalog_stt_sar_t CYBSP_SAR_ADC_stt[];

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_ADC)
extern mtb_hal_adc_configurator_t CYBSP_SAR_ADC_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_ADC) */

extern const cy_stc_pdm_pcm_config_v2_t CYBSP_PDM_config;
extern const cy_stc_pdm_pcm_channel_config_t channel_2_config;
extern const cy_stc_pdm_pcm_channel_config_t channel_3_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_PDM_channel_2_obj;
extern const cyhal_resource_inst_t CYBSP_PDM_channel_3_obj;
#endif /* defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) && !defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK)
extern const cyhal_clock_t CYBSP_PDM_clock;
#endif /* defined (CY_USING_HAL) && !defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK) */

#if defined (CY_USING_HAL) && defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK)
extern cyhal_clock_t CYBSP_PDM_clock;
#endif /* defined (CY_USING_HAL) && defined(CYHAL_API_AVAILABLE_CLOCK_GET_PCLK) */

#if defined (CY_USING_HAL)
extern const cyhal_pdm_pcm_configurator_t CYBSP_PDM_channel_2_hal_config;
extern const cyhal_pdm_pcm_configurator_t CYBSP_PDM_channel_3_hal_config;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_scb_i2c_config_t CYBSP_I2C_CONTROLLER_0_config;

#define CYBSP_I2C_1V8_config CYBSP_I2C_CONTROLLER_0_config

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_resource_inst_t CYBSP_I2C_CONTROLLER_0_obj;
#define CYBSP_I2C_1V8_obj CYBSP_I2C_CONTROLLER_0_obj
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
extern const cyhal_clock_t CYBSP_I2C_CONTROLLER_0_clock;
#define CYBSP_I2C_1V8_clock CYBSP_I2C_CONTROLLER_0_clock
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_i2c_configurator_t CYBSP_I2C_CONTROLLER_0_hal_config;
#define CYBSP_I2C_1V8_hal_config CYBSP_I2C_CONTROLLER_0_hal_config
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_peri_div_t CYBSP_I2C_CONTROLLER_0_clock_ref;
#define CYBSP_I2C_1V8_clock_ref CYBSP_I2C_CONTROLLER_0_clock_ref
extern const mtb_hal_clock_t CYBSP_I2C_CONTROLLER_0_hal_clock;
#define CYBSP_I2C_1V8_hal_clock CYBSP_I2C_CONTROLLER_0_hal_clock
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C)
extern const mtb_hal_i2c_configurator_t CYBSP_I2C_CONTROLLER_0_hal_config;
#define CYBSP_I2C_1V8_hal_config CYBSP_I2C_CONTROLLER_0_hal_config
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C) */

extern const cy_stc_scb_spi_config_t CYBSP_SPI_CONTROLLER_10_config;

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_resource_inst_t CYBSP_SPI_CONTROLLER_10_obj;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
extern const cyhal_clock_t CYBSP_SPI_CONTROLLER_10_clock;
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_spi_configurator_t CYBSP_SPI_CONTROLLER_10_hal_config;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_peri_div_t CYBSP_SPI_CONTROLLER_10_clock_ref;
extern const mtb_hal_clock_t CYBSP_SPI_CONTROLLER_10_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI)
extern const mtb_hal_spi_configurator_t CYBSP_SPI_CONTROLLER_10_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI) */

extern const cy_stc_scb_i2c_config_t CYBSP_I2C_CONTROLLER_11_config;

#define CYBSP_I2C_3V3_config CYBSP_I2C_CONTROLLER_11_config

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_resource_inst_t CYBSP_I2C_CONTROLLER_11_obj;
#define CYBSP_I2C_3V3_obj CYBSP_I2C_CONTROLLER_11_obj
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
extern const cyhal_clock_t CYBSP_I2C_CONTROLLER_11_clock;
#define CYBSP_I2C_3V3_clock CYBSP_I2C_CONTROLLER_11_clock
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_i2c_configurator_t CYBSP_I2C_CONTROLLER_11_hal_config;
#define CYBSP_I2C_3V3_hal_config CYBSP_I2C_CONTROLLER_11_hal_config
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_peri_div_t CYBSP_I2C_CONTROLLER_11_clock_ref;
#define CYBSP_I2C_3V3_clock_ref CYBSP_I2C_CONTROLLER_11_clock_ref
extern const mtb_hal_clock_t CYBSP_I2C_CONTROLLER_11_hal_clock;
#define CYBSP_I2C_3V3_hal_clock CYBSP_I2C_CONTROLLER_11_hal_clock
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C)
extern const mtb_hal_i2c_configurator_t CYBSP_I2C_CONTROLLER_11_hal_config;
#define CYBSP_I2C_3V3_hal_config CYBSP_I2C_CONTROLLER_11_hal_config
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_I2C) */

extern const cy_stc_scb_uart_config_t CYBSP_DEBUG_UART_config;

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_resource_inst_t CYBSP_DEBUG_UART_obj;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
extern const cyhal_clock_t CYBSP_DEBUG_UART_clock;
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_uart_configurator_t CYBSP_DEBUG_UART_hal_config;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_peri_div_t CYBSP_DEBUG_UART_clock_ref;
extern const mtb_hal_clock_t CYBSP_DEBUG_UART_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_UART)
extern const mtb_hal_uart_configurator_t CYBSP_DEBUG_UART_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_UART) */

extern const cy_stc_scb_spi_config_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_config;

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_resource_inst_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_obj;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
extern const cyhal_clock_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_clock;
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_spi_configurator_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_hal_config;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_peri_div_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_clock_ref;
extern const mtb_hal_clock_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI)
extern const mtb_hal_spi_configurator_t CYBSP_SPI_CONTROLLER_3CYBSP_RS_SPI_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SPI) */

extern const cy_stc_scb_uart_config_t CYBSP_BT_UART_config;

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_resource_inst_t CYBSP_BT_UART_obj;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
extern const cyhal_clock_t CYBSP_BT_UART_clock;
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_uart_configurator_t CYBSP_BT_UART_hal_config;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_peri_div_t CYBSP_BT_UART_clock_ref;
extern const mtb_hal_clock_t CYBSP_BT_UART_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_UART)
extern const mtb_hal_uart_configurator_t CYBSP_BT_UART_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_UART) */

extern cy_en_sd_host_card_capacity_t CYBSP_WIFI_SDIO_cardCapacity;
extern cy_en_sd_host_card_type_t CYBSP_WIFI_SDIO_cardType;
extern uint32_t CYBSP_WIFI_SDIO_rca;
extern const cy_stc_sd_host_init_config_t CYBSP_WIFI_SDIO_config;
extern cy_stc_sd_host_sd_card_config_t CYBSP_WIFI_SDIO_card_cfg;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_WIFI_SDIO_obj;
extern const cyhal_clock_t CYBSP_WIFI_SDIO_clock;
extern const cyhal_sdhc_configurator_t CYBSP_WIFI_SDIO_sdhc_hal_config;
extern const cyhal_sdio_configurator_t CYBSP_WIFI_SDIO_sdio_hal_config;
#endif /* defined (CY_USING_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDHC)
extern const mtb_hal_peri_div_t CYBSP_WIFI_SDIO_clock_ref;
extern const mtb_hal_clock_t CYBSP_WIFI_SDIO_hal_clock;
extern const mtb_hal_sdhc_configurator_t CYBSP_WIFI_SDIO_sdhc_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDHC) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDIO)
extern const mtb_hal_sdio_configurator_t CYBSP_WIFI_SDIO_sdio_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDIO) */

extern cy_en_sd_host_card_capacity_t CYBSP_SDHC_1_cardCapacity;
extern cy_en_sd_host_card_type_t CYBSP_SDHC_1_cardType;
extern uint32_t CYBSP_SDHC_1_rca;
extern const cy_stc_sd_host_init_config_t CYBSP_SDHC_1_config;
extern cy_stc_sd_host_sd_card_config_t CYBSP_SDHC_1_card_cfg;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_SDHC_1_obj;
extern const cyhal_clock_t CYBSP_SDHC_1_clock;
extern const cyhal_sdhc_configurator_t CYBSP_SDHC_1_sdhc_hal_config;
extern const cyhal_sdio_configurator_t CYBSP_SDHC_1_sdio_hal_config;
#endif /* defined (CY_USING_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDHC)
extern const mtb_hal_peri_div_t CYBSP_SDHC_1_clock_ref;
extern const mtb_hal_clock_t CYBSP_SDHC_1_hal_clock;
extern const mtb_hal_sdhc_configurator_t CYBSP_SDHC_1_sdhc_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDHC) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDIO)
extern const mtb_hal_sdio_configurator_t CYBSP_SDHC_1_sdio_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_SDIO) */

extern const cy_stc_smif_config_t CYBSP_SMIF_CORE_0_XSPI_FLASH_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_SMIF_CORE_0_XSPI_FLASH_obj;
extern const cyhal_qspi_configurator_t CYBSP_SMIF_CORE_0_XSPI_FLASH_hal_config;
#endif /* defined (CY_USING_HAL) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_hf_clock_t CYBSP_SMIF_CORE_0_XSPI_FLASH_clock_ref;
extern const mtb_hal_clock_t CYBSP_SMIF_CORE_0_XSPI_FLASH_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI)
extern const mtb_hal_memoryspi_configurator_t CYBSP_SMIF_CORE_0_XSPI_FLASH_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI) */

extern const cy_stc_smif_config_t CYBSP_SMIF_CORE_1_PSRAM_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_SMIF_CORE_1_PSRAM_obj;
extern const cyhal_qspi_configurator_t CYBSP_SMIF_CORE_1_PSRAM_hal_config;
#endif /* defined (CY_USING_HAL) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_hf_clock_t CYBSP_SMIF_CORE_1_PSRAM_clock_ref;
extern const mtb_hal_clock_t CYBSP_SMIF_CORE_1_PSRAM_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI)
extern const mtb_hal_memoryspi_configurator_t CYBSP_SMIF_CORE_1_PSRAM_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI) */

extern const cy_stc_mcwdt_config_t CYBSP_CM33_LPTIMER_0_config;

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER)
extern const mtb_hal_lptimer_configurator_t CYBSP_CM33_LPTIMER_0_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER) */

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_CM33_LPTIMER_0_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_mcwdt_config_t CYBSP_CM55_LPTIMER_1_config;

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER)
extern const mtb_hal_lptimer_configurator_t CYBSP_CM55_LPTIMER_1_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_LPTIMER) */

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_CM55_LPTIMER_1_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_rtc_config_t CYBSP_RTC_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_RTC_obj;
extern const cyhal_rtc_configurator_t CYBSP_RTC_hal_config;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_counter_config_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_config;

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_resource_inst_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_obj;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
extern const cyhal_clock_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_clock;
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_timer_configurator_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_hal_config;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL)
extern const mtb_hal_peri_div_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_clock_ref;
extern const mtb_hal_clock_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_TIMER)
extern const mtb_hal_timer_configurator_t CYBSP_TCPWM_0_GRP_0_COUNTER_0_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_TIMER) */

extern cy_stc_tcpwm_counter_config_t emUSB_OS_Timer_config;

#if defined (COMPONENT_MTB_HAL)
extern mtb_hal_peri_div_t emUSB_OS_Timer_clock_ref;
extern mtb_hal_clock_t emUSB_OS_Timer_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_TIMER)
extern mtb_hal_timer_configurator_t emUSB_OS_Timer_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_TIMER) */

extern cy_stc_tcpwm_pwm_config_t CYBSP_TCPWM_0_GRP_1_PWM_5_config;

#if defined (COMPONENT_MTB_HAL)
extern mtb_hal_peri_div_t CYBSP_TCPWM_0_GRP_1_PWM_5_clock_ref;
extern mtb_hal_clock_t CYBSP_TCPWM_0_GRP_1_PWM_5_hal_clock;
#endif /* defined (COMPONENT_MTB_HAL) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_PWM)
extern mtb_hal_pwm_configurator_t CYBSP_TCPWM_0_GRP_1_PWM_5_hal_config;
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_PWM) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cyhal_resource_inst_t CYBSP_TCPWM_0_GRP_1_PWM_5_obj;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

#if defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL)
extern cyhal_clock_t CYBSP_TCPWM_0_GRP_1_PWM_5_clock;
#endif /* defined(CY_USING_HAL_LITE) || defined (CY_USING_HAL) */

#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern cyhal_pwm_configurator_t CYBSP_TCPWM_0_GRP_1_PWM_5_hal_config;
#endif /* defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE) */

extern cy_stc_tdm_config_tx_t CYBSP_TDM_CONTROLLER_0_tx_config;
extern cy_stc_tdm_config_rx_t CYBSP_TDM_CONTROLLER_0_rx_config;
extern const cy_stc_tdm_config_t CYBSP_TDM_CONTROLLER_0_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_TDM_CONTROLLER_0_obj;
extern const cyhal_clock_t CYBSP_TDM_CONTROLLER_0_clock;
extern const cyhal_tdm_configurator_t CYBSP_TDM_CONTROLLER_0_hal_config;
#endif /* defined (CY_USING_HAL) */

void init_cycfg_peripherals(void);
void reserve_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_PERIPHERALS_H */
