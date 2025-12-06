/*******************************************************************************
 * File Name:   app_speaker.h
 *
 * Description:  Public interface for speaker control.
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

#ifndef APP_INCLUDE_APP_AUDIO_H_
#define APP_INCLUDE_APP_AUDIO_H_

/*******************************************************************************
 *                                INCLUDES
 *******************************************************************************/
#include "app_i2s/app_i2s.h"

/*******************************************************************************
 *                                MACROS
 *******************************************************************************/

/*******************************************************************************
 *                                CONSTANTS
 *******************************************************************************/

/*******************************************************************************
 *                                DATA TYPES
 *******************************************************************************/
typedef enum {
	AUDIO_LVL_OFF = 0x00,
	AUDIO_LVL_LOW = 0x49,		//100-27=73
	AUDIO_LVL_MED = 0x64,		//127-27=100
	AUDIO_LVL_HIGH = 0x7F,		//127
} audio_lvl_t;

typedef enum {
    AUDIO_SUCCESS,
    AUDIO_ERROR
} audio_type_t;

/*******************************************************************************
 *                                GLOBAL VARIABLES
 *******************************************************************************/

/*******************************************************************************
 *                                FUNCTION PROTOTYPES
 *******************************************************************************/
/**
 * @brief Initialize the speaker module.
 *
 * This function initializes the I2S interface and the TLV audio codec.
 */
void app_speaker_init(void);

/**
 * @brief Start speaker playback.
 *
 * This function enables and activates the I2S interface to begin audio transmission.
 */
void app_speaker_play(audio_type_t type);

/**
 * @brief Clear speaker buffer and deactivate transmission.
 *
 * This function deactivates the I2S interface and resets internal flags, if
 * audio transmission is completed.
 */
void app_speaker_clear(void);

/**
 * @brief Control speaker output level.
 *
 * @param level Audio level to set. If level is 0, the speaker will be turned off.
 *
 * This function adjusts the speaker volume if the speaker has been initialized.
 */
void app_speaker_audio_lvl_ctrl(audio_lvl_t level);


#endif /* APP_INCLUDE_APP_AUDIO_H_ */

/* [] END OF FILE */
