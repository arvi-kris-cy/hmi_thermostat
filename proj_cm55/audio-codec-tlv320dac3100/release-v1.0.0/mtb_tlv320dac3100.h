/******************************************************************************
* File Name : mtb_tlv320dac3100.h
*
* Description : Header for hardware codec.
*
*******************************************************************************
* Copyright 2024-2025, Cypress Semiconductor Corporation (an Infineon company) or
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

#ifndef __MTB_TLV_CODEC__
#define __MTB_TLV_CODEC__

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus */

#include <stdint.h>

#include "cy_result.h"
#include "mtb_hal_i2c.h"


#define TLV320DAC3100_PAGE_NUM_Pos                      (8)       /**< Register page mask */
#define TLV320DAC3100_REG_ADDR_Pos                      (0)       /**< Register address mask */

#define TLV320DAC3100_PAGE_AND_REG_VAL(page, addr)     \
    (((page) << TLV320DAC3100_PAGE_NUM_Pos) | ((addr) << TLV320DAC3100_REG_ADDR_Pos)) \
    /**< Merging register address and page number used by @ref mtb_tlv320dac3100_write_byte */


/**
 * Enumeration for the TLV320DAC3100 I2C Interface Register Addresses
 *
 */
typedef enum
{
    /* Page 0 */
    TLV320DAC3100_P0_REG_PAGE_CTRL =
        TLV320DAC3100_PAGE_AND_REG_VAL(0, 0),    /**< 0x00: Page Control Register */

    TLV320DAC3100_P0_REG_SW_RST,                 /**< 0x01: Software Reset */
    TLV320DAC3100_P0_REG_CLK_GEN_MUX =
        TLV320DAC3100_PAGE_AND_REG_VAL(0, 4),    /**< 0x04:  Clock-Gen Muxing */

    TLV320DAC3100_P0_REG_CLK_PLL_PR,             /**< 0x05: : PLL P and R Values */
    TLV320DAC3100_P0_REG_CLK_PLL_J,              /**< 0x06: : PLL J-Value */
    TLV320DAC3100_P0_REG_CLK_PLL_D_MSB,          /**< 0x07: : PLL D-Value MSB */
    TLV320DAC3100_P0_REG_NDAC_VAL =
        TLV320DAC3100_PAGE_AND_REG_VAL(0, 11),   /**< 0x0B:  DAC NDAC_VAL */

    TLV320DAC3100_P0_REG_MDAC_VAL,               /**< 0x0C:  DAC MDAC_VAL */
    TLV320DAC3100_P0_REG_DAC_DOSR_MSB,           /**< 0x0D:  DAC DOSR_VAL MSB */
    TLV320DAC3100_P0_REG_DAC_DOSR_LSB,           /**< 0x0E:  DAC DOSR_VAL LSB */
    TLV320DAC3100_P0_REG_CLKOUT_MUX =
        TLV320DAC3100_PAGE_AND_REG_VAL(0, 25),   /**< 0x19: CLKOUT MUX */

    TLV320DAC3100_P0_REG_CLKOUT_M_VAL,           /**< 0x1A:  CLKOUT M_VAL */
    TLV320DAC3100_P0_REG_CODEC_INTERFACE_1,      /**< 0x1B:  Codec Interface Control 1 */
    TLV320DAC3100_P0_REG_BCLK_N_VAL =
        TLV320DAC3100_PAGE_AND_REG_VAL(0, 30),   /**< 0x1E: BCLK N_VAL */

    TLV320DAC3100_P0_REG_CSI1_CTL,               /**< 0x1F: Codec Secondary Interface Control 1 */
    TLV320DAC3100_P0_REG_CSI2_CTL,               /**< 0x20: Codec Secondary Interface Control 2 */
    TLV320DAC3100_P0_REG_CSI3_CTL,               /**< 0x21: Codec Secondary Interface Control 3 */
    TLV320DAC3100_P0_REG_I2C_BUS_CND,            /**< 0x22: I2C Bus Condition */
    TLV320DAC3100_P0_REG_DAC_BSEL =
        TLV320DAC3100_PAGE_AND_REG_VAL(0, 60),   /**< 0x3C:  DAC Processing Block Selection */

    TLV320DAC3100_P0_REG_DAC_DPATH =
        TLV320DAC3100_PAGE_AND_REG_VAL(0, 63),   /**< 0x3F:  DAC Data-Path Setup */

    TLV320DAC3100_P0_REG_DAC_VOL_CTL,           /**< 0x40:  DAC Volume Control */
    TLV320DAC3100_P0_REG_DAC_LVOL_CTL,          /**< 0x41:  DAC Left Volume Control */
    TLV320DAC3100_P0_REG_DAC_RVOL_CTL,          /**< 0x42:  DAC Right Volume Control */
    TLV320DAC3100_P0_REG_VOL_MICDET_CTL =
        TLV320DAC3100_PAGE_AND_REG_VAL(0, 116), /**<  0x74: MICDET-Pin SAR ADC — Volume Control */

    TLV320DAC3100_P0_REG_VOL_MICDET_GAIN,       /**< 0x75:  VOL/MICDET-Pin Gain */

    /* Page 1 */
    TLV320DAC3100_P1_REG_HP_DRV =
        TLV320DAC3100_PAGE_AND_REG_VAL(1, 31),  /**< 0x1F: Headphone Drivers */

    TLV320DAC3100_P1_REG_CLS_D_SPK_AMP,     /**< 0x20:  Class-D Speaker Amplifier */
    TLV320DAC3100_P1_REG_HP_OUT_POP,        /**< 0x21:  HP Output Drivers POP Removal Settings */
    TLV320DAC3100_P1_REG_PGA_PER_CTL,       /**< 0x22:  Driver PGA Ramp-Down Period Control */
    TLV320DAC3100_P1_REG_DAC_LR_OUT_MIX,    /**< 0x23:  DAC_L and DAC_R Output Mixer Routing */
    TLV320DAC3100_P1_REG_HP_LAV,            /**< 0x24:  Left Analog Volume to HPL */
    TLV320DAC3100_P1_REG_HP_RAV,            /**< 0x25:  Right Analog Volume to HPR */
    TLV320DAC3100_P1_REG_SP_LAV,            /**< 0x26:  Left Analog Volume to SPK */
    TLV320DAC3100_P1_REG_SP_RESERVED,       /**< 0x27:  Reserved */
    TLV320DAC3100_P1_REG_HPL_DRV,           /**< 0x28:  HPL Driver */
    TLV320DAC3100_P1_REG_HPR_DRV,           /**< 0x29:  HPR Driver */
    TLV320DAC3100_P1_REG_CLS_D_SPK_DRV,     /**< 0x2A:  Class-D Speaker (SPK) Driver */
    TLV320DAC3100_P1_REG_HP_DRV_CTL =
        TLV320DAC3100_PAGE_AND_REG_VAL(1, 44),    /**< 0x2C:  HP Driver Control */

    /* Page 8 */
    TLV320DAC3100_P8_REG_DAC_COEF_RAM =
        TLV320DAC3100_PAGE_AND_REG_VAL(8, 1)   /**< 0x01:  DAC Coefficient RAM Control */
} mtb_tlv320dac3100_reg_t;

/**
 * Enumeration for supported sample rates for DAC in the TLV320DAC3100 audio codec.
 */
typedef enum
{
    TLV320DAC3100_DAC_SAMPLE_RATE_192_KHZ    = 192000,   /**< 192 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_176_4_KHZ  = 176400,   /**< 176.4 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_96_KHZ     = 96000,    /**< 96 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_88_2_KHZ   = 88200,    /**< 88.2 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_48_KHZ     = 48000,    /**< 48 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_44_1_KHZ   = 44100,    /**< 44.1 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_32_KHZ     = 32000,    /**< 32 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_24_KHZ     = 24000,    /**< 24 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_22_05_KHZ  = 22050,    /**< 22.05 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_16_KHZ     = 16000,    /**< 16 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_12_KHZ     = 12000,    /**< 12 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_11_025_KHZ = 11025,    /**< 11.025 KHz sample rate */
    TLV320DAC3100_DAC_SAMPLE_RATE_8_KHZ      = 8000      /**< 8 KHz sample rate */
} mtb_tlv320dac3100_dac_sample_rate_t;

/**
 * Enumeration for supported I2S word sizes in Right Justified Mode
 * for the TLV320DAC3100 audio codec.
 */
typedef enum
{
    TLV320DAC3100_I2S_WORD_SIZE_16 = 0,   /**< 16 bit word size */
    TLV320DAC3100_I2S_WORD_SIZE_20,       /**< 20 bit word size */
    TLV320DAC3100_I2S_WORD_SIZE_24,       /**< 24 bit word size */
    TLV320DAC3100_I2S_WORD_SIZE_32        /**< 32 bit word size */
} mtb_tlv320dac3100_i2s_word_size_t;

/**
 * \{
 * @name Device address
 * The I2C address of TLV320DAC3100 audio codec according to datasheet
 */
#define MTB_TLV320DAC3100_I2C_ADDR                (0x18u) /**< I2C address of the codec */

/** \} Device address */


/** Initialization failure error */
#define CY_RSLT_TLV320DAC3100_INIT_FAIL \
    (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_BOARD_HARDWARE_TLV320DAC3100, 0))
/** Clock configuration failure error */
#define CY_RSLT_TLV320DAC3100_CLK_CONFIG_FAIL \
    (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_BOARD_HARDWARE_TLV320DAC3100, 1))
/** Invalid parameter error */
#define CY_RSLT_TLV320DAC3100_BAD_PARAM_FAIL \
    (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_BOARD_HARDWARE_TLV320DAC3100, 2))

/**
 * Initialize the I2C communication with the audio codec and do basic configuration of
 * the codec.
 * @param[in] i2c_inst I2C instance to use for communicating with the TLV320DAC3100 audio codec.
 *            The object should be pre-initialized by the user with help of corresponding driver's
 * init function.
 * @return CY_RSLT_SUCCESS if properly initialized, else an error indicating what went wrong.
 */
cy_rslt_t mtb_tlv320dac3100_init(mtb_hal_i2c_t* i2c_inst);

/**
 * Frees up any resources allocated by the codec as part of \ref mtb_tlv320dac3100_init().
 */
void mtb_tlv320dac3100_free(void);

/**
 * This function configure internal clock dividers to achieve desired sample rate.
 *
 * @param[in] mclk_hz          Input MCLK frequency in HZ
 * @param[in] sample_rate      Desired sample rate
 * @param[in] word_length      I2S word length
 * @return CY_RSLT_SUCCESS if properly configured, else an error indicating what went wrong.
 */
cy_rslt_t mtb_tlv320dac3100_configure_clocking(uint32_t mclk_hz,
                                               mtb_tlv320dac3100_dac_sample_rate_t sample_rate,
                                               mtb_tlv320dac3100_i2s_word_size_t word_length);

/**
 * This function updates the volume of both the left and right channels of the
 * speaker output.
 *
 * @param[in] volume - Steps of 0.5dB
 */
void mtb_tlv320dac3100_adjust_speaker_output_volume(uint8_t volume);

/**
 * This function updates the volume of both the left and right channels of the
 * headphone output.
 *
 * @param[in] volume - Steps of 0.5dB
 */
void mtb_tlv320dac3100_adjust_headphone_output_volume(uint8_t volume);

/**
 * Activates the codec - This function is called in conjunction with
 * @ref mtb_tlv320dac3100_deactivate API after successful configuration update of the codec.
 */
void mtb_tlv320dac3100_activate(void);

/**
 * Deactivates the codec - the configuration is retained, just the codec
 * input/outputs are disabled. The function should be called before changing
 * any setting in the codec over I2C
 */
void mtb_tlv320dac3100_deactivate(void);

/**
 * This function writes a data byte to an audio codec register
 *
 * @param[in] reg   The audio codec register to update
 * @param[in] data  The byte to be written to the audio codec register
 */
void mtb_tlv320dac3100_write_byte(mtb_tlv320dac3100_reg_t reg, uint8_t data);

/**
 * This function sets certain bits (defined as mask) in a register. This function can be used
 * instead of @ref mtb_tlv320dac3100_write_byte() if change in a single or multiple selected bits in
 * the register is needed and the value of other bits in the register should not be changed.
 * Only the bits set to 1 in the mask are effected.
 *
 * @param[in] reg   The audio codec register to update
 * @param[in] mask  The mask used to set bits in the register
 */
void mtb_tlv320dac3100_set(mtb_tlv320dac3100_reg_t reg, uint8_t mask);

/**
 * This function clears certain bits (defined as mask) in a register. This function can be used
 * instead of @ref mtb_tlv320dac3100_write_byte() if change in a single or multiple selected bits in
 * the register is needed and the value of other bits in the register should not be changed.
 * Only the bits set to 1 in the mask are effected.
 *
 * @param[in] reg   The audio codec register to update
 * @param[in] mask  The mask used to clear bits in the register
 */
void mtb_tlv320dac3100_clear(mtb_tlv320dac3100_reg_t reg, uint8_t mask);

/**
 * This function reads a data byte from an audio codec register
 *
 * @param[in] reg    The audio codec register read
 * @return data The byte read from the audio codec register
 */
uint8_t mtb_tlv320dac3100_read_byte(mtb_tlv320dac3100_reg_t reg);


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __MTB_TLV_CODEC__ */

/* [] END OF FILE */
