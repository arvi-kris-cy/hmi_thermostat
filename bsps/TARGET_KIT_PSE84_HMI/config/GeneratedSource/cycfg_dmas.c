/*******************************************************************************
 * File Name: cycfg_dmas.c
 *
 * Description:
 * DMA configuration
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

#include "cycfg_dmas.h"

const cy_stc_dma_descriptor_config_t CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_Descriptor_0_config =
{
    .retrigger = CY_DMA_RETRIG_IM,
    .interruptType = CY_DMA_DESCR,
    .triggerOutType = CY_DMA_1ELEMENT,
    .channelState = CY_DMA_CHANNEL_DISABLED,
    .triggerInType = CY_DMA_1ELEMENT,
    .dataSize = CY_DMA_BYTE,
    .srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
    .dstTransferSize = CY_DMA_TRANSFER_SIZE_WORD,
    .descriptorType = CY_DMA_1D_TRANSFER,
    .srcAddress = NULL,
    .dstAddress = NULL,
    .srcXincrement = 1,
    .dstXincrement = 0,
    .xCount = 12,
    .srcYincrement = 0,
    .dstYincrement = 0,
    .yCount = 1,
    .nextDescriptor = &CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_Descriptor_0,
};
cy_stc_dma_descriptor_t CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_Descriptor_0 =
{
    .ctl = 0UL,
    .src = 0UL,
    .dst = 0UL,
    .xCtl = 0UL,
    .yCtl = 0UL,
    .nextPtr = 0UL,
};
const cy_stc_dma_channel_config_t CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_channelConfig =
{
    .descriptor = &CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_Descriptor_0,
    .preemptable = false,
    .priority = 3,
    .enable = false,
    .bufferable = false,
};
const cy_stc_dma_crc_config_t CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_crcConfig =
{
    .dataReverse = false,
    .dataXor = 0,
    .reminderReverse = false,
    .reminderXor = 0,
    .polynomial = 79764919,
};

#if defined (CY_USING_HAL) || defined (CY_USING_HAL_LITE)
const cyhal_resource_inst_t CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_obj =
{
    .type = CYHAL_RSC_DW,
    .block_num = 0U,
    .channel_num = CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_CHANNEL,
};
const cyhal_dma_configurator_t CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_hal_config =
{
    .resource = &CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_obj,
    .dw_channel_config = &CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_channelConfig,
    .dw_descriptor_config = &CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_Descriptor_0_config,
};
#endif /* defined (CY_USING_HAL) || defined (CY_USING_HAL_LITE) */

#if defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_DMA)
const mtb_hal_dma_configurator_t CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_hal_config =
{
    .dma_type = MTB_HAL_DMA_DW,
    .channel = CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_CHANNEL,
    .dw_base = CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_HW,
    .dw_descriptor = &CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_Descriptor_0,
};
#endif /* defined (COMPONENT_MTB_HAL) && (MTB_HAL_DRIVER_AVAILABLE_DMA) */

void reserve_cycfg_dmas(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&CYBSP_SPI_CONTROLLER_TX_DMA_0_CH_1_obj);
#endif /* defined (CY_USING_HAL) */
}
