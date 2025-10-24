#ifndef __MFCC_H__
#define __MFCC_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

/* Please note this header file should not be used (included) unless floating-point feature extraction function is needed */

uint16_t mel_features_init(uint16_t feature_option, uint16_t sample_rate, uint16_t frame_size, uint16_t frame_shift, uint16_t num_fbank_bins, uint16_t num_mfcc_coeffs);
void mel_features_free();
void mel_features_compute(const int16_t *data, float *features);

#if defined(__cplusplus)
}
#endif

#endif // __MFCC_H__