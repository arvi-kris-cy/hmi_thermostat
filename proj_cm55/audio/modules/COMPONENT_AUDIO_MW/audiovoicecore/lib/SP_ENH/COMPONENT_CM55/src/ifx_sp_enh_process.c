/******************************************************************************
* File Name: ifx_sp_enh_process.c
*
* Description: This file contains Infineon speech enhancement process functions.
*
*
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
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#ifdef ENABLE_AFE_MW_SUPPORT
#include "cy_afe_configurator_settings.h"
#endif
#include "ifx_sp_enh_priv.h"
#ifdef PROFILER
#include "ifx_cycle_profiler.h"
#endif
#if (defined(ENABLE_SWITCH_NOISE_SUPPRESSION) || defined(ENABLE_RESET_AFE_COMPONENTS))
#include "cyhal.h"
#include "cy_retarget_io.h"
#endif

/******************************************************************************
* Constants and Marco
*****************************************************************************/

/*******************************************************************************
* Function Name: ifx_sp_enh_process
********************************************************************************
* Summary:
*  This function computes complete Infineon high performance speech enhancement
*  algorithms using component-by-component architecture. This function is also
*  responsible for sound meter output and monitor output, working buffer management.
*
* Parameters:
*  void *modelPt : contains model definition and parameters
*
*******************************************************************************/

int32_t ifx_sp_enh_process(void* modelPt, void* input1, void* input2, void* reference_input, void* output, void* ifx_output,
    void* meter_output)
{
    IFX_SP_DATA_TYPE_T* in1_pt = (IFX_SP_DATA_TYPE_T*)input1;
    IFX_SP_DATA_TYPE_T* in2_pt = (IFX_SP_DATA_TYPE_T*)input2;
    IFX_SP_DATA_TYPE_T* ref_pt = (IFX_SP_DATA_TYPE_T*)reference_input;
    IFX_SP_DATA_TYPE_T* out_pt = (IFX_SP_DATA_TYPE_T*)output;
    IFX_SP_DATA_TYPE_T* meter_pt = (IFX_SP_DATA_TYPE_T*)meter_output;
    char* ifx_out = ifx_output;
    complex_float* analysis_out1_pt = NULL;
    complex_float* analysis_out2_pt = NULL;
    complex_float* subband_out_pt = NULL;
    float* dsns_gain_mask = NULL;

    sp_enh_struct* dPt;
    dPt = (sp_enh_struct*)modelPt;
    component_struct_t* lPt;

    int32_t in_out_byte_size = 0, subband_byte_size = 0;
    /* Warning fix */
    (void)subband_byte_size; (void)ifx_out; (void)ref_pt; (void)dsns_gain_mask;

#ifdef PROFILER
    ifx_cycle_profile_start(&(dPt->profile));
#endif

    /* Sanity checking of input arguments */
    if (modelPt == NULL || input1 == NULL || output == NULL || ifx_output == NULL)
    {
        return IFX_SP_ENH_ERROR(IFX_SP_ENH_COMPONENT_ID_INVALID, IFX_SP_ENH_ERR_INVALID_ARGUMENT);
    }

    // set time-domain and subband-domain buffer sizes
    in_out_byte_size = dPt->frame_size * sizeof(IFX_SP_DATA_TYPE_T);
    subband_byte_size = dPt->kfft_size * sizeof(complex_float);

    /* Allocate scratch memory for various imtermediate subband buffer */
    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
    {
        // first mic subband
        analysis_out1_pt = dPt->analysis_out_pt[0];
        if (analysis_out1_pt == NULL)
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_SCRATCH_MEM_NULL);
        }
        if (dPt->num_mics == 2)
        {
            // second mic subband
            analysis_out2_pt = dPt->analysis_out_pt[1];
            if (analysis_out2_pt == NULL)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_SCRATCH_MEM_NULL);
            }
        }

        // output subband
        subband_out_pt = dPt->subband_out_pt;
        if (subband_out_pt == NULL)
        {
            return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_SCRATCH_MEM_NULL);
        }
#ifdef ENABLE_IFX_DSES
        if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
        {// DSES input/output subband
            if (dPt->aec_farend_pt == NULL || dPt->main_mic_pt == NULL ||
                dPt->dses_ref_pt == NULL || dPt->subband_dses_ref_pt == NULL ||
                dPt->subband_post_aec_pt == NULL || dPt->subband_farend_pt == NULL)
            {
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSES, IFX_SP_ENH_ERR_SCRATCH_MEM_NULL);
            }
            // store latest farend and mic audio frames
            int bytes = dPt->frame_size * sizeof(int16_t);
            memcpy(dPt->aec_farend_pt, ref_pt, bytes);
            memcpy(dPt->main_mic_pt, in1_pt, bytes);

            if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSNS))
            {/* This is only needed for DSNS + DSES processed in parallel and then do post processing */
                dsns_gain_mask = dPt->dsns_gain_mask;
                if (dsns_gain_mask == NULL)
                {
                    return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSES, IFX_SP_ENH_ERR_SCRATCH_MEM_NULL);
                }
            }
        }
#endif
    }

#if defined(ENABLE_SWITCH_NOISE_SUPPRESSION)
    /* This is debug code to switch noise & echo suppressions on embedded system via UART message during live demo */
    /* Type 's' to switch between DSNS and NS, type 'e' to switch between DSES and ES */
    uint8_t uart_read_value;
    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_NS) &&
        COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSNS) &&
        cyhal_uart_getc(&cy_retarget_io_uart_obj, &uart_read_value, 1) == CY_RSLT_SUCCESS)
    {
        if (uart_read_value == 's')
        {
            if (dPt->ns_es_type & IFX_DSNS == IFX_NS)
            {
                dPt->ns_es_type |= IFX_DSNS;
                dPt->active_flag = COMPONENT_CLEAR_FLAG(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_NS);
                dPt->active_flag = COMPONENT_SET_FLAG(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_DSNS);
                printf("DSNS activated.\r\n");
            }
            else
            {
                dPt->ns_es_type &= (~IFX_DSNS);
                dPt->active_flag = COMPONENT_CLEAR_FLAG(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_DSNS);
                dPt->active_flag = COMPONENT_SET_FLAG(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_NS);
                printf("NS activated.\r\n");
            }
        }
        if (uart_read_value == 'e')
        {
            if (dPt->ns_es_type & IFX_ES_BITS == IFX_ES)
            {
                dPt->ns_es_type = (dPt->ns_es_type & (~IFX_ES)) | IFX_DSES;
                dPt->active_flag = COMPONENT_CLEAR_FLAG(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_ES);
                dPt->active_flag = COMPONENT_SET_FLAG(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_DSES);
                printf("DSES activated.\r\n");
            }
            else
            {
                dPt->ns_es_type = (dPt->ns_es_type & (~IFX_DSES)) | IFX_ES;
                dPt->active_flag = COMPONENT_CLEAR_FLAG(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_DSES);
                dPt->active_flag = COMPONENT_SET_FLAG(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_ES);
                printf("ES activated.\r\n");
            }
        }
}
#endif

#if defined(ENABLE_RESET_AFE_COMPONENTS)
    /* This is debug code to reset AFE components on embedded system via UART message during live testing */
    uint8_t uart_read_value;
    if (cyhal_uart_getc(&cy_retarget_io_uart_obj, &uart_read_value, 1) == CY_RSLT_SUCCESS)
    {
        if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_AEC) && uart_read_value == 'a')
        {
        	// reset AEC
        	dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_AEC);
        	dPt->reset_flag = COMPONENT_SET_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_AEC);
            printf("AEC reset.\r\n");
        }
    }
#endif

    if (dPt->n_components <= NUM_PRIV_COMPONENT)
    {
        // bypass all components and copy in1_pt directly to output because there is no component enabled except meters
        memcpy(out_pt, in1_pt, in_out_byte_size);
    }

    {
        int32_t status = IFX_SP_ENH_SUCCESS, syn_ind = 0, ana_ind = 0;
        uint32_t delay = 0;
        /* Warning fix */
        (void)status; (void)syn_ind, (void)ana_ind, (void)delay;

        for (dPt->component = 0; dPt->component < dPt->n_components; dPt->component++)
        {
            lPt = &dPt->l[dPt->component];
#ifdef PROFILER
            ifx_cycle_profile_start(&(lPt->profile));
#endif
            /* Call the process function for each component */
            switch (lPt->component_typeb)
            {
            case IFX_SP_ENH_IP_COMPONENT_HPF:
            {
#ifdef ENABLE_IFX_HPF
                if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_HPF))
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_HPF))
                    {
                        ifx_hpf_init(&dPt->scratch, lPt, dPt->frame_size, true);
                        dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_HPF);
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_HPF))
                    {
                        int32_t gain_dB = ((hpf_settings_struct_t*)lPt->settings_struct)->gain_q8_dB[0];
                        status |= ifx_hpf_process(in1_pt, 0, lPt, gain_dB); /* output buffer is same as input */
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_HPF))
                    {
                        int bytes = dPt->frame_size * sizeof(int16_t);
                        memcpy(ifx_out, in1_pt, bytes);
                        ifx_out += bytes;
                    }

                    if (!COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS) &&
                        !COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_AEC))
                    {
                        // copy result directly to output if AEC and analysis are turned off
                        memcpy(out_pt, in1_pt, in_out_byte_size);
                    }
                    if (dPt->num_mics == 2 && in2_pt && COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                    {
                        // run only if second input and analysis are turned on
                        if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_HPF))
                        {
                            int32_t gain_dB = ((hpf_settings_struct_t*)lPt->settings_struct)->gain_q8_dB[1];
                            status |= ifx_hpf_process(in2_pt, 1, lPt, gain_dB); /* output buffer is same as input */
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_HPF))
                        {
                            int bytes = dPt->frame_size * sizeof(int16_t);
                            memcpy(ifx_out, in2_pt, bytes);
                            ifx_out += bytes;
                        }
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_HPF, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_PRIV_COMPONENT_INPUT_METER:
            {
                ifx_meter_process(meter_pt, 0, lPt, in1_pt);
                if (dPt->num_mics == 2 && in2_pt)
                {
                    meter_pt++;
                    ifx_meter_process(meter_pt, 1, lPt, in2_pt);
                }
                break;
            }
            case IFX_SP_ENH_IP_COMPONENT_AEC:
            {
#ifdef ENABLE_IFX_AEC
                if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_AEC))
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_AEC))
                    {
                        init_delay_buffer(&dPt->farend_delay_buffer_struct, NULL, 0, true);
                        ifx_aec_reset(lPt, dPt->frame_size);
                        dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_AEC);
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_AEC) && (ref_pt != NULL))
                    {
                        int32_t bulk_delay_msec = ((aec_settings_struct_t*)lPt->settings_struct)->bulk_delay_msec;
                        bulk_delay_msec = MAX(bulk_delay_msec - BULK_DELAY_COMP_ADJUST_MSEC, 0);
                        int32_t bulk_delay = convert_ms_to_samples(dPt->sampling_rate, bulk_delay_msec);
#if 1
                        delay_compensation(&dPt->farend_delay_buffer_struct, ref_pt, dPt->frame_size, bulk_delay);
#else
                        write_to_delay_buffer(&dPt->farend_delay_buffer_struct, ref_pt, dPt->frame_size);
                        read_from_delay_buffer(&dPt->farend_delay_buffer_struct, ref_pt, dPt->frame_size, bulk_delay);
#endif
                        status |= ifx_aec_process(in1_pt, ref_pt, dPt->frame_size, lPt->state_struct[0]); /* Output overwrite input */
#ifdef ENABLE_IFX_DSES
                    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
                    {/* DSES requires AEC output reference signal */
                        status |= ifx_aec_get_echo_reference(dPt->dses_ref_pt, lPt->state_struct[0]);
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
                        {
                            int bytes = dPt->frame_size * sizeof(int16_t);
                            memcpy(ifx_out, dPt->dses_ref_pt, bytes);
                            ifx_out += bytes;
                        }
                    }
#endif
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_AEC))
                    {
                        int bytes = dPt->frame_size * sizeof(int16_t);
                        memcpy(ifx_out, in1_pt, bytes);
                        ifx_out += bytes;
                    }
                    if (!COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                    {
                        // copy result directly to output if analysis is turned off
                        memcpy(out_pt, in1_pt, in_out_byte_size);
                    }
                    if (dPt->num_mics == 2 && in2_pt && COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                    {
                        // run only if second input and analysis are turned on
                        if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_AEC) && (ref_pt != NULL))
                        {
                            status |= ifx_aec_process(in2_pt, ref_pt, dPt->frame_size, lPt->state_struct[1]); /* Output overwrite input */
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_AEC))
                        {
                            int bytes = dPt->frame_size * sizeof(int16_t);
                            memcpy(ifx_out, in2_pt, bytes);
                            ifx_out += bytes;
                        }
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_AEC, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
#ifdef ENABLE_IFX_PRIV_GDE
            case IFX_SP_ENH_IP_PRIV_COMPONENT_GDE:
            {// estimate group delay
                if (dPt->aec_farend_pt != NULL) {
                    // here we reuse dPt->farend_delay_buffer_struct for group delay compensation
                    // for NLP assuming that it has been updated with latest audio frame during AEC
                    status |= ifx_dses_gde_process(lPt, dPt->aec_farend_pt, dPt->main_mic_pt, &delay);
                    read_from_delay_buffer(&dPt->farend_delay_buffer_struct, dPt->aec_farend_pt, dPt->frame_size, delay);
                    if (status != IFX_SP_ENH_SUCCESS) return status;
                }
                break;
            }
#endif
            case IFX_SP_ENH_IP_COMPONENT_ANALYSIS:
            {
#ifdef ENABLE_IFX_ANALYSIS
                if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                    {
                        ifx_anasyn_init(lPt, NULL, dPt->block_size, dPt->block_size, true);
                        dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS);
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                    {
                        analysis_wloaFFT(analysis_out1_pt, dPt->trans_struct, lPt->state_struct[0], in1_pt);
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                        {
                            memcpy(ifx_out, analysis_out1_pt, subband_byte_size);
                            ifx_out += subband_byte_size;
                        }
                    }
                    else
                    {
                        // copy first mic input directly to AFE output if analysis is not activated
                        memcpy(out_pt, in1_pt, in_out_byte_size);
                        /* When it is de-actived, monitor output is audio samples */
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                        {
                            int bytes = dPt->frame_size * sizeof(int16_t);
                            memcpy(ifx_out, in1_pt, bytes);
                            ifx_out += bytes;
                        }
                    }
                    ana_ind++;
                    if (!COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_BF) ||
                        !COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_BF))
                    {
                        // copy first analysis output directly to subband output if BF is not instantiated or not activated
                        memcpy(subband_out_pt, analysis_out1_pt, subband_byte_size);
                    }
                    if (dPt->num_mics == 2 && input2 &&
                        COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_BF) &&
                        COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_BF))
                    {
                        // process second mic input only if BF is instantiated and activated
                        if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                        {
                            analysis_wloaFFT(analysis_out2_pt, dPt->trans_struct, lPt->state_struct[ana_ind], in2_pt);
                        }
                        ana_ind++;
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_ANALYSIS))
                        {
                            memcpy(ifx_out, analysis_out2_pt, subband_byte_size);
                            ifx_out += subband_byte_size;
                        }
                    }
                    else
                    {
                        analysis_out2_pt = NULL;
                    }
#ifdef ENABLE_IFX_DSES
                    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
                    {/* DSES requires analysis of AEC reference signal */
#ifndef ENABLE_IFX_PRIV_GDE
                        {
                            // copy farend data from AEC reference adjusted by bulk delay estimate if GDE is disabled
                            int bytes = dPt->frame_size * sizeof(int16_t);
                            memcpy(dPt->aec_farend_pt, ref_pt, bytes);
                        }
#endif
                        analysis_wloaFFT(dPt->subband_dses_ref_pt, dPt->trans_struct, lPt->state_struct[ana_ind], dPt->dses_ref_pt);
                        ana_ind++;
                        analysis_wloaFFT(dPt->subband_farend_pt, dPt->trans_struct, lPt->state_struct[ana_ind], dPt->aec_farend_pt);
                        ana_ind++;
                        analysis_wloaFFT(dPt->subband_post_aec_pt, dPt->trans_struct, lPt->state_struct[ana_ind], in1_pt);
                        ana_ind++;
                    }
                    if (ana_ind > dPt->num_mics + EXTRA_ANALYSIS) {
#else
                    if (ana_ind > dPt->num_mics) {
#endif
                        return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_PARAM_RANGE);
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ANALYSIS, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_COMPONENT_BF:
            {/* must two mics and two or more beams */
#ifdef ENABLE_IFX_BF
                if (dPt->num_mics < 2 || input2 == NULL)
                {
                    return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_BF, IFX_SP_ENH_ERR_INVALID_CONFIG);
                }
                if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_BF))
                {
                    bf_settings_struct_t* bf_pt = lPt->settings_struct;
                    int num_beams = bf_pt->num_beams;
                    int aggressiveness = bf_pt->aggressiveness;
                    if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_BF))
                    {
                        ifx_bf_init(lPt, dPt->sampling_rate, dPt->frame_size, dPt->num_mics, dPt->kfft_size, dPt->block_size, num_beams);
                        dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_BF);
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_BF))
                    {
                        ifx_bf_set_weights(lPt, aggressiveness);
                        status |= ifx_bf_process(lPt, subband_out_pt, analysis_out1_pt, analysis_out2_pt);
                    }
                    else
                    {/* BF bypass and only take input1 */
                        memcpy(subband_out_pt, analysis_out1_pt, subband_byte_size);
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_BF))
                    {/* convert to audio samples to monitor output */
                        synthesis_wloaFFT((int16_t*)ifx_out, dPt->trans_struct, dPt->l[dPt->synthesis_proc_ind].state_struct[syn_ind], subband_out_pt);
                        ifx_out += dPt->frame_size * sizeof(int16_t);
                        syn_ind++;
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_BF, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_COMPONENT_DRVB:
            {
#ifdef ENABLE_IFX_DRVB
                if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DRVB))
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_DRVB))
                    {
                        ifx_drvb_init(lPt, &dPt->scratch, true);
                        dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_DRVB);
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_DRVB))
                    {
                        status |= ifx_drvb_process(subband_out_pt, lPt); /* Output overwrite input */
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_DRVB))
                    {/* convert to audio samples to monitor output */
                        synthesis_wloaFFT((int16_t*)ifx_out, dPt->trans_struct, dPt->l[dPt->synthesis_proc_ind].state_struct[syn_ind], subband_out_pt);
                        ifx_out += dPt->frame_size * sizeof(int16_t);
                        syn_ind++;
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DRVB, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_COMPONENT_ES:
            {
#ifdef ENABLE_IFX_ES
#ifdef ENABLE_IFX_DSES
                // check for ns_es_type to run only if both ES and DSES are enabled to make sure at least one
                // echo suppression is executed when only one is enabled but ns_es_type is set incorrectly
                if (((dPt->ns_es_type & IFX_ES_BITS) == IFX_ES && COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ES) &&
                    COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES)) ||
                    !COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
#endif
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ES))
                    {
                        if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_ES))
                        {
                            ifx_es_reset(lPt, dPt->frame_size, dPt->sampling_rate);
                            dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_ES);
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_ES))
                        {
                            int32_t aggressiveness = ((es_settings_struct_t*)lPt->settings_struct)->aggressiveness;
                            ifx_es_set_gain(lPt->state_struct[0], aggressiveness);
                            status |= ifx_es_process(out_pt, lPt->state_struct[0]); /* Output overwrite input */
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_ES))
                        {
                            int bytes = dPt->frame_size * sizeof(int16_t);
                            memcpy(ifx_out, out_pt, bytes);
                            ifx_out += bytes;
                        }
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_ES, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_COMPONENT_DSES:
            {
#ifdef ENABLE_IFX_DSES
#ifdef ENABLE_IFX_ES
                // check for ns_es_type to run only if both ES and DSES are enabled to make sure at least one
                // echo suppression is executed when only one is enabled but ns_es_type is set incorrectly
                if (((dPt->ns_es_type & IFX_ES_BITS) == IFX_DSES && COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ES) &&
                    COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES)) ||
                    !COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_ES))
#endif
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
                    {// DSES
                        if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
                        {
                            ifx_dses_reset(lPt);
                            dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_DSES);
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
                        {
#ifndef DISABLE_IFX_DSES_DSNS_PARALLEL
                            if (dsns_gain_mask)
                            {/* Both DSES & DSNS active and run paralell */
                                status |= ifx_dses_process(subband_out_pt, dPt, lPt, dsns_gain_mask, true); /* Output overwrite subband_out_pt */
                            }
                            else
#endif
                            {
                                status |= ifx_dses_process(subband_out_pt, dPt, lPt, NULL, false); /* Output overwrite subband_out_pt */
                            }
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_DSES))
                        {/* convert to audio samples to monitor output */
                            synthesis_wloaFFT((int16_t*)ifx_out, dPt->trans_struct, dPt->l[dPt->synthesis_proc_ind].state_struct[syn_ind], subband_out_pt);
                            ifx_out += dPt->frame_size * sizeof(int16_t);
                            syn_ind++;
                        }
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSNS, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_COMPONENT_NS:
            {
#ifdef ENABLE_IFX_NS
#ifdef ENABLE_IFX_DSNS
                // check for ns_es_type to run only if both NS and DSNS are enabled to make sure at least one
                // noise suppression is executed when only one is enabled but ns_es_type is set incorrectly
                if (((dPt->ns_es_type & IFX_DSNS) == IFX_NS && COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_NS) &&
                    COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSNS)) ||
                    !COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSNS))
#endif
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_NS))
                    {

                        // traditional NS
                        ns_settings_struct_t* ns_pt = lPt->settings_struct;
                        if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_NS))
                        {
                            ifx_ns_init(&dPt->scratch, lPt, dPt->sampling_rate, dPt->frame_size, dPt->kfft_size, dPt->skip_size, dPt->rfft_size, true);
                            dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_NS);
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_NS))
                        {
                            status |= ifx_ns_process(subband_out_pt, dPt->table_struct, lPt, (float)ns_pt->ns_gain_dB); /* Output overwrite input */
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_NS))
                        {/* convert to audio samples to monitor output */
                            synthesis_wloaFFT((int16_t*)ifx_out, dPt->trans_struct, dPt->l[dPt->synthesis_proc_ind].state_struct[syn_ind], subband_out_pt);
                            ifx_out += dPt->frame_size * sizeof(int16_t);
                            syn_ind++;
                        }
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_NS, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_COMPONENT_DSNS:
            {
#ifdef ENABLE_IFX_DSNS
#ifdef ENABLE_IFX_NS
                // check for ns_es_type to run only if both NS and DSNS are enabled to make sure at least one
                // noise suppression is executed when only one is enabled but ns_es_type is set incorrectly
                if (((dPt->ns_es_type & IFX_DSNS) == IFX_DSNS && COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_NS) &&
                    COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSNS)) ||
                    !COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_NS))
#endif
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_DSNS))
                    {
                        // DSNS
                        dsns_settings_struct_t* dsns_pt = lPt->settings_struct;
                        if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_DSNS))
                        {
                            ifx_dsns_reset(lPt);
                            dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_DSNS);
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_DSNS))
                        {
                            if (dsns_gain_mask)
                            {/* Both DSES & DSNS active and post process will be in DSES */
                                status |= ifx_dsns_process(subband_out_pt, lPt, &(dPt->scratch), dsns_pt->ns_gain_dB, true); /* Output overwrite input */
                            }
                            else
                            {/* Only DSNS active */
                                status |= ifx_dsns_process(subband_out_pt, lPt, &(dPt->scratch), dsns_pt->ns_gain_dB, false); /* Output overwrite input */
                            }
                        }
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_DSNS))
                        {/* convert to audio samples to monitor output */
                            synthesis_wloaFFT((int16_t*)ifx_out, dPt->trans_struct, dPt->l[dPt->synthesis_proc_ind].state_struct[syn_ind], subband_out_pt);
                            ifx_out += dPt->frame_size * sizeof(int16_t);
                            syn_ind++;
                        }
                    }
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_DSNS, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_COMPONENT_SYNTHESIS:
            {
#ifdef ENABLE_IFX_SYNTHESIS
                if (COMPONENT_FLAG_IS_SET(dPt->enable_flag, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS))
                {
                    if (COMPONENT_FLAG_IS_SET(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS))
                    {/* Reset synthesis delay buffer */
                        ifx_anasyn_init(lPt, NULL, dPt->block_size, dPt->overlap_size, true);
                        dPt->reset_flag = COMPONENT_CLEAR_FLAG(dPt->reset_flag, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS);
                    }
                    if (COMPONENT_FLAG_IS_SET(dPt->active_flag, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS))
                    {
                        synthesis_wloaFFT(out_pt, dPt->trans_struct, lPt->state_struct[syn_ind], subband_out_pt);
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS))
                        {
                            int bytes = dPt->frame_size * sizeof(int16_t);
                            memcpy(ifx_out, out_pt, bytes);
                            ifx_out += bytes;
                        }
                    }
                    else
                    {
                        if (COMPONENT_FLAG_IS_SET(dPt->monitor_flag, IFX_SP_ENH_IP_COMPONENT_SYNTHESIS))
                        {/* When it is de-actived, monitor output is spectrogram */
                            memcpy(ifx_out, subband_out_pt, subband_byte_size);
                            ifx_out += subband_byte_size;
                        }
                    }
                    syn_ind++;
                }
#else
                return IFX_SP_ENH_ERROR(IFX_SP_ENH_IP_COMPONENT_SYNTHESIS, IFX_SP_ENH_ERR_INVALID_COMPONENT_ID);
#endif
                break;
            }
            case IFX_SP_ENH_IP_PRIV_COMPONENT_OUTPUT_METER:
            {
                meter_pt++;
                ifx_meter_process(meter_pt, 0, lPt, out_pt);
                break;
            }
            default:
                return IFX_SP_ENH_ERROR(lPt->component_typeb, IFX_SP_ENH_FEATURE_NOT_SUPPORTED);
                break;
            }
            /* Check if status error (e.g. license expired). If error then return output filled with all 0's. */
            if (status != IFX_SP_ENH_SUCCESS)
            {
                memset(out_pt, 0, in_out_byte_size);
                return IFX_SP_ENH_ERROR(lPt->component_typeb, status);
            }
#ifdef PROFILER
            ifx_cycle_profile_stop(&(lPt->profile));
            if (lPt->profile.profile_config & IFX_SP_ENH_PROFILE_FRAME)
            {
                ifx_profile_per_frame_print(lPt->profile);
            }
#endif
        }
    }

#ifdef PROFILER
    ifx_cycle_profile_stop(&(dPt->profile));
    if (dPt->profile.profile_config & IFX_SP_ENH_PROFILE_FRAME)
    {
        ifx_profile_per_frame_print(dPt->profile);
    }
#endif

    return IFX_SP_ENH_SUCCESS;
}
