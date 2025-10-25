/*******************************************************************************
 * File Name: cycfg_protection.c
 *
 * Description:
 * System configuration
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

#include "cycfg_protection.h"

#if defined (CY_PDL_TZ_ENABLED)
const cy_stc_mpc_rot_cfg_t M33S_mpc_cfg[] =
{
    {
        .pc = CY_MPC_PC_2,
        .secure = CY_MPC_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
    {
        .pc = CY_MPC_PC_7,
        .secure = CY_MPC_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
};
const cy_stc_mpc_rot_cfg_t M33_mpc_cfg[] =
{
    {
        .pc = CY_MPC_PC_2,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
    {
        .pc = CY_MPC_PC_5,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
    {
        .pc = CY_MPC_PC_7,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
};
const cy_stc_mpc_rot_cfg_t M55_mpc_cfg[] =
{
    {
        .pc = CY_MPC_PC_2,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
    {
        .pc = CY_MPC_PC_6,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
    {
        .pc = CY_MPC_PC_7,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
};
const cy_stc_mpc_rot_cfg_t M33NSC_mpc_cfg[] =
{
    {
        .pc = CY_MPC_PC_2,
        .secure = CY_MPC_SECURE,
        .access = CY_MPC_ACCESS_R,
    },
    {
        .pc = CY_MPC_PC_5,
        .secure = CY_MPC_SECURE,
        .access = CY_MPC_ACCESS_R,
    },
    {
        .pc = CY_MPC_PC_6,
        .secure = CY_MPC_SECURE,
        .access = CY_MPC_ACCESS_R,
    },
    {
        .pc = CY_MPC_PC_7,
        .secure = CY_MPC_SECURE,
        .access = CY_MPC_ACCESS_R,
    },
};
const cy_stc_mpc_rot_cfg_t M33_M55_mpc_cfg[] =
{
    {
        .pc = CY_MPC_PC_2,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
    {
        .pc = CY_MPC_PC_5,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
    {
        .pc = CY_MPC_PC_6,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
    {
        .pc = CY_MPC_PC_7,
        .secure = CY_MPC_NON_SECURE,
        .access = CY_MPC_ACCESS_RW,
    },
};
const cy_stc_mpc_regions_t M33S_mpc_regions[] =
{
    {
        .base = (MPC_Type*)RRAMC0_MPC0,
        .offset = 0x00011000,
        .size = 0x00019000,
    },
    {
        .base = (MPC_Type*)SMIF0_CACHE_BLOCK_CACHEBLK_AHB_MPC0,
        .offset = 0x00100000,
        .size = 0x00280000,
    },
    {
        .base = (MPC_Type*)SMIF0_CORE_AXI_MPC0,
        .offset = 0x00100000,
        .size = 0x00280000,
    },
    {
        .base = (MPC_Type*)RAMC0_MPC0,
        .offset = 0x00001000,
        .size = 0x00057000,
    },
};
const cy_stc_mpc_regions_t M33_mpc_regions[] =
{
    {
        .base = (MPC_Type*)SMIF0_CACHE_BLOCK_CACHEBLK_AHB_MPC0,
        .offset = 0x00380000,
        .size = 0x00280000,
    },
    {
        .base = (MPC_Type*)SMIF0_CORE_AXI_MPC0,
        .offset = 0x00380000,
        .size = 0x00280000,
    },
    {
        .base = (MPC_Type*)RAMC0_MPC0,
        .offset = 0x00058000,
        .size = 0x00028000,
    },
    {
        .base = (MPC_Type*)RAMC1_MPC0,
        .offset = 0x00000000,
        .size = 0x0002D000,
    },
};
const cy_stc_mpc_regions_t M55_mpc_regions[] =
{
    {
        .base = (MPC_Type*)SMIF0_CACHE_BLOCK_CACHEBLK_AHB_MPC0,
        .offset = 0x00600000,
        .size = 0x008C0000,
    },
    {
        .base = (MPC_Type*)SMIF0_CORE_AXI_MPC0,
        .offset = 0x00600000,
        .size = 0x008C0000,
    },
    {
        .base = (MPC_Type*)SOCMEM_SRAM_MPC0,
        .offset = 0x00000000,
        .size = 0x002EC000,
    },
};
const cy_stc_mpc_regions_t M33NSC_mpc_regions[] = {0};
const cy_stc_mpc_regions_t M33_M55_mpc_regions[] =
{
    {
        .base = (MPC_Type*)RRAMC0_MPC0,
        .offset = 0x0002A000,
        .size = 0x00040000,
    },
    {
        .base = (MPC_Type*)SOCMEM_SRAM_MPC0,
        .offset = 0x002EC000,
        .size = 0x00214000,
    },
    {
        .base = (MPC_Type*)RAMC1_MPC0,
        .offset = 0x0002D000,
        .size = 0x00053000,
    },
};
const cy_stc_mpc_resp_cfg_t cy_response_mpcs[] =
{
    {
        .base = (MPC_Type*)SOCMEM_SRAM_MPC0,
        .response = CY_MPC_BUS_ERR,
    },
    {
        .base = (MPC_Type*)RAMC0_MPC0,
        .response = CY_MPC_BUS_ERR,
    },
    {
        .base = (MPC_Type*)RAMC1_MPC0,
        .response = CY_MPC_BUS_ERR,
    },
    {
        .base = (MPC_Type*)SMIF0_CACHE_BLOCK_CACHEBLK_AHB_MPC0,
        .response = CY_MPC_RZWI,
    },
    {
        .base = (MPC_Type*)SMIF0_CORE_AXI_MPC0,
        .response = CY_MPC_RZWI,
    },
};
const size_t cy_response_mpcs_count = sizeof(cy_response_mpcs) / sizeof(cy_stc_mpc_resp_cfg_t);
const cy_stc_mpc_unified_t unified_mpc_domains[] =
{
    {
        .regions = M33S_mpc_regions,
        .region_count = sizeof(M33S_mpc_regions) / sizeof(cy_stc_mpc_regions_t),
        .cfg = M33S_mpc_cfg,
        .cfg_count = sizeof(M33S_mpc_cfg) / sizeof(cy_stc_mpc_rot_cfg_t),
    },
    {
        .regions = M33_mpc_regions,
        .region_count = sizeof(M33_mpc_regions) / sizeof(cy_stc_mpc_regions_t),
        .cfg = M33_mpc_cfg,
        .cfg_count = sizeof(M33_mpc_cfg) / sizeof(cy_stc_mpc_rot_cfg_t),
    },
    {
        .regions = M55_mpc_regions,
        .region_count = sizeof(M55_mpc_regions) / sizeof(cy_stc_mpc_regions_t),
        .cfg = M55_mpc_cfg,
        .cfg_count = sizeof(M55_mpc_cfg) / sizeof(cy_stc_mpc_rot_cfg_t),
    },
    {
        .regions = M33NSC_mpc_regions,
        .region_count = sizeof(M33NSC_mpc_regions) / sizeof(cy_stc_mpc_regions_t),
        .cfg = M33NSC_mpc_cfg,
        .cfg_count = sizeof(M33NSC_mpc_cfg) / sizeof(cy_stc_mpc_rot_cfg_t),
    },
    {
        .regions = M33_M55_mpc_regions,
        .region_count = sizeof(M33_M55_mpc_regions) / sizeof(cy_stc_mpc_regions_t),
        .cfg = M33_M55_mpc_cfg,
        .cfg_count = sizeof(M33_M55_mpc_cfg) / sizeof(cy_stc_mpc_rot_cfg_t),
    },
};
const size_t unified_mpc_domains_count = sizeof(unified_mpc_domains) / sizeof(cy_stc_mpc_unified_t);
#endif /* defined (CY_PDL_TZ_ENABLED) */

#if defined (COMPONENT_SECURE_DEVICE) && defined(COMPONENT_MW_MTB_SRF)
const mtb_srf_memory_protection_s_t mtb_srf_memory_protection_s[] =
{
    {
        .ranges = mxrramc_0_mpc_0_srf_protection_range_s,
        .size = 4U,
    },
    {
        .ranges = smif_0_mpc_0_srf_protection_range_s,
        .size = 6U,
    },
    {
        .ranges = socmem_0_mpc_0_srf_protection_range_s,
        .size = 2U,
    },
    {
        .ranges = mxsramc_0_mpc_0_srf_protection_range_s,
        .size = 3U,
    },
    {
        .ranges = mxsramc_1_mpc_0_srf_protection_range_s,
        .size = 2U,
    },
};
const uint8_t mtb_srf_protection_range_s_count = sizeof(mtb_srf_memory_protection_s) / sizeof(mtb_srf_memory_protection_s_t);
#endif /* defined (COMPONENT_SECURE_DEVICE) && defined(COMPONENT_MW_MTB_SRF) */

const cy_stc_ppc_attribute_t cycfg_unused_ppc_cfg =
{
    .pcMask = 0xFF,
    .secAttribute = CY_PPC_NON_SECURE,
    .privAttribute = CY_PPC_NONPRIV,
};
const cy_en_prot_region_t cycfg_unused_ppc_0_regions[] =
{
    PROT_PERI0_MAIN, 
    PROT_PERI0_GR1_GROUP, 
    PROT_PERI0_GR2_GROUP, 
    PROT_PERI0_GR3_GROUP, 
    PROT_PERI0_GR4_GROUP, 
    PROT_PERI0_GR5_GROUP, 
    PROT_PERI0_TR, 
    PROT_PERI0_PERI_PCLK0_MAIN, 
    PROT_PERI0_RRAMC0_RRAM_EXTRA_AREA_RRAMC_GENERAL, 
    PROT_PERI0_RRAMC0_RRAMC0_RRAMC_USER, 
    PROT_PERI0_RRAMC0_RRAMC0_RRAMC_ALLUSER, 
    PROT_PERI0_RRAMC0_RRAM_SFR_RRAMC_SFR_USER, 
    PROT_PERI0_M33SYSCPUSS, 
    PROT_PERI0_PASS, 
    PROT_PERI0_RAMC0_CM33, 
    PROT_PERI0_RAMC1_CM33, 
    PROT_PERI0_RAMC0_RAM_PWR, 
    PROT_PERI0_RAMC1_RAM_PWR, 
    PROT_PERI0_RAMC0_MPC0_PPC_MPC_MAIN, 
    PROT_PERI0_RAMC1_MPC0_PPC_MPC_MAIN, 
    PROT_PERI0_RAMC0_MPC0_PPC_MPC_PC, 
    PROT_PERI0_RAMC1_MPC0_PPC_MPC_PC, 
    PROT_PERI0_RAMC0_MPC0_PPC_MPC_ROT, 
    PROT_PERI0_RAMC1_MPC0_PPC_MPC_ROT, 
    PROT_PERI0_MXCM33_CM33, 
    PROT_PERI0_MXCM33_CM33_S, 
    PROT_PERI0_MXCM33_CM33_NS, 
    PROT_PERI0_MXCM33_BOOT_PC2, 
    PROT_PERI0_MXCM33_CM33_INT, 
    PROT_PERI0_DW0_DW, 
    PROT_PERI0_DW1_DW, 
    PROT_PERI0_DW0_DW_CRC, 
    PROT_PERI0_DW1_DW_CRC, 
    PROT_PERI0_DW0_CH_STRUCT0_CH, 
    PROT_PERI0_DW0_CH_STRUCT1_CH, 
    PROT_PERI0_DW0_CH_STRUCT2_CH, 
    PROT_PERI0_DW0_CH_STRUCT3_CH, 
    PROT_PERI0_DW0_CH_STRUCT4_CH, 
    PROT_PERI0_DW0_CH_STRUCT5_CH, 
    PROT_PERI0_DW0_CH_STRUCT6_CH, 
    PROT_PERI0_DW0_CH_STRUCT7_CH, 
    PROT_PERI0_DW0_CH_STRUCT8_CH, 
    PROT_PERI0_DW0_CH_STRUCT9_CH, 
    PROT_PERI0_DW0_CH_STRUCT10_CH, 
    PROT_PERI0_DW0_CH_STRUCT11_CH, 
    PROT_PERI0_DW0_CH_STRUCT12_CH, 
    PROT_PERI0_DW0_CH_STRUCT13_CH, 
    PROT_PERI0_DW0_CH_STRUCT14_CH, 
    PROT_PERI0_DW0_CH_STRUCT15_CH, 
    PROT_PERI0_DW1_CH_STRUCT0_CH, 
    PROT_PERI0_DW1_CH_STRUCT1_CH, 
    PROT_PERI0_DW1_CH_STRUCT2_CH, 
    PROT_PERI0_DW1_CH_STRUCT3_CH, 
    PROT_PERI0_DW1_CH_STRUCT4_CH, 
    PROT_PERI0_DW1_CH_STRUCT5_CH, 
    PROT_PERI0_DW1_CH_STRUCT6_CH, 
    PROT_PERI0_DW1_CH_STRUCT7_CH, 
    PROT_PERI0_DW1_CH_STRUCT8_CH, 
    PROT_PERI0_DW1_CH_STRUCT9_CH, 
    PROT_PERI0_DW1_CH_STRUCT10_CH, 
    PROT_PERI0_DW1_CH_STRUCT11_CH, 
    PROT_PERI0_DW1_CH_STRUCT12_CH, 
    PROT_PERI0_DW1_CH_STRUCT13_CH, 
    PROT_PERI0_DW1_CH_STRUCT14_CH, 
    PROT_PERI0_DW1_CH_STRUCT15_CH, 
    PROT_PERI0_CPUSS_ALL_PC, 
    PROT_PERI0_CPUSS_DDFT, 
    PROT_PERI0_CPUSS_CM33_S, 
    PROT_PERI0_CPUSS_CM33_NS, 
    PROT_PERI0_CPUSS_MSC_INT, 
    PROT_PERI0_CPUSS_BOOT, 
    PROT_PERI0_MS_PC0_PRIV, 
    PROT_PERI0_MS_PC6_PRIV, 
    PROT_PERI0_MS_PC9_PRIV, 
    PROT_PERI0_MS_PC11_PRIV, 
    PROT_PERI0_MS_PC0_PRIV_MIR, 
    PROT_PERI0_MS_PC6_PRIV_MIR, 
    PROT_PERI0_MS_PC9_PRIV_MIR, 
    PROT_PERI0_MS_PC11_PRIV_MIR, 
    PROT_PERI0_MS_PC31_PRIV_MIR, 
    PROT_PERI0_MSC_ACG, 
    PROT_PERI0_IPC0_STRUCT0_IPC, 
    PROT_PERI0_IPC0_STRUCT1_IPC, 
    PROT_PERI0_IPC0_STRUCT2_IPC, 
    PROT_PERI0_IPC0_STRUCT3_IPC, 
    PROT_PERI0_IPC0_STRUCT4_IPC, 
    PROT_PERI0_IPC0_STRUCT5_IPC, 
    PROT_PERI0_IPC0_STRUCT6_IPC, 
    PROT_PERI0_IPC0_STRUCT7_IPC, 
    PROT_PERI0_IPC0_STRUCT8_IPC, 
    PROT_PERI0_IPC0_STRUCT9_IPC, 
    PROT_PERI0_IPC0_STRUCT10_IPC, 
    PROT_PERI0_IPC0_STRUCT11_IPC, 
    PROT_PERI0_IPC0_STRUCT12_IPC, 
    PROT_PERI0_IPC0_STRUCT13_IPC, 
    PROT_PERI0_IPC0_STRUCT14_IPC, 
    PROT_PERI0_IPC0_STRUCT15_IPC, 
    PROT_PERI0_IPC0_INTR_STRUCT0_INTR, 
    PROT_PERI0_IPC0_INTR_STRUCT1_INTR, 
    PROT_PERI0_IPC0_INTR_STRUCT2_INTR, 
    PROT_PERI0_IPC0_INTR_STRUCT3_INTR, 
    PROT_PERI0_IPC0_INTR_STRUCT4_INTR, 
    PROT_PERI0_IPC0_INTR_STRUCT5_INTR, 
    PROT_PERI0_IPC0_INTR_STRUCT6_INTR, 
    PROT_PERI0_IPC0_INTR_STRUCT7_INTR, 
    PROT_PERI0_FAULT_STRUCT0_MAIN, 
    PROT_PERI0_FAULT_STRUCT1_MAIN, 
    PROT_PERI0_SRSS_GENERAL, 
    PROT_PERI0_SRSS_GENERAL2, 
    PROT_PERI0_SRSS_HIB_DATA, 
    PROT_PERI0_SRSS_MAIN, 
    PROT_PERI0_RAM_TRIM_SRSS_SRAM, 
    PROT_PERI0_SRSS_WDT, 
    PROT_PERI0_SRSS_MCWDTA, 
    PROT_PERI0_PWRMODE_PWRMODE, 
    PROT_PERI0_RTC_BACKUP, 
    PROT_PERI0_RTC_B_BREG0, 
    PROT_PERI0_RTC_B_BREG1, 
    PROT_PERI0_RTC_B_BREG2, 
    PROT_PERI0_DEBUG600_DEBUG600, 
    PROT_PERI0_CRYPTO_MAIN, 
    PROT_PERI0_CRYPTO_CRYPTO, 
    PROT_PERI0_CRYPTO_BOOT, 
    PROT_PERI0_CRYPTO_KEY0, 
    PROT_PERI0_CRYPTO_KEY1, 
    PROT_PERI0_CRYPTO_BUF, 
    PROT_PERI0_HSIOM_PRT0_PRT, 
    PROT_PERI0_HSIOM_PRT1_PRT, 
    PROT_PERI0_HSIOM_PRT2_PRT, 
    PROT_PERI0_HSIOM_PRT3_PRT, 
    PROT_PERI0_HSIOM_PRT4_PRT, 
    PROT_PERI0_HSIOM_PRT5_PRT, 
    PROT_PERI0_HSIOM_PRT6_PRT, 
    PROT_PERI0_HSIOM_PRT7_PRT, 
    PROT_PERI0_HSIOM_PRT8_PRT, 
    PROT_PERI0_HSIOM_PRT9_PRT, 
    PROT_PERI0_HSIOM_PRT10_PRT, 
    PROT_PERI0_HSIOM_PRT11_PRT, 
    PROT_PERI0_HSIOM_PRT12_PRT, 
    PROT_PERI0_HSIOM_PRT13_PRT, 
    PROT_PERI0_HSIOM_PRT14_PRT, 
    PROT_PERI0_HSIOM_PRT15_PRT, 
    PROT_PERI0_HSIOM_PRT16_PRT, 
    PROT_PERI0_HSIOM_PRT17_PRT, 
    PROT_PERI0_HSIOM_PRT18_PRT, 
    PROT_PERI0_HSIOM_PRT19_PRT, 
    PROT_PERI0_HSIOM_PRT20_PRT, 
    PROT_PERI0_HSIOM_PRT21_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT0_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT1_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT2_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT3_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT4_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT5_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT6_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT7_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT8_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT9_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT10_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT11_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT12_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT13_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT14_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT15_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT16_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT17_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT18_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT19_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT20_SECURE_PRT, 
    PROT_PERI0_HSIOM_SECURE_PRT21_SECURE_PRT, 
    PROT_PERI0_HSIOM_AMUX, 
    PROT_PERI0_HSIOM_MON, 
    PROT_PERI0_GPIO_PRT0_PRT, 
    PROT_PERI0_GPIO_PRT1_PRT, 
    PROT_PERI0_GPIO_PRT2_PRT, 
    PROT_PERI0_GPIO_PRT3_PRT, 
    PROT_PERI0_GPIO_PRT4_PRT, 
    PROT_PERI0_GPIO_PRT5_PRT, 
    PROT_PERI0_GPIO_PRT6_PRT, 
    PROT_PERI0_GPIO_PRT7_PRT, 
    PROT_PERI0_GPIO_PRT8_PRT, 
    PROT_PERI0_GPIO_PRT9_PRT, 
    PROT_PERI0_GPIO_PRT10_PRT, 
    PROT_PERI0_GPIO_PRT11_PRT, 
    PROT_PERI0_GPIO_PRT12_PRT, 
    PROT_PERI0_GPIO_PRT13_PRT, 
    PROT_PERI0_GPIO_PRT14_PRT, 
    PROT_PERI0_GPIO_PRT15_PRT, 
    PROT_PERI0_GPIO_PRT16_PRT, 
    PROT_PERI0_GPIO_PRT17_PRT, 
    PROT_PERI0_GPIO_PRT18_PRT, 
    PROT_PERI0_GPIO_PRT19_PRT, 
    PROT_PERI0_GPIO_PRT20_PRT, 
    PROT_PERI0_GPIO_PRT21_PRT, 
    PROT_PERI0_GPIO_PRT0_CFG, 
    PROT_PERI0_GPIO_PRT1_CFG, 
    PROT_PERI0_GPIO_PRT2_CFG, 
    PROT_PERI0_GPIO_PRT3_CFG, 
    PROT_PERI0_GPIO_PRT4_CFG, 
    PROT_PERI0_GPIO_PRT5_CFG, 
    PROT_PERI0_GPIO_PRT6_CFG, 
    PROT_PERI0_GPIO_PRT7_CFG, 
    PROT_PERI0_GPIO_PRT8_CFG, 
    PROT_PERI0_GPIO_PRT9_CFG, 
    PROT_PERI0_GPIO_PRT10_CFG, 
    PROT_PERI0_GPIO_PRT11_CFG, 
    PROT_PERI0_GPIO_PRT12_CFG, 
    PROT_PERI0_GPIO_PRT13_CFG, 
    PROT_PERI0_GPIO_PRT14_CFG, 
    PROT_PERI0_GPIO_PRT15_CFG, 
    PROT_PERI0_GPIO_PRT16_CFG, 
    PROT_PERI0_GPIO_PRT17_CFG, 
    PROT_PERI0_GPIO_PRT18_CFG, 
    PROT_PERI0_GPIO_PRT19_CFG, 
    PROT_PERI0_GPIO_PRT20_CFG, 
    PROT_PERI0_GPIO_PRT21_CFG, 
    PROT_PERI0_GPIO_SEC_GPIO, 
    PROT_PERI0_GPIO_GPIO, 
    PROT_PERI0_GPIO_TEST, 
    PROT_PERI0_SMARTIO_PRT11_PRT, 
    PROT_PERI0_SMARTIO_PRT17_PRT, 
    PROT_PERI0_CANFD0_CH0_CH, 
    PROT_PERI0_CANFD0_CH1_CH, 
    PROT_PERI0_CANFD0_MAIN, 
    PROT_PERI0_CANFD0_BUF, 
    PROT_PERI0_TCPWM0_GRP0_CNT0_CNT, 
    PROT_PERI0_TCPWM0_GRP0_CNT1_CNT, 
    PROT_PERI0_TCPWM0_GRP0_CNT2_CNT, 
    PROT_PERI0_TCPWM0_GRP0_CNT3_CNT, 
    PROT_PERI0_TCPWM0_GRP0_CNT4_CNT, 
    PROT_PERI0_TCPWM0_GRP0_CNT5_CNT, 
    PROT_PERI0_TCPWM0_GRP0_CNT6_CNT, 
    PROT_PERI0_TCPWM0_GRP0_CNT7_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT0_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT1_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT2_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT3_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT4_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT5_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT6_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT7_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT8_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT9_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT10_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT11_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT12_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT13_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT14_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT15_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT16_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT17_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT18_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT19_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT20_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT21_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT22_CNT, 
    PROT_PERI0_TCPWM0_GRP1_CNT23_CNT, 
    PROT_PERI0_LPCOMP, 
    PROT_PERI0_SCB0, 
    PROT_PERI0_SCB2, 
    PROT_PERI0_SCB3, 
    PROT_PERI0_SCB4, 
    PROT_PERI0_SCB5, 
    PROT_PERI0_SCB6, 
    PROT_PERI0_SCB7, 
    PROT_PERI0_SCB8, 
    PROT_PERI0_SCB9, 
    PROT_PERI0_SCB10, 
    PROT_PERI0_SCB11, 
    PROT_PERI0_SCB1, 
    PROT_PERI0_I3C, 
    PROT_PERI0_DFT, 
    PROT_PERI0_ETH0, 
    PROT_PERI0_MXNNLITE_2_0, 
};
const cy_en_prot_region_t cycfg_unused_ppc_1_regions[] =
{
    PROT_PERI1_MAIN, 
    PROT_PERI1_GR0_GROUP, 
    PROT_PERI1_GR1_GROUP, 
    PROT_PERI1_GR2_GROUP, 
    PROT_PERI1_GR3_GROUP, 
    PROT_PERI1_GR4_GROUP, 
    PROT_PERI1_GR0_BOOT, 
    PROT_PERI1_GR1_BOOT, 
    PROT_PERI1_GR2_BOOT, 
    PROT_PERI1_GR3_BOOT, 
    PROT_PERI1_GR4_BOOT, 
    PROT_PERI1_TR, 
    PROT_PERI1_PERI_PCLK1_MAIN, 
    PROT_PERI1_M55APPCPUSS, 
    PROT_PERI1_MXCM55_CM55, 
    PROT_PERI1_MXCM55_CM55_S, 
    PROT_PERI1_MXCM55_CM55_NS, 
    PROT_PERI1_MXCM55_BOOT, 
    PROT_PERI1_MXCM55_CM55_INT, 
    PROT_PERI1_SAXI_DMAC_TOP, 
    PROT_PERI1_SAXI_DMAC_CH0_CH, 
    PROT_PERI1_SAXI_DMAC_CH1_CH, 
    PROT_PERI1_SAXI_DMAC_CH2_CH, 
    PROT_PERI1_SAXI_DMAC_CH3_CH, 
    PROT_PERI1_APPCPUSS_ALL_PC, 
    PROT_PERI1_APPCPUSS_CM33_NS, 
    PROT_PERI1_APPCPUSS_MSC_INT, 
    PROT_PERI1_APPCPUSS_AHB2AXI_INT, 
    PROT_PERI1_APPCPUSS_AP, 
    PROT_PERI1_MS_CTL_MS13_MAIN, 
    PROT_PERI1_MS_CTL_MS14_MAIN, 
    PROT_PERI1_MS_CTL_MS15_MAIN, 
    PROT_PERI1_MS_CTL_MS19_MAIN, 
    PROT_PERI1_MS_CTL_MS23_MAIN, 
    PROT_PERI1_MS_CTL_MS25_MAIN, 
    PROT_PERI1_MS_CTL_MS26_MAIN, 
    PROT_PERI1_MS_CTL_MS27_MAIN, 
    PROT_PERI1_MS_CTL_MS28_MAIN, 
    PROT_PERI1_MS_CTL_MS_PC13_PRIV, 
    PROT_PERI1_MS_CTL_MS_PC14_PRIV, 
    PROT_PERI1_MS_CTL_MS_PC15_PRIV, 
    PROT_PERI1_MS_CTL_MS_PC19_PRIV, 
    PROT_PERI1_MS_CTL_MS_PC25_PRIV, 
    PROT_PERI1_MS_CTL_MS_PC26_PRIV, 
    PROT_PERI1_MS_CTL_MS_PC27_PRIV, 
    PROT_PERI1_MS_CTL_MS_PC28_PRIV, 
    PROT_PERI1_MS_CTL_MS_PC13_PRIV_MIR, 
    PROT_PERI1_MS_CTL_MS_PC14_PRIV_MIR, 
    PROT_PERI1_MS_CTL_MS_PC15_PRIV_MIR, 
    PROT_PERI1_MS_CTL_MS_PC19_PRIV_MIR, 
    PROT_PERI1_MS_CTL_MS_PC25_PRIV_MIR, 
    PROT_PERI1_MS_CTL_MS_PC26_PRIV_MIR, 
    PROT_PERI1_MS_CTL_MS_PC27_PRIV_MIR, 
    PROT_PERI1_MS_CTL_MS_PC28_PRIV_MIR, 
    PROT_PERI1_MS_CTL_MSC_ACG, 
    PROT_PERI1_APPCPUSS_SL_CTL_GROUP, 
    PROT_PERI1_IPC1_STRUCT0_IPC, 
    PROT_PERI1_IPC1_STRUCT1_IPC, 
    PROT_PERI1_IPC1_STRUCT2_IPC, 
    PROT_PERI1_IPC1_STRUCT3_IPC, 
    PROT_PERI1_IPC1_STRUCT4_IPC, 
    PROT_PERI1_IPC1_STRUCT5_IPC, 
    PROT_PERI1_IPC1_STRUCT6_IPC, 
    PROT_PERI1_IPC1_STRUCT7_IPC, 
    PROT_PERI1_IPC1_STRUCT8_IPC, 
    PROT_PERI1_IPC1_STRUCT9_IPC, 
    PROT_PERI1_IPC1_STRUCT10_IPC, 
    PROT_PERI1_IPC1_STRUCT11_IPC, 
    PROT_PERI1_IPC1_STRUCT12_IPC, 
    PROT_PERI1_IPC1_STRUCT13_IPC, 
    PROT_PERI1_IPC1_STRUCT14_IPC, 
    PROT_PERI1_IPC1_STRUCT15_IPC, 
    PROT_PERI1_IPC1_INTR_STRUCT0_INTR, 
    PROT_PERI1_IPC1_INTR_STRUCT1_INTR, 
    PROT_PERI1_IPC1_INTR_STRUCT2_INTR, 
    PROT_PERI1_IPC1_INTR_STRUCT3_INTR, 
    PROT_PERI1_IPC1_INTR_STRUCT4_INTR, 
    PROT_PERI1_IPC1_INTR_STRUCT5_INTR, 
    PROT_PERI1_IPC1_INTR_STRUCT6_INTR, 
    PROT_PERI1_IPC1_INTR_STRUCT7_INTR, 
    PROT_PERI1_PDM0_MAIN, 
    PROT_PERI1_PDM0_CH0_RX, 
    PROT_PERI1_PDM0_CH1_RX, 
    PROT_PERI1_PDM0_CH2_RX, 
    PROT_PERI1_PDM0_CH3_RX, 
    PROT_PERI1_PDM0_CH4_RX, 
    PROT_PERI1_PDM0_CH5_RX, 
    PROT_PERI1_TDM0_TDM_STRUCT0_TDM_TX_STRUCT_TX, 
    PROT_PERI1_TDM0_TDM_STRUCT1_TDM_TX_STRUCT_TX, 
    PROT_PERI1_TDM0_TDM_STRUCT0_TDM_RX_STRUCT_RX, 
    PROT_PERI1_TDM0_TDM_STRUCT1_TDM_RX_STRUCT_RX, 
    PROT_PERI1_SMIF0_CACHE_BLOCK_MAIN, 
    PROT_PERI1_SMIF1_CACHE_BLOCK_MAIN, 
    PROT_PERI1_SMIF0_CACHE_BLOCK_MMIO_MAIN, 
    PROT_PERI1_SMIF1_CACHE_BLOCK_MMIO_MAIN, 
    PROT_PERI1_SMIF0_CACHE_BLOCK_CACHEBLK_AHB_MPC0_PPC_MPC_MAIN, 
    PROT_PERI1_SMIF1_CACHE_BLOCK_CACHEBLK_AHB_MPC0_PPC_MPC_MAIN, 
    PROT_PERI1_SMIF0_CACHE_BLOCK_CACHEBLK_AHB_MPC0_PPC_MPC_PC, 
    PROT_PERI1_SMIF1_CACHE_BLOCK_CACHEBLK_AHB_MPC0_PPC_MPC_PC, 
    PROT_PERI1_SMIF0_CACHE_BLOCK_CACHEBLK_AHB_MPC0_PPC_MPC_ROT, 
    PROT_PERI1_SMIF1_CACHE_BLOCK_CACHEBLK_AHB_MPC0_PPC_MPC_ROT, 
    PROT_PERI1_SMIF0_CORE_MAIN, 
    PROT_PERI1_SMIF1_CORE_MAIN, 
    PROT_PERI1_SMIF0_CORE_CRYPTO, 
    PROT_PERI1_SMIF1_CORE_CRYPTO, 
    PROT_PERI1_SMIF0_CORE_MAIN2, 
    PROT_PERI1_SMIF1_CORE_MAIN2, 
    PROT_PERI1_SMIF0_CORE_DEVICE, 
    PROT_PERI1_SMIF1_CORE_DEVICE, 
    PROT_PERI1_SMIF0_CORE_AXI_MPC0_PPC_MPC_MAIN, 
    PROT_PERI1_SMIF1_CORE_AXI_MPC0_PPC_MPC_MAIN, 
    PROT_PERI1_SMIF0_CORE_AXI_MPC0_PPC_MPC_PC, 
    PROT_PERI1_SMIF1_CORE_AXI_MPC0_PPC_MPC_PC, 
    PROT_PERI1_SMIF0_CORE_AXI_MPC0_PPC_MPC_ROT, 
    PROT_PERI1_SMIF1_CORE_AXI_MPC0_PPC_MPC_ROT, 
    PROT_PERI1_SMIF0_CORE_SMIF_HSIOM_SMIF_PRT0_PRT, 
    PROT_PERI1_SMIF0_CORE_SMIF_HSIOM_SMIF_PRT1_PRT, 
    PROT_PERI1_SMIF0_CORE_SMIF_HSIOM_SMIF_PRT2_PRT, 
    PROT_PERI1_SMIF1_CORE_SMIF_HSIOM_SMIF_PRT0_PRT, 
    PROT_PERI1_SMIF1_CORE_SMIF_HSIOM_SMIF_PRT1_PRT, 
    PROT_PERI1_SMIF1_CORE_SMIF_HSIOM_SMIF_PRT2_PRT, 
    PROT_PERI1_SMIF0_CORE_SMIF_HSIOM_MON, 
    PROT_PERI1_SMIF1_CORE_SMIF_HSIOM_MON, 
    PROT_PERI1_SMIF0_CORE_SMIF_GPIO_SMIF_PRT0_PRT, 
    PROT_PERI1_SMIF0_CORE_SMIF_GPIO_SMIF_PRT1_PRT, 
    PROT_PERI1_SMIF0_CORE_SMIF_GPIO_SMIF_PRT2_PRT, 
    PROT_PERI1_SMIF1_CORE_SMIF_GPIO_SMIF_PRT0_PRT, 
    PROT_PERI1_SMIF1_CORE_SMIF_GPIO_SMIF_PRT1_PRT, 
    PROT_PERI1_SMIF1_CORE_SMIF_GPIO_SMIF_PRT2_PRT, 
    PROT_PERI1_SMIF0_CORE_SMIF_GPIO_SMIF_PRT0_CFG, 
    PROT_PERI1_SMIF0_CORE_SMIF_GPIO_SMIF_PRT1_CFG, 
    PROT_PERI1_SMIF0_CORE_SMIF_GPIO_SMIF_PRT2_CFG, 
    PROT_PERI1_SMIF1_CORE_SMIF_GPIO_SMIF_PRT0_CFG, 
    PROT_PERI1_SMIF1_CORE_SMIF_GPIO_SMIF_PRT1_CFG, 
    PROT_PERI1_SMIF1_CORE_SMIF_GPIO_SMIF_PRT2_CFG, 
    PROT_PERI1_SMIF0_CORE_SMIF_GPIO_GPIO, 
    PROT_PERI1_SMIF1_CORE_SMIF_GPIO_GPIO, 
    PROT_PERI1_SMIF0_CORE_SMIF_GPIO_TEST, 
    PROT_PERI1_SMIF1_CORE_SMIF_GPIO_TEST, 
    PROT_PERI1_U550_U55, 
    PROT_PERI1_U550_ROT, 
    PROT_PERI1_U550_ACG, 
    PROT_PERI1_U550_MXU55_PPU_PPU, 
    PROT_PERI1_U550_PD_PD, 
    PROT_PERI1_SOCMEM_MAIN, 
    PROT_PERI1_SOCMEM_BOOT, 
    PROT_PERI1_SOCMEM_SOCMEM_PWR, 
    PROT_PERI1_SOCMEM_SRAM_MPC0_PPC_MPC_MAIN, 
    PROT_PERI1_SOCMEM_SRAM_MPC0_PPC_MPC_PC, 
    PROT_PERI1_SOCMEM_SRAM_MPC0_PPC_MPC_ROT, 
    PROT_PERI1_SOCMEM_MAIN_PORT, 
    PROT_PERI1_SOCMEM_PPU_SOCMEM_PPU, 
    PROT_PERI1_SOCMEM_SOCMEM_PD_PD, 
    PROT_PERI1_GFXSS_GPU_GFXSS, 
    PROT_PERI1_GFXSS_DC_GFXSS, 
    PROT_PERI1_GFXSS_MIPIDSI_GFXSS, 
    PROT_PERI1_SDHC0, 
    PROT_PERI1_SDHC1, 
    PROT_PERI1_USBHS_DWC_otg_intreg_CONTROLLER, 
    PROT_PERI1_USBHS_SS_SUBSYSTEM, 
    PROT_PERI1_ITCM, 
    PROT_PERI1_DTCM, 
};
const size_t cycfg_unused_ppc_0_regions_count = sizeof(cycfg_unused_ppc_0_regions) / sizeof(cy_en_prot_region_t);
const size_t cycfg_unused_ppc_1_regions_count = sizeof(cycfg_unused_ppc_1_regions) / sizeof(cy_en_prot_region_t);

cy_rslt_t Cy_PPC_UnsecureInit(PPC_Type* base, const cy_en_prot_region_t regions[], size_t count);

cy_rslt_t Cy_PPC_UnsecureInit(PPC_Type* base, const cy_en_prot_region_t regions[], size_t count)
{
    cy_rslt_t ret = Cy_Ppc_InitPpc(base, CY_PPC_BUS_ERR);
    
    for (size_t index = 0; ret == CY_PPC_SUCCESS && index < count; index++)
    {
        ret = Cy_Ppc_ConfigAttrib(base, regions[index], &cycfg_unused_ppc_cfg);
        if (ret == CY_RSLT_SUCCESS)
            ret = Cy_Ppc_SetPcMask(base, regions[index], PPC_PC_MASK_ALL_ACCESS);
    }
    return ret;
}
cy_rslt_t Cy_PPC0_Init(void)
{
    return Cy_PPC_UnsecureInit(PPC0, cycfg_unused_ppc_0_regions, cycfg_unused_ppc_0_regions_count);
}
cy_rslt_t Cy_PPC1_Init(void)
{
    return Cy_PPC_UnsecureInit(PPC1, cycfg_unused_ppc_1_regions, cycfg_unused_ppc_1_regions_count);
}
void init_cycfg_protection(void)
{
#if (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE) && !defined(CYBSP_SKIP_SAU_INIT)
    Cy_SauInit();
#endif /* (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE) && !defined(CYBSP_SKIP_SAU_INIT) */
#if (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE) && defined(CY_PDL_TZ_ENABLED) && !defined(CYBSP_SKIP_MPC_INIT)
    Cy_MPC_Init();
#endif /* (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE) && defined(CY_PDL_TZ_ENABLED) && !defined(CYBSP_SKIP_MPC_INIT) */
#if (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE) && defined(CY_PDL_TZ_ENABLED) && !defined(CYBSP_SKIP_PPC_INIT)
    Cy_PPC0_Init();
    Cy_PPC1_Init();
#endif /* (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE) && defined(CY_PDL_TZ_ENABLED) && !defined(CYBSP_SKIP_PPC_INIT) */
#if (CY_SYSTEM_CPU_M55) && !defined(CYBSP_SKIP_MPU_INIT)
    Cy_MpuInit_cm55_ns_0();
#endif /* (CY_SYSTEM_CPU_M55) && !defined(CYBSP_SKIP_MPU_INIT) */
}
