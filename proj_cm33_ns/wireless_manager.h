/*
 * wireless_manager.h
 *
 * Description: Header file for managing BLE and Wi-Fi functionalities
 *  Created on: 10-Jun-2025
 *      Author: Infineon
 */

#ifndef WIRELESS_MANAGER_H_
#define WIRELESS_MANAGER_H_

/* Header file includes */
#include "retarget_io_init.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "cycfg_bt_settings.h"
#include "cycfg_gap.h"
#include "cybsp_bt_config.h"
#include "wiced_bt_stack.h"
#include "cycfg_gatt_db.h"
#include "wiced_memory.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cyabs_rtos.h"
#include "cyabs_rtos_impl.h"
#include "cycfg_peripherals.h"
#include <inttypes.h>
#include "cybsp.h"
#include "cy_time.h"
#include "cy_wcm.h"
#include "wifi_task.h"
#include "app_utils.h"

/**
 * @brief This will init the wifi & BLE module
 *
 * @return API response
 */
 cy_rslt_t wirelessdevice_init(void);

// Function prototypes for Wi-Fi functionality

/**
 * @brief This will init the wifi module
 *
 * @return API response
 */
 cy_rslt_t wifi_init(void);

/**
 * @brief Connect to a Wi-Fi network that was received from the BLE network.
 *
 * @return status of the connection
 */
cy_rslt_t wifi_connect();

/**
 * @brief Disconnect from the current Wi-Fi network.
 *
 * This function disconnects from any Wi-Fi network the device is currently
 * connected to.
 *
 * @return API response
 */
cy_rslt_t wifi_disconnect(void);

/**
 * @brief Get the current Wi-Fi connection status.
 *
 * @return The current status of the Wi-Fi connection (0 = disconnected, 1 = connected).
 */
bool wifi_get_status(void);

/**
 * @brief Get the Wi-Fi MAC Address.
 */
void wifi_get_macaddr(uint8_t *mac);

/**
 * @brief Delete Wi-Fi credential from NVM flash.
 */
void delete_wifi_credential(void);


// Function prototypes for BLE functionality

/**
 * @brief This will init the BLE module
 *
 * @return API response
 */
cy_rslt_t ble_init(void);

/**
 * @brief Disconnect from the current BLE device.
 *
 * This function terminates the BLE connection with the currently connected device.
 */
void ble_disconnect(void);

/**
 * @brief Start BLE advertising to allow other devices to discover this device.
 *
 * This function enables BLE advertising so that other Bluetooth devices can
 * detect and initiate a connection.
 *
 * @param mode of the advertising
 *
 *
 * @return API response
 */
wiced_result_t ble_start_advertising(wiced_bt_ble_advert_mode_t mode);

/**
 * @brief Stop BLE advertising.
 *
 * This function stops BLE advertising and makes the device undetectable by other devices.
 */
void ble_stop_advertising(void);


extern TaskHandle_t wifi_task_handle;
extern uint8_t ble_name[MAX_LEN_GAP_DEVICE_NAME+1];


#endif /* WIRELESS_MANAGER_H_ */
