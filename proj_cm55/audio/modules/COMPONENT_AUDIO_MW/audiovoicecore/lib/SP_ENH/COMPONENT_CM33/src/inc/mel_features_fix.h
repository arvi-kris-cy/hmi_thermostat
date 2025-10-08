#ifndef __MFCC_H__
#define __MFCC_H__

#include "arm_math.h"
#include "ifx_sp_common_priv.h"

#define DANYMIC 1

#define LPS_Q           (9)             /* Log Power Spectrum Q */
#if DANYMIC
#define LOG_MEL_FLOOR   ((-18) * (1 << LPS_Q) + 192)  /* -17.625 for minimum ln power value */
#else
#define LOG_MEL_FLOOR   ((-16) * (1 << LPS_Q))        /* -16 for minimum ln power value */
#endif
#define DCT_COEF_Q      (13)            /* DCT coefficent Q */
#define MEL_LOG_16BIT_Q (LPS_Q)
#define MFCC_16BIT_Q    (DCT_COEF_Q + LPS_Q - 16)  /* 6 */

typedef struct ifx_spectrogram_struc_t
{
    int16_t* window_func_fix;
    uint16_t* mel_fbank_fix;
    int16_t* dct_matrix_fix;
    arm_rfft_instance_q15 rfft_q15;
    uint16_t *fbank_filter_first;
    uint8_t *fbank_filter_size;
}ifx_spectrogram_struc_t;

#endif // __MFCC_H__
