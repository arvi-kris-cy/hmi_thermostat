/*******************************************************************************
 * File Name:   app_ui_receiver.h
 *
 * Description:  Public interface for the UI receiver module.
 *
 * This file contains the headers and structures to support the UI receiver
 * task, which handles events and messages from various sources, including the
 * inter-processor communication (IPC) pipe.
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

#ifndef PROJ_CM33_NS_APP_INCLUDE_APP_UI_RECEIVER_H_
#define PROJ_CM33_NS_APP_INCLUDE_APP_UI_RECEIVER_H_

/*******************************************************************************
 *                              INCLUDES
 *******************************************************************************/
#include "app_main.h"

/*******************************************************************************
 *                              MACROS
 *******************************************************************************/

/*******************************************************************************
 *                              CONSTANTS
 *******************************************************************************/

/*******************************************************************************
 *                              DATA TYPES
 *******************************************************************************/

/*******************************************************************************
 *                              GLOBAL VARIABLES
 *******************************************************************************/
extern QueueHandle_t xUiRxQueue;
extern TaskHandle_t cm33_ui_rx_task_handle;

/*******************************************************************************
 *                              FUNCTION PROTOTYPES
 *******************************************************************************/
/**
 * @brief Initializes and starts the UI receiver thread.
 *
 * This function creates the FreeRTOS task responsible for handling events and
 * messages for the user interface.
 */
void ui_rx_thread_init(void);
void get_latet_FW_version(void);
#endif /* PROJ_CM33_NS_APP_INCLUDE_APP_UI_RECEIVER_H_ */

/* [] END OF FILE */
