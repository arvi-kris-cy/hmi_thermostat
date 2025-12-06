/******************************************************************************
* File Name: wifi_task.h
*
* Description: This is the header file for wifi_task.c. It contains macros,
* enums and structures used by the functions in wifi_task.c. It also contains
* function prototypes and externs of global variables that can be used by
* other files
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

#ifndef __WIFI_TASK_H__
#define __WIFI_TASK_H__

#include <FreeRTOS.h>
#include <task.h>
#include "cybsp.h"
#include "stdio.h"
#include "cy_wcm.h"
#include "wifi_task.h"
#include "cycfg_gatt_db.h"
#include "wiced_bt_stack.h"
#include "app_utils.h"
#include "cycfg_gap.h"
#include "wiced_memory.h"
#include "retarget_io_init.h"
#include "mqtt_task.h"
#include "wireless_manager.h"
#include "comm_manager.h"

/******************************************************************************
 * Constants
 ******************************************************************************/
/* Task parameters for WiFi tasks */
#define WIFI_TASK_STACK_SIZE              (2048U)
#define WIFI_TASK_PRIORITY                (2U)

/* Macros defining the packet type */
#define DATA_PACKET_TYPE_SSID             (1U)
#define DATA_PACKET_TYPE_PASSWORD         (2U)

/* Macros defining packet type for scan data */
#define SCAN_PACKET_TYPE_SSID             (1U)
#define SCAN_PACKET_TYPE_SECURITY         (2U)
#define SCAN_PACKET_TYPE_RSSI             (3U)

/* Macros defining the commands for WiFi control point characteristic */
#define WIFI_CONTROL_DISCONNECT           (0U)
#define WIFI_CONTROL_CONNECT              (1U)
#define WIFI_CONTROL_SCAN                 (2U)
#define COMM_SWITCH_TO_WIFI               (3U)

#define APP_RRAM_NVM_MAIN_NS_START        0x22000000
/* Task notification value to indicate whether to use
 * WiFi credentials from NVM or from GATT DB
 */
enum wifi_task_notifications
{
    NOTIF_SCAN               = 0x0001,
    NOTIF_CONNECT            = 0x0002,
    NOTIF_DISCONNECT         = 0x0004,
    NOTIF_ERASE_DATA         = 0x0008,
    NOTIF_GATT_NOTIFICATION  = 0x0010
};

/******************************************************************************
 * Structures
 ******************************************************************************/
/* Structure to store WiFi details that goes into NVM */
typedef __PACKED_STRUCT
{
    uint8_t wifi_ssid[CY_WCM_MAX_SSID_LEN];
    uint8_t ssid_len;
    uint8_t wifi_password[CY_WCM_MAX_PASSPHRASE_LEN];
    uint8_t password_len;
}wifi_details_t;

/******************************************************************************
 * Extern Variables
 ******************************************************************************/
extern wifi_details_t wifi_details;

extern cy_wcm_connect_params_t wifi_conn_param;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
cy_rslt_t wcm_init(void);

void wifi_task(void * arg);

/**
 * @brief Connect to a Wi-Fi network that was received from the BLE network.
 *
 * @return status of the connection
 */
cy_rslt_t wifi_connect(void);

void ble_notify_switch_to_wifi(void);

#endif      /* __WIFI_TASK_H__ */


/* [] END OF FILE */
