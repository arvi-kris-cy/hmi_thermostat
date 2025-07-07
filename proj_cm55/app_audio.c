/*
 * app_audio.c
 *
 */

/*******************************************************************************
 *                                Includes
 *******************************************************************************/
#include "app_audio.h"

/*******************************************************************************
 *                                Global Variable
 *******************************************************************************/

/*******************************************************************************
 *                                Static Variable
 *******************************************************************************/
static bool speaker_init_state = false;
static bool is_speaker_off = false;

/*******************************************************************************
 *                                Function Definitions
 *******************************************************************************/
void app_speaker_init(void)
{
    /* I2S initialization */
    app_i2s_init();

    /* TLV codec initiailization */
    app_tlv_codec_init();

    speaker_init_state = true;
}

void app_speaker_play(void)
{
	if((false == is_speaker_off) && (true == speaker_init_state))
	{
	    app_i2s_enable();
	    app_i2s_activate();
	}
}

void app_speaker_clear(void)
{
	if(true == speaker_init_state)
	{
	    if(audio_playback_ended)
	    {
	        audio_playback_ended = false;
	        app_i2s_deactivate();
	    }
	}
}

void app_speaker_audio_lvl_ctrl(audio_lvl_t level)
{
	is_speaker_off = (level > 0) ? false:true;

	if(true == speaker_init_state)
	{
		mtb_tlv320dac3100_adjust_speaker_output_volume(level);
	}
}
