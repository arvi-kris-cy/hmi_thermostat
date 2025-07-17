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


/******************************************************************************
 * Global Variables
 ******************************************************************************/
/* WCM related structures */
cy_wcm_connect_params_t wifi_conn_param;

/* Variable to be used to store the WiFi credentials which can be copied to
 * NVRAM
 */
wifi_details_t wifi_details;

/******************************************************************************
 * Extern Functions and Variables
 ******************************************************************************/
/* Maintains the connection id of the current connection */
extern uint16_t conn_id;

extern QueueHandle_t mqtt_task_q;

/* Task Handles for WiFi Task */
extern TaskHandle_t wifi_task_handle;
static mtb_hal_sdio_t sdio_instance;
static cy_stc_sd_host_context_t sdhc_host_context;
static cy_wcm_config_t wcm_config;

/******************************************************************************
 * Temp Code
 ******************************************************************************/
volatile bool button_debouncing = false;
volatile uint32_t button_debounce_timestamp = 0;
#define DEBOUNCE_TIME_MS                 (1U)
#define BTN1_INTERRUPT_PRIORITY         (7U)


/* Interrupt config structure */
cy_stc_sysint_t intrCfg =
{
    .intrSrc = CYBSP_USER_BTN_IRQ,
    .intrPriority = BTN1_INTERRUPT_PRIORITY
};
/*******************************************************************************
* Function Name: button_interrupt_handler
********************************************************************************
* Summary:
*  GPIO interrupt handler.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void button_interrupt_handler(void)
{
    if (Cy_GPIO_GetInterruptStatus(CYBSP_USER_BTN1_PORT, CYBSP_USER_BTN1_PIN))
    {
        Cy_GPIO_ClearInterrupt(CYBSP_USER_BTN1_PORT, CYBSP_USER_BTN1_PIN);
        NVIC_ClearPendingIRQ(CYBSP_USER_BTN1_IRQ);

        if (!(Cy_GPIO_Read(CYBSP_USER_BTN1_PORT, CYBSP_USER_BTN1_PIN)))
        {
            if (!button_debouncing)
            {
                /* Set the debouncing flag */
                button_debouncing = true;

                /* Record the current timestamp */
                button_debounce_timestamp = (uint32_t) (xTaskGetTickCount() * portTICK_PERIOD_MS);
            }

            if (button_debouncing && (((xTaskGetTickCount() * portTICK_PERIOD_MS)) - button_debounce_timestamp >= DEBOUNCE_TIME_MS * portTICK_PERIOD_MS))
            {
                button_debouncing = false;

                mqtt_task_cmd_t mqtt_task_cmd = HANDLE_MANUAL_DISCONNECTION;
                xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
            }
        }
    }

    /* CYBSP_USER_BTN1 (SW2) and CYBSP_USER_BTN2 (SW4) share the same port and
     * hence they share the same NVIC IRQ line. Since both the buttons are
     * configured for falling edge interrupt in the BSP, pressing any button
     * will trigger the execution of this ISR. Therefore, we must clear the
     * interrupt flag of the user button (CYBSP_USER_BTN2) to avoid issues in
     * case if user presses BTN2 by mistake.
     */
    Cy_GPIO_ClearInterrupt(CYBSP_USER_BTN2_PORT, CYBSP_USER_BTN2_PIN);
    NVIC_ClearPendingIRQ(CYBSP_USER_BTN2_IRQ);
}

/*******************************************************************************
* Function Name: user_button_init
********************************************************************************
*
* Summary:
*  Initialize the button with Interrupt
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void user_button_init(void)
{
    cy_en_sysint_status_t btn_interrupt_init_status ;

    /* CYBSP_USER_BTN1 (SW2) and CYBSP_USER_BTN2 (SW4) share the same port and
    * hence they share the same NVIC IRQ line. Since both are configured in the BSP
    * via the Device Configurator, the interrupt flags for both the buttons are set
    * right after they get initialized through the call to cybsp_init(). The flags
    * must be cleared otherwise the interrupt line will be constantly asserted.
    */
    Cy_GPIO_ClearInterrupt(CYBSP_USER_BTN1_PORT,CYBSP_USER_BTN1_PIN);
    Cy_GPIO_ClearInterrupt(CYBSP_USER_BTN2_PORT,CYBSP_USER_BTN2_PIN);
    NVIC_ClearPendingIRQ(CYBSP_USER_BTN1_IRQ);
    NVIC_ClearPendingIRQ(CYBSP_USER_BTN2_IRQ);

    /* Initialize the interrupt and register interrupt callback */
    btn_interrupt_init_status = Cy_SysInt_Init(&intrCfg, &button_interrupt_handler);

    /* Button interrupt initialization failed. Stop program execution. */
    if(CY_SYSINT_SUCCESS != btn_interrupt_init_status)
    {
        handle_app_error();
    }

    printf("Press the upper-left button to delete and disconnect from Wi-Fi.\n");

    /* Enable the interrupt in the NVIC */
    NVIC_EnableIRQ(intrCfg.intrSrc);
}

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
        handle_app_error();
    }

    /* Enable NVIC interrupt. */
    NVIC_EnableIRQ(CYBSP_WIFI_SDIO_IRQ);

    /* Setup SDIO using the HAL object and desired configuration */
    result = mtb_hal_sdio_setup(&sdio_instance, &CYBSP_WIFI_SDIO_sdio_hal_config, NULL, &sdhc_host_context);

    /* SDIO setup failed. Stop program execution. */
    if(CY_RSLT_SUCCESS != result)
    {
        handle_app_error();
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
        handle_app_error();
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
        handle_app_error();
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
		printf("\nERROR: Wireless module initialization failed, Error Code: %lu\n", result);
		handle_app_error();
	}

	user_button_init();

	/* Read data from NVM if present */
	result =  Cy_RRAM_NvmReadByteArray(RRAMC0,
			APP_RRAM_NVM_MAIN_NS_START + RRAM_NVM_DATA_NS_OFFSET,
			( uint8_t *)&wifi_details.wifi_ssid[0], sizeof(wifi_details) );

	if ((!result) && (wifi_details.ssid_len))
	{
		printf("WiFi credentials present in NVM\n");

		/* Set the WiFi Connection parameters structure to 0 before copying
		 * data */
		memset(&wifi_conn_param, 0, sizeof(cy_wcm_connect_params_t));

		/* Copy the WiFi credentials to the global variable */
		memcpy(wifi_conn_param.ap_credentials.SSID, &wifi_details.wifi_ssid[0],
			   wifi_details.ssid_len);

		memcpy(wifi_conn_param.ap_credentials.password,
			   &wifi_details.wifi_password[0], wifi_details.password_len);

		update_conn_state(DEV_ST_WIFI_CONNECTING);

		/* Unblock WiFi task with notification */
		xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT),
					eSetValueWithOverwrite);
	}
	else /* WiFi credentials not found in NVM */
	{
		printf("WiFi credentials not present in NVM\n");

		update_conn_state(DEV_ST_UNPROVISIONED);
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
        if(NOTIF_SCAN & ulNotifiedValue)
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
                printf("Starting scan with SSID: %s\n", scan_filter.param.SSID);

                result = cy_wcm_start_scan(scan_callback, &ulNotifiedValue,
                                           &scan_filter);
            }
            /* Else start scan without any filter */
            else
            {
                result = cy_wcm_start_scan(scan_callback, &ulNotifiedValue, NULL);
            }

            if(result)
            {
                printf("Start scan failed\n");
            }
        }

        /* Check for connection notification */
        else if(NOTIF_CONNECT == ulNotifiedValue)
        {
            /* Variable to track the number of connection retries to the Wi-Fi
             * AP
             */
            uint8_t conn_retries = 0;

            /* Join the Wi-Fi AP */
            result = CY_RSLT_TYPE_ERROR;

            while((CY_RSLT_SUCCESS != result) && (conn_retries <
                                                        MAX_CONNECTION_RETRIES))
            {
                printf("\nTrying to connect SSID: %s, Password: %s\n",
                        wifi_conn_param.ap_credentials.SSID,
                        wifi_conn_param.ap_credentials.password);

                /* Connect to the given AP */
                result = cy_wcm_connect_ap(&wifi_conn_param, &ip_address);
                conn_retries++;

            }

            if(CY_RSLT_SUCCESS == result)
            {

                display_ble_pin(0);
                printf("Successfully joined the Wi-Fi network\n");

                /* Store WiFi credentials in NVM */
                /* Write data to NVM. */
                nvm_result =  Cy_RRAM_WriteByteArray(RRAMC0,
                        APP_RRAM_NVM_MAIN_NS_START + RRAM_NVM_DATA_NS_OFFSET ,
                        &wifi_details.wifi_ssid[0],sizeof(wifi_details) );
                if (CY_RRAM_SUCCESS != nvm_result)
                {
                    printf("Failed to write to NVM \n");
                }
                else
                {
                	printf("Stored WiFi credentials in NVM.\n");
                }

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
                    wiced_bt_gatt_server_send_notification(conn_id,
                                    HDLC_CUSTOM_SERVICE_WIFI_CONTROL_VALUE,
                                    sizeof(app_custom_service_wifi_control[0]),
                                    p_attr, NULL);
                }
                else /* Notification not sent */
                {
                    printf("Notification not sent\n");
                }

                mqtt_task_cmd_t mqtt_task_cmd = HANDLE_CONNECT;
                xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
            }
            else /* WiFi connection failed */
            {
                /* Update GATT DB about unsuccessful connection */
                app_custom_service_wifi_control[0] = WIFI_CONTROL_DISCONNECT;

            	update_conn_state(DEV_ST_WIFI_DISCONNECTED);

                /* Send notification for unsuccessful connection */
                /* Check if the connection is active and notifications are
                 * enabled and then send the notification
                 */
                if ((conn_id) &&
                    ((app_custom_service_wifi_control_client_char_config[0] &
                      GATT_CLIENT_CONFIG_NOTIFICATION)))
                {
                    uint8_t *p_attr = (uint8_t*)app_custom_service_wifi_control;
                    wiced_bt_gatt_server_send_notification(conn_id,
                                     HDLC_CUSTOM_SERVICE_WIFI_CONTROL_VALUE,
                                     sizeof(app_custom_service_wifi_control[0]),
                                     p_attr, NULL);
                }
                else /* Notification not sent */
                {
                    printf("Notification not sent\n");
                }

                printf("Failed to join Wi-Fi network\n");

                /* Disconnect BLE device if connected */
                ble_disconnect();

				vTaskDelay(5000);
                update_conn_state(DEV_ST_UNPROVISIONED);

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
                printf("Deleting Wi-Fi data from NVM\n");

                /* Set the data to 0*/
                memset(&wifi_details, 0, sizeof(wifi_details));

                nvm_result =  Cy_RRAM_WriteByteArray(RRAMC0, APP_RRAM_NVM_MAIN_NS_START + RRAM_NVM_DATA_NS_OFFSET , &wifi_details.wifi_ssid[0],sizeof(wifi_details));
                if (nvm_result)
                {
                    printf("Failed to write to NVM\n");
                }
            }

            printf("Disconnecting Wi-Fi\n");
            result = cy_wcm_disconnect_ap();
            if(CY_RSLT_SUCCESS == result)
            {
                /* Update GATT DB about disconnection */
                app_custom_service_wifi_control[0] = WIFI_CONTROL_DISCONNECT;

                printf("Successfully disconnected from AP\n");
				update_conn_state(DEV_ST_WIFI_DISCONNECTED);
				vTaskDelay(2000);

                update_conn_state(DEV_ST_UNPROVISIONED);
                vTaskDelay(2000);
            }
            else /* Disconnection failed */
            {
                printf("Failed to disconnect\n");
            }
        }
    }
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
void scan_callback(cy_wcm_scan_result_t *result_ptr, void *user_data,
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
            printf("%-32s\t", result_ptr->SSID);
            printf("%s\n", get_wifi_security_name(result_ptr->security));

        /* If scan bit is set then send the scan result to the BLE Client */
        if(NOTIF_SCAN == *(uint32_t *) user_data)
        {
            /* Get memory for the buffer to send the scan data */
            scan_data = (uint8_t *)wiced_bt_get_buffer(SCAN_DATA_HEADER + ssid_len + SCAN_DATA_HEADER + sizeof(uint32_t));
            if(NULL == scan_data)
            {
                printf("Buffer for notification not allocated\n");
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
                wiced_bt_gatt_server_send_notification(conn_id,
                        HDLC_CUSTOM_SERVICE_WIFI_NETWORKS_VALUE, byte_no, scan_data,
                    (wiced_bt_gatt_app_context_t)wiced_bt_free_buffer);

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


/* [] END OF FILE */

