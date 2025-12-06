/******************************************************************************
* File Name: speech_enhancement.c
*
* Description: This file contains functions for speech enhancement.
*
*
*
*******************************************************************************
* (c) 2023, Infineon Technologies Company. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Infineon Technologies Company (Infineon) or one of its
* subsidiaries and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Infineon hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Infineon's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Infineon.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Infineon
* reserves the right to make changes to the Software without notice. Infineon
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Infineon does
* not authorize its products for use in any products where a malfunction or
* failure of the Infineon product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Infineon's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Infineon against all liability.
*******************************************************************************/

/*******************************************************************************
* Include header file
******************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifdef ENABLE_AFE_MW_SUPPORT
#include "cy_afe_configurator_settings.h"
#endif
#if defined(USE_MTB_ML)
    #ifndef DISABLE_MW_ML_INIT
    #include "mtb_ml.h"
    #define NPU_PRIORITY 3
    #endif
#endif
#include "ifx_sp_enh.h"
#include "cy_sp_enh.h"

/******************************************************************************
* Defines
*****************************************************************************/

/******************************************************************************
* Constants
*****************************************************************************/

/******************************************************************************
* Variables
*****************************************************************************/
void* sp_enh_obj;

#ifndef ENABLE_AFE_MW_SUPPORT
extern uint8_t bf_coeffs[];
extern uint32_t bf_coeffs_total_len;
#endif

/**
 * Application can set this global variable, to redirect the memory allocation
 * and free at the application side. Application can choose any memory sector
 * depends on the memory configuration.
 */
cy_sp_alloc_memory_callback_t cy_sp_alloc_memory = NULL;
cy_sp_free_memory_callback_t cy_sp_free_memory = NULL;

#ifdef ENABLE_AFE_ALGO_SOCMEM_SUPPORT
#include "DSNS_LSTM_tflm_model_int16x8.h"
uint8_t ap_mem[163 * 1024] __attribute__((section(".cy_socmem_data")));
static uint8_t as_mem[24 * 1024] __attribute__((section(".cy_socmem_data")));
static uint8_t dsns_mem[DSNS_LSTM_ARENA_SIZE + 192] __attribute__((section(".cy_socmem_data")))  __attribute__((aligned(16)));
#else
void *dsns_persistent_mem_unalgned = NULL;
void *dses_persistent_mem_unalgned = NULL;
#endif /* ENABLE_AFE_ALGO_SOCMEM_SUPPORT */
/******************************************************************************
* Functions
*****************************************************************************/

#ifdef PROFILER
int cy_sp_profile_log(void* arg, char* buf, int32_t type)
{
    (void)arg, (void)buf, (void)type;
    CY_SP_PRINTF("AFE perf: %s\n", buf);
    return 0;
}
#endif

#ifdef ENABLE_AFE_MW_SUPPORT
cy_rslt_t cy_sp_enh_init(int32_t* afe_filter_settings, uint8_t* mw_settings,
    uint32_t mw_settings_length, cy_sp_enh_handle** handle)
#else
cy_rslt_t cy_sp_enh_init(cy_sp_enh_config_params* config_params, cy_sp_enh_handle** handle)
#endif /* ENABLE_AFE_MW_SUPPORT */
{
    /* Variable to track Error ID defined in ifx_sp_ehn.h */
    uint32_t ErrIdx = 0;
    int ret = !IFX_SP_ENH_SUCCESS;
#ifndef ENABLE_AFE_MW_SUPPORT
    int32_t sp_enh_config_prms[100]; // reserve large enough config array
    int32_t sp_enh_config_prms_len = 0, prm_idx = 0;
    (void)sp_enh_config_prms_len;
#endif
    /* Variables for model container and infomation */
    cy_sp_enh_handle* internal_handle = NULL;
    ifx_stc_sp_enh_info_t* sp_enh_info = NULL;
    uint8_t* fn_coeffs = NULL;
    int32_t num_coeff_bytes = 0;

    /* allocate memory for speech enhancement handle */
    if (cy_sp_alloc_memory)
    {
        cy_sp_alloc_memory(IFX_SP_MEM_ID_HANDLE, sizeof(cy_sp_enh_handle), (void**)&internal_handle);
    }
    else
    {
        internal_handle = (cy_sp_enh_handle*)calloc(1, sizeof(cy_sp_enh_handle));
    }
    if (NULL == internal_handle)
    {
        CY_SP_PRINTF("Failed to allocate memory for inference handle \r\n");
        return CY_RSLT_OUT_OF_MEMORY;
    }

#ifndef ENABLE_AFE_MW_SUPPORT
    /* set values which are provided by application */
    sp_enh_config_prms[0] = 0;                          // set configuration version to zero
    sp_enh_config_prms[1] = config_params->sampling_rate;
    sp_enh_config_prms[2] = config_params->input_frame_size;
    sp_enh_config_prms[3] = config_params->num_mics;
    sp_enh_config_prms[4] = 0;                          // monitor output
    sp_enh_config_prms[5] = 0x04c11db7;                 // 32-bit crc polynomial
    sp_enh_config_prms[6] =                             // num components
        (int)(config_params->hpf_enable > 0) +          // hpf component
        (int)(config_params->aec_enable > 0) +          // aec component
        (int)(config_params->bf_enable > 0) +           // bf component
        (int)(config_params->drvb_enable > 0) +         // drvb component
        (int)(config_params->ns_enable > 0) +           // ns component
        (int)(config_params->dsns_enable > 0) +         // dsns component
        (int)(config_params->es_enable > 0) +           // es component
        (int)(config_params->anasyn_enable > 0) * 2 +   // analysis + synthesis components
        (int)(config_params->dses_enable > 0);          // dses component
    sp_enh_config_prms_len = 7;
    prm_idx = 7;
    if (config_params->hpf_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_HPF;
        sp_enh_config_prms_len++;
    }
    if (config_params->aec_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_AEC;
        sp_enh_config_prms_len++;
    }
    if (config_params->bf_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_BF;
        sp_enh_config_prms_len++;
        fn_coeffs = bf_coeffs;
        num_coeff_bytes = bf_coeffs_total_len;
    }
    if (config_params->drvb_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_DRVB;
        sp_enh_config_prms_len++;
    }
    if (config_params->ns_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_NS;
        sp_enh_config_prms_len++;
    }
    if (config_params->dsns_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_DSNS;
        sp_enh_config_prms_len++;
    }
    if (config_params->es_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_ES;
        sp_enh_config_prms_len++;
    }
    if (config_params->dses_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_DSES;
        sp_enh_config_prms_len++;
    }
    if (config_params->anasyn_enable)
    {
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_ANALYSIS;
        sp_enh_config_prms[prm_idx++] = IFX_SP_ENH_IP_COMPONENT_SYNTHESIS;
        sp_enh_config_prms_len += 2;
    }
    if (config_params->hpf_enable)
    {
        sp_enh_config_prms[prm_idx++] = 1 + config_params->num_mics; // number of parameters in hpf
        sp_enh_config_prms[prm_idx++] = 200;    // cut-off frequency
        sp_enh_config_prms[prm_idx++] = 0;      // input gain in dB for first channel
        sp_enh_config_prms_len += 3;
        if (config_params->num_mics == 2)
        {
            sp_enh_config_prms[prm_idx++] = 0;  // input gain in dB for second channel
            sp_enh_config_prms_len++;
        }

    }
    if (config_params->aec_enable)
    {
        sp_enh_config_prms[prm_idx++] = 2;      // number of parameters in aec
        sp_enh_config_prms[prm_idx++] = config_params->tail_len_msec;
        if (config_params->control_params)
        {
            sp_enh_config_prms[prm_idx++] = config_params->control_params->bulk_delay_msec;
        }
        else
        {
            sp_enh_config_prms[prm_idx++] = 5;  // bulk delay in msec
        }
        sp_enh_config_prms_len += 3;
    }
    if (config_params->bf_enable && config_params->anasyn_enable)
    {
        sp_enh_config_prms[prm_idx++] = 5;      // number of parameters in bf
        sp_enh_config_prms[prm_idx++] = 40;     // mic distance in mm
        sp_enh_config_prms[prm_idx++] = 0;      // angle range start
        sp_enh_config_prms[prm_idx++] = 180;    // angle range stop
        sp_enh_config_prms[prm_idx++] = 9;      // num beams
        if (config_params->control_params)
        {
            sp_enh_config_prms[prm_idx++] = config_params->control_params->bf_aggressiveness;
        }
        else
        {
            sp_enh_config_prms[prm_idx++] = 2;  // bf aggressiveness (0,1,2)
        }
        sp_enh_config_prms_len += 6;
    }
    if (config_params->drvb_enable && config_params->anasyn_enable)
    {
        sp_enh_config_prms[prm_idx++] = 0;      // number of parameters in drvb
        sp_enh_config_prms_len += 1;
    }
    if (config_params->ns_enable && config_params->anasyn_enable)
    {
        sp_enh_config_prms[prm_idx++] = 1;      // number of parameters in ns
        if (config_params->control_params)
        {
            sp_enh_config_prms[prm_idx++] = config_params->control_params->ns_gain_dB;
        }
        else
        {
            sp_enh_config_prms[prm_idx++] = 10; // noise suppresion gain in dB
        }
        sp_enh_config_prms_len += 2;
    }
    if (config_params->dsns_enable && config_params->anasyn_enable)
    {
        sp_enh_config_prms[prm_idx++] = 1;      // number of parameters in dsns
        if (config_params->control_params)
        {
            sp_enh_config_prms[prm_idx++] = config_params->control_params->ns_gain_dB;
        }
        else
        {
            sp_enh_config_prms[prm_idx++] = 10; // deep subband noise suppresion gain in dB
        }
        sp_enh_config_prms_len += 2;
    }
    if (config_params->es_enable && config_params->aec_enable)
    {
        sp_enh_config_prms[prm_idx++] = 1;      // number of parameters in es
        if (config_params->control_params)
        {
            sp_enh_config_prms[prm_idx++] = config_params->control_params->es_aggressiveness;
        }
        else
        {
            sp_enh_config_prms[prm_idx++] = 2;  // echo suppression aggressiveness (0,1,2)
        }
        sp_enh_config_prms_len += 2;
    }
    if (config_params->dses_enable && config_params->aec_enable)
    {
        sp_enh_config_prms[prm_idx++] = 1;      // number of parameters in dses
        if (config_params->control_params)
        {
            sp_enh_config_prms[prm_idx++] = config_params->control_params->es_aggressiveness;
        }
        else
        {
            sp_enh_config_prms[prm_idx++] = 2;  // echo suppression aggressiveness (0,1,2)
        }
        sp_enh_config_prms_len += 2;
    }
    if (config_params->anasyn_enable)
    {
        sp_enh_config_prms[prm_idx++] = 0;      // number of parameters in analysis
        sp_enh_config_prms[prm_idx++] = 0;      // number of parameters in synthesis
        sp_enh_config_prms_len += 2;
    }
    sp_enh_config_prms[prm_idx++] = 0xaabbccdd; // CRC check sum, not active yet
    sp_enh_config_prms_len++;

    /* Step 1: Parse and get required memory for speech enhancement */
    sp_enh_info = &internal_handle->sp_enh_info;
    ret = ifx_sp_enh_model_parse(sp_enh_config_prms, sp_enh_info);

    sp_enh_info->ns_es_type = config_params->control_params->ns_es_type;
#else
    /* Step 1: Parse and get required memory for speech enhancement */
    sp_enh_info = &internal_handle->sp_enh_info;
    ret = ifx_sp_enh_model_parse(afe_filter_settings, sp_enh_info);

    fn_coeffs = mw_settings;
    num_coeff_bytes = mw_settings_length;
#endif

    /* Step 2: Allocate memory */
    if (ret == 0) /*Model Parsing successful*/
    {
        CY_SP_PRINTF("Memory to be allocated for scratch memory                 : %8.2f kB\r\n", sp_enh_info->memory.scratch_mem / 1024.0);
        CY_SP_PRINTF("Memory to be allocated for persistent memory              : %8.2f kB\r\n", sp_enh_info->memory.persistent_mem / 1024.0);

        // persistent memory for components other than BF
#if defined (ENABLE_AFE_ALGO_SOCMEM_SUPPORT)
        sp_enh_info->memory.persistent_mem_pt = (char*)ap_mem;
        if (sp_enh_info->memory.persistent_mem > sizeof(ap_mem))
        {
            CY_SP_PRINTF("Error! Allocate persistent memory failed, exiting Speech Enhancement!\r\n");
            return -1;
        }
#else
        if (cy_sp_alloc_memory)
        {
            cy_sp_alloc_memory(IFX_SP_MEM_ID_PERSISTENT_MEM, sp_enh_info->memory.persistent_mem, (void**)&sp_enh_info->memory.persistent_mem_pt);
        }
        else
        {
            sp_enh_info->memory.persistent_mem_pt = (char*)malloc(sp_enh_info->memory.persistent_mem);
        }
#endif /* ENABLE_AFE_ALGO_SOCMEM_SUPPORT */
        if (sp_enh_info->memory.persistent_mem_pt == NULL)
        {
            CY_SP_PRINTF("Error! Allocate persistent memory failed, exiting Speech Enhancement!\r\n");
            return -1;
        }

        // persistent memory for BF
        // TODO: do this for all other components so memory allocation can be controlled individually
        if (sp_enh_info->bf_pm.persistent_mem != 0)
        {
#if defined (ENABLE_AFE_ALGO_SOCMEM_SUPPORT)
            sp_enh_info->bf_pm.persistent_mem_pt = sp_enh_info->memory.persistent_mem_pt + sp_enh_info->memory.persistent_mem;
            sp_enh_info->memory.persistent_mem += sp_enh_info->bf_pm.persistent_mem;
            if (sp_enh_info->memory.persistent_mem > sizeof(ap_mem))
            {
                CY_SP_PRINTF("Error! Allocate BF persistent memory failed, exiting Speech Enhancement!\r\n");
                return -1;
            }
#else
            if (cy_sp_alloc_memory)
            {
                cy_sp_alloc_memory(IFX_SP_MEM_ID_BF_PERSISTENT_MEM, sp_enh_info->bf_pm.persistent_mem, (void**)&sp_enh_info->bf_pm.persistent_mem_pt);
            }
            else
            {
                sp_enh_info->bf_pm.persistent_mem_pt = (char*)malloc(sp_enh_info->bf_pm.persistent_mem);
            }
#endif
            if (sp_enh_info->bf_pm.persistent_mem_pt == NULL)
            {
                CY_SP_PRINTF("Error! Allocate BF persistent memory failed, exiting Speech Enancement!\r\n");
                return -1;
            }
        }
        // persistent memory for GDE
        if (sp_enh_info->gde_pm.persistent_mem != 0)
        {
#if defined (ENABLE_AFE_ALGO_SOCMEM_SUPPORT)
            sp_enh_info->gde_pm.persistent_mem_pt = sp_enh_info->memory.persistent_mem_pt + sp_enh_info->memory.persistent_mem;
            sp_enh_info->memory.persistent_mem += sp_enh_info->gde_pm.persistent_mem;
            if (sp_enh_info->memory.persistent_mem > sizeof(ap_mem))
            {
                CY_SP_PRINTF("Error! Allocate GDE persistent memory failed, exiting Speech Enhancement!\r\n");
                return -1;
            }
#else
            if (cy_sp_alloc_memory)
            {
                cy_sp_alloc_memory(IFX_SP_MEM_ID_GDE_PERSISTENT_MEM, sp_enh_info->gde_pm.persistent_mem, (void**)&sp_enh_info->gde_pm.persistent_mem_pt);
            }
            else
            {
                sp_enh_info->gde_pm.persistent_mem_pt = (char*)malloc(sp_enh_info->gde_pm.persistent_mem);
            }
#endif
            if (sp_enh_info->gde_pm.persistent_mem_pt == NULL)
            {
                CY_SP_PRINTF("Error! Allocate GDE persistent memory failed, exiting Speech Enancement!\r\n");
                return -1;
            }
        }
        if (sp_enh_info->dsns_socmem.persistent_mem != 0)
        {
#if defined (ENABLE_AFE_ALGO_SOCMEM_SUPPORT)
            sp_enh_info->dsns_socmem.persistent_mem_pt = (char*)dsns_mem;
            if (sp_enh_info->dsns_socmem.persistent_mem > sizeof(dsns_mem))
            {
                CY_SP_PRINTF("Error! Allocate DSNS SOCMEM persistent memory failed, exiting Speech Enhancement!\r\n");
                return -1;
            }
#else
            if (cy_sp_alloc_memory)
            {
                cy_sp_alloc_memory(IFX_SP_MEM_ID_DSNS_SOCMEM_PERSISTENT_MEM, sp_enh_info->dsns_socmem.persistent_mem, (void**)&sp_enh_info->dsns_socmem.persistent_mem_pt);
            }
            else
            {/* Need to make sure the allocated memory is in SOCMEM and aligned to 16 */
                dsns_persistent_mem_unalgned = (char*)malloc(sp_enh_info->dsns_socmem.persistent_mem + 15);
                /* Force align to 16 for U55 */
                sp_enh_info->dsns_socmem.persistent_mem_pt = (char*)(((uintptr_t)dsns_persistent_mem_unalgned + 15) & ~((uintptr_t)0xF));
            }
#endif
            if (sp_enh_info->dsns_socmem.persistent_mem_pt == NULL)
            {
                CY_SP_PRINTF("Error! Allocate DSNS SOCMEM persistent memory failed, exiting Speech Enancement!\r\n");
                return -1;
            }
        }


        if (sp_enh_info->dses_socmem.persistent_mem != 0)
        {
#if defined (ENABLE_AFE_ALGO_SOCMEM_SUPPORT)
            sp_enh_info->dses_socmem.persistent_mem_pt = (char*)dsns_mem;
            if (sp_enh_info->dses_socmem.persistent_mem > sizeof(dsns_mem))
            {
                CY_SP_PRINTF("Error! Allocate DENS SOCMEM persistent memory failed, exiting Speech Enhancement!\r\n");
                return -1;
            }
#else
            if (cy_sp_alloc_memory)
            {
                cy_sp_alloc_memory(IFX_SP_MEM_ID_DSES_SOCMEM_PERSISTENT_MEM, sp_enh_info->dses_socmem.persistent_mem, (void**)&sp_enh_info->dses_socmem.persistent_mem_pt);
            }
            else
            {/* Need to make sure the allocated memory is in SOCMEM and aligned to 16 */
                dses_persistent_mem_unalgned = (char*)malloc(sp_enh_info->dses_socmem.persistent_mem + 15);
                /* Force align to 16 for U55 */
                sp_enh_info->dses_socmem.persistent_mem_pt = (char*)(((uintptr_t)dses_persistent_mem_unalgned + 15) & ~((uintptr_t)0xF));
            }
#endif
            if (sp_enh_info->dses_socmem.persistent_mem_pt == NULL)
            {
                CY_SP_PRINTF("Error! Allocate DSES SOCMEM persistent memory failed, exiting Speech Enancement!\r\n");
                return -1;
            }
        }

        // scratch memory
#if defined (ENABLE_AFE_ALGO_SOCMEM_SUPPORT)
        sp_enh_info->memory.scratch_mem_pt = (char*)as_mem;
        if (sp_enh_info->memory.scratch_mem > sizeof(as_mem))
        {
            CY_SP_PRINTF("Error! Allocate persistent memory failed, exiting Speech Enhancement!\r\n");
            return -1;
        }
#else
        if (cy_sp_alloc_memory)
        {
            cy_sp_alloc_memory(IFX_SP_MEM_ID_SCRATCH_MEM, sp_enh_info->memory.scratch_mem, (void**)&sp_enh_info->memory.scratch_mem_pt);
        }
        else
        {
            sp_enh_info->memory.scratch_mem_pt = (char*)malloc(sp_enh_info->memory.scratch_mem);
        }
#endif /* ENABLE_AFE_ALGO_SOCMEM_SUPPORT*/
        if (sp_enh_info->memory.scratch_mem_pt == NULL)
        {
            CY_SP_PRINTF("Error! Allocate scratch memory failed, exiting Speech Enhancement!\r\n");
            return -1;
        }
    }
    else
    {
        CY_SP_PRINTF("Error! Model Parsing failed! Error code=%x, Component index=%d, Line number=%d, exiting Speech Enhancement!\r\n",
            IFX_SP_ENH_ERR_CODE(ret), IFX_SP_ENH_ERR_COMPONENT_INDEX(ret), IFX_SP_ENH_ERR_LINE_NUMBER(ret));
        return -1;
    }

#if defined(USE_MTB_ML)
    #ifndef DISABLE_MW_ML_INIT
        /* Set the priority of NPU interrupt handler */
        mtb_ml_init(NPU_PRIORITY);
    #endif
#endif
    /* Step 3: Initialize and get speech enhancement Container/object */
    ErrIdx |= ifx_sp_enh_init(&sp_enh_obj, sp_enh_info, fn_coeffs, num_coeff_bytes);
    if (ErrIdx)
    {
        CY_SP_PRINTF("Error! Speech enhancement initialization failed! Error code=%x, Component index=%d, Line number=%d, exiting Speech Enhancement!\r\n",
            IFX_SP_ENH_ERR_CODE(ErrIdx), IFX_SP_ENH_ERR_COMPONENT_INDEX(ErrIdx), IFX_SP_ENH_ERR_LINE_NUMBER(ErrIdx));
        return -1;
    }
    else
    {
        CY_SP_PRINTF("\r\n*****************************************************************\r\n");
        CY_SP_PRINTF("            Speech Enhancement Initialization successful!!");
        CY_SP_PRINTF("\r\n*****************************************************************\r\n");
    }

#ifdef PROFILER
    ifx_sp_enh_profile_init(sp_enh_obj, IFX_SP_ENH_PROFILE_ENABLE_COMPONENT, cy_sp_profile_log, NULL);
#endif
    /* Step 4: Setup profile configuration */
    // TODO

    *handle = internal_handle;

    return 0;
}

cy_rslt_t cy_sp_enh_process(cy_sp_enh_handle* handle, int16_t* input1, int16_t* input2, int16_t* reference, int16_t* output, int16_t* ifx_output,
    int16_t* meter_output)
{
    uint32_t ErrIdx = 0;

    if (NULL == handle)
    {
        CY_SP_PRINTF("Handle passed cannot be NULL \r\n");
        return CY_RSLT_BAD_ARG;
    }

    ErrIdx |= ifx_sp_enh_process((void*)sp_enh_obj, (void*)input1, (void*)input2, (void*)reference, (void*)output, (void*)ifx_output, (void*)meter_output);

    if (ErrIdx)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

cy_rslt_t cy_sp_enh_deinit(cy_sp_enh_handle* handle)
{

#ifdef PROFILER
    ifx_sp_enh_profile_print(sp_enh_obj);
#endif

    /* free allocated internal memories */
    ifx_sp_enh_deinit_internal_mem(sp_enh_obj);
#if defined (ENABLE_AFE_ALGO_SOCMEM_SUPPORT)
    /* Noting to be done here */
    if (NULL == handle)
    {
        CY_SP_PRINTF("Handle pass cannot be NULL \r\n");
        return CY_RSLT_BAD_ARG;
    }
#else
    ifx_stc_sp_enh_info_t* sp_enh_info = NULL;

    /* Noting to be done here */
    if (NULL == handle)
    {
        CY_SP_PRINTF("Handle pass cannot be NULL \r\n");
        return CY_RSLT_BAD_ARG;
    }

    sp_enh_info = &handle->sp_enh_info;

    if (cy_sp_free_memory)
    {
        cy_sp_free_memory(IFX_SP_MEM_ID_PERSISTENT_MEM, sp_enh_info->memory.persistent_mem_pt);
    }
    else
    {
        free(sp_enh_info->memory.persistent_mem_pt);
    }

    if (cy_sp_free_memory)
    {
        cy_sp_free_memory(IFX_SP_MEM_ID_SCRATCH_MEM, sp_enh_info->memory.scratch_mem_pt);
    }
    else
    {
        free(sp_enh_info->memory.scratch_mem_pt);
    }

    if (cy_sp_free_memory)
    {
        cy_sp_free_memory(IFX_SP_MEM_ID_BF_PERSISTENT_MEM, sp_enh_info->bf_pm.persistent_mem_pt);
    }
    else
    {
        free(sp_enh_info->bf_pm.persistent_mem_pt);
    }

    if (cy_sp_free_memory)
    {
        cy_sp_free_memory(IFX_SP_MEM_ID_GDE_PERSISTENT_MEM, sp_enh_info->gde_pm.persistent_mem_pt);
    }
    else
    {
        free(sp_enh_info->gde_pm.persistent_mem_pt);
    }

    if (cy_sp_free_memory)
    {
        cy_sp_free_memory(IFX_SP_MEM_ID_DSNS_SOCMEM_PERSISTENT_MEM, sp_enh_info->dsns_socmem.persistent_mem_pt);
    }
    else
    {
        free(dsns_persistent_mem_unalgned);
        dsns_persistent_mem_unalgned = NULL;
    }

    if (cy_sp_free_memory)
    {
        cy_sp_free_memory(IFX_SP_MEM_ID_DSES_SOCMEM_PERSISTENT_MEM, sp_enh_info->dses_socmem.persistent_mem_pt);
    }
    else
    {
        free(dses_persistent_mem_unalgned);
        dses_persistent_mem_unalgned = NULL;
    }

    if (cy_sp_free_memory)
    {
        cy_sp_free_memory(IFX_SP_MEM_ID_HANDLE, handle);
    }
    else
    {
        free(handle);
    }
#endif

#if defined(USE_MTB_ML)
#ifndef DISABLE_MW_ML_INIT
        mtb_ml_deinit();
#endif
#endif

    sp_enh_obj = NULL;
    cy_sp_free_memory = NULL;
    cy_sp_alloc_memory = NULL;

    CY_SP_PRINTF("\r\n***************************************************\r\n");
    CY_SP_PRINTF("          Speech Enhancement Deinitialized!!");
    CY_SP_PRINTF("\r\n***************************************************\r\n");

    return 0;
}

cy_rslt_t cy_sp_enh_enable_disable_component(cy_sp_enh_handle* handle,
    ifx_sp_enh_ip_component_config_t component_name, bool enable)
{
    cy_rslt_t result = CY_RSLT_ERROR_GENERIC;
    uint32_t ErrIdx = 0;
    ifx_active_control_config_t active = ACTIVE_ON;
    ifx_stc_sp_enh_info_t* sp_enh_info = NULL;

    /* Noting to be done here */
    if (NULL == handle)
    {
        CY_SP_PRINTF("Handle pass cannot be NULL \r\n");
        return CY_RSLT_BAD_ARG;
    }

    if (NULL == sp_enh_obj)
    {
        CY_SP_PRINTF("HP nor initalized\r\n");
        return CY_RSLT_BAD_ARG;
    }

    if (true == enable)
    {
        active = ACTIVE_ON;
    }
    else
    {
        active = ACTIVE_OFF;
    }

    sp_enh_info = &handle->sp_enh_info;

    ErrIdx = ifx_sp_enh_mode_control(sp_enh_obj, sp_enh_info, component_name,
        active, false, false, NULL);
    if (ErrIdx)
    {
#ifndef DISABLE_PRINT
        CY_SP_PRINTF("Error! ifx_sp_enh_mode_control! Error code=%x, Component index=%d, Line number=%d, exiting Speech Enhancement!\r\n",
            IFX_SP_ENH_ERR_CODE(ErrIdx), IFX_SP_ENH_ERR_COMPONENT_INDEX(ErrIdx), IFX_SP_ENH_ERR_LINE_NUMBER(ErrIdx));
#endif
        if (IFX_SP_ENH_ERR_CODE(ErrIdx) == IFX_SP_ENH_ERR_PARAM_RANGE)
        {
            result = CY_RSLT_INVALID_PARAMS;
        }
        else
        {
            result = CY_RSLT_ERROR_GENERIC;
        }
    }
    else
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}

cy_rslt_t cy_sp_enh_update_config_value(cy_sp_enh_handle* handle,
    ifx_sp_enh_ip_component_config_t component_name, int32_t* value)
{
    cy_rslt_t result = CY_RSLT_ERROR_GENERIC;
    uint32_t ErrIdx = 0;
    ifx_active_control_config_t active = ACTIVE_ON;
    unsigned int active_int = ACTIVE_ON;
    ifx_stc_sp_enh_info_t* sp_enh_info = NULL;

    /* Noting to be done here */
    if (NULL == handle)
    {
        CY_SP_PRINTF("Handle pass cannot be NULL \r\n");
        return CY_RSLT_BAD_ARG;
    }

    if (NULL == sp_enh_obj)
    {
        CY_SP_PRINTF("HP nor initalized\r\n");
        return CY_RSLT_BAD_ARG;
    }

    sp_enh_info = &handle->sp_enh_info;

    active_int = ACTIVE_ON+1; //No Change
    active = (ifx_active_control_config_t)active_int; //Waring fix
    ErrIdx = ifx_sp_enh_mode_control( sp_enh_obj, sp_enh_info,component_name,
                                        active, false, true, value);
    if (ErrIdx)
    {
#ifndef DISABLE_PRINT
        CY_SP_PRINTF("Error! ifx_sp_enh_mode_control! Error code=%x, Component index=%d, Line number=%d, exiting Speech Enhancement!\r\n",
            IFX_SP_ENH_ERR_CODE(ErrIdx), IFX_SP_ENH_ERR_COMPONENT_INDEX(ErrIdx), IFX_SP_ENH_ERR_LINE_NUMBER(ErrIdx));
#endif
        ErrIdx = IFX_SP_ENH_ERR_CODE(ErrIdx);
        if (ErrIdx == IFX_SP_ENH_ERR_PARAM_RANGE)
        {
            result = CY_RSLT_INVALID_PARAMS;
        }
        else
        {
            result = CY_RSLT_ERROR_GENERIC;
        }
    }
    else
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}

cy_rslt_t cy_sp_enh_get_config_value(cy_sp_enh_handle* handle,
    ifx_sp_enh_ip_component_config_t component_name, void* value_struct_pt)
{
    cy_rslt_t result = CY_RSLT_ERROR_GENERIC;
    uint32_t ErrIdx = 0;

    /* Noting to be done here */
    if (NULL == handle || NULL == value_struct_pt)
    {
        CY_SP_PRINTF("Handle or value pt pass cannot be NULL %p-%p \r\n", handle, value_struct_pt);
        return CY_RSLT_BAD_ARG;
    }

    if (NULL == sp_enh_obj)
    {
        CY_SP_PRINTF("HP nor initalized\r\n");
        return CY_RSLT_BAD_ARG;
    }

    ErrIdx = ifx_sp_enh_get_mode_control_value(sp_enh_obj, component_name, value_struct_pt);
    if (ErrIdx)
    {
#ifndef DISABLE_PRINT
        CY_SP_PRINTF("Error! ifx_sp_enh_get_mode_control_value! Error code=%x, Component index=%d, Line number=%d, exiting Speech Enhancement!\r\n",
            IFX_SP_ENH_ERR_CODE(ErrIdx), IFX_SP_ENH_ERR_COMPONENT_INDEX(ErrIdx), IFX_SP_ENH_ERR_LINE_NUMBER(ErrIdx));
#endif
        ErrIdx = IFX_SP_ENH_ERR_CODE(ErrIdx);
        if (ErrIdx == IFX_SP_ENH_ERR_PARAM_RANGE)
        {
            result = CY_RSLT_INVALID_PARAMS;
        }
        else
        {
            result = CY_RSLT_ERROR_GENERIC;
        }
    }
    else
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}

cy_rslt_t cy_sp_enh_get_component_status(cy_sp_enh_handle* handle,
    ifx_sp_enh_ip_component_config_t component_name, bool* enable)
{
    if (NULL == handle)
    {
        CY_SP_PRINTF("Handle pass cannot be NULL \r\n");
        return CY_RSLT_BAD_ARG;
    }

    if (NULL == sp_enh_obj)
    {
        CY_SP_PRINTF("HP nor initalized\r\n");
        return CY_RSLT_BAD_ARG;
    }

    *enable = ifx_sp_enh_active_status(sp_enh_obj, component_name);

    return CY_RSLT_SUCCESS;
}


cy_rslt_t cy_sp_enh_configure_dbg_out(cy_sp_enh_handle* handle,
        ifx_sp_enh_ip_component_config_t component_name, bool enable)
{
    if (NULL == handle)
    {
        CY_SP_PRINTF("Handle pass cannot be NULL \r\n");
        return CY_RSLT_BAD_ARG;
    }

    if (NULL == sp_enh_obj)
    {
        CY_SP_PRINTF("HP not initalized\r\n");
        return CY_RSLT_BAD_ARG;
    }

    return (ifx_sp_enh_configure_monitor_out(sp_enh_obj, component_name,enable));
}
