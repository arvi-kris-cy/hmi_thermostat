/******************************************************************************
* File Name: cy_fe.h
*
* Description: Feature extraction module header file with APIs and data structures.
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
* Include guard
*******************************************************************************/
#ifndef __CY_FE_H
#define __CY_FE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "cy_errors.h"
#include "ifx_pre_post_process.h"

extern int16_t* input_block;

typedef struct
{
    int component_id;           /**< pre or post process compnent ID */
    int sampling_rate;          /**< audio and voice data sampling rate in Hz */
    int input_frame_size;       /**< input data frame size in samples */
    int frame_shift;            /**< input data frame shift in samples */
    int output_size;            /**< spreech enhancement application output size in its data type */
    mem_info_t memory;          /**< memory structure contains info required for process */
    int libsp_version;          /**< version number of Infineon pre and post process IP library */
    int configurator_version;   /**< Configurator version number */
} cy_fe_config_prms;

typedef struct
{
    ifx_stc_pre_post_process_info_t fe_info;
    int block_size;
    int audio_frame_size;
    int num_features;
    void* fe_handle;
} cy_fe_handle;

typedef struct
{
    int sampling_rate; // Sampling rate
    uint8_t component_id;
    int frame_size; // feature extraction audio frame size
    int frame_shift;
    int number_of_filter_banks;
    int number_of_dct_coefficients;
    int audio_frame_size; // incoming audio frame size
} cy_fe_config_params_t;

cy_rslt_t cy_fe_init(cy_fe_config_params_t* config_params, cy_fe_handle **handle);
cy_rslt_t cy_fe_update_audio_buffer(cy_fe_handle* handle, int16_t* input_data);
cy_rslt_t cy_fe_reset_audio_buffer(cy_fe_handle* handle);
#ifdef RUN_FLOAT_FE
cy_rslt_t cy_fe_process(cy_fe_handle* handle, float* output_data);
#else
cy_rslt_t cy_fe_process(cy_fe_handle* handle, IFX_FE_DATA_TYPE_T* output_data, int32_t* out_q);
#endif
cy_rslt_t cy_fe_deinit(cy_fe_handle *handle);

#if defined(__cplusplus)
}
#endif

#endif /*__CY_FE_H */

/* [] END OF FILE */
