/*******************************************************************************
 * File Name:   retarget_io_init.h
 *
 * Description:  This file is the public interface of retarget_io_init.c and 
 *               contains the necessary UART configuration parameters.
 *
 * Related Document: See README.md
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

#ifndef _RETARGET_IO_INIT_H_
#define _RETARGET_IO_INIT_H_

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cybsp.h"
#include "mtb_hal.h"
#include "cy_retarget_io.h"
#include "mtb_syspm_callbacks.h"

/*******************************************************************************
* Macros
*******************************************************************************/

/* retarget-io deepsleep callback macros */
#define DEBUG_UART_RTS_PORT     (NULL)
#define DEBUG_UART_RTS_PIN      (0U)

/* Default syspm callback configuration elements */
#define SYSPM_SKIP_MODE         (0U)
#define SYSPM_CALLBACK_ORDER    (1U)

<<<<<<< HEAD
=======
#define APP_ERROR(error_code) app_error_handler(error_code, __FILE__, __LINE__)
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97

/*******************************************************************************
* Function prototypes
*******************************************************************************/
void init_retarget_io(void);
<<<<<<< HEAD
=======
void app_error_handler(uint32_t error_code, const char *file, int line);
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97

/*******************************************************************************
* Function Name: handle_app_error
********************************************************************************
* Summary:
* User defined error handling function
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
__STATIC_INLINE void handle_app_error(void)
{
<<<<<<< HEAD
    /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);

    /* Infinite loop */
    while(true);

}

=======
   /* Disable all interrupts. */
   __disable_irq();
   printf("handle_app_error\n");

   /* Optional: allow debug breakpoint */
   // CY_ASSERT(0);

   /* Perform software reset */
   NVIC_SystemReset();
}


>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#endif /* _RETARGET_IO_INIT_H_ */

/* [] END OF FILE */
