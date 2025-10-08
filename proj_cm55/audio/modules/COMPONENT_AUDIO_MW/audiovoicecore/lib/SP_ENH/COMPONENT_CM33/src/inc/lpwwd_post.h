/***************************************************************************************
* File: lpwwd_post.h
*
* Purpose:  Contains files for Low Power Wake Word Detection Post Processing
*
* External Functions:
*
*   void fixed_reset_lpwwd_post(struct PPHMM_struct *pphmmmem)
*   int fixed_init_lpwwd_post(struct PPHMM_struct *pphmmmem, int16_t *ppmodel)
*   int fixed_lpwwd_post(struct PPHMM_struct *pphmmmem, int16_t *feature)
*
*
* Infineon Technologies, 2022
* Robert Zopf
****************************************************************************************/
#ifndef LPWWD_POST_H
#define LPWWD_POST_H

#ifdef __cplusplus
extern "C" {
#endif

#define DATA_SCALE 8 // these are not used
#define DATA_Q 12 // scaled by 16, not used in the code yet

#define MAX_Q   10
#define MAX_D   4
#define MAX_MIX 2
#define SIL_Q   3

#define Q_LOOKBACK  12 

//#define KW_LOOKBACK 0.5 //seconds
//#define SOD_LOOKBACK 0.15 //seconds
#define NOISE_TIMEOUT_SAFETY 1.0 //2.0 // seconds
#define NSMOOTH 15
#define NSMOOTH_POW2 4  //2^ must be < NSMOOTH

//#define FIXED_KW_LOOKBACK  ((int16_t)(KW_LOOKBACK*4096+0.5))  // Q12 
//#define FIXED_SOD_LOOKBACK ((int16_t)(SOD_LOOKBACK*4096+0.5)) // Q12
#define FIXED_NOISE_TIMEOUT_SAFETY ((int16_t)(NOISE_TIMEOUT_SAFETY*(1l<<Q_LOOKBACK)+0.5)) // Q12

#define Q_LOGLL     16
#define MAX_REJECT_FRAMES  100  // need to define this better


#define REJECT_MODEL 0

struct FIXED_PPHMM_struct
{
    int32_t alpha1[MAX_Q];
    int   Q;
    int   Dim;
    int   n;
    int   minKWL;
    int   maxKWL;
    int   Qend;
    int   noiseTO;
    int   kwTO;
    int   End;
    int   ActiveState;
    int   ResetCheck;
    int   PCcount;
    int   maxstartfrm;
    int16_t mintoken;
    int16_t maxstart;
    int16_t Qterm;
    int16_t Qsigma;
    int32_t max;
    int32_t llstart;
    int16_t TH;
    int16_t stateBank[MAX_Q*(MAX_D * 2 + 1+1)*MAX_MIX];
    int16_t transP[MAX_Q * 2];
};

struct FIXED_PPNHMM_struct
{
    int32_t alpha1[SIL_Q];
    int   Q;
    int   Dim;
    int16_t stateBank[SIL_Q*(MAX_D * 2 + 1 + 1)*MAX_MIX];
    int16_t transP[SIL_Q * 2 + 1];
    int16_t Qterm;
    int16_t Qsigma;
};

struct FIXED_SMOOTH_struct
{
    int16_t buf[NSMOOTH - 1][MAX_D];
    int32_t sum[MAX_D];
    int     ndx;
};

struct FIXED_PP_struct
{
    int   frame;
    int   method;
    int   nMixPos;
    int   nMixRej;
    int   stateI[MAX_Q];
    int   nMixSil;
    int   startfail;
    int   maxKWtokenIdx;
    struct FIXED_PPHMM_struct kw_model;
    struct FIXED_PPHMM_struct g_model;
    struct FIXED_PPNHMM_struct n_model;
    struct FIXED_PPHMM_struct reject_model;
    int16_t reject_th[MAX_REJECT_FRAMES][MAX_Q];
    int16_t nth;
    struct FIXED_SMOOTH_struct smooth;
    int32_t ll;   // final PP score = ((float)ll)/normll
    int16_t normll;
};


#if defined(__cplusplus)
}
#endif

#endif // LPWWD_POST_H