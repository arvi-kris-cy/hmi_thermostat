/******************************************************************************
* File Name : mtb_tlv320dac3100.c
*
* Description : Source code for TLV320DAC3100 hardware codec.
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

/******************************************************************************
* Header Files
*******************************************************************************/

#include <stdbool.h>

#include "mtb_tlv320dac3100.h"

/******************************************************************************
* Macros
*******************************************************************************/
#define _TLV320DAC3100_I2S_MCTRL_WRD_SIZE_POS     (4)

/******************************************************************************
* Global Variables
*******************************************************************************/
static mtb_hal_i2c_t* _mtb_dac3100_i2c_ptr = NULL;

uint8_t ndac_div = 1;
uint8_t mdac_div = 1;
uint16_t dosr_div = 2;


/******************************************************************************
* Function Name: mtb_tlv320dac3100_init
*******************************************************************************
* Summary:
* Initialize the Audio codec.
*
*
*******************************************************************************/

cy_rslt_t mtb_tlv320dac3100_init(mtb_hal_i2c_t* i2c_inst)
{
    if (i2c_inst == NULL)
    {
        return CY_RSLT_TLV320DAC3100_INIT_FAIL;
    }

    _mtb_dac3100_i2c_ptr = i2c_inst;

    return CY_RSLT_SUCCESS;
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_free
*******************************************************************************
* Summary:
* Free the resources used with the Audio codec.
*
*
*******************************************************************************/

void mtb_tlv320dac3100_free(void)
{
    _mtb_dac3100_i2c_ptr = NULL;
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_configure_clocking
*******************************************************************************
* Summary:
* This function configure internal clock dividers to achieve desired sample rate
*
*
*******************************************************************************/

cy_rslt_t mtb_tlv320dac3100_configure_clocking(uint32_t mclk_hz,
                                               mtb_tlv320dac3100_dac_sample_rate_t sample_rate,
                                               mtb_tlv320dac3100_i2s_word_size_t word_length)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_PAGE_CTRL, 0);
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_SW_RST, 0x01);

    // Clock settings
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_CLK_GEN_MUX, 0x00);   //pll_clkin=MCLK,
                                                                            // codec_CLKIN=MCLK
    // mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_CLK_PLL_J, 0x05);   //J =5 //8
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_CLK_PLL_D_MSB, 0x00);   //0x0000 16 bit, D=0
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_CLK_PLL_PR, 0x13);   //Power down PLL, P=1, R=3

    /* Set sample rate for the codec
        The next formula used for DAC_Fs calculation:

                        CLK_IN
        DAC_Fs = --------------------
                   NDAC x MDAC x DOSR

        The maximum frequencies for the main clocks:
        - DAC_CLK:     <= 49.152 MHz  (DAC_CLK is getting from NDAC divider)
        - DAC_MOD_CLK:  = 6.758  MHz  (DAC_MOD_CLK is getting from MDAC divider)
     */
    const uint32_t MAX_DAC_CLK_FREQ_HZ = 49152000;
    const uint32_t MAX_DAC_MOD_CLK_FREQ_HZ = 6200000;
    // uint8_t ndac_div = 1;
    // uint8_t mdac_div = 1;
    // uint16_t dosr_div = 2;
    uint32_t mdac_freq, ndac_freq, dac_fs;

    ndac_freq = mclk_hz;
    while ((ndac_freq / ndac_div) > MAX_DAC_CLK_FREQ_HZ)
    {
        ndac_div++;
    }

    /* Set MDAC divider
     * DOSR is limited in its range by the following condition:
     * 2.8 MHz < DOSR × DAC_Fs < 6.2 MHz
     * So the frequency after MDAC divider should be in this range
     */
    mdac_freq = (ndac_freq / ndac_div);
    while ((mdac_freq / mdac_div) > MAX_DAC_MOD_CLK_FREQ_HZ)
    {
        mdac_div++;
    }
    /* Set DOSR divider to achieve desierd sample rate */
    dac_fs = (mdac_freq / mdac_div);
    while ((dac_fs / dosr_div) > (uint32_t)sample_rate)
    {
        dosr_div++;
    }

    // The datasheet recommends to use even DOSR divider.
    // If the DOSR divider is odd, decrease it by 1.
    if (dosr_div % 2 != 0)
    {
        dosr_div--;
    }

    // // NDAC and MDAC power up
    // mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_NDAC_VAL, 0x80 | ndac_div);
    // mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_MDAC_VAL, 0x80 | mdac_div);
    // Set DOSR MSB and LSB values
    // DOSR is a 10-bit register, so get only 2 MSB from the divider value
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_DOSR_MSB, ((dosr_div >> 8) & 0x3));
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_DOSR_LSB, (dosr_div & 0xFF));

    // I2S interface, BLCK input, WCLK input
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_CODEC_INTERFACE_1,
                                 (word_length << _TLV320DAC3100_I2S_MCTRL_WRD_SIZE_POS));

    // mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_NDAC_VAL, 0x81);   //NDAC power up =3
    // mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_MDAC_VAL, 0x81);   //MDAC power up =5
    // mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_DOSR_MSB, 0x00);
    // mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_DOSR_LSB, 0x80);    // 0x0080 16 bit =128
    // mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_CODEC_INTERFACE_1, 0x00); //  I2S interface, wordlength 16, bit, BLCK input, WCLK input

    //Processing blocks -> PRB_P17
     mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_BSEL, 17u); //Processing Block 17 selected

    //mtb_tlv320dac3100_write_byte(PAGE_SELECT, 0x08);
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P8_REG_DAC_COEF_RAM, 0x04); // adaptive filtering
                                                                           // enabled

    //volume control through pin disable
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_VOL_MICDET_CTL, 0x00); // Volume control
                                                                             // through register on

    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HP_DRV, 0x04); // Headphone output Power Down
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HP_OUT_POP, 0x4E); // Driver-poweron-time=
                                                                         // 1.22s Driver ramp up
                                                                         // time= 3.9ms
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_DAC_LR_OUT_MIX, 0x44); // DAC_L to left
                                                                             //  channel mixer and
                                                                             // DAC_R to Right
                                                                             // channel mixer
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HPL_DRV, 0x06); // HPL unmute, 0dB, gain not
                                                                      // applied
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HPR_DRV, 0x06); // HPR unmute, 0dB, gain not
                                                                      // applied
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_CLS_D_SPK_DRV, 0x1D); // Output gain 24dB,
                                                                            // Class-D driver
                                                                            // unmute, gain not
                                                                            // applied

    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HP_DRV, 0xC4); // Headphone output Power up

    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_CLS_D_SPK_AMP, 0x86);    // Speaker output
                                                                               // Power up,
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HP_LAV, 0x92); // Left Channel analog routed
                                                                     // to output HPL = 18 (0-127)
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HP_RAV, 0x92); // Right Channel analog routed
                                                                     // to output HPR = 18 (0-127)
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_SP_LAV, 0x92); //Left Channel analog routed to
                                                                     // output SPL = 18 (0-127)
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_DPATH, 0xD4); // Power up Left and Right
                                                                        // channel
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_LVOL_CTL, 0xFD);  // DAC Left volume gain =
                                                                            // -22dB
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_RVOL_CTL, 0xFD); // DAC Right volume gain
                                                                           // = -22dB
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_VOL_CTL, 0x00); // unmute the Left-Right
                                                                          // channel

    return result;
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_write_byte
*******************************************************************************
* Summary:
* This function writes a data byte to an audio codec register
*
*
*******************************************************************************/

void mtb_tlv320dac3100_write_byte(mtb_tlv320dac3100_reg_t reg, uint8_t data)
{
    CY_ASSERT(NULL != _mtb_dac3100_i2c_ptr);

    cy_rslt_t rslt;

    uint8_t  page[] =
        { TLV320DAC3100_P0_REG_PAGE_CTRL, (reg >> TLV320DAC3100_PAGE_NUM_Pos) & 0xFF };
    rslt =
        mtb_hal_i2c_controller_write(_mtb_dac3100_i2c_ptr, MTB_TLV320DAC3100_I2C_ADDR, page, sizeof(page),
                               0,
                               true);

    if (CY_RSLT_SUCCESS == rslt)
    {
        uint8_t buf[] = { (reg & 0xFF), data };
        rslt =
            mtb_hal_i2c_controller_write(_mtb_dac3100_i2c_ptr, MTB_TLV320DAC3100_I2C_ADDR, buf,
                                   sizeof(buf),
                                   0,
                                   true);
    }
    CY_UNUSED_PARAMETER(rslt); // CY_ASSERT only processes in DEBUG, ignores for others
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_read_byte
*******************************************************************************
* Summary:
* This function reads a data byte from an audio codec register
*
*
*******************************************************************************/

uint8_t mtb_tlv320dac3100_read_byte(mtb_tlv320dac3100_reg_t reg)
{
    CY_ASSERT(NULL != _mtb_dac3100_i2c_ptr);

    cy_rslt_t rslt;
    uint8_t   data = 0;

    uint8_t  page[] =
        { TLV320DAC3100_P0_REG_PAGE_CTRL, (reg >> TLV320DAC3100_PAGE_NUM_Pos) & 0xFF };
    rslt =
        mtb_hal_i2c_controller_write(_mtb_dac3100_i2c_ptr, MTB_TLV320DAC3100_I2C_ADDR, page, sizeof(page),
                               0,
                               true);

    if (CY_RSLT_SUCCESS == rslt)
    {
        uint8_t buf[] = { (reg & 0xFF), data };
        rslt =
            mtb_hal_i2c_controller_write(_mtb_dac3100_i2c_ptr, MTB_TLV320DAC3100_I2C_ADDR, buf,
                                   sizeof(buf),
                                   0,
                                   true);
    }

    CY_UNUSED_PARAMETER(rslt); // CY_ASSERT only processes in DEBUG, ignores for others
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);

    rslt =
        mtb_hal_i2c_controller_read(_mtb_dac3100_i2c_ptr, MTB_TLV320DAC3100_I2C_ADDR, &data, 1, 0, true);
    CY_UNUSED_PARAMETER(rslt); // CY_ASSERT only processes in DEBUG, ignores for others
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);

    return data;
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_set
*******************************************************************************
* Summary:
* This function sets bits in a register.
*
*
*******************************************************************************/

void mtb_tlv320dac3100_set(mtb_tlv320dac3100_reg_t reg, uint8_t mask)
{
    uint8_t data = mtb_tlv320dac3100_read_byte(reg) | mask;
    mtb_tlv320dac3100_write_byte(reg, data);
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_clear
*******************************************************************************
* Summary:
* This function clears bits in a register.
*
*
*******************************************************************************/
void mtb_tlv320dac3100_clear(mtb_tlv320dac3100_reg_t reg, uint8_t mask)
{
    uint8_t data = mtb_tlv320dac3100_read_byte(reg) & ~mask;
    mtb_tlv320dac3100_write_byte(reg, data);
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_adjust_speaker_output_volume
*******************************************************************************
* Summary:
* This function updates the volume of both the left and right channels of the speaker output.
*
*
*******************************************************************************/

void mtb_tlv320dac3100_adjust_speaker_output_volume(uint8_t volume)
{
    uint8_t out = volume|0x80;

    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_LVOL_CTL, out);  //DAC Left volume gain =
                                                                               // -22dB 0 -127
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_DAC_RVOL_CTL, out); // DAC Right volume gain
                                                                              // = -22dB
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_adjust_headphone_output_volume
*******************************************************************************
* Summary:
* This function updates the volume of both the left and right channels of the headphone output.
*
*
*******************************************************************************/
void mtb_tlv320dac3100_adjust_headphone_output_volume(uint8_t volume)
{
    uint8_t out = volume | 0x80;
    // Left Channel analog routed to output HPL (0-127)
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HP_LAV, out);
    // Right Channel analog routed to output HPR (0-127)
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P1_REG_HP_RAV, out);
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_activate
*******************************************************************************
* Summary:
* Activates the codec - This function is called in conjunction with
* tlv320dac3100_deactivate API after successful configuration update of codec.
*
*
*******************************************************************************/

void mtb_tlv320dac3100_activate(void)
{
    // NDAC and MDAC power up
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_NDAC_VAL, 0x80 | ndac_div);
    mtb_tlv320dac3100_write_byte(TLV320DAC3100_P0_REG_MDAC_VAL, 0x80 | mdac_div);
}

/******************************************************************************
* Function Name: mtb_tlv320dac3100_deactivate
*******************************************************************************
* Summary:
* Deactivates the codec - the configuration is retained, just the codec
* input/outputs are disabled. The function should be called before changing
* any setting in the codec over I2C.
*
*
*******************************************************************************/
void mtb_tlv320dac3100_deactivate(void)
{
    // NDAC and MDAC power down
    mtb_tlv320dac3100_clear(TLV320DAC3100_P0_REG_NDAC_VAL, 0x80);
    mtb_tlv320dac3100_clear(TLV320DAC3100_P0_REG_MDAC_VAL, 0x80);
}

/* [] END OF FILE */
