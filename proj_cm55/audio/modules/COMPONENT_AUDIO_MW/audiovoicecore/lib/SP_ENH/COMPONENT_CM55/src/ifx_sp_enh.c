/******************************************************************************
* File Name: ifx_sp_enh.c
*
* Description: This file contains HP speech enhancement app
*
* Related Document: See README.md
*
*******************************************************************************
* (c) 2021, Infineon Technologies Company. All rights reserved.
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

#ifdef ENABLE_AFE_MW_SUPPORT
#include "cy_afe_configurator_settings.h"
#endif
#include "ifx_sp_enh.h"
#include "ifx_sp_enh_priv.h"

/******************************************************************************
* Global, Constants and Marco
*****************************************************************************/

char* echoGlobalPersistentPtr = NULL;       // pointer to echo control memory
long echoGlobalPesistentCumulateCount = 0;  // echo control memory count
long echoGlobalPersistentSize = 0;          // echo control memory size

extern int esNumBands;

bool checkCRC32Value(char* ucBuffer, uint32_t ulSize, uint32_t ulPolynomial);


int32_t ifx_sp_enh_model_parse(int32_t* fn_prms, ifx_stc_sp_enh_info_t* mdl_infoPt)
{
    int32_t* int_idx = (int32_t*)fn_prms;
    int32_t sampling_rate, frame_size, num_mics, num_of_components, sz, num_of_syn = 0, num_of_ana = 0;
    uint32_t enable_hpf=false, enable_aec = false, enable_bf = false, enable_es = false, enable_ns = false,
        enable_dsns = false, enable_dses = false, enable_drvb = false, enable_ana = false, enable_syn = false;
    int32_t num_monitor_outputs, monitor_flag=0, num_parm, *component_id_ptr, *parm_ptr;
    uint32_t crc_polynomial, enable_flag, subband_buf_sz = 0, scratch_sz = 0, max_scratch_mem_size = 0;
    /* Waring fix */
    (void)scratch_sz; (void)parm_ptr; (void)enable_es; (void)enable_aec; (void)enable_hpf;

    /*** Sanity check of input arguments ***/
    if (fn_prms == NULL || mdl_infoPt == NULL)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_ARGUMENT);
    }

    /* read parameters*/
    memset(mdl_infoPt, 0, sizeof(ifx_stc_sp_enh_info_t)); /* clear mdl_infoPt memory at start */
    sz = *int_idx++;             /* Configurator version, no checking its validation for now */
    mdl_infoPt->configurator_version = sz;
    mdl_infoPt->libsp_version = IFX_SP_ENH_VERSION;
    sampling_rate = *int_idx++;
    frame_size = *int_idx++;
    if (sampling_rate != HP_SP_ENH_SAMPLING_RATE || frame_size != HP_SP_ENH_FRAME_SAMPLES)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_PARAM);
    }
    num_mics = *int_idx++;
    if (num_mics < MIN_NUM_MICS || num_mics > MAX_NUM_MICS)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_PARAM);
    }
    mdl_infoPt->common.sampling_rate = sampling_rate;
    mdl_infoPt->common.frame_size = frame_size;
    mdl_infoPt->common.num_mics = num_mics;

    num_monitor_outputs = *int_idx++;
    if (num_monitor_outputs > MAX_NUM_MONITOR_COMPONENT || num_monitor_outputs < 0)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_PARAM);
    }
    mdl_infoPt->common.num_monitor_components = num_monitor_outputs;
    mdl_infoPt->num_of_monitor_outputs = num_monitor_outputs; /* this is default */
    for (int i = 0; i < num_monitor_outputs; i++)
    {
        mdl_infoPt->common.monitor_component_id[i] = *int_idx++;
        monitor_flag = COMPONENT_SET_FLAG(monitor_flag, mdl_infoPt->common.monitor_component_id[i]);
    }
    mdl_infoPt->monitor_flag = monitor_flag;
#ifndef CY_AFE_ENABLE_TUNING_FEATURE
    /* Overriding the monitors to zero, for optimized performance */
    mdl_infoPt->monitor_flag = 0;
    mdl_infoPt->num_of_monitor_outputs = 0;
    mdl_infoPt->common.num_monitor_components = 0;
#endif /* CY_AFE_ENABLE_TUNING_FEATURE */

    crc_polynomial = *int_idx++;
    num_of_components = *int_idx++;
    mdl_infoPt->num_config_components = num_of_components;
    component_id_ptr = int_idx;
    int_idx += num_of_components;
    for (int i = 0; i < num_of_components; i++)
    {
        num_parm = *int_idx++;
        if (num_parm > 0)
        {
            parm_ptr = int_idx;
        }
        else
        {
            parm_ptr = NULL;
        }
        int_idx += num_parm;
        switch (component_id_ptr[i])
        {
        case IFX_SP_ENH_IP_COMPONENT_ANALYSIS:
#ifdef ENABLE_IFX_ANALYSIS
            enable_ana = true;
            if (num_parm != 0)
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
#else
            enable_ana = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_SYNTHESIS:
#ifdef ENABLE_IFX_SYNTHESIS
            enable_syn = true;
            if (num_parm != 0)
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
#else
            enable_syn = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_SYNTHESIS, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_HPF:
#ifdef ENABLE_IFX_HPF
            enable_hpf = true;
            if (num_parm != (int32_t)(sizeof(hpf_settings_struct_t) >> 2) + num_mics - MAX_NUM_MICS)
            {/* each mic has its own gain */
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
            if (parm_ptr[0] > MAX_CUTOFF_FREQ || parm_ptr[0] < MIN_CUTOFF_FREQ)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_HPF, IFX_SP_ENH_ERR_PARAM_RANGE);
            }
            mdl_infoPt->hpf_setting.cutoff_freq_hz = parm_ptr[0];
            for (int j = 0; j < num_mics; j++)
            {
                if (parm_ptr[j + 1] > MAX_INPUT_GAIN_DB || parm_ptr[j + 1] < MIN_INPUT_GAIN_DB)
                {
                    return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_HPF, IFX_SP_ENH_ERR_PARAM_RANGE);
                }
                mdl_infoPt->hpf_setting.gain_q8_dB[j] = parm_ptr[j + 1];
            }
#else
            enable_hpf = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_HPF, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_AEC:
#ifdef ENABLE_IFX_AEC
            enable_aec = true;
            if (num_parm != (sizeof(aec_settings_struct_t) >> 2))
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
            if (parm_ptr[0] > MAX_TAIL_LEN_MSEC || parm_ptr[0] < MIN_TAIL_LEN_MSEC ||
                parm_ptr[1] > MAX_BULK_DELAY_MSEC || parm_ptr[1] < MIN_BULK_DELAY_MSEC)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_AEC, IFX_SP_ENH_ERR_PARAM_RANGE);
            }
            mdl_infoPt->aec_setting.tail_len_msec = parm_ptr[0];
            mdl_infoPt->aec_setting.bulk_delay_msec = parm_ptr[1];
#else
            enable_aec = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_AEC, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_DRVB:
#ifdef ENABLE_IFX_DRVB
            enable_drvb = true;
            if (num_parm == 1)
            {
                if (*parm_ptr > IFX_SP_ENH_AGGRESSIVE_HIGH || *parm_ptr < IFX_SP_ENH_AGGRESSIVE_LOW)
                {
                    return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DRVB, IFX_SP_ENH_ERR_PARAM_RANGE);
                }
                mdl_infoPt->drvb_setting.aggressiveness = *parm_ptr;
            }
            else if (num_parm == 0)
            {
                mdl_infoPt->drvb_setting.aggressiveness = IFX_SP_ENH_AGGRESSIVE_HIGH;
            }
            else
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
#else
            enable_drvb = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DRVB, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_BF:
#ifdef ENABLE_IFX_BF
            enable_bf = true;
            if (num_parm != (sizeof(bf_settings_struct_t) >> 2))
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
            if (parm_ptr[0] > MAX_MIC_DISTANCE_MM || parm_ptr[0] < MIN_MIC_DISTANCE_MM ||
                parm_ptr[1] > parm_ptr[2] || parm_ptr[1] < MIN_ANGLE_RANGE || parm_ptr[2] > MAX_ANGLE_RANGE ||
                parm_ptr[3] > MAX_NUM_BEAMS || parm_ptr[3] < MIN_NUM_BEAMS ||
                parm_ptr[4] > IFX_SP_ENH_AGGRESSIVE_HIGH || parm_ptr[4] < IFX_SP_ENH_AGGRESSIVE_LOW)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_BF, IFX_SP_ENH_ERR_PARAM_RANGE);
            }
            if (num_mics < 2)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_BF, IFX_SP_ENH_ERR_INVALID_CONFIG);
            }
            mdl_infoPt->bf_setting.mic_distance_mm = parm_ptr[0];
            mdl_infoPt->bf_setting.angle_range_start = parm_ptr[1];
            mdl_infoPt->bf_setting.angle_range_stop = parm_ptr[2];
            mdl_infoPt->bf_setting.num_beams = parm_ptr[3];
            mdl_infoPt->bf_setting.aggressiveness = parm_ptr[4];
#else
            enable_bf = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_BF, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_ES:
#ifdef ENABLE_IFX_ES
            enable_es = true;
            if (num_parm != (sizeof(es_settings_struct_t) >> 2))
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
            if (*parm_ptr > IFX_SP_ENH_AGGRESSIVE_HIGH || *parm_ptr < IFX_SP_ENH_AGGRESSIVE_LOW)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ES, IFX_SP_ENH_ERR_PARAM_RANGE);
            }
            mdl_infoPt->es_setting.aggressiveness = *parm_ptr;
#else
            enable_es = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ES, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_DSES:
#ifdef ENABLE_IFX_DSES
            enable_dses = true;
            if (num_parm != (sizeof(dses_settings_struct_t) >> 2))
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
            if (*parm_ptr > IFX_SP_ENH_AGGRESSIVE_HIGH || *parm_ptr < IFX_SP_ENH_AGGRESSIVE_LOW)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSES, IFX_SP_ENH_ERR_PARAM_RANGE);
            }
            mdl_infoPt->dses_setting.aggressiveness = *parm_ptr;
#else
            enable_dses = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSES, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_NS:
#ifdef ENABLE_IFX_NS
            enable_ns = true;
            if (num_parm != (sizeof(ns_settings_struct_t) >> 2))
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
            if (*parm_ptr > MAX_NS_GAIN_DB || *parm_ptr < MIN_NS_GAIN_DB)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_NS, IFX_SP_ENH_ERR_PARAM_RANGE);
            }
            mdl_infoPt->ns_setting.ns_gain_dB = *parm_ptr;
#else
            enable_ns = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_NS, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_DSNS:
#ifdef ENABLE_IFX_DSNS
            enable_dsns = true;
            if (num_parm != (sizeof(dsns_settings_struct_t) >> 2))
            {
                return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_NUMBER_PARMS);
            }
            if (*parm_ptr > MAX_NS_GAIN_DB || *parm_ptr < MIN_NS_GAIN_DB)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSNS, IFX_SP_ENH_ERR_PARAM_RANGE);
            }
            mdl_infoPt->dsns_setting.ns_gain_dB = *parm_ptr;
#else
            enable_dsns = false;
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSNS, IFX_SP_ENH_ERR_NOT_CONFIG_ID);
#endif
            break;
        default:
            return IFX_SP_ENH_ERROR(component_id_ptr[i], IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
            break;
        }
    }
    if (mdl_infoPt->configurator_version != 0)
    {// manually configured is version 0.
        uint32_t configutator_prms_len = (uint32_t)(int_idx - fn_prms) * sizeof(int32_t);

        if (checkCRC32Value((char*)fn_prms, configutator_prms_len, crc_polynomial))
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_MISMATCH_PARM_CHECKSUM);
        }
        if (mdl_infoPt->configurator_version >= 0x00020000)
        {/* Only support below Audio Front End Configurator version 2.0.0 */
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_CONFIGURATOR_VERSION);
        }
    }

    // These parameters need to be determined before memory parsing, where we should force fft_size to be power to 2
    mdl_infoPt->block_size = BLOCK_SIZE;
    mdl_infoPt->overlap_size = BLOCK_SIZE - frame_size;
    mdl_infoPt->fft_size = FFT_SIZE;
    mdl_infoPt->kfft_size = KFFT;
    mdl_infoPt->skip_size = SKIP_SIZE;
    mdl_infoPt->rfft_size = RFFT;

    /*** add up required memory spaces and number of components to be allocated over persistent memory ***/
    enable_flag = 0;

    // init persistent memory size for main container structure
    mdl_infoPt->memory.persistent_mem = ALIGN_WORD(sizeof(sp_enh_struct));

    // two HPFs for left & right channels, one HPF for AEC reference channel
#ifdef ENABLE_IFX_HPF
    if (enable_hpf)
    {
        mdl_infoPt->num_of_hpf = num_mics;
        if (COMPONENT_FLAG_IS_SET(mdl_infoPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_HPF))
        {
            mdl_infoPt->num_of_monitor_outputs += num_mics - 1;
            if (mdl_infoPt->num_of_monitor_outputs > MAX_NUM_MONITOR_OUTPUT)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_CONFIG);
            }
        }
        scratch_sz = 2 * ALIGN_WORD(mdl_infoPt->common.frame_size * sizeof(float)); // for temp float input & output buffers
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }
        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + ifx_hpf_get_coeff_mem_size()
            + num_mics * ifx_hpf_get_state_mem_size() + ALIGN_WORD(sizeof(hpf_settings_struct_t));
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_HPF);
    }
#endif
    // two AECs for left & right channels
#ifdef ENABLE_IFX_AEC
    if (enable_aec)
    {
        int32_t tail_len = convert_ms_to_samples_per_frame_size(sampling_rate, frame_size, mdl_infoPt->aec_setting.tail_len_msec);
        int32_t num_bands = esNumBands;
        long int aec_persistent_size, aec_scratch_size;

        // aec memory
        echo_free(); // this needs to be called to reset echo control global memory
        ifx_echo_state_get_memory(frame_size, tail_len, 1, 1, &aec_persistent_size, &aec_scratch_size);
        scratch_sz = num_mics * aec_scratch_size;
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }
        echoGlobalPersistentSize = num_mics * aec_persistent_size;

        // es memory
        if (enable_es)
        {
            echoGlobalPersistentSize += ifx_preprocess_state_get_memory(frame_size, num_bands);
        }

        // bulk delay memory
        mdl_infoPt->bulk_delay_buf_size = frame_size + convert_ms_to_samples(sampling_rate, MAX_BULK_DELAY_MSEC);

        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + get_aec_coeff_struct_size()
            + num_mics * get_aec_state_struct_size() + ALIGN_WORD(sizeof(aec_settings_struct_t) + echoGlobalPersistentSize) +
            ALIGN_WORD(mdl_infoPt->bulk_delay_buf_size * sizeof(IFX_SP_DATA_TYPE_T)) + ALIGN_WORD(num_mics * frame_size * sizeof(IFX_SP_DATA_TYPE_T));
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_AEC);
        if (COMPONENT_FLAG_IS_SET(mdl_infoPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_AEC)) {
            mdl_infoPt->num_of_monitor_outputs += num_mics - 1;
            if (mdl_infoPt->num_of_monitor_outputs > MAX_NUM_MONITOR_OUTPUT)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_CONFIG);
            }
        }
    }
#endif
    // Up to num_mics + 3 (DSES&NLP) analysis's
#ifdef ENABLE_IFX_ANALYSIS
    if (enable_ana)
    {
        /* various imtermediate subband buffer */
        subband_buf_sz = (num_mics + 1) * ALIGN_WORD(mdl_infoPt->kfft_size * sizeof(complex_float)); // num_mics + 1 shared output/input subband buffers
        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + ifx_anasyn_get_coeff_mem_size()
            + num_mics * (ifx_anasyn_get_state_mem_size() + ALIGN_WORD(sizeof(float) * mdl_infoPt->block_size));
        num_of_ana = num_mics;
#ifdef ENABLE_IFX_DSNS
        if (enable_dsns)
        {
           subband_buf_sz += ALIGN_WORD(mdl_infoPt->kfft_size * sizeof(float)); /* DSES+DSNS dsns_gain_mask buffer */
        }
#endif
#ifdef ENABLE_IFX_DSES
        if (enable_dses)
        {/* DSES reference analysis */
            subband_buf_sz += ALIGN_WORD(mdl_infoPt->kfft_size * sizeof(float)); /* DSES+DSNS dses_gain_mask buffer */
            subband_buf_sz += EXTRA_ANALYSIS * ALIGN_WORD(mdl_infoPt->kfft_size * sizeof(complex_float)); /* DSES&NLP 3 subband buffers */
            subband_buf_sz += EXTRA_ANALYSIS * ALIGN_WORD(frame_size * sizeof(IFX_SP_DATA_TYPE_T)); /* main mic input, DSES reference (AEC est) & AEC reference audio sample frame */
            mdl_infoPt->memory.persistent_mem += EXTRA_ANALYSIS * (ifx_anasyn_get_state_mem_size() + ALIGN_WORD(sizeof(float) * mdl_infoPt->block_size));
            num_of_ana += EXTRA_ANALYSIS;
        }
#endif
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS);
        if (COMPONENT_FLAG_IS_SET(mdl_infoPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS)) {
            if (enable_aec)
            {
                mdl_infoPt->num_of_monitor_outputs += num_mics;
            }
            else
            {
                mdl_infoPt->num_of_monitor_outputs += num_mics - 1;
            }
            if (mdl_infoPt->num_of_monitor_outputs > MAX_NUM_MONITOR_OUTPUT)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_CONFIG);
            }
        }
    }
#endif
    // one BF
#ifdef ENABLE_IFX_BF
    if (enable_bf)
    {
        scratch_sz = (
            ALIGN_WORD(mdl_infoPt->kfft_size * sizeof(float)) +             // 1 float temporary buffer
            2 * ALIGN_WORD(mdl_infoPt->kfft_size * sizeof(complex_float))   // 2 complex float temporary buffers
            );
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }
        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + ifx_bf_get_coeff_mem_size() + \
            ifx_bf_get_state_mem_size() + ALIGN_WORD(sizeof(bf_settings_struct_t));
        mdl_infoPt->bf_pm.persistent_mem_pt = NULL;
        mdl_infoPt->bf_pm.persistent_mem = ifx_bf_get_cofigurable_coeff_size(num_mics, mdl_infoPt->kfft_size, mdl_infoPt->bf_setting.num_beams) * 4;
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_BF);
        if (COMPONENT_FLAG_IS_SET(mdl_infoPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_BF)) {
            num_of_syn++;
        }
    }
    else
    {
        mdl_infoPt->bf_pm.persistent_mem_pt = NULL;
        mdl_infoPt->bf_pm.persistent_mem = 0;
    }
#endif
    // one echo suppression
#ifdef ENABLE_IFX_ES
    if (enable_es)
    {
        mdl_infoPt->memory.persistent_mem += (ALIGN_WORD(sizeof(component_struct_t)) + get_es_coeff_struct_size() +
            get_es_state_struct_size() + ALIGN_WORD(sizeof(es_settings_struct_t)));
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_ES);
        if (!enable_aec)
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ES, IFX_SP_ENH_ERR_INVALID_CONFIG);
        }
     }
#endif
    // one private internal GDE
#ifdef ENABLE_IFX_PRIV_GDE
    {
        /* Seperate GDE state memory form general persistent memory so it can be allocated differntly */
        mdl_infoPt->gde_pm.persistent_mem = ifx_gde_get_state_mem_size() + ifx_gde_get_delay_farend_persistent_mem_size();
        mdl_infoPt->gde_pm.persistent_mem_pt = NULL;
        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + ifx_gde_get_coeff_mem_size(); /* no setting */
        scratch_sz = ifx_gde_get_scratch_mem_size();
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }
     }
#endif
    // one deep subband echo suppression
#ifdef ENABLE_IFX_DSES
    if (enable_dses)
    {
        int32_t persistent_size;
        uint32_t status;

#ifdef USE_MTB_ML
        mdl_infoPt->dses_socmem.persistent_mem = ifx_dses_get_socmem_persistent_mem_size();
        mdl_infoPt->dses_socmem.persistent_mem_pt = NULL;
#endif
        status = ifx_dses_get_persist_mem_size(&persistent_size);
        if (status != IFX_SP_ENH_SUCCESS)
        {
            return status;
        }

        scratch_sz = ifx_dses_get_scratch_mem_size();
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }
        persistent_size += (ALIGN_WORD(sizeof(component_struct_t)) + ALIGN_WORD(sizeof(dses_settings_struct_t))) + ifx_dses_get_coeff_mem_size();
        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(persistent_size);
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES);
        if (!enable_aec)
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSES, IFX_SP_ENH_ERR_INVALID_CONFIG);
        }
        if (COMPONENT_FLAG_IS_SET(mdl_infoPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_DSES)) {
            num_of_syn++;
            mdl_infoPt->num_of_monitor_outputs++; /* Add AEC echo estimate reference to be monitored */
            if (mdl_infoPt->num_of_monitor_outputs > MAX_NUM_MONITOR_OUTPUT)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_CONFIG);
            }
        }
#if defined(DEBUG_FLAG_INFERENCE)
        printf("DSES scratch memory size=%ld, DSES persistent memory size=%ld, running total scratch size = %ld, running total persistent size=%ld\n",
            scratch_sz, persistent_size, max_scratch_mem_size, mdl_infoPt->memory.persistent_mem);
#endif
    }
    else
    {
        mdl_infoPt->dses_socmem.persistent_mem_pt = NULL;
        mdl_infoPt->dses_socmem.persistent_mem = 0;
    }
#endif
    // one noise suppression
#ifdef ENABLE_IFX_NS
    if (enable_ns)
    {
        scratch_sz = ifx_ns_get_scratch_memory_size(frame_size, mdl_infoPt->kfft_size, mdl_infoPt->rfft_size);
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }
        mdl_infoPt->memory.persistent_mem += (ALIGN_WORD(sizeof(component_struct_t)) + ifx_ns_get_coeff_mem_size()
            + ifx_ns_get_state_mem_size() + ALIGN_WORD(sizeof(ns_settings_struct_t))) + ifx_ns_get_table_mem_size();
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_NS);
        if (COMPONENT_FLAG_IS_SET(mdl_infoPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_NS)) {
            num_of_syn++;
        }
    }
#endif
    // one deep subband noise suppression
#ifdef ENABLE_IFX_DSNS
    if (enable_dsns)
    {
        int32_t persistent_size;
        uint32_t status;

#ifdef USE_MTB_ML
        mdl_infoPt->dsns_socmem.persistent_mem = ifx_dsns_get_socmem_persistent_mem_size();
        mdl_infoPt->dsns_socmem.persistent_mem_pt = NULL;
#endif
        status = ifx_dsns_get_persist_mem_size(&persistent_size);
        if (status != IFX_SP_ENH_SUCCESS)
        {
            return status;
        }

        scratch_sz = ifx_dsns_get_scratch_mem_size();
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }

        persistent_size += ALIGN_WORD(sizeof(component_struct_t)) + ALIGN_WORD(sizeof(dsns_settings_struct_t)) + ifx_dsns_get_coeff_mem_size();
        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(persistent_size);
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_DSNS);
        if (COMPONENT_FLAG_IS_SET(mdl_infoPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_DSNS)) {
            num_of_syn++;
        }
#if defined(DEBUG_FLAG_INFERENCE)
        printf("DSNS scratch memory size=%ld, DSNS persistent memory size=%ld, running total scratch size = %ld, running total persistent size=%ld\n",
            scratch_sz, persistent_size, max_scratch_mem_size, mdl_infoPt->memory.persistent_mem);
#endif
    }
    else
    {
        mdl_infoPt->dsns_socmem.persistent_mem_pt = NULL;
        mdl_infoPt->dsns_socmem.persistent_mem = 0;
    }
#endif
    // one dereverberation
#ifdef ENABLE_IFX_DRVB
    if (enable_drvb)
    {
        scratch_sz = ifx_drvb_get_scratch_memory_size();
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }
        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + ifx_drvb_get_persistent_memory_size();
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_DRVB);
        if (COMPONENT_FLAG_IS_SET(mdl_infoPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_DRVB)) {
            num_of_syn++;
        }
    }
#endif
    // one synthesis
#ifdef ENABLE_IFX_SYNTHESIS
    if (enable_syn)
    {
        num_of_syn++;
        mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + ifx_anasyn_get_coeff_mem_size()
            + num_of_syn * (ifx_anasyn_get_state_mem_size() + ALIGN_WORD(sizeof(float) * mdl_infoPt->overlap_size));
        enable_flag = COMPONENT_SET_FLAG(enable_flag, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS);
    }
#endif
    // one transform instance
#if defined(ENABLE_IFX_ANALYSIS) || defined(ENABLE_IFX_SYNTHESIS)
    if (enable_ana || enable_syn)
    {
        scratch_sz = ALIGN_WORD(mdl_infoPt->fft_size * sizeof(float)); // for temp FFT float buffer
        if (scratch_sz > max_scratch_mem_size)
        {
            max_scratch_mem_size = scratch_sz;
        }
        mdl_infoPt->memory.persistent_mem += ifx_transform_get_trans_struc_size();
    }
#endif
    // two meters for input channels, always enabled
    mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + ifx_meter_get_coeff_mem_size()
        + num_mics * ifx_meter_get_state_mem_size();

    // one meter for output channel, always enabled
    mdl_infoPt->memory.persistent_mem += ALIGN_WORD(sizeof(component_struct_t)) + ifx_meter_get_coeff_mem_size()
        + ifx_meter_get_state_mem_size();

    /*** assign rest of parameters  ***/
    mdl_infoPt->enable_flag = enable_flag;
    mdl_infoPt->num_of_ana = num_of_ana;
    mdl_infoPt->num_of_syn = num_of_syn;
    mdl_infoPt->memory.scratch_mem = subband_buf_sz + max_scratch_mem_size;
    mdl_infoPt->ns_es_type = IFX_DSNS | IFX_DSES; // use DSNS & DSES by default

    if (num_of_syn > MAX_NUM_SYNTHESIS)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_CONFIG);
    }
    if (enable_drvb || enable_bf || enable_ns || enable_dsns || enable_dses)
    {/* AEC may need anasyn in future */
        if (!enable_ana || !enable_syn)
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_CONFIG);
        }
    }

    return IFX_SP_ENH_SUCCESS;
}

int32_t read_component_coeff(uint8_t* fn_coeffs, int32_t* byte_count, ifx_stc_sp_enh_info_t* mdl_infoPt, sp_enh_struct* dPt)
{
    struct COEFF_HEADER* config_coeff_header;
    float* flt_idx = (float*)fn_coeffs;


    //Warnings fix - Unused variable (results from enabling certain algos */
    (void)flt_idx;

    if (fn_coeffs == NULL)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_COEFF);
    }
    config_coeff_header = (struct COEFF_HEADER*)fn_coeffs;
    *byte_count = config_coeff_header->size;
#if defined (ENABLE_IFX_HPF) || defined (ENABLE_IFX_ANALYSIS) || defined (ENABLE_IFX_SYNTHESIS)
    int num_coeff = config_coeff_header->size - sizeof(struct COEFF_HEADER);
#endif
    int16_t componet_id = config_coeff_header->component_id;
    if ((uint32_t)config_coeff_header->size < sizeof(struct COEFF_HEADER))
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_COEFF_SIZE);
    }
    if (config_coeff_header->type > IFX_HPSE_DATA_FLOAT || config_coeff_header->type <= IFX_HPSE_DATA_UNKNOWN)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_COEFF);
    }
    flt_idx += sizeof(struct COEFF_HEADER) >> 2;
    if (componet_id == IFX_SP_ENH_IP_COMPONENT_HPF)
    {
#ifdef ENABLE_IFX_HPF
        int status;
        status = ifx_hpf_ceff_init(flt_idx, num_coeff);
        if (status != IFX_SP_ENH_SUCCESS) return status;
#endif
    }
    else if (componet_id == IFX_SP_ENH_IP_COMPONENT_ANALYSIS)
    {
#ifdef ENABLE_IFX_ANALYSIS
        int status;
        status = ifx_ana_ceff_init(flt_idx, num_coeff);
        if (status != IFX_SP_ENH_SUCCESS) return status;
#endif
    }
    else if (componet_id == IFX_SP_ENH_IP_COMPONENT_SYNTHESIS)
    {
#ifdef ENABLE_IFX_SYNTHESIS
        int status;
        status = ifx_syn_ceff_init(flt_idx, num_coeff);
        if (status != IFX_SP_ENH_SUCCESS) return status;
#endif
    }
    else if (componet_id == IFX_SP_ENH_IP_COMPONENT_BF)
    {
#ifdef ENABLE_IFX_BF
        ifx_bf_ceff_init(dPt, flt_idx, mdl_infoPt->bf_setting.num_beams);
#endif
    }
    else
    {
        return IFX_SP_ENH_ERROR(componet_id, IFX_SP_ENH_ERR_COEFF);
    }

    return IFX_SP_ENH_SUCCESS;
}

int32_t ifx_sp_enh_init(void** dPt_container, ifx_stc_sp_enh_info_t* mdl_infoPt, uint8_t* fn_coeffs, int32_t num_coeff_bytes)
{
    int sz, status = 0;

    /*** Sanity check of input arguments ***/
    if (dPt_container == NULL || mdl_infoPt == NULL)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_ARGUMENT);
    }
    if ((mdl_infoPt->memory.persistent_mem_pt == NULL) || (mdl_infoPt->memory.scratch_mem_pt == NULL))
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_ARGUMENT);
    }
    if (mdl_infoPt->common.sampling_rate != HP_SP_ENH_SAMPLING_RATE)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_PARAM);
    }

    /*** Set up memory ***/

    // main struct memory
    sp_enh_struct* dPt = (sp_enh_struct*)(mdl_infoPt->memory.persistent_mem_pt);
    sz = ALIGN_WORD(sizeof(sp_enh_struct));
    memset(dPt, 0, sz);
    dPt->sampling_rate = mdl_infoPt->common.sampling_rate;
    dPt->frame_size = mdl_infoPt->common.frame_size;
    dPt->num_mics = mdl_infoPt->common.num_mics;
    dPt->n_components = mdl_infoPt->num_config_components;
    dPt->block_size = mdl_infoPt->block_size;
    dPt->overlap_size = mdl_infoPt->overlap_size;
    dPt->fft_size = mdl_infoPt->fft_size;
    dPt->kfft_size = mdl_infoPt->kfft_size;
    dPt->skip_size = mdl_infoPt->skip_size;
    dPt->rfft_size = mdl_infoPt->rfft_size;
    dPt->ns_es_type = mdl_infoPt->ns_es_type;
    dPt->enable_flag = mdl_infoPt->enable_flag;
    dPt->active_flag = mdl_infoPt->enable_flag; /* always active right after init */
    dPt->monitor_flag = mdl_infoPt->monitor_flag;
    dPt->persistent_pt = (char*)dPt;
    dPt->persistent_size = sz;

    // increase number of components up to 3 for input & output meters + GDE if compilation switch is enabled
    dPt->n_components += NUM_PRIV_COMPONENT;

    // save scratch memory pointer and size
    dPt->scratch.scratch_pad = mdl_infoPt->memory.scratch_mem_pt;
    dPt->scratch.scratch_size = mdl_infoPt->memory.scratch_mem;
    ifx_mem_reset(&dPt->scratch);

#if defined(ENABLE_IFX_ANALYSIS)
    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
    {/* Allocate scratch memory for various imtermediate subband buffer */
        int subband_byte_size = dPt->kfft_size * sizeof(complex_float);
        // first mic subband
        dPt->analysis_out_pt[0] = ifx_mem_allocate(&dPt->scratch, subband_byte_size);
        if (dPt->analysis_out_pt[0] == NULL)
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_OVER_MAX_SCRATCH_MEM);
        }
        if (dPt->num_mics == 2)
        {
            // second mic subband
            dPt->analysis_out_pt[1] = ifx_mem_allocate(&dPt->scratch, subband_byte_size);
            if (dPt->analysis_out_pt[1] == NULL)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_OVER_MAX_SCRATCH_MEM);
            }
        }

        // output subband
        dPt->subband_out_pt = ifx_mem_allocate(&dPt->scratch, subband_byte_size);
        if (dPt->subband_out_pt == NULL)
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_OVER_MAX_SCRATCH_MEM);
        }
#ifdef ENABLE_IFX_DSNS
        if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSNS))
        {
            sz = dPt->kfft_size * sizeof(float);
            dPt->dsns_gain_mask = ifx_mem_allocate(&dPt->scratch, sz);
        }
#endif
#ifdef ENABLE_IFX_DSES
        if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
        {
            /* DSES GDE requires AEC farend and mic (pre-AFE) audio sample buffer pointers */
            dPt->aec_farend_pt = ifx_mem_allocate(&dPt->scratch, dPt->frame_size * sizeof(IFX_SP_DATA_TYPE_T));
            dPt->main_mic_pt = ifx_mem_allocate(&dPt->scratch, dPt->frame_size * sizeof(IFX_SP_DATA_TYPE_T));
            /* DSES requires reference (aec farend or echo estimate) audio sample buffer pointer */
            dPt->dses_ref_pt = ifx_mem_allocate(&dPt->scratch, dPt->frame_size * sizeof(IFX_SP_DATA_TYPE_T));
            /* DSES requires reference and NLP requires farend & post-AEC (residual) subband buffer pointers */
            dPt->subband_dses_ref_pt = ifx_mem_allocate(&dPt->scratch, subband_byte_size);
            dPt->subband_farend_pt = ifx_mem_allocate(&dPt->scratch, subband_byte_size);
            dPt->subband_post_aec_pt = ifx_mem_allocate(&dPt->scratch, subband_byte_size);
            if (dPt->aec_farend_pt == NULL || dPt->main_mic_pt == NULL || dPt->dses_ref_pt == NULL ||
                dPt->subband_dses_ref_pt == NULL || dPt->subband_farend_pt == NULL || dPt->subband_post_aec_pt == NULL)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSES, IFX_SP_ENH_ERR_OVER_MAX_SCRATCH_MEM);
            }
            sz = dPt->kfft_size * sizeof(float);
            dPt->dses_gain_mask = ifx_mem_allocate(&dPt->scratch, sz);
        }
#endif
    }
#endif
    // init tranform procedure
#if defined(ENABLE_IFX_ANALYSIS) || defined(ENABLE_IFX_SYNTHESIS)
    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS) ||
        COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS))
    {
        ifx_transform_configure_persistent_mem(dPt);
        status = transform_init(&dPt->scratch, dPt->trans_struct, dPt->block_size, dPt->fft_size, dPt->frame_size, dPt->overlap_size);
        if (status != IFX_SP_ENH_SUCCESS)
        {
            return status;
        }
    }
#endif

    // memory to hold all component structures
    dPt->l = (component_struct_t*)((char*)dPt + dPt->persistent_size);
    sz = ALIGN_WORD(sizeof(component_struct_t)) * dPt->n_components;
    memset(dPt->l, 0, sz);
    dPt->persistent_size += sz;

    // init each component
    /*** make sure to follow the actual sequence of components, e.g., analysis must preceed beamforming ***/
#ifdef ENABLE_IFX_HPF
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_HPF, mdl_infoPt, mdl_infoPt->num_of_hpf,
        ifx_hpf_get_coeff_mem_size(), ifx_hpf_get_state_mem_size(), sizeof(hpf_settings_struct_t));
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_PRIV_COMPONENT_INPUT_METER, mdl_infoPt, mdl_infoPt->common.num_mics,
        ifx_meter_get_coeff_mem_size(), ifx_meter_get_state_mem_size(), 0);
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#ifdef ENABLE_IFX_AEC
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_AEC, mdl_infoPt, mdl_infoPt->common.num_mics,
        get_aec_coeff_struct_size(), get_aec_state_struct_size(), sizeof(aec_settings_struct_t));
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_PRIV_GDE
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_PRIV_COMPONENT_GDE, mdl_infoPt, 1,
        ifx_gde_get_coeff_mem_size(), ifx_gde_get_state_mem_size(), 0);
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_ANALYSIS
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_ANALYSIS, mdl_infoPt, mdl_infoPt->num_of_ana,
        ifx_anasyn_get_coeff_mem_size(), ifx_anasyn_get_state_mem_size(), 0);
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_BF
    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_BF))
    {
        if (fn_coeffs == NULL || num_coeff_bytes == 0)
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_BF, IFX_SP_ENH_ERR_COEFF);
        }
    }
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_BF, mdl_infoPt, 1,
        ifx_bf_get_coeff_mem_size(), ifx_bf_get_state_mem_size(), sizeof(bf_settings_struct_t));
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_DRVB
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_DRVB, mdl_infoPt, 1,
        ifx_drvb_get_prm_struc_size(), ifx_drvb_get_persistent_struc_size(), sizeof(drvb_settings_struct_t));
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_NS
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_NS, mdl_infoPt, 1,
        ifx_ns_get_coeff_mem_size(), ifx_ns_get_state_mem_size(), sizeof(ns_settings_struct_t));
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_DSNS
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_DSNS, mdl_infoPt, 1,
        ifx_dsns_get_coeff_mem_size(), 0, sizeof(dsns_settings_struct_t));
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_DSES
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_DSES, mdl_infoPt, 1,
        ifx_dses_get_coeff_mem_size(), ifx_dses_nlp_get_state_mem_size(), sizeof(dses_settings_struct_t));
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_SYNTHESIS
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS, mdl_infoPt, mdl_infoPt->num_of_syn,
        ifx_anasyn_get_coeff_mem_size(), ifx_anasyn_get_state_mem_size(), 0);
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
#ifdef ENABLE_IFX_ES
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_COMPONENT_ES, mdl_infoPt, 1,
        get_es_coeff_struct_size(), get_es_state_struct_size(), sizeof(es_settings_struct_t));
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }
#endif
    status = init_component_structure_memory(dPt, IFX_SP_ENH_IP_PRIV_COMPONENT_OUTPUT_METER, mdl_infoPt, 1,
        ifx_meter_get_coeff_mem_size(), ifx_meter_get_state_mem_size(), 0);
    if (status != IFX_SP_ENH_SUCCESS)
    {
        return status;
    }

    /* parse configurable coefficients */
    if (fn_coeffs && num_coeff_bytes > 0)
    {
        uint8_t* coeffs_pt = fn_coeffs;
        int32_t total_bytes = 0;
        /* sanity check */
        if ((uint32_t)num_coeff_bytes < sizeof(struct COEFF_HEADER))
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_COEFF_SIZE);
        }
        while (total_bytes < num_coeff_bytes)
        {
            int32_t num_read_bytes;
            status = read_component_coeff(coeffs_pt, &num_read_bytes, mdl_infoPt, dPt);
            if (status != IFX_SP_ENH_SUCCESS) return status;
            coeffs_pt += num_read_bytes;
            total_bytes += num_read_bytes;
            if (num_coeff_bytes < total_bytes)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_COEFF_SIZE);
            }
        }
    }

    // set the main speech enhancement object pointer
    *dPt_container = dPt;

#ifdef ENABLE_IFX_AEC
    /* Check global persisent memory usage is correct */
	if (echoGlobalPersistentPtr == NULL)
	{
        // this is quite convoluted since OVERRIDE_SPEEX_ALLOC is not visible here, but if
        // it was not defined, then echoGlobalPersistentPtr == NULL, in which case make sure
        // to subtract the amount of global memory reserved during parsing stage so that the
        // final check for perisistent memory usage passes
        mdl_infoPt->memory.persistent_mem -= echoGlobalPersistentSize;
	}
	else if (echoGlobalPesistentCumulateCount != echoGlobalPersistentSize)
	{
        // if OVERRIDE_SPEEX_ALLOC was defined, then echoGlobalPersistentPtr != NULL, in which
        // case make sure to check for match in global memory space reserved and actually used
		return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_AEC, IFX_SP_ENH_ERR_GLOBAL_PERSIST_MEM);
	}
#endif
    /* Check pesistent memory usage is correct */
    if (dPt->persistent_size != mdl_infoPt->memory.persistent_mem)
    {
#if defined(DEBUG_FLAG_INFERENCE)
        printf("Persistent allocated size=%d, used size=%d!\r\n", mdl_infoPt->memory.persistent_mem, dPt->persistent_size);
#endif
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_PERSISTEMT_MEM);
    }

    return IFX_SP_ENH_SUCCESS;
}

int init_component_structure_memory(sp_enh_struct* dPt, int32_t component_id, ifx_stc_sp_enh_info_t* mdl_infoPt, int32_t num_instances,
    int32_t coeff_struct_size, int32_t state_struct_size, int32_t settings_structure_size)
{
    uint32_t sz;
    int status = IFX_SP_ENH_SUCCESS;

    //To bypass the compiler optimization added workaround, by introducing the 'is_valid_comp' variable and using it in the if check.
    bool is_valid_comp = (component_id == ((int32_t)IFX_SP_ENH_IP_PRIV_COMPONENT_INPUT_METER)) ||
                         (component_id == ((int32_t)IFX_SP_ENH_IP_PRIV_COMPONENT_OUTPUT_METER)) ||
                         (component_id == ((int32_t)IFX_SP_ENH_IP_PRIV_COMPONENT_GDE)) ||
                         COMPONENT_FLAG_IS_SET(dPt->enable_flag, component_id);
    if (is_valid_comp)
    {
        // obtain pointer of component and save id
        component_struct_t* lPt = &dPt->l[dPt->component++];
        lPt->component_typeb = component_id;

        // init coeff memory
        sz = ALIGN_WORD(coeff_struct_size);
        lPt->coeff_struct = (void*)(dPt->persistent_pt + dPt->persistent_size);
        memset(lPt->coeff_struct, 0, sz);
        dPt->persistent_size += sz;

        // init state memory
        lPt->n_states = num_instances;
        sz = ALIGN_WORD(state_struct_size);
        for (uint32_t i = 0; i < lPt->n_states; i++)
        {
            if (component_id == IFX_SP_ENH_IP_PRIV_COMPONENT_GDE && mdl_infoPt->gde_pm.persistent_mem_pt != NULL)
            {
                lPt->state_struct[i] = (void*)(mdl_infoPt->gde_pm.persistent_mem_pt);
                memset(lPt->state_struct[i], 0, sz);
            }
            else
            {
                lPt->state_struct[i] = (void*)(dPt->persistent_pt + dPt->persistent_size);
                memset(lPt->state_struct[i], 0, sz);
                dPt->persistent_size += sz;
            }
        }

        // init structure settings memory
        if (settings_structure_size > 0) {
            sz = ALIGN_WORD(settings_structure_size);
            lPt->settings_struct = (void*)(dPt->persistent_pt + dPt->persistent_size);
            memset(lPt->settings_struct, 0, sz);
            dPt->persistent_size += sz;
        }

        // call each component init function
        switch (component_id)
        {
#ifdef ENABLE_IFX_HPF
        case IFX_SP_ENH_IP_COMPONENT_HPF:
            ((hpf_settings_struct_t*)lPt->settings_struct)->cutoff_freq_hz = mdl_infoPt->hpf_setting.cutoff_freq_hz;
            for (int i = 0; i < mdl_infoPt->common.num_mics; i++)
            {
                ((hpf_settings_struct_t*)lPt->settings_struct)->gain_q8_dB[i] = mdl_infoPt->hpf_setting.gain_q8_dB[i];
            }
            status = ifx_hpf_init(&dPt->scratch, lPt, dPt->frame_size, false);
            break;
#endif
#ifdef ENABLE_IFX_AEC
        case IFX_SP_ENH_IP_COMPONENT_AEC:
            sz = ALIGN_WORD(mdl_infoPt->bulk_delay_buf_size * sizeof(IFX_SP_DATA_TYPE_T)); // allocate memory for int16_t reference bulk delay buffer
            init_delay_buffer(&dPt->farend_delay_buffer_struct, (IFX_SP_DATA_TYPE_T*)(dPt->persistent_pt + dPt->persistent_size), mdl_infoPt->bulk_delay_buf_size, false);
            dPt->persistent_size += sz;
            ((aec_settings_struct_t*)lPt->settings_struct)->bulk_delay_msec = mdl_infoPt->aec_setting.bulk_delay_msec;
            ((aec_settings_struct_t*)lPt->settings_struct)->tail_len_msec = mdl_infoPt->aec_setting.tail_len_msec;
            status = ifx_aec_init(dPt, lPt, dPt->frame_size);
            break;
#endif
#ifdef ENABLE_IFX_PRIV_GDE
        case IFX_SP_ENH_IP_PRIV_COMPONENT_GDE:
            if (mdl_infoPt->gde_pm.persistent_mem_pt == NULL)
            {
                sz = ifx_gde_get_delay_farend_persistent_mem_size();
                dPt->delay_farend_pt = (IFX_SP_DATA_TYPE_T*)(dPt->persistent_pt + dPt->persistent_size);
                memset(dPt->delay_farend_pt, 0, sz);
                dPt->persistent_size += sz;
            }
            else
            {
                sz = ifx_gde_get_state_mem_size();
                dPt->delay_farend_pt = (IFX_SP_DATA_TYPE_T*)(((char*)mdl_infoPt->gde_pm.persistent_mem_pt) + sz);
                sz = ifx_gde_get_delay_farend_persistent_mem_size();
                memset(dPt->delay_farend_pt, 0, sz);
            }
            status = ifx_dses_gde_init(lPt, &dPt->scratch, false);
            break;
#endif
#ifdef ENABLE_IFX_ANALYSIS
        case IFX_SP_ENH_IP_COMPONENT_ANALYSIS:
            {
            sz = lPt->n_states * ALIGN_WORD(dPt->block_size * sizeof(float)); // allocate memory for float input delay buffer
            float* analysis_delay_buffer = (float*)(dPt->persistent_pt + dPt->persistent_size);
            dPt->persistent_size += sz;
            status = ifx_anasyn_init(lPt, analysis_delay_buffer, dPt->block_size, dPt->block_size, false);
            }
            break;
#endif
#ifdef ENABLE_IFX_SYNTHESIS
        case IFX_SP_ENH_IP_COMPONENT_SYNTHESIS:
            {
            sz = lPt->n_states * ALIGN_WORD(dPt->overlap_size * sizeof(float)); // allocate memory for float output delay buffer
            void* synthesis_delay_buffer = (void*)(dPt->persistent_pt + dPt->persistent_size);
            dPt->persistent_size += sz;
            dPt->synthesis_proc_ind = dPt->component -1;
            status = ifx_anasyn_init(lPt, synthesis_delay_buffer, dPt->block_size, dPt->overlap_size, false);
            }
            break;
#endif
#ifdef ENABLE_IFX_BF
        case IFX_SP_ENH_IP_COMPONENT_BF:
            ((bf_settings_struct_t*)lPt->settings_struct)->mic_distance_mm = mdl_infoPt->bf_setting.mic_distance_mm;
            ((bf_settings_struct_t*)lPt->settings_struct)->angle_range_start = mdl_infoPt->bf_setting.angle_range_start;
            ((bf_settings_struct_t*)lPt->settings_struct)->angle_range_stop = mdl_infoPt->bf_setting.angle_range_stop;
            ((bf_settings_struct_t*)lPt->settings_struct)->num_beams = mdl_infoPt->bf_setting.num_beams;
            ((bf_settings_struct_t*)lPt->settings_struct)->aggressiveness = mdl_infoPt->bf_setting.aggressiveness;
            /* init BF persistent configurable coeff array memory */
            status = ifx_bf_configure_coeff_mem(dPt, lPt, mdl_infoPt->bf_pm, dPt->num_mics, dPt->kfft_size, mdl_infoPt->bf_setting.num_beams);
            if (status != IFX_SP_ENH_SUCCESS) return status;
            else {
                /* Init here is ok but may be more clear doing it later */
                status = ifx_bf_init(lPt, dPt->sampling_rate, dPt->frame_size, dPt->num_mics, dPt->kfft_size, dPt->block_size, mdl_infoPt->bf_setting.num_beams);
            }
            break;
#endif
#ifdef ENABLE_IFX_DRVB
        case IFX_SP_ENH_IP_COMPONENT_DRVB:
            ((drvb_settings_struct_t*)lPt->settings_struct)->aggressiveness = mdl_infoPt->drvb_setting.aggressiveness;
            status = ifx_drvb_init(lPt, &dPt->scratch, false);
            break;
#endif
#ifdef ENABLE_IFX_ES
        case IFX_SP_ENH_IP_COMPONENT_ES:
            ((es_settings_struct_t*)lPt->settings_struct)->aggressiveness = mdl_infoPt->es_setting.aggressiveness;
            status = ifx_es_init(dPt, lPt, dPt->frame_size);
            break;
#endif
#ifdef ENABLE_IFX_NS
        case IFX_SP_ENH_IP_COMPONENT_NS:
            ((ns_settings_struct_t*)lPt->settings_struct)->ns_gain_dB = mdl_infoPt->ns_setting.ns_gain_dB;
            status = ifx_ns_init(&dPt->scratch, lPt, dPt->sampling_rate, dPt->frame_size, dPt->kfft_size, dPt->skip_size, dPt->rfft_size, false);
            /* assign lookup table memory in persistent memory */
            sz = ifx_ns_get_table_mem_size();
            dPt->table_struct = (void*)((char*)dPt + dPt->persistent_size);
            dPt->persistent_size += sz;
            // init lookup table memory
            status = ifx_table_init(dPt->table_struct);
            break;
#endif
#ifdef ENABLE_IFX_DSNS
        case IFX_SP_ENH_IP_COMPONENT_DSNS:
            ((dsns_settings_struct_t*)lPt->settings_struct)->ns_gain_dB = mdl_infoPt->dsns_setting.ns_gain_dB;
            status = ifx_dsns_init(dPt, lPt, mdl_infoPt->dsns_socmem);
            break;
#endif
#ifdef ENABLE_IFX_DSES
        case IFX_SP_ENH_IP_COMPONENT_DSES:
            ((dses_settings_struct_t*)lPt->settings_struct)->aggressiveness = mdl_infoPt->dses_setting.aggressiveness;
            status = ifx_dses_init(dPt, lPt, mdl_infoPt->dses_socmem);
            break;
#endif
        case IFX_SP_ENH_IP_PRIV_COMPONENT_INPUT_METER:
            status = ifx_meter_init(&dPt->scratch, lPt, dPt->sampling_rate, dPt->frame_size);
            break;
        case IFX_SP_ENH_IP_PRIV_COMPONENT_OUTPUT_METER:
            status = ifx_meter_init(&dPt->scratch, lPt, dPt->sampling_rate, dPt->frame_size);
            break;
        default:
            return IFX_SP_ENH_ERROR(component_id, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
            break;
        }
    }

    return status;
}

int32_t ifx_sp_enh_deinit_internal_mem(void* dPt_container)
{
    /*** Sanity check of input arguments ***/
    if (dPt_container == NULL)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_ARGUMENT);
    }

    sp_enh_struct* dPt;
    dPt = (sp_enh_struct*)dPt_container;
    component_struct_t* lPt;
    uint32_t status = IFX_SP_ENH_SUCCESS;

    if (dPt->n_components <= NUM_PRIV_COMPONENT)
    {/* Nothing to free */
        return IFX_SP_ENH_SUCCESS;
    }

    for (dPt->component = 0; dPt->component < dPt->n_components; dPt->component++)
    {
        lPt = &dPt->l[dPt->component];
        /* Call the deinit process function for each component */
        switch (lPt->component_typeb)
        {
        case IFX_SP_ENH_IP_COMPONENT_DSNS:
#ifdef ENABLE_IFX_DSNS
            status = ifx_dsns_free(lPt);
#endif
            break;
        case IFX_SP_ENH_IP_COMPONENT_DSES:
#ifdef ENABLE_IFX_DSES
            status = ifx_dses_free(lPt);
#endif
            break;
        default:
            break;
        }
    }
    return status;
}
