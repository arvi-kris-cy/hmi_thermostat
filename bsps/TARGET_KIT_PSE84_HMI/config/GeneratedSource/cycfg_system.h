<<<<<<< HEAD
/*******************************************************************************
 * File Name: cycfg_system.h
 *
 * Description:
 * System configuration
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

#if !defined(CYCFG_SYSTEM_H)
#define CYCFG_SYSTEM_H

#include "cycfg_notices.h"
#include "cy_sysclk.h"
#include "cy_wdt.h"
#include "cy_gpio.h"
#include "cy_syspm.h"
#include "system_cat1d.h"

#if defined (CY_USING_HAL)
#include "cyhal_hwmgr.h"
#endif /* defined (CY_USING_HAL) */

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define srss_0_clock_0_ENABLED 1U
#define srss_0_clock_0_bakclk_0_ENABLED 1U
#define srss_0_clock_0_eco_0_ENABLED 1U
#define srss_0_clock_0_ext_0_ENABLED 1U
#define srss_0_clock_0_hfclk_0_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF0 0UL
#define CY_CFG_SYSCLK_CLKHF0_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_10_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF10 10UL
#define CY_CFG_SYSCLK_CLKHF10_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_11_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF11 11UL
#define CY_CFG_SYSCLK_CLKHF11_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_12_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF12 12UL
#define CY_CFG_SYSCLK_CLKHF12_CLKPATH_NUM 4UL
#define srss_0_clock_0_hfclk_13_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF13 13UL
#define CY_CFG_SYSCLK_CLKHF13_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_1_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF1 1UL
#define CY_CFG_SYSCLK_CLKHF1_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_2_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF2 2UL
#define CY_CFG_SYSCLK_CLKHF2_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_3_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF3 3UL
#define CY_CFG_SYSCLK_CLKHF3_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_4_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF4 4UL
#define CY_CFG_SYSCLK_CLKHF4_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_5_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF5 5UL
#define CY_CFG_SYSCLK_CLKHF5_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_6_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF6 6UL
#define CY_CFG_SYSCLK_CLKHF6_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_7_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF7 7UL
#define CY_CFG_SYSCLK_CLKHF7_CLKPATH_NUM 1UL
#define srss_0_clock_0_hfclk_8_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF8 8UL
#define CY_CFG_SYSCLK_CLKHF8_CLKPATH_NUM 0UL
#define srss_0_clock_0_hfclk_9_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF9 9UL
#define CY_CFG_SYSCLK_CLKHF9_CLKPATH_NUM 0UL
#define srss_0_clock_0_iho_0_ENABLED 1U
#define srss_0_clock_0_lfclk_0_ENABLED 1U
#define CY_CFG_SYSCLK_CLKLF_FREQ_HZ 32768
#define CY_CFG_SYSCLK_CLKLF_SOURCE CY_SYSCLK_CLKLF_IN_PILO
#define srss_0_clock_0_pathmux_0_ENABLED 1U
#define srss_0_clock_0_pathmux_1_ENABLED 1U
#define srss_0_clock_0_pathmux_2_ENABLED 1U
#define srss_0_clock_0_pathmux_3_ENABLED 1U
#define srss_0_clock_0_pathmux_4_ENABLED 1U
#define srss_0_clock_0_pathmux_5_ENABLED 1U
#define srss_0_clock_0_pilo_0_ENABLED 1U
#define srss_0_clock_0_pll250m_0_ENABLED 1U
#define srss_0_clock_0_pll250m_1_ENABLED 1U
#define srss_0_clock_0_timerclk_0_ENABLED 1U
#define srss_0_power_0_ENABLED 1U
#define CY_CFG_PWR_MODE_LP 0x01UL
#define CY_CFG_PWR_MODE_ULP 0x02UL
#define CY_CFG_PWR_MODE_HP 0x03UL
#define CY_CFG_PWR_MODE_ACTIVE 0x04UL
#define CY_CFG_PWR_MODE_SLEEP 0x08UL
#define CY_CFG_PWR_MODE_DEEPSLEEP 0x10UL
#define CY_CFG_PWR_MODE_DEEPSLEEP_RAM 0x11UL
#define CY_CFG_PWR_MODE_DEEPSLEEP_OFF 0x12UL
#define CY_CFG_PWR_SYS_IDLE_MODE CY_CFG_PWR_MODE_DEEPSLEEP
#define CY_CFG_PWR_DEEPSLEEP_LATENCY 20UL
#define CY_CFG_PWR_SYS_ACTIVE_MODE CY_CFG_PWR_MODE_HP
#define CY_CFG_PWR_SYS_ACTIVE_PROFILE CY_CFG_PWR_MODE_HP
#define CY_CFG_PWR_VDDA_MV 1800
#define CY_CFG_PWR_VDDD_MV 1800
#define CY_CFG_PWR_VDDIO0_MV 1800
#define CY_CFG_PWR_VDDIO1_MV 1800
#define CY_CFG_PWR_CBUCK_VOLT CY_SYSPM_CORE_BUCK_VOLTAGE_0_90V
#define CY_CFG_PWR_CBUCK_MODE CY_SYSPM_CORE_BUCK_MODE_HP
#define CY_CFG_PWR_SRAMLDO_VOLT CY_SYSPM_SRAMLDO_VOLTAGE_0_80V
#define CY_CFG_PWR_PD1_DOMAIN 1

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_0_obj;
extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_1_obj;
extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_2_obj;
extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_3_obj;
extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_4_obj;
extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_5_obj;
#endif /* defined (CY_USING_HAL) */

void init_cycfg_ns_system(void);
void init_cycfg_system(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_SYSTEM_H */
=======
/*******************************************************************************
 * File Name: cycfg_system.h
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

#if !defined(CYCFG_SYSTEM_H)
#define CYCFG_SYSTEM_H

#include "cycfg_notices.h"
#include "cy_syspm.h"
#include "system_edge.h"
#include "cy_sysclk.h"
#include "cy_syspm_pdcm.h"
#include "cycfg_mpc.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define srss_0_power_0_ENABLED 1U
#define CY_CFG_PWR_MODE_LP 0x01UL
#define CY_CFG_PWR_MODE_ULP 0x02UL
#define CY_CFG_PWR_MODE_HP 0x03UL
#define CY_CFG_PWR_MODE_ACTIVE 0x04UL
#define CY_CFG_PWR_MODE_SLEEP 0x08UL
#define CY_CFG_PWR_MODE_DEEPSLEEP 0x10UL
#define CY_CFG_PWR_MODE_DEEPSLEEP_RAM 0x11UL
#define CY_CFG_PWR_MODE_DEEPSLEEP_OFF 0x12UL
#define CY_CFG_PWR_SYS_IDLE_MODE CY_CFG_PWR_MODE_ACTIVE
#define CY_CFG_PWR_DEEPSLEEP_LATENCY 20UL
#define CY_CFG_PWR_SYS_ACTIVE_MODE CY_CFG_PWR_MODE_HP
#define CY_CFG_PWR_SYS_ACTIVE_PROFILE CY_CFG_PWR_MODE_HP
#define CY_CFG_PWR_VDDA_MV 1800
#define CY_CFG_PWR_VDDD_MV 1800
#define CY_CFG_PWR_VDDIO0_MV 1800
#define CY_CFG_PWR_VDDIO1_MV 1800
#define CY_CFG_PWR_CBUCK_VOLT CY_SYSPM_CORE_BUCK_VOLTAGE_0_90V
#define CY_CFG_PWR_CBUCK_MODE CY_SYSPM_CORE_BUCK_MODE_HP
#define CY_CFG_PWR_SRAMLDO_VOLT CY_SYSPM_SRAMLDO_VOLTAGE_0_80V
#define CY_CFG_PWR_PD1_DOMAIN 1
#define CY_CFG_PWR_PPU_MAIN PPU_V1_MODE_ON
#define CY_CFG_PWR_PPU_PD1 PPU_V1_MODE_ON
#define CY_CFG_PWR_PPU_SRAM0 PPU_V1_MODE_ON
#define CY_CFG_PWR_PPU_SRAM1 PPU_V1_MODE_ON
#define CY_CFG_PWR_PPU_SYSCPU PPU_V1_MODE_ON
#define CY_CFG_PWR_PPU_APPCPUSS PPU_V1_MODE_ON
#define CY_CFG_PWR_PPU_APPCPU PPU_V1_MODE_ON
#define CY_CFG_PWR_PPU_SOCMEM PPU_V1_MODE_ON
#define CY_CFG_PWR_PPU_U55 PPU_V1_MODE_ON
#define m33syscpuss_0_cm33_0_sau_0_ENABLED 1U
#define m55appcpuss_0_cm55_0_mpu_ns_0_ENABLED 1U
#define mxrramc_0_mpc_0_RESPONSE CY_MPC_BUS_ERR
#define mxrramc_0_mpc_0_REGION_COUNT 4U
#define mxsramc_0_mpc_0_RESPONSE CY_MPC_BUS_ERR
#define mxsramc_0_mpc_0_REGION_COUNT 3U
#define mxsramc_1_mpc_0_RESPONSE CY_MPC_BUS_ERR
#define mxsramc_1_mpc_0_REGION_COUNT 2U
#define smif_0_mpc_0_RESPONSE CY_MPC_RZWI
#define smif_0_mpc_0_REGION_COUNT 6U
#define smif_1_mpc_0_RESPONSE CY_MPC_RZWI
#define smif_1_mpc_0_REGION_COUNT 0U
#define socmem_0_mpc_0_RESPONSE CY_MPC_RZWI
#define socmem_0_mpc_0_REGION_COUNT 2U

#if defined(COMPONENT_SECURE_DEVICE) && defined(COMPONENT_MW_MTB_SRF)
extern const mtb_srf_protection_range_s_t mxrramc_0_mpc_0_srf_protection_range_s[mxrramc_0_mpc_0_REGION_COUNT];
extern const mtb_srf_protection_range_s_t mxsramc_0_mpc_0_srf_protection_range_s[mxsramc_0_mpc_0_REGION_COUNT];
extern const mtb_srf_protection_range_s_t mxsramc_1_mpc_0_srf_protection_range_s[mxsramc_1_mpc_0_REGION_COUNT];
extern const mtb_srf_protection_range_s_t smif_0_mpc_0_srf_protection_range_s[smif_0_mpc_0_REGION_COUNT];
extern const mtb_srf_protection_range_s_t socmem_0_mpc_0_srf_protection_range_s[socmem_0_mpc_0_REGION_COUNT];
#endif /* defined(COMPONENT_SECURE_DEVICE) && defined(COMPONENT_MW_MTB_SRF) */

void init_cycfg_ns_power(void);
void init_cycfg_power(void);
void init_cycfg_system(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_SYSTEM_H */
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
