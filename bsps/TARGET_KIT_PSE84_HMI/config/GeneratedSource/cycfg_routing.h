<<<<<<< HEAD
/*******************************************************************************
 * File Name: cycfg_routing.h
 *
 * Description:
 * Establishes all necessary connections between hardware elements.
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

#if !defined(CYCFG_ROUTING_H)
#define CYCFG_ROUTING_H

#include "cycfg_notices.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define ioss_0_port_0_pin_0_HSIOM P0_0_SCB3_SPI_SELECT0
#define ioss_0_port_0_pin_1_HSIOM P0_1_SMIF1_SMIF0_SPIHB_SELECT2
#define ioss_0_port_1_pin_0_AUX SMIF0_SPIHB_DATA0_SMIF0_SMIF0_SPIHB_DATA0_PAD
#define ioss_0_port_1_pin_1_AUX SMIF0_SPIHB_DATA1_SMIF0_SMIF0_SPIHB_DATA1_PAD
#define ioss_0_port_1_pin_2_AUX SMIF0_SPIHB_DATA2_SMIF0_SMIF0_SPIHB_DATA2_PAD
#define ioss_0_port_1_pin_3_AUX SMIF0_SPIHB_DATA3_SMIF0_SMIF0_SPIHB_DATA3_PAD
#define ioss_0_port_2_pin_0_HSIOM P2_0_SMIF0_SMIF0_SPIHB_SELECT1
#define ioss_0_port_4_pin_0_AUX SMIF1_SPIHB_DATA0_SMIF1_SMIF0_SPIHB_DATA0_PAD
#define ioss_0_port_4_pin_1_AUX SMIF1_SPIHB_DATA1_SMIF1_SMIF0_SPIHB_DATA1_PAD
#define ioss_0_port_4_pin_2_AUX SMIF1_SPIHB_DATA2_SMIF1_SMIF0_SPIHB_DATA2_PAD
#define ioss_0_port_4_pin_3_AUX SMIF1_SPIHB_DATA3_SMIF1_SMIF0_SPIHB_DATA3_PAD
#define ioss_0_port_4_pin_4_AUX SMIF1_SPIHB_DATA4_SMIF1_SMIF0_SPIHB_DATA4_PAD
#define ioss_0_port_4_pin_5_AUX SMIF1_SPIHB_DATA5_SMIF1_SMIF0_SPIHB_DATA5_PAD
#define ioss_0_port_4_pin_6_AUX SMIF1_SPIHB_DATA6_SMIF1_SMIF0_SPIHB_DATA6_PAD
#define ioss_0_port_4_pin_7_AUX SMIF1_SPIHB_DATA7_SMIF1_SMIF0_SPIHB_DATA7_PAD
#define ioss_0_port_6_pin_5_HSIOM P6_5_SCB2_UART_RX
#define ioss_0_port_6_pin_7_HSIOM P6_7_SCB2_UART_TX
#define ioss_0_port_7_pin_0_HSIOM P7_0_SDHC1_CARD_CMD
#define ioss_0_port_7_pin_1_HSIOM P7_1_SDHC1_CLK_CARD
#define ioss_0_port_7_pin_3_HSIOM P7_3_SDHC1_CARD_DAT_3TO00
#define ioss_0_port_7_pin_4_HSIOM P7_4_SRSS_EXT_CLK
#define ioss_0_port_7_pin_5_HSIOM P7_5_SDHC1_CARD_DAT_3TO01
#define ioss_0_port_7_pin_6_HSIOM P7_6_SDHC1_CARD_DAT_3TO02
#define ioss_0_port_7_pin_7_HSIOM P7_7_SDHC1_CARD_DAT_3TO03
#define ioss_0_port_8_pin_0_HSIOM P8_0_SCB0_I2C_SCL
#define ioss_0_port_8_pin_1_HSIOM P8_1_SCB0_I2C_SDA
#define ioss_0_port_8_pin_5_HSIOM P8_5_PDM_PDM_CLK3
#define ioss_0_port_8_pin_6_HSIOM P8_6_PDM_PDM_DATA3
#define ioss_0_port_10_pin_0_HSIOM P10_0_SCB4_UART_RX
#define ioss_0_port_10_pin_1_HSIOM P10_1_SCB4_UART_TX
#define ioss_0_port_10_pin_2_HSIOM P10_2_SCB4_UART_CTS
#define ioss_0_port_10_pin_3_HSIOM P10_3_SCB4_UART_RTS
#define ioss_0_port_12_pin_0_HSIOM P12_0_SDHC0_CLK_CARD
#define ioss_0_port_12_pin_1_HSIOM P12_1_SDHC0_CARD_DAT_3TO00
#define ioss_0_port_12_pin_2_HSIOM P12_2_SDHC0_CARD_DAT_3TO01
#define ioss_0_port_12_pin_3_HSIOM P12_3_TDM_TDM_TX_FSYNC0
#define ioss_0_port_12_pin_4_HSIOM P12_4_SDHC0_CARD_DAT_3TO02
#define ioss_0_port_12_pin_5_HSIOM P12_5_SDHC0_CARD_DAT_3TO03
#define ioss_0_port_14_pin_0_ANALOG P14_0_PASS_CTBL_PADS0
#define ioss_0_port_14_pin_1_ANALOG P14_1_PASS_CTBL_PADS1
#define ioss_0_port_14_pin_2_ANALOG P14_2_PASS_CTBL_PADS2
#define ioss_0_port_14_pin_3_ANALOG P14_3_PASS_CTBL_PADS3
#define ioss_0_port_14_pin_4_ANALOG P14_4_PASS_CTBL_PADS4
#define ioss_0_port_14_pin_5_ANALOG P14_5_PASS_CTBL_PADS5
#define ioss_0_port_16_pin_0_HSIOM P16_0_SCB10_SPI_CLK
#define ioss_0_port_16_pin_1_HSIOM P16_1_SCB10_SPI_MOSI
#define ioss_0_port_16_pin_2_HSIOM P16_2_SCB10_SPI_MISO
#define ioss_0_port_16_pin_3_HSIOM P16_3_SCB10_SPI_SELECT0
#define ioss_0_port_16_pin_4_HSIOM P16_4_SCB10_SPI_SELECT1
#define ioss_0_port_16_pin_5_HSIOM P16_5_TCPWM0_LINE261
#define ioss_0_port_17_pin_2_HSIOM P17_2_SCB11_I2C_SCL
#define ioss_0_port_17_pin_3_HSIOM P17_3_SCB11_I2C_SDA
#define ioss_0_port_19_pin_0_ANALOG P19_0_SRSS_ECO_IN
#define ioss_0_port_19_pin_1_ANALOG P19_1_SRSS_ECO_OUT
#define ioss_0_port_21_pin_0_HSIOM P21_0_SDHC0_CARD_CMD
#define ioss_0_port_21_pin_1_HSIOM P21_1_TDM_TDM_TX_SD0
#define ioss_0_port_21_pin_2_HSIOM P21_2_TDM_TDM_TX_SCK0
#define ioss_0_port_21_pin_3_HSIOM P21_3_TDM_TDM_TX_MCK0
#define ioss_0_port_21_pin_4_HSIOM P21_4_SCB3_SPI_MISO
#define ioss_0_port_21_pin_5_HSIOM P21_5_SCB3_SPI_MOSI
#define ioss_0_port_21_pin_6_HSIOM P21_6_SCB3_SPI_CLK
#define CYBSP_SPI_CONTROLLER_10_tr_tx_req_0_TRIGGER_IN PERI_0_TRIG_IN_MUX_0_SCB_TX_TR_OUT10
#define CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_tr_in_0_TRIGGER_OUT PERI_0_TRIG_OUT_MUX_0_PDMA0_TR_IN1

void init_cycfg_routing(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_ROUTING_H */
=======
/*******************************************************************************
 * File Name: cycfg_routing.h
 *
 * Description:
 * Establishes all necessary connections between hardware elements.
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

#if !defined(CYCFG_ROUTING_H)
#define CYCFG_ROUTING_H

#include "cycfg_notices.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define ioss_0_port_0_pin_1_HSIOM P0_1_SMIF1_SMIF0_SPIHB_SELECT2
#define ioss_0_port_1_pin_0_AUX SMIF0_SPIHB_DATA0_SMIF0_SMIF0_SPIHB_DATA0_PAD
#define ioss_0_port_1_pin_1_AUX SMIF0_SPIHB_DATA1_SMIF0_SMIF0_SPIHB_DATA1_PAD
#define ioss_0_port_1_pin_2_AUX SMIF0_SPIHB_DATA2_SMIF0_SMIF0_SPIHB_DATA2_PAD
#define ioss_0_port_1_pin_3_AUX SMIF0_SPIHB_DATA3_SMIF0_SMIF0_SPIHB_DATA3_PAD
#define ioss_0_port_1_pin_4_AUX SMIF0_SPIHB_DATA4_SMIF0_SMIF0_SPIHB_DATA4_PAD
#define ioss_0_port_1_pin_5_AUX SMIF0_SPIHB_DATA5_SMIF0_SMIF0_SPIHB_DATA5_PAD
#define ioss_0_port_1_pin_6_AUX SMIF0_SPIHB_DATA6_SMIF0_SMIF0_SPIHB_DATA6_PAD
#define ioss_0_port_1_pin_7_AUX SMIF0_SPIHB_DATA7_SMIF0_SMIF0_SPIHB_DATA7_PAD
#define ioss_0_port_4_pin_0_AUX SMIF1_SPIHB_DATA0_SMIF1_SMIF0_SPIHB_DATA0_PAD
#define ioss_0_port_4_pin_1_AUX SMIF1_SPIHB_DATA1_SMIF1_SMIF0_SPIHB_DATA1_PAD
#define ioss_0_port_4_pin_2_AUX SMIF1_SPIHB_DATA2_SMIF1_SMIF0_SPIHB_DATA2_PAD
#define ioss_0_port_4_pin_3_AUX SMIF1_SPIHB_DATA3_SMIF1_SMIF0_SPIHB_DATA3_PAD
#define ioss_0_port_4_pin_4_AUX SMIF1_SPIHB_DATA4_SMIF1_SMIF0_SPIHB_DATA4_PAD
#define ioss_0_port_4_pin_5_AUX SMIF1_SPIHB_DATA5_SMIF1_SMIF0_SPIHB_DATA5_PAD
#define ioss_0_port_4_pin_6_AUX SMIF1_SPIHB_DATA6_SMIF1_SMIF0_SPIHB_DATA6_PAD
#define ioss_0_port_4_pin_7_AUX SMIF1_SPIHB_DATA7_SMIF1_SMIF0_SPIHB_DATA7_PAD
#define ioss_0_port_5_pin_0_HSIOM P5_0_SMIF0_SMIF0_SPIHB_SELECT0
#define ioss_0_port_6_pin_5_HSIOM P6_5_SCB2_UART_RX
#define ioss_0_port_6_pin_7_HSIOM P6_7_SCB2_UART_TX
#define ioss_0_port_7_pin_0_HSIOM P7_0_SDHC1_CARD_CMD
#define ioss_0_port_7_pin_1_HSIOM P7_1_SDHC1_CLK_CARD
#define ioss_0_port_7_pin_3_HSIOM P7_3_SDHC1_CARD_DAT_3TO00
#define ioss_0_port_7_pin_4_HSIOM P7_4_SRSS_EXT_CLK
#define ioss_0_port_7_pin_5_HSIOM P7_5_SDHC1_CARD_DAT_3TO01
#define ioss_0_port_7_pin_6_HSIOM P7_6_SDHC1_CARD_DAT_3TO02
#define ioss_0_port_7_pin_7_HSIOM P7_7_SDHC1_CARD_DAT_3TO03
#define ioss_0_port_8_pin_0_HSIOM P8_0_SCB0_I2C_SCL
#define ioss_0_port_8_pin_1_HSIOM P8_1_SCB0_I2C_SDA
#define ioss_0_port_8_pin_2_HSIOM P8_2_PDM_PDM_CLK2
#define ioss_0_port_8_pin_4_HSIOM P8_4_PDM_PDM_DATA2
#define ioss_0_port_8_pin_5_HSIOM P8_5_PDM_PDM_CLK3
#define ioss_0_port_8_pin_6_HSIOM P8_6_PDM_PDM_DATA3
#define ioss_0_port_10_pin_0_HSIOM P10_0_SCB4_UART_RX
#define ioss_0_port_10_pin_1_HSIOM P10_1_SCB4_UART_TX
#define ioss_0_port_10_pin_2_HSIOM P10_2_SCB4_UART_CTS
#define ioss_0_port_10_pin_3_HSIOM P10_3_SCB4_UART_RTS
#define ioss_0_port_12_pin_0_HSIOM P12_0_SDHC0_CLK_CARD
#define ioss_0_port_12_pin_1_HSIOM P12_1_SDHC0_CARD_DAT_3TO00
#define ioss_0_port_12_pin_2_HSIOM P12_2_SDHC0_CARD_DAT_3TO01
#define ioss_0_port_12_pin_3_HSIOM P12_3_TDM_TDM_TX_FSYNC0
#define ioss_0_port_12_pin_4_HSIOM P12_4_SDHC0_CARD_DAT_3TO02
#define ioss_0_port_12_pin_5_HSIOM P12_5_SDHC0_CARD_DAT_3TO03
#define ioss_0_port_14_pin_0_ANALOG P14_0_PASS_CTBL_PADS0
#define ioss_0_port_14_pin_1_ANALOG P14_1_PASS_CTBL_PADS1
#define ioss_0_port_14_pin_2_ANALOG P14_2_PASS_CTBL_PADS2
#define ioss_0_port_14_pin_3_ANALOG P14_3_PASS_CTBL_PADS3
#define ioss_0_port_14_pin_4_ANALOG P14_4_PASS_CTBL_PADS4
#define ioss_0_port_14_pin_5_ANALOG P14_5_PASS_CTBL_PADS5
#define ioss_0_port_16_pin_0_HSIOM P16_0_SCB10_SPI_CLK
#define ioss_0_port_16_pin_1_HSIOM P16_1_SCB10_SPI_MOSI
#define ioss_0_port_16_pin_2_HSIOM P16_2_SCB10_SPI_MISO
#define ioss_0_port_16_pin_3_HSIOM P16_3_SCB10_SPI_SELECT0
#define ioss_0_port_16_pin_4_HSIOM P16_4_SCB10_SPI_SELECT1
#define ioss_0_port_16_pin_5_HSIOM P16_5_TCPWM0_LINE261
#define ioss_0_port_17_pin_2_HSIOM P17_2_SCB11_I2C_SCL
#define ioss_0_port_17_pin_3_HSIOM P17_3_SCB11_I2C_SDA
#define ioss_0_port_19_pin_0_ANALOG P19_0_SRSS_ECO_IN
#define ioss_0_port_19_pin_1_ANALOG P19_1_SRSS_ECO_OUT
#define ioss_0_port_21_pin_0_HSIOM P21_0_SDHC0_CARD_CMD
#define ioss_0_port_21_pin_1_HSIOM P21_1_TDM_TDM_TX_SD0
#define ioss_0_port_21_pin_2_HSIOM P21_2_TDM_TDM_TX_SCK0
#define ioss_0_port_21_pin_3_HSIOM P21_3_TDM_TDM_TX_MCK0
#define ioss_0_port_21_pin_4_HSIOM P21_4_SCB3_SPI_MISO
#define ioss_0_port_21_pin_5_HSIOM P21_5_SCB3_SPI_MOSI
#define ioss_0_port_21_pin_6_HSIOM P21_6_SCB3_SPI_CLK
#define CYBSP_DMA_TX_SPI_CONTROLLER_tr_in_0_TRIGGER_OUT PERI_0_TRIG_OUT_MUX_0_PDMA0_TR_IN1
#define CYBSP_SPI_CONTROLLER_tr_tx_req_0_TRIGGER_IN PERI_0_TRIG_IN_MUX_0_SCB_TX_TR_OUT10

void init_cycfg_routing(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_ROUTING_H */
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
