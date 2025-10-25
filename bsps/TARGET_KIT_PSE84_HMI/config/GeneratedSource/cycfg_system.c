/*******************************************************************************
 * File Name: cycfg_system.c
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

#include "cycfg_system.h"

#define CY_CFG_PWR_ENABLED 1
#define CY_CFG_PWR_INIT 1
#define CY_CFG_PWR_USING_PMIC 0
#define CY_CFG_PWR_VBACKUP_USING_VDDD 1
#define CY_CFG_PWR_REGULATOR_MODE_MIN 0
#define CY_CFG_PWR_USING_ULP 0
#define CY_CFG_PWR_USING_LP 0
#define CY_CFG_PWR_USING_HP 1

#if defined(COMPONENT_SECURE_DEVICE) && defined(COMPONENT_MW_MTB_SRF)
const mtb_srf_protection_range_s_t mxrramc_0_mpc_0_srf_protection_range_s[mxrramc_0_mpc_0_REGION_COUNT] =
{
    {
        .start = (void *) 0x22011000 ,
        .length = 0x19000U,
        .is_secure = true,
    },
    {
        .start = (void *) 0x2202A000 ,
        .length = 0x40000U,
        .is_secure = false,
    },
};
const mtb_srf_protection_range_s_t mxsramc_0_mpc_0_srf_protection_range_s[mxsramc_0_mpc_0_REGION_COUNT] =
{
    {
        .start = (void *) 0x24001000 ,
        .length = 0x57000U,
        .is_secure = true,
    },
    {
        .start = (void *) 0x24058000 ,
        .length = 0x28000U,
        .is_secure = false,
    },
};
const mtb_srf_protection_range_s_t mxsramc_1_mpc_0_srf_protection_range_s[mxsramc_1_mpc_0_REGION_COUNT] =
{
    {
        .start = (void *) 0x24080000 ,
        .length = 0x2D000U,
        .is_secure = false,
    },
    {
        .start = (void *) 0x240AD000 ,
        .length = 0x53000U,
        .is_secure = false,
    },
};
const mtb_srf_protection_range_s_t smif_0_mpc_0_srf_protection_range_s[smif_0_mpc_0_REGION_COUNT] =
{
    {
        .start = (void *) 0x60100000 ,
        .length = 0x280000U,
        .is_secure = true,
    },
    {
        .start = (void *) 0x60380000 ,
        .length = 0x280000U,
        .is_secure = false,
    },
    {
        .start = (void *) 0x60600000 ,
        .length = 0x8C0000U,
        .is_secure = false,
    },
    {
        .start = (void *) 0x60100000,
        .length = 0x280000U,
        .is_secure = true,
    },
    {
        .start = (void *) 0x60380000,
        .length = 0x280000U,
        .is_secure = false,
    },
    {
        .start = (void *) 0x60600000,
        .length = 0x8C0000U,
        .is_secure = false,
    },
};
const mtb_srf_protection_range_s_t socmem_0_mpc_0_srf_protection_range_s[socmem_0_mpc_0_REGION_COUNT] =
{
    {
        .start = (void *) 0x26000000 ,
        .length = 0x2EC000U,
        .is_secure = false,
    },
    {
        .start = (void *) 0x262EC000 ,
        .length = 0x214000U,
        .is_secure = false,
    },
};
#endif /* defined(COMPONENT_SECURE_DEVICE) && defined(COMPONENT_MW_MTB_SRF) */

void init_cycfg_ns_power(void)
{
    /* Update global variables used by DeepSleep functions */
    #if (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP)
        Cy_SysPm_DeepSleepSetup(CY_SYSPM_MODE_DEEPSLEEP);
    #elif (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP_RAM)
        Cy_SysPm_DeepSleepSetup(CY_SYSPM_MODE_DEEPSLEEP_RAM);
    #elif (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP_OFF)
        Cy_SysPm_DeepSleepSetup(CY_SYSPM_MODE_DEEPSLEEP_OFF);
    #endif /* CY_CFG_PWR_SYS_IDLE_MODE */
    
}
void init_cycfg_power(void)
{
       Cy_SysPm_Init();
    
       /* **System Active Power Mode Profile Configuration** */
       #if (CY_CFG_PWR_SYS_ACTIVE_PROFILE == CY_CFG_PWR_MODE_HP)
           Cy_SysPm_SystemEnterHp();
       #elif (CY_CFG_PWR_SYS_ACTIVE_PROFILE == CY_CFG_PWR_MODE_ULP)
           Cy_SysPm_SystemEnterUlp();
       #elif (CY_CFG_PWR_SYS_ACTIVE_PROFILE == CY_CFG_PWR_MODE_LP)
           Cy_SysPm_SystemEnterLp();
       #endif /* CY_CFG_PWR_SYS_IDLE_MODE */
    
       /* **System Domain Idle Power Mode Configuration** */
       #if (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP)
           Cy_SysPm_SetDeepSleepMode(CY_SYSPM_MODE_DEEPSLEEP);
           Cy_SysPm_SetAppDeepSleepMode(CY_SYSPM_MODE_DEEPSLEEP);
       #elif (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP_RAM)
           Cy_SysPm_SetDeepSleepMode(CY_SYSPM_MODE_DEEPSLEEP_RAM);
           Cy_SysPm_SetAppDeepSleepMode(CY_SYSPM_MODE_DEEPSLEEP_RAM);
       #elif (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP_OFF)
           Cy_SysPm_SetDeepSleepMode(CY_SYSPM_MODE_DEEPSLEEP_OFF);
           Cy_SysPm_SetAppDeepSleepMode(CY_SYSPM_MODE_DEEPSLEEP_OFF);
       #endif /* CY_CFG_PWR_SYS_IDLE_MODE */
    
       Cy_SysPm_DeepSleepUpdate();
    
       /* **Power domains Configuration** */
       #if (CY_CFG_PWR_PD1_DOMAIN)
           #if defined(CORE_NAME_CM33_0) && defined(CY_PDL_TZ_ENABLED)
               /* Enables PD1 power domain */
               Cy_System_EnablePD1();
               /* Enables APP_MMIO_TCM memory for CM55 core */
               Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_CM55_TCM_512K_PERI_NR, CY_MMIO_CM55_TCM_512K_GROUP_NR, \
                                     CY_MMIO_CM55_TCM_512K_SLAVE_NR, CY_MMIO_CM55_TCM_512K_CLK_HF_NR);
    
               /* Clear SYSCPU and APPCPU power domain dependency set by boot code */
               cy_pd_pdcm_clear_dependency(CY_PD_PDCM_APPCPU, CY_PD_PDCM_SYSCPU);
    
           #endif /* defined(CORE_NAME_CM33_0) && defined(CY_PDL_TZ_ENABLED) */
       #endif /* (CY_CFG_PWR_PD1_DOMAIN) */
}
void init_cycfg_system(void)
{
#if defined (CY_CFG_PWR_ENABLED) && defined (CY_CFG_PWR_INIT)
    init_cycfg_power();
#endif /* defined (CY_CFG_PWR_ENABLED) && defined (CY_CFG_PWR_INIT) */
}
