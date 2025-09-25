/*******************************************************************************
 * File Name:  app_speaker.c
 *
 * Description: Speaker control module implementation.
 *
 * This file implements the speaker control functions including initialization,
 * playback handling, and volume control using the I2S interface and
 * TLV320DAC3100 codec.
 *
 *
 *******************************************************************************
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
*******************************************************************************/

/*******************************************************************************
 *                                INCLUDES
 ******************************************************************************/
#include "app_speaker.h"
#include "error_wav.h"
#include "success_wav.h"

/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/

/*******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/

/*******************************************************************************
 *                             STATIC VARIABLES
 ******************************************************************************/
static bool speaker_init_state = false;
static bool is_speaker_off = false;

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

void app_speaker_init(void)
{
    /* I2S initialization */
    app_i2s_init();

    /* TLV codec initialization */
    app_tlv_codec_init();

    /* Update state */
    speaker_init_state = true;
}

void app_speaker_play(audio_type_t type)
{
    if ((false == is_speaker_off) && (true == speaker_init_state))
    {
        switch(type)
        {
            case AUDIO_ERROR:
                /* Pointer to the audio data */
                wave_data = (uint16_t*)&error_wav[0];
                wave_data_size =  error_wav_size;
                break;

            case AUDIO_SUCCESS:
                /* Pointer to the audio data */
                wave_data = (uint16_t*)&success_wav[0];
                wave_data_size =  success_wav_size;
                break;
        }

        /* Enable I2S interface and clear I2S buffers */
        app_i2s_enable();

        /* Activate the I2S transmission */
        app_i2s_activate();
    }
}

void app_speaker_clear(void)
{
    /* Check if speaker initialized */
    if (true == speaker_init_state)
    {
        /* Check if I2S buffer transmission complete */
        if (audio_playback_ended)
        {
            /* Clear flag */
            audio_playback_ended = false;

            /* Clear I2S buffer, deactivate and disable I2S */
            app_i2s_deactivate();
        }
    }
}

void app_speaker_audio_lvl_ctrl(audio_lvl_t level)
{
    is_speaker_off = (level > 0) ? false : true;

    /* Check if speaker initialized */
    if (true == speaker_init_state)
    {
        /* Update the speaker output volume */
        mtb_tlv320dac3100_adjust_speaker_output_volume(level);
    }
}

/* [] END OF FILE */
