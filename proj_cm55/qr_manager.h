<<<<<<< HEAD
#ifndef QR_MANAGER_H
#define QR_MANAGER_H

#include "lvgl.h"

// QR Manager structure
typedef struct {
    lv_obj_t * qr_obj;      // QR code LVGL object
    lv_obj_t * parent;      // Parent container
    uint16_t size;          // QR code size
} qr_manager_t;

// Initialize QR Manager (create QR object)
void qr_manager_init(qr_manager_t * manager, lv_obj_t * parent_container, uint16_t size);

// Update QR Code data
void qr_manager_update(qr_manager_t * manager, const char * data);

// Deinitialize QR Manager (optional, only if you want to clean up)
void qr_manager_deinit(qr_manager_t * manager);

#endif
=======
/*******************************************************************************
 * File Name:   comm_manager.h
 *
 * Description:  Public interface for updating thermostat events from
 *               CM55 core to CM33 core using IPC.
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

#ifndef QR_MANAGER_H
#define QR_MANAGER_H

/*******************************************************************************
 *                                INCLUDES
 *******************************************************************************/
#include "lvgl.h"

/*******************************************************************************
 *                                MACROS
 *******************************************************************************/

/*******************************************************************************
 *                                CONSTANTS
 *******************************************************************************/

/*******************************************************************************
 *                                DATA TYPES
 *******************************************************************************/
/* QR Manager structure */
typedef struct {
    lv_obj_t * qr_obj;      /* QR code LVGL object */
    lv_obj_t * parent;      /* Parent container */
    uint16_t size;          /* QR code size */
} qr_manager_t;

/*******************************************************************************
 *                                GLOBAL VARIABLES
 *******************************************************************************/


/*******************************************************************************
 *                                FUNCTION PROTOTYPES
 *******************************************************************************/

/**
 * @brief Initializes the QR manager and creates a QR code object.
 *
 * This function initializes the QR manager structure, creates a QR code
 * object inside the given parent container, and configures its size and colors.
 *
 * @param manager          Pointer to the QR manager structure.
 * @param parent_container Parent LVGL container where the QR code will be placed.
 * @param size             Size (width and height) of the QR code in pixels.
 */
void qr_manager_init(qr_manager_t * manager, lv_obj_t * parent_container, uint16_t size);

/**
 * @brief Updates the QR code with new data.
 *
 * This function updates the QR code object with the specified string data.
 *
 * @param manager Pointer to the QR manager structure.
 * @param data    Null-terminated string to encode in the QR code.
 */
void qr_manager_update(qr_manager_t * manager, const char * data);

/**
 * @brief Deinitializes the QR manager and deletes the QR code object.
 *
 * This function removes the QR code object from the screen and clears
 * the reference in the QR manager structure.
 *
 * @param manager Pointer to the QR manager structure.
 */
void qr_manager_deinit(qr_manager_t * manager);

#endif /* QR_MANAGER_H */

/* [] END OF FILE */
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
