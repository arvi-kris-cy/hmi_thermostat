/*******************************************************************************
 * File Name:   wifi_task.c
 *
 * Description: This file contains the task definition for initializing the
 * Wi-Fi device, connecting to the AP, disconnecting from AP, scanning and 
 * EEPROM related functionality.
 *
 * Related Document: See Readme.md
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

#include "wifi_task.h"

#define SCAN_DATA_HEADER                            (2U)
#define MASK_8BITS                                  0x000000ff
#define MASK_16BITS                                 0x0000ff00
#define MASK_24BITS                                 0x00ff0000
#define MASK_32BITS                                 0xff000000
#define SHIFT_8BITS                                  (8U)
#define SHIFT_16BITS                                 (16U)
#define SHIFT_24BITS                                 (24U)
#define APP_SDIO_INTERRUPT_PRIORITY                  (7U)
#define APP_HOST_WAKE_INTERRUPT_PRIORITY             (2U)
#define APP_SDIO_FREQUENCY_HZ                        (25000000U)
#define SDHC_SDIO_64BYTES_BLOCK                      (64U)
#define RRAM_NVM_DATA_NS_OFFSET  0x0002A000

/******************************************************************************
 * Global Variables
 ******************************************************************************/
/* WCM related structures */
cy_wcm_connect_params_t wifi_conn_param;

/* Variable to be used to store the WiFi credentials which can be copied to
 * NVRAM
 */
wifi_details_t wifi_details;

bool is_wifi_connection_cancel = false;

bool is_wifi_connected = false;
/******************************************************************************
 * Extern Functions and Variables
 ******************************************************************************/
/* Maintains the connection id of the current connection */
extern uint16_t conn_id;

extern QueueHandle_t mqtt_task_q;

extern bool update_credential;
/* Task Handles for WiFi Task */
extern TaskHandle_t wifi_task_handle;
static mtb_hal_sdio_t sdio_instance;
static cy_stc_sd_host_context_t sdhc_host_context;
static cy_wcm_config_t wcm_config;

#define CHECK_WIFI_CONNECTION_NEED_TO_CANCEL 		\
		if(is_wifi_connection_cancel){				\
			is_wifi_connection_cancel = false;		\
			if (cy_wcm_is_connected_to_ap() == 1){	\
			    LOG_INFO(CYLF_DEF, "Disconnecting Wifi.....\n");\
				cy_wcm_disconnect_ap();				\
			}										\
			continue;								\
		}											\

/* Interrupt config structure */

/*******************************************************************************
* Function Declaration
*******************************************************************************/
static void scan_callback(cy_wcm_scan_result_t *result_ptr, void *user_data,
                   cy_wcm_scan_status_t status);

static void start_wifi_scanning(void *notifiedvalue, cy_wcm_scan_filter_t *filter);
/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name: sdio_interrupt_handler
********************************************************************************
* Summary:
* Interrupt handler function for SDIO instance.
*******************************************************************************/
static void sdio_interrupt_handler(void)
{
    mtb_hal_sdio_process_interrupt(&sdio_instance);
}

/*******************************************************************************
* Function Name: host_wake_interrupt_handler
********************************************************************************
* Summary:
* Interrupt handler function for the host wake up input pin.
*******************************************************************************/
static void host_wake_interrupt_handler(void)
{
    mtb_hal_gpio_process_interrupt(&wcm_config.wifi_host_wake_pin);
}

static void wifi_event_callback(cy_wcm_event_t event, cy_wcm_event_data_t *event_data)
{
	switch (event)
	{
	    case CY_WCM_EVENT_CONNECTING:
	    	if(cy_wcm_is_connected_to_ap() == 0)
	    	{
	    		update_conn_state(DEV_ST_WIFI_CONNECTING);
	    		LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: Connecting to AP...\n");
	    	}
	        break;

	    case CY_WCM_EVENT_CONNECTED:
	        LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: Connected to AP.\n");
	        break;

	    case CY_WCM_EVENT_CONNECT_FAILED:
	        LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: Connection to AP failed.\n");
	        break;

	    case CY_WCM_EVENT_RECONNECTED:
	        LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: Reconnected to AP.\n");
	        break;

	    case CY_WCM_EVENT_DISCONNECTED:
	        LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: Disconnected from AP.\n");
	        break;

	    case CY_WCM_EVENT_IP_CHANGED:
	        LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: IP address changed.\n");
	        break;

	    case CY_WCM_EVENT_INITIATED_RETRY:
	        LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: Retrying connection to AP...\n");
	        break;

	    case CY_WCM_EVENT_STA_JOINED_SOFTAP:
	        LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: A station joined the SoftAP.\n");
	        break;

	    case CY_WCM_EVENT_STA_LEFT_SOFTAP:
	        LOG_INFO(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: A station left the SoftAP.\n");
	        break;

	    default:
	        LOG_ERROR(CYLF_DEF, "<<<<<<<<<<<<<WIFI>>>>>>>>>>>>>: Unknown WCM event: %d\n", event);
	        break;
	}
}
/*******************************************************************************
* Function Name: app_sdio_init
********************************************************************************
* Summary:
* This function configures and initializes the SDIO instance used in
* communication between the host MCU and the wireless device.
*******************************************************************************/
static void app_sdio_init(void)
{
    cy_rslt_t result;
    mtb_hal_sdio_cfg_t sdio_hal_cfg;
    cy_stc_sysint_t sdio_intr_cfg =
    {
        .intrSrc = CYBSP_WIFI_SDIO_IRQ,
        .intrPriority = APP_SDIO_INTERRUPT_PRIORITY
    };

    cy_stc_sysint_t host_wake_intr_cfg =
    {
            .intrSrc = CYBSP_WIFI_HOST_WAKE_IRQ,
            .intrPriority = APP_HOST_WAKE_INTERRUPT_PRIORITY
    };

    /* Initialize the SDIO interrupt and specify the interrupt handler. */
    cy_en_sysint_status_t interrupt_init_status = Cy_SysInt_Init(&sdio_intr_cfg, sdio_interrupt_handler);

    /* SDIO interrupt initialization failed. Stop program execution. */
    if(CY_SYSINT_SUCCESS != interrupt_init_status)
    {
//        handle_app_error();
        APP_ERROR(interrupt_init_status);
    }

    /* Enable NVIC interrupt. */
    NVIC_EnableIRQ(CYBSP_WIFI_SDIO_IRQ);

    /* Setup SDIO using the HAL object and desired configuration */
    result = mtb_hal_sdio_setup(&sdio_instance, &CYBSP_WIFI_SDIO_sdio_hal_config, NULL, &sdhc_host_context);

    /* SDIO setup failed. Stop program execution. */
    if(CY_RSLT_SUCCESS != result)
    {
//        handle_app_error();
        APP_ERROR(result);
    }

    /* Initialize and Enable SD HOST */
    Cy_SD_Host_Enable(CYBSP_WIFI_SDIO_HW);
    Cy_SD_Host_Init(CYBSP_WIFI_SDIO_HW, CYBSP_WIFI_SDIO_sdio_hal_config.host_config, &sdhc_host_context);
    Cy_SD_Host_SetHostBusWidth(CYBSP_WIFI_SDIO_HW, CY_SD_HOST_BUS_WIDTH_4_BIT);

    sdio_hal_cfg.frequencyhal_hz = APP_SDIO_FREQUENCY_HZ;
    sdio_hal_cfg.block_size = SDHC_SDIO_64BYTES_BLOCK;

    /* Configure SDIO */
    mtb_hal_sdio_configure(&sdio_instance, &sdio_hal_cfg);

    /* Setup GPIO using the HAL object for WIFI WL REG ON  */
    mtb_hal_gpio_setup(&wcm_config.wifi_wl_pin, CYBSP_WIFI_WL_REG_ON_PORT_NUM, CYBSP_WIFI_WL_REG_ON_PIN);

    /* Setup GPIO using the HAL object for WIFI HOST WAKE PIN  */
    mtb_hal_gpio_setup(&wcm_config.wifi_host_wake_pin, CYBSP_WIFI_HOST_WAKE_PORT_NUM, CYBSP_WIFI_HOST_WAKE_PIN);

    /* Initialize the Host wakeup interrupt and specify the interrupt handler. */
    cy_en_sysint_status_t interrupt_init_status_host_wake =  Cy_SysInt_Init(&host_wake_intr_cfg, host_wake_interrupt_handler);

    /* Host wake up interrupt initialization failed. Stop program execution. */
    if(CY_SYSINT_SUCCESS != interrupt_init_status_host_wake)
    {
//        handle_app_error();
        APP_ERROR(interrupt_init_status_host_wake);
    }

    /* Enable NVIC interrupt. */
    NVIC_EnableIRQ(CYBSP_WIFI_HOST_WAKE_IRQ);
}

cy_rslt_t wcm_init()
{
	cy_rslt_t result = CY_RSLT_SUCCESS;

	app_sdio_init();

    /* Initialize wcm */
    wcm_config.interface = CY_WCM_INTERFACE_TYPE_STA;
    wcm_config.wifi_interface_instance = &sdio_instance;

    /* Initialize WCM */
    printf("\n");
    result = cy_wcm_init(&wcm_config);
    printf("\n");
    if (CY_RSLT_SUCCESS != result)
    {
//        handle_app_error();
        APP_ERROR(result);
    }

    result = cy_wcm_register_event_callback(&wifi_event_callback);
    if (CY_RSLT_SUCCESS != result)
    {
//        handle_app_error();
        APP_ERROR(result);
    }

    return result;
}

/*******************************************************************************
* Function Name: wifi_task
********************************************************************************
* Summary:
* This function initializes the WCM module and handles task notifications
*
* Parameters
* void *
*
* Return
* void
*******************************************************************************/
void wifi_task(void * arg)
{
    cy_rslt_t result ;

    /* Variable to use for filtering WiFi scan results */
    cy_wcm_scan_filter_t scan_filter;

    /* Notification values received from other tasks */
    uint32_t ulNotifiedValue;

    /* Return status for NVM. */
    cy_rslt_t nvm_result ;

    /* WCM configuration and IP address variables */
    cy_wcm_ip_address_t ip_address;

    result = wirelessdevice_init();
	if(CY_RSLT_SUCCESS != result)
	{
	    LOG_ERROR(CYLF_DEF, "Wireless module initialization failed, Error Code: %u\n", result);
//		handle_app_error();
        APP_ERROR(result);
	}

    while(true)
    {
        /* Wait for a notification */
        xTaskNotifyWait( 0x00,      /* Don't clear any notification bits on
                                       entry. */
                         UINT32_MAX, /* Reset the notification value to 0 on
                                        exit. */
                         &ulNotifiedValue, /* Notified value pass out in
                                              ulNotifiedValue. */
                         portMAX_DELAY );  /* Block indefinitely. */

        /* Check if the notification value has the scan bit set. In some cases both
         * the scan bit and connect bit are set. In both the cases we start with
         * the scan, pass the notification value to the scan callback and if
         * the connect bit is also set then the scan callback will pass the
         * connect message to this task and this task will try and connect to
         * the WiFi network. If the notification value passed to the scan
         * callback contains only the scan bit then it will try and send
         * GATT notifications to the BLE client
         */
        if((cy_wcm_is_connected_to_ap() == 0) && (NOTIF_SCAN & ulNotifiedValue))
        {
            memset(&scan_filter, 0, sizeof(cy_wcm_scan_filter_t));

            /* If the Connect bit is set then we need to only scan for networks
             * with the given SSID. Set the correct filter values and start scan
             */
            if(NOTIF_CONNECT & ulNotifiedValue)
            {
                /* Configure the scan filter for SSID */
                scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_SSID;
                memcpy((char *)scan_filter.param.SSID,(char *)wifi_conn_param.ap_credentials.SSID,
                strlen((char *)wifi_conn_param.ap_credentials.SSID) + 1);
                LOG_INFO(CYLF_DEF, "Starting scan with SSID: %s\n", scan_filter.param.SSID);

                start_wifi_scanning(&ulNotifiedValue, &scan_filter);
            }
            /* Else start scan without any filter */
            else
            {
                start_wifi_scanning(&ulNotifiedValue, NULL);
            }
        }

        /* Check for connection notification */
        else if((cy_wcm_is_connected_to_ap() == 0) && (NOTIF_CONNECT == ulNotifiedValue))
        {
            /* Variable to track the number of connection retries to the Wi-Fi
             * AP
             */
            uint8_t conn_retries = 0;

            /* Join the Wi-Fi AP */
            result = CY_RSLT_TYPE_ERROR;

            while((CY_RSLT_SUCCESS != result)
                    && ((get_retry_state() == WIFI_CONN_RETRY_INFINITE) || (conn_retries++ < MAX_CONNECTION_RETRIES))
					&& (!is_wifi_connection_cancel))
            {
                LOG_INFO(CYLF_DEF, "Trying to connect SSID: %s, Password: %s\n",
                        wifi_conn_param.ap_credentials.SSID,
                        wifi_conn_param.ap_credentials.password);

                LOG_INFO(CYLF_DEF, "Starting scan with SSID: %s\n", scan_filter.param.SSID);
                start_wifi_scanning(&ulNotifiedValue, &scan_filter);

                /* Connect to the given AP */
                result = cy_wcm_connect_ap(&wifi_conn_param, &ip_address);
            }

            if(CY_RSLT_SUCCESS == result)
            {
                CHECK_WIFI_CONNECTION_NEED_TO_CANCEL;

                display_ble_pin(0);
                LOG_INFO(CYLF_DEF, "Successfully joined the Wi-Fi network\n");

                /* Store WiFi credentials in NVM */
                /* Write data to NVM. */
                nvm_result =  Cy_RRAM_WriteByteArray(RRAMC0,
                        APP_RRAM_NVM_MAIN_NS_START + RRAM_NVM_DATA_NS_OFFSET ,
                        &wifi_details.wifi_ssid[0],sizeof(wifi_details) );
                if (CY_RRAM_SUCCESS != nvm_result)
                {
                    LOG_ERROR(CYLF_DEF, "Failed to write to NVM \n");
                }
                else
                {
                    LOG_INFO(CYLF_DEF, "Stored WiFi credentials in NVM.\n");
                }

                handle_connectivity_state(STATE_WIFI_CONNECTED);
            	update_conn_state(DEV_ST_WIFI_CONNECTED);
				vTaskDelay(2000);

                /* Update GATT DB about connection */
                app_custom_service_wifi_control[0] = WIFI_CONTROL_CONNECT;

                /* Check if the connection is active and  notifications are
                 * enabled and then send the notification
                 */
                if ((conn_id) &&
                   ((app_custom_service_wifi_control_client_char_config[0] &
                     GATT_CLIENT_CONFIG_NOTIFICATION)))
                {
                    uint8_t *p_attr = (uint8_t*)app_custom_service_wifi_control;

                    /* Wait if GATT congestion flag enabled. */
                    while (is_gatt_congested);

                    wiced_bt_gatt_server_send_notification(conn_id,
                                    HDLC_CUSTOM_SERVICE_WIFI_CONTROL_VALUE,
                                    sizeof(app_custom_service_wifi_control[0]),
                                    p_attr, NULL);

                    /* Wait for GATT congestion to clear */
                    while (is_gatt_congested);
                }
                else /* Notification not sent */
                {
                    LOG_INFO(CYLF_DEF, "Notification not sent\n");
                }

                mqtt_task_cmd_t mqtt_task_cmd = HANDLE_CONNECT;
                xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
            }
            else /* WiFi connection failed */
            {
                CHECK_WIFI_CONNECTION_NEED_TO_CANCEL;

                /* Update GATT DB about unsuccessful connection */
                app_custom_service_wifi_control[0] = WIFI_CONTROL_DISCONNECT;

            	handle_connectivity_state(STATE_WIFI_DISCONNECTED);

                /* Send notification for unsuccessful connection */
                /* Check if the connection is active and notifications are
                 * enabled and then send the notification
                 */
                if ((conn_id) &&
                    ((app_custom_service_wifi_control_client_char_config[0] &
                      GATT_CLIENT_CONFIG_NOTIFICATION)))
                {
                    uint8_t *p_attr = (uint8_t*)app_custom_service_wifi_control;

                    /* Wait if GATT congestion flag enabled. */
                    while (is_gatt_congested);

                    wiced_bt_gatt_server_send_notification(conn_id,
                                     HDLC_CUSTOM_SERVICE_WIFI_CONTROL_VALUE,
                                     sizeof(app_custom_service_wifi_control[0]),
                                     p_attr, NULL);

                    /* Wait for GATT congestion to clear */
                    while (is_gatt_congested);
                }
                else /* Notification not sent */
                {
                    LOG_INFO(CYLF_DEF, "Notification not sent\n");
                }

                LOG_INFO(CYLF_DEF, "Failed to join Wi-Fi network\n");

                /* Check if BLE connected, update the screen to BLE */
                if(conn_id != false)
                {
                    LOG_INFO(CYLF_DEF, "WiFi not connected. Update UI to BLE conn.\n");
                    update_conn_state(DEV_ST_BLE_CONNECTED);
                }
                else
                {
                    LOG_INFO(CYLF_DEF, "WiFi &  BLE not connected. Update UI to BLE ADV.\n");
                    handle_connectivity_state(STATE_BLE_ADV);
                }
            }
        }
        /* Task notification for disconnection */
        else if(NOTIF_DISCONNECT & ulNotifiedValue)
        {
            /* Check if the erase bit is also set in the notification. If yes
             * then erase the data from NVM
             */
            if(NOTIF_ERASE_DATA & ulNotifiedValue)
            {
                LOG_INFO(CYLF_DEF, "Deleting Wi-Fi data from NVM\n");

                /* Set the data to 0*/
                memset(&wifi_details, 0, sizeof(wifi_details));

                nvm_result =  Cy_RRAM_WriteByteArray(RRAMC0, APP_RRAM_NVM_MAIN_NS_START + RRAM_NVM_DATA_NS_OFFSET , &wifi_details.wifi_ssid[0],sizeof(wifi_details));
                if (nvm_result)
                {
                    LOG_ERROR(CYLF_DEF, "Failed to write to NVM\n");
                }
            }

            LOG_INFO(CYLF_DEF, "Disconnecting Wi-Fi\n");
            result = cy_wcm_disconnect_ap();
            if(CY_RSLT_SUCCESS == result)
            {
                /* Update GATT DB about disconnection */
                app_custom_service_wifi_control[0] = WIFI_CONTROL_DISCONNECT;

                LOG_INFO(CYLF_DEF, "Successfully disconnected from AP\n");

                if(NOTIF_ERASE_DATA & ulNotifiedValue)
                {
                	handle_connectivity_state(STATE_UNPROVISIONED);
                    vTaskDelay(1000);

                    update_conn_state(DEV_ST_WIFI_DISCONNECTED);
                    vTaskDelay(2000);

                    update_conn_state(DEV_ST_UNPROVISIONED);
                    vTaskDelay(2000);
                }
                else
                {
                	handle_connectivity_state(STATE_WIFI_DISCONNECTED);
                }
            }
            else /* Disconnection failed */
            {
                LOG_INFO(CYLF_DEF, "Failed to disconnect\n");
            }

            if(update_credential)
            {
                xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT), eSetValueWithOverwrite);
            	update_credential = false;
            }
        }
        else if(cy_wcm_is_connected_to_ap() == true)
        {
            LOG_INFO(CYLF_DEF, "Wi-Fi already connected\n");
            if(get_mqtt_status() & FLAG_MQTT_CONNECTION_SUCCESS)
            {
                update_conn_state(DEV_ST_CLOUD_CONNECTED);
            }
        }
    }
}

cy_rslt_t wifi_connect(void)
{
	cy_wcm_ip_address_t ip_address;
	cy_rslt_t result = CY_RSLT_TYPE_ERROR;
	uint8_t conn_retries = 0;
    uint32_t ulNotifiedValue = 0;
    cy_wcm_scan_filter_t scan_filter;

    scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_SSID;
    memcpy((char *)scan_filter.param.SSID,(char *)wifi_conn_param.ap_credentials.SSID,
    strlen((char *)wifi_conn_param.ap_credentials.SSID) + 1);
    LOG_INFO(CYLF_DEF, "Starting scan with SSID: %s\n", scan_filter.param.SSID);


    while((CY_RSLT_SUCCESS != result)
            && ((get_retry_state() == WIFI_CONN_RETRY_INFINITE) || (conn_retries++ < MAX_CONNECTION_RETRIES))
			&& (!is_wifi_connection_cancel))
    {
        start_wifi_scanning(&ulNotifiedValue, &scan_filter);

        vTaskDelay(100);

        LOG_INFO(CYLF_DEF, "Trying to connect SSID: %s, Password: %s\n",
                wifi_conn_param.ap_credentials.SSID,
                wifi_conn_param.ap_credentials.password);

        /* Connect to the given AP */
        result = cy_wcm_connect_ap(&wifi_conn_param, &ip_address);
        conn_retries++;

        vTaskDelay(1000);
    }

    return result;
}

/*******************************************************************************
 * Function Name: scan_callback
 *******************************************************************************
 * Summary: The callback function which accumulates the scan results.
 * If the CONNECT bit is set in the user_data then send back task notification
 * to the WiFi task to connect to the AP. If not then send the results back to
 * the BLE Client
 *
 * Parameters:
 *  cy_wcm_scan_result_t *result_ptr: Pointer to the scan result
 *  void *user_data: User data.
 *  cy_wcm_scan_status_t status: Status of scan completion.
 *
 * Return:
 *  void
 *
 ******************************************************************************/
static void scan_callback(cy_wcm_scan_result_t *result_ptr, void *user_data,
                   cy_wcm_scan_status_t status)
{
    uint8_t *scan_data;
    uint8_t byte_no = 0;
    uint8_t ssid_len = 0;


    if(NULL != result_ptr)
    {
        ssid_len = strlen((const char *)result_ptr->SSID);
    }

    if ((ssid_len != 0) && (CY_WCM_SCAN_INCOMPLETE == status))
    {
        wifi_conn_param.ap_credentials.security = result_ptr->security;
        LOG_DEBUG(CYLF_DEF, "%-32s\t", result_ptr->SSID);
        LOG_DEBUG(CYLF_DEF, "%s\n", get_wifi_security_name(result_ptr->security));

        /* If scan bit is set then send the scan result to the BLE Client */
        if(NOTIF_SCAN == *(uint32_t *) user_data)
        {
            /* Get memory for the buffer to send the scan data */
            scan_data = (uint8_t *)wiced_bt_get_buffer(SCAN_DATA_HEADER + ssid_len + SCAN_DATA_HEADER + sizeof(uint32_t));
            if(NULL == scan_data)
            {
                LOG_INFO(CYLF_DEF, "Buffer for notification not allocated\n");
                return;
            }
            /* Fill in the SSID first */
            scan_data[byte_no++] = SCAN_PACKET_TYPE_SSID;
            scan_data[byte_no++] = ssid_len;
            memcpy(&scan_data[byte_no], (const char *)result_ptr->SSID, ssid_len);
            byte_no = byte_no + ssid_len;

            /* Fill in the 4 bytes of security value */
            scan_data[byte_no++] = SCAN_PACKET_TYPE_SECURITY;
            scan_data[byte_no++] = sizeof(uint32_t);
            scan_data[byte_no++] = (uint8_t)(result_ptr->security & MASK_8BITS);
            scan_data[byte_no++] = (uint8_t)((result_ptr->security &
                                              MASK_16BITS) >> SHIFT_8BITS);
            scan_data[byte_no++] = (uint8_t)((result_ptr->security &
                                              MASK_24BITS) >> SHIFT_16BITS);
            scan_data[byte_no++] = (uint8_t)((result_ptr->security &
                                              MASK_32BITS) >> SHIFT_24BITS);
            if ((conn_id != 0) &&
                (app_custom_service_wifi_networks_client_char_config[0] &
                GATT_CLIENT_CONFIG_NOTIFICATION))
            {
                /* Wait if GATT congestion flag enabled. */
                while (is_gatt_congested);

                wiced_bt_gatt_server_send_notification(conn_id,
                        HDLC_CUSTOM_SERVICE_WIFI_NETWORKS_VALUE, byte_no, scan_data,
                    (wiced_bt_gatt_app_context_t)wiced_bt_free_buffer);

                /* Wait for GATT congestion to clear */
                while (is_gatt_congested);
            }
        }
    }

    /* If the connect bit is set in the user data then connect notification to
     * the WiFi task
     */
    if ((CY_WCM_SCAN_COMPLETE == status) && ((NOTIF_SCAN | NOTIF_CONNECT) ==
                                              *(uint32_t *) user_data))
    {
        xTaskNotifyFromISR(wifi_task_handle, NOTIF_CONNECT,
                           eSetValueWithOverwrite, NULL);
    }
}

static void start_wifi_scanning(void *notifiedvalue, cy_wcm_scan_filter_t *filter)
{
    cy_rslt_t result = 0;

    result = cy_wcm_start_scan(scan_callback, notifiedvalue, filter);

    if(result)
    {
        LOG_ERROR(CYLF_DEF, "Start scan failed error: %x\n", result);
        if(CY_RSLT_WCM_SCAN_IN_PROGRESS == result)
        {
            (void) cy_wcm_stop_scan();
            if(CY_RSLT_SUCCESS != cy_wcm_start_scan(scan_callback, notifiedvalue, filter))
            {
                LOG_ERROR(CYLF_DEF, "Start scan again failed after retry error: %x\n", result);
            }
        }
    }
}

/* [] END OF FILE */

