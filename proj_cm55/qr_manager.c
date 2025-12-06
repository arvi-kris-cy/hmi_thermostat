<<<<<<< HEAD
=======
/*******************************************************************************
 * File Name:  qr_manager.c
 *
 * Description:
 *
 * This file implements the functions to create and update the QR code on UI
 * using LVGL library.
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
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
#include "qr_manager.h"
#include <stdio.h>
#include <string.h>

<<<<<<< HEAD
=======
/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/

/*******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/

/*******************************************************************************
 *                             STATIC VARIABLES
 ******************************************************************************/

/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
void qr_manager_init(qr_manager_t * manager, lv_obj_t * parent_container, uint16_t size)
{
    if (!manager) return;

    manager->parent = parent_container;
    manager->size = size;

    manager->qr_obj = lv_qrcode_create(manager->parent);
    if (!manager->qr_obj)
    {
        printf("QR Manager: Failed to create QR object\n");
        return;
    }

    lv_qrcode_set_size(manager->qr_obj, size);
<<<<<<< HEAD
    lv_qrcode_set_dark_color(manager->qr_obj, lv_color_white());  // White modules
    lv_qrcode_set_light_color(manager->qr_obj, lv_color_black()); // Black background

    // Optional: center inside parent
=======
    lv_qrcode_set_dark_color(manager->qr_obj, lv_color_white());
    lv_qrcode_set_light_color(manager->qr_obj, lv_color_black());

>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
    lv_obj_center(manager->qr_obj);
}

void qr_manager_update(qr_manager_t * manager, const char * data)
{
    if (!manager || !manager->qr_obj) return;

    lv_result_t res = lv_qrcode_update(manager->qr_obj, data, strlen(data));
    if (res != LV_RESULT_OK)
    {
        printf("QR Manager: Failed to update QR code\n");
    }
<<<<<<< HEAD

//    lv_obj_set_pos(manager->qr_obj, 6, -28);
=======
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
}

void qr_manager_deinit(qr_manager_t * manager)
{
    if (!manager || !manager->qr_obj) return;

    lv_obj_del(manager->qr_obj);
    manager->qr_obj = NULL;
}
<<<<<<< HEAD
=======


/* [] END OF FILE */
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
