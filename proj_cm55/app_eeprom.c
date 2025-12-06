<<<<<<< HEAD
/*
 * app_eeprom.c
 *
 *  Created on: 07-Jul-2025
 *      Author: Tejas.Patel
 */

#include "app_eeprom.h"



/*******************************************************************************
* Global Variables
*******************************************************************************/
/* EEPROM configuration and context structure. */
cy_stc_eeprom_config_t em_eeprom_config =
{
    .eepromSize         = EEPROM_SIZE,
    .simpleMode         = SIMPLE_MODE,
    .wearLevelingFactor = WEAR_LEVELLING_FACTOR,
    .redundantCopy      = REDUNDANT_COPY,
    .blockingWrite      = BLOCKING_WRITE,
};


cy_stc_eeprom_context_t em_eeprom_context;

#if ((EMULATED_EEPROM_NVM) == (NVM_REGION_TO_USE))
CY_SECTION(".cy_em_eeprom") \
        CY_ALIGN(CY_EM_EEPROM_FLASH_SIZEOF_ROW) \
        uint8_t eeprom_storage[CY_EM_EEPROM_GET_PHYSICAL_SIZE(EEPROM_SIZE, SIMPLE_MODE, WEAR_LEVELLING_FACTOR, REDUNDANT_COPY)] = {0U};
#else
uint8_t *eeprom_storage = (uint8_t *)CY_RRAM_ADDRESS;
#endif /* #if(NVM_REGION_TO_USE) */


/* RAM arrays for holding EEPROM read and write data respectively. */
uint8_t eeprom_read_array[LOGICAL_EEPROM_SIZE];
uint8_t eeprom_write_array[LOGICAL_EEPROM_SIZE] = {'P', 'o', 'w', 'e', 'r', '0', '0'};


bool eeprom_wr_setting = false;

/*******************************************************************************
* Function Name: process_error
********************************************************************************
* Summary:
*  This function processes unrecoverable errors such as any component
*  initialization errors etc. In case of such error USER LED1 remains ON
*  followed by the error message displayed on the debug terminal. It asserts
*  and halt the processor in the debug state.
*
* Parameters:
*  status: Contains the status.
*  message: Contains the message to be printed on the serial terminal.
*
* Return:
*  void
*
*******************************************************************************/
static void process_error(uint32_t status, char *message)
{
    if (status)
    {
        if (MTB_EM_EEPROM_REDUNDANT_COPY_USED != status)
        {
            if (NULL != message)
            {
                printf("%s", message);
            }

//            CY_ASSERT(0);
        }
        else
        {
            printf("%s","Main copy is corrupted. Redundant copy in Emulated EEPROM is used \r\n");
        }
    }
}

void app_eeprom_init(void)
{

    /* Initialize the flash start address in EEPROM configuration structure. */
    em_eeprom_config.userFlashStartAddr = (uint32_t)eeprom_storage;

    cy_rslt_t result = Cy_Em_EEPROM_Init(&em_eeprom_config, &em_eeprom_context);
    process_error(result, "Emulated EEPROM initialization failed.\r\n");
}

uint32_t app_eeprom_write(device_settings_t *settings) {

	cy_rslt_t result;
	static uint8_t retries = 0;

	result = Cy_Em_EEPROM_Write(LOGICAL_EEPROM_START,
								settings,
								sizeof(device_settings_t),
								&em_eeprom_context);
    process_error(result, "Emulated EEPROM write failed.\r\n");

    if(result == 0)
    {
    	eeprom_wr_setting = false;
    	retries = 0;
    }

    retries++;

    /* Retry attempts */
    if(retries == 5)
    {
    	eeprom_wr_setting = false;
    	retries = 0;
    }

	return result;
}

uint32_t app_eeprom_read(device_settings_t *settings) {
	 cy_rslt_t result;

	    result = Cy_Em_EEPROM_Read(LOGICAL_EEPROM_START,
	                               settings,
	                               sizeof(device_settings_t),
	                               &em_eeprom_context);
	    process_error(result, "Emulated EEPROM read failed.\r\n" );

	    return result;
}

=======
/*******************************************************************************
 * File Name:  app_eeprom.c
 *
 * Description: Emulated EEPROM module implementation.
 *
 * This file implements the Emulated EEPROM initialization, write
 * and read operation.
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
#include "app_eeprom.h"

/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/

/*******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/
/* EEPROM configuration and context structure. */
cy_stc_eeprom_config_t em_eeprom_config =
{
        .eepromSize = EEPROM_SIZE,
        .simpleMode = SIMPLE_MODE,
        .wearLevelingFactor =
        WEAR_LEVELLING_FACTOR,
        .redundantCopy = REDUNDANT_COPY,
        .blockingWrite = BLOCKING_WRITE,
};

/* EEPROM context handler */
cy_stc_eeprom_context_t em_eeprom_context;

/* Storage address  */
uint8_t *eeprom_storage = (uint8_t*) CY_RRAM_ADDRESS;

/* Flag indicating if EEPROM write operation need to be performed */
bool eeprom_wr_setting = false;

/*******************************************************************************
 *                             STATIC VARIABLES
 ******************************************************************************/

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

uint32_t app_eeprom_init(void)
{
    /* Initialize the flash start address in EEPROM configuration structure. */
    em_eeprom_config.userFlashStartAddr = (uint32_t) eeprom_storage;

    /* Initialize the emulated EEPROM */
    cy_en_em_eeprom_status_t result = Cy_Em_EEPROM_Init(&em_eeprom_config, &em_eeprom_context);
    if (CY_EM_EEPROM_SUCCESS != result)
    {
        LOG_ERROR(CYLF_DEF, "EEPROM initialization error.\n");
    }
    else
    {
        LOG_INFO(CYLF_DEF, "Emulated EEPROM initialization Ok.\n");
    }

    return result;
}

uint32_t app_eeprom_write(device_settings_t *settings)
{
    cy_en_em_eeprom_status_t result = MTB_EM_EEPROM_BAD_PARAM;

    /* Write device settings at given address. */
    result = Cy_Em_EEPROM_Write(LOGICAL_EEPROM_START, settings, sizeof(device_settings_t), &em_eeprom_context);
    if (CY_EM_EEPROM_SUCCESS != result)
    {
        LOG_ERROR(CYLF_DEF, "EEPROM write failed. Error code : %ld\n", result);
    }
    else
    {
        LOG_DEBUG(CYLF_DEF, "EEPROM write success\n");
    }

    return result;
}

uint32_t app_eeprom_read(device_settings_t *settings)
{
    cy_en_em_eeprom_status_t result = MTB_EM_EEPROM_BAD_PARAM;

    /* Read device settings at given address. */
    result = Cy_Em_EEPROM_Read(LOGICAL_EEPROM_START, settings, sizeof(device_settings_t), &em_eeprom_context);
    if (CY_EM_EEPROM_SUCCESS != result)
    {
        LOG_ERROR(CYLF_DEF, "EEPROM read failed. Error code : %ld\n", result);
    }
    else
    {
        LOG_DEBUG(CYLF_DEF, "EEPROM read success\n");
    }

    return result;
}

/* [] END OF FILE */
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
