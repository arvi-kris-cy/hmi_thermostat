/*******************************************************************************
 * File Name: cycfg_protection.h
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

#if !defined(CYCFG_PROTECTION_H)
#define CYCFG_PROTECTION_H

#include "cycfg_notices.h"
#include "cy_mpc.h"
#include "cycfg_system.h"
#include "cycfg_mpc.h"
#include "cy_ppc.h"

#if (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE) && !defined(CYBSP_SKIP_SAU_INIT)
#include "cycfg_sau.h"
#endif /* (CY_SYSTEM_CPU_M33) && defined(COMPONENT_SECURE_DEVICE) && !defined(CYBSP_SKIP_SAU_INIT) */

#if (CY_SYSTEM_CPU_M55) && !defined(CYBSP_SKIP_MPU_INIT)
#include "cycfg_mpu_cm55_ns_0.h"
#endif /* (CY_SYSTEM_CPU_M55) && !defined(CYBSP_SKIP_MPU_INIT) */

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define vres_0_protection_0_ENABLED 1U
#define M33S_ENABLED 1U
#define M33_ENABLED 1U
#define M55_ENABLED 1U
#define M33NSC_ENABLED 1U
#define M33_M55_ENABLED 1U
#define reserved_ENABLED 1U
#define vres_0_protection_0_mpc_0_ENABLED 1U
#define M33S_UNIFIED_MPC_DOMAIN_IDX 0U
#define M33_UNIFIED_MPC_DOMAIN_IDX 1U
#define M55_UNIFIED_MPC_DOMAIN_IDX 2U
#define M33NSC_UNIFIED_MPC_DOMAIN_IDX 3U
#define M33_M55_UNIFIED_MPC_DOMAIN_IDX 4U
#define PPC_PC_MASK_ALL_ACCESS 0xFFU

#if defined (CY_PDL_TZ_ENABLED)
extern const cy_stc_mpc_rot_cfg_t M33S_mpc_cfg[];
extern const cy_stc_mpc_rot_cfg_t M33_mpc_cfg[];
extern const cy_stc_mpc_rot_cfg_t M55_mpc_cfg[];
extern const cy_stc_mpc_rot_cfg_t M33NSC_mpc_cfg[];
extern const cy_stc_mpc_rot_cfg_t M33_M55_mpc_cfg[];
extern const cy_stc_mpc_regions_t M33S_mpc_regions[];
extern const cy_stc_mpc_regions_t M33_mpc_regions[];
extern const cy_stc_mpc_regions_t M55_mpc_regions[];
extern const cy_stc_mpc_regions_t M33NSC_mpc_regions[];
extern const cy_stc_mpc_regions_t M33_M55_mpc_regions[];
extern const cy_stc_mpc_resp_cfg_t cy_response_mpcs[];
extern const size_t cy_response_mpcs_count;
extern const cy_stc_mpc_unified_t unified_mpc_domains[];
extern const size_t unified_mpc_domains_count;
#endif /* defined (CY_PDL_TZ_ENABLED) */

#if defined (COMPONENT_SECURE_DEVICE) && defined(COMPONENT_MW_MTB_SRF)
extern const mtb_srf_memory_protection_s_t mtb_srf_memory_protection_s[];
extern const uint8_t mtb_srf_protection_range_s_count;
#endif /* defined (COMPONENT_SECURE_DEVICE) && defined(COMPONENT_MW_MTB_SRF) */

extern const cy_stc_ppc_attribute_t cycfg_unused_ppc_cfg;
extern const cy_en_prot_region_t cycfg_unused_ppc_0_regions[];
extern const cy_en_prot_region_t cycfg_unused_ppc_1_regions[];
extern const size_t cycfg_unused_ppc_0_regions_count;
extern const size_t cycfg_unused_ppc_1_regions_count;

cy_rslt_t Cy_PPC0_Init(void);
cy_rslt_t Cy_PPC1_Init(void);
void init_cycfg_protection(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_PROTECTION_H */
