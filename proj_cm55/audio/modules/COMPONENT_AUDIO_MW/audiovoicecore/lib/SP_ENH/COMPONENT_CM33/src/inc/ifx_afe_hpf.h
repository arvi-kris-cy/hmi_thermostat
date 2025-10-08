/*
 * Copyright 2025, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
 *
 */

#ifndef HPF_FX_H
#define HPF_FX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//
// Initialization of 4th-order Butterworth HPF implemented by two stages
// of Direct Form II biquads.
//
// Run hpf_design/design_hpf.m to generate coefficients corresponding
// to given sample rate and cutoff frequency.
//

#if 1
// cutoff = 60 Hz
#define HPF_A11_FX_8kHz (-32152)    // = (((int16_t)(-1.962403775249819f*16384))); // Q14
#define HPF_A12_FX_8kHz (31607)     // = (((int16_t)(0.964584709925638f*32768))); // Q15
#define HPF_A21_FX_8kHz (-31366)    // = (((int16_t)(-1.914461090592186f*16384))); // Q14
#define HPF_A22_FX_8kHz (30034)     // = (((int16_t)(0.916588743744223f*32768))); // Q15
#define HPF_GAIN_FX_8kHz (30811)    // = (((int16_t)(0.940280536598276f*32768))); // Q15
#define HPF_A11_FX_16kHz (-32466)   // = (((int16_t)(-1.981579078857877f*16384))); // Q14
#define HPF_A12_FX_16kHz (32182)    // = (((int16_t)(0.982129258043927f*32768))); // Q15
#define HPF_A21_FX_16kHz (-32061)   // = (((int16_t)(-1.956851282961522f*16384))); // Q14
#define HPF_A22_FX_16kHz (31371)    // = (((int16_t)(0.957394596552974f*32768))); // Q15
#define HPF_GAIN_FX_16kHz (31774)   // = (((int16_t)(0.969683064082197f*32768))); // Q15
#else
// cutoff = 120 Hz
#define HPF_A11_FX_8kHz (-31488)    // = (((int16_t)(-1.921908893578822f*16384))); // Q14
#define HPF_A12_FX_8kHz (30489)     // = (((int16_t)(0.930476416247046f*32768))); // Q15
#define HPF_A21_FX_8kHz (-30013)    // = (((int16_t)(-1.831853863092364f*16384))); // Q14
#define HPF_A22_FX_8kHz (27525)     // = (((int16_t)(0.840019936702488f*32768))); // Q15
#define HPF_GAIN_FX_8kHz (28969)    // = (((int16_t)(0.884092042866511f*32768))); // Q15
#define HPF_A11_FX_16kHz (-32152)   // = (((int16_t)(-1.962403775249819f*16384))); // Q14
#define HPF_A12_FX_16kHz (31607)    // = (((int16_t)(0.964584709925638f*32768))); // Q15
#define HPF_A21_FX_16kHz (-31366)   // = (((int16_t)(-1.914461090592186f*16384))); // Q14
#define HPF_A22_FX_16kHz (30034)    // = (((int16_t)(0.916588743744223f*32768))); // Q15
#define HPF_GAIN_FX_16kHz (30811)   // = (((int16_t)(0.940280536598276f*32768))); // Q15
#endif

// Q safety shift for overlow protection during filtering
#define HPF_QS_8kHz (7)
#define HPF_QS_16kHz (7)

typedef struct 
{
    int16_t af[2*2];   /* filter coefficients */
    int16_t gainf;
    int16_t qs;
} ifx_afe_hpf_coeff_struct_t;

typedef struct
{
    int32_t wf[2][3];   /* filter states of one mic */
} ifx_afe_hpf_state_struct_t;

typedef struct
{
    ifx_afe_hpf_coeff_struct_t coeff;
    ifx_afe_hpf_state_struct_t state[2];   /* filter states of two mics */
} ifx_afe_hpf_struct_t;

void afe_hpf(int16_t* inbuf, int16_t* outbuf, uint32_t frame_size, ifx_afe_hpf_coeff_struct_t coef_mem, ifx_afe_hpf_state_struct_t* state_mem);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif // HPF_FX_H
