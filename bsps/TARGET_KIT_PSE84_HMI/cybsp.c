/***************************************************************************//**
* \file cybsp.c
*
* Description:
* Provides initialization code for starting up the hardware contained on the
* Infineon board.
*
********************************************************************************
* \copyright
* Copyright 2018-2022 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation
*
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

#include <stdlib.h>
#include "cy_syspm.h"
#include "cy_sysclk.h"
#include "cybsp.h"
#include "cycfg.h"
#if defined(CY_PDL_TZ_ENABLED) && !defined(CYBSP_SKIP_MPC_INIT)
#include "cy_mpc.h"
#include "ifx_se_platform.h"
#include "ifx_se_crc32.h"
#endif // defined(CY_PDL_TZ_ENABLED) && !defined(CYBSP_SKIP_MPC_INIT)

#if defined(__cplusplus)
extern "C" {
#endif

// The sysclk deep sleep callback is recommended to be the last callback that is executed before
// entry into deep sleep mode and the first one upon exit the deep sleep mode.
// Doing so minimizes the time spent on low power mode entry and exit.
#ifndef CYBSP_SYSCLK_PM_CALLBACK_ORDER
    #define CYBSP_SYSCLK_PM_CALLBACK_ORDER  (255u)
#endif

#if defined(CY_PDL_TZ_ENABLED) && !defined(CYBSP_SKIP_MPC_INIT)
//--------------------------------------------------------------------------------------------------
// _cybsp_init_mpc
//--------------------------------------------------------------------------------------------------
cy_rslt_t _cybsp_init_mpc(MPC_Type* base, const cy_stc_mpc_rot_cfg_t* cfg_arr, size_t arr_len,
                          bool response)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    for (size_t i = 0; i < arr_len; i++)
    {
        cy_rslt_t cfg_rslt = Cy_Mpc_ConfigRotMpcStruct(base, &cfg_arr[i]);
        if (CY_RSLT_SUCCESS != cfg_rslt)
        {
            result = cfg_rslt;
        }
    }

    cy_rslt_t resp_rslt = Cy_Mpc_SetViolationResponse(base, response);
    if (CY_RSLT_SUCCESS != resp_rslt)
    {
        result = resp_rslt;
    }

    return result;
}


//--------------------------------------------------------------------------------------------------
// _cybsp_init_rram_mpc
//--------------------------------------------------------------------------------------------------
cy_rslt_t _cybsp_init_rram_mpc(MPC_Type* base, const ifx_se_mpc_rot_cfg_t* cfg_arr, size_t arr_len)
{
    CY_UNUSED_PARAMETER(base);
    cy_rslt_t result = CY_RSLT_SUCCESS;
    ifx_se_mpc_rot_cfg_crc_t rotConfigCrc;
    /* MPC protection can not be programmed for Protection Context < 2 for RRAM */
    for (uint8_t i = CYBSP_PC2; i < arr_len; i++)
    {
        rotConfigCrc.mpc_config = cfg_arr[i];
        rotConfigCrc.crc =
            IFX_CRC32_CALC((uint8_t*)(&rotConfigCrc), sizeof(rotConfigCrc) - sizeof(uint32_t));
        ifx_se_status_t cfg_rslt = ifx_se_mpc_config_rot_mpc_struct(&rotConfigCrc, IFX_SE_NULL_CTX);

        if (!IFX_SE_IS_STATUS_SUCCESS(cfg_rslt))
        {
            result = CY_MPC_FAILURE;
        }
    }

    return result;
}


#endif /* defined(CY_PDL_TZ_ENABLED) && !defined(CYBSP_SKIP_MPC_INIT) */

//--------------------------------------------------------------------------------------------------
// cybsp_init
//--------------------------------------------------------------------------------------------------
cy_rslt_t cybsp_init(void)
{
    // Setup hardware manager to track resource usage then initialize all system (clock/power) board
    // configuration
    cy_rslt_t result = CY_RSLT_SUCCESS;

    // CM33 secure is the first to boot. Initialize peripherals, SAU, any enabled MPCs, and the MPU
    // for this core
    #if (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE)
    // Initialize system clocks, peripheral clock dividers, pin configurations and routing
    cycfg_config_init();
    cycfg_config_reservations();
    #if defined(m33syscpuss_0_cm33_0_mpu_s_0_ENABLED) && !defined(CYBSP_SKIP_MPU_INIT)
    Cy_MpuInit_cm33_s_0();
    #endif

    #if !defined(CYBSP_SKIP_MPC_INIT)
    #if (defined(mxsramc_0_mpc_0_ENABLED) && (mxsramc_0_mpc_0_REGION_NUMBER > 0U))
    result = _cybsp_init_mpc((MPC_Type*)RAMC0_MPC0, mxsramc_0_mpc_0_config,
                             mxsramc_0_mpc_0_REGION_NUMBER, mxsramc_0_mpc_0_RESPONCE);
    #endif
    #if (defined(mxsramc_1_mpc_0_ENABLED) && (mxsramc_1_mpc_0_REGION_NUMBER > 0U))
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cybsp_init_mpc((MPC_Type*)RAMC1_MPC0, mxsramc_1_mpc_0_config,
                                 mxsramc_1_mpc_0_REGION_NUMBER, mxsramc_1_mpc_0_RESPONCE);
    }
    #endif
    #if (defined(rramc_0_mpc_0_ENABLED) && (rramc_0_mpc_0_REGION_NUMBER > 0U))
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cybsp_init_rram_mpc((MPC_Type*)RRAMC0_MPC0, rramc_0_mpc_0_config,
                                      rramc_0_mpc_0_REGION_NUMBER);
    }
    #endif
    #if (defined(socmem_0_mpc_0_ENABLED) && (socmem_0_mpc_0_REGION_NUMBER > 0U) && \
    defined(CY_PDL_TZ_ENABLED))
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cybsp_init_mpc((MPC_Type*)SOCMEM_SRAM_MPC0, socmem_0_mpc_0_config,
                                 socmem_0_mpc_0_REGION_NUMBER, socmem_0_mpc_0_RESPONCE);
    }
    #endif
    #if (defined(smif_0_mpc_0_ENABLED) && (smif_0_mpc_0_REGION_NUMBER > 0U))
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cybsp_init_mpc((MPC_Type*)SMIF0_CORE0_SLOW_AHB_MPC0, smif_0_mpc_0_config,
                                 smif_0_mpc_0_REGION_NUMBER, smif_0_mpc_0_RESPONCE);
    }
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cybsp_init_mpc((MPC_Type*)SMIF0_CORE0_AXI_MPC0, smif_0_mpc_0_config,
                                 smif_0_mpc_0_REGION_NUMBER, smif_0_mpc_0_RESPONCE);
    }
    #endif // if defined(smif_0_mpc_0_ENABLED)
    #if (defined(smif_0_mpc_1_ENABLED) && (smif_0_mpc_1_REGION_NUMBER > 0U))
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cybsp_init_mpc((MPC_Type*)SMIF0_CORE1_SLOW_AHB_MPC0, smif_0_mpc_1_config,
                                 smif_0_mpc_1_REGION_NUMBER, smif_0_mpc_1_RESPONCE);
    }
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cybsp_init_mpc((MPC_Type*)SMIF0_CORE1_AXI_MPC0, smif_0_mpc_1_config,
                                 smif_0_mpc_1_REGION_NUMBER, smif_0_mpc_1_RESPONCE);
    }
    #endif // if defined(smif_0_mpc_1_ENABLED)
    #endif // if !defined(CYBSP_SKIP_MPC_INIT)
    #else // CM33 non-secure or CM55

    // Initialize the peripheral-related data structures (but do not
    // re-initialize the peripherals themselves) and the MPU for this core
    init_cycfg_ns_system();
    cycfg_config_reservations();
    #if !defined(CYBSP_SKIP_MPU_INIT)
    #if (CY_SYSTEM_CPU_M33) && defined(m33syscpuss_0_cm33_0_mpu_ns_0_ENABLED) // CM33 non-secure
    Cy_MpuInit_cm33_ns_0();
    #elif (CY_SYSTEM_CPU_M55) && defined(m55appcpuss_0_cm55_0_mpu_ns_0_ENABLED) // CM55
    Cy_MpuInit_cm55_ns_0();
    #endif
    #endif // !defined(CYBSP_SKIP_MPU_INIT)
    #endif // (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE)
    // Always initialize peripheral-related data structures

    if (CY_RSLT_SUCCESS == result)
    {
        #if defined(CYBSP_CUSTOM_SYSCLK_PM_CALLBACK)
        result = cybsp_register_custom_sysclk_pm_callback();
        #endif
    }

    return result;
}


#if defined(__cplusplus)
}
#endif
