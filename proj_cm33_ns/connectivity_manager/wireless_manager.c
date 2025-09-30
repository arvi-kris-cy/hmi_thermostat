/*
 * wireless_manager.c
 *
 * Description: Implementation of Wireless Manager handling BLE and Wi-Fi communication.
 *  Created on: 10-Jun-2025
 *      Author: Infineon
 */

/* Header file includes */
#include "wireless_manager.h"
#include "subscriber_task.h"
#include "comm_manager.h"
#include "mqtt_command_handler.h"

/******************************************************************************
 * Macros
 ******************************************************************************/
/* LE Key Size */
#define MAX_KEY_SIZE (16U)

/* WICED Heap size for app */
#define APP_HEAP_SIZE              (0x1000U)
#define DEBOUNCE_DELAY_MS          (300U)
#define INITIAL_LEN_FILLED         (0U)
#define CRED_INIT_VALUE            (0U)

/******************************************************************************
* TYPEDEFS
******************************************************************************/
typedef void (*pfn_free_buffer_t)(uint8_t *);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
extern cy_wcm_connect_params_t wifi_conn_param;

/* Task Handle for WiFi Task */
TaskHandle_t wifi_task_handle;

/* Maintains the connection id of the current connection */
uint16_t conn_id = 0;
static wiced_bt_device_address_t peer_addr;

/* Pointer to the heap created */
wiced_bt_heap_t * app_heap_pointer;

uint8_t wifi_mac[6] = {0};
uint8_t ble_name[MAX_LEN_GAP_DEVICE_NAME+1] = {0};

bool provision_inprogress = false;
static bool ble_stack_init_state = false;
bool device_provisioned = false;
static uint8_t ble_retry_attempt = 0;
static bool is_manual_low_adv = false;
static connectivity_state_t current_state = STATE_UNPROVISIONED;
connection_retries_t auto_reconnect_retry = MAX_CONN_RETRY;

/*****************************************************************************
 * Static Function Prototype
 *****************************************************************************/
/*******************************************************************************
* Function Name: app_get_attribute
********************************************************************************
* Summary:
* This function searches through the GATT DB to point to the attribute
* corresponding to the given handle
*
* Parameters:
*  uint16_t handle: Handle to search for in the GATT DB
*
* Return:
*  gatt_db_lookup_table_t *: Pointer to the correct attribute in the GATT DB
*
*******************************************************************************/
static gatt_db_lookup_table_t *app_get_attribute(uint16_t handle);

/*******************************************************************************
* Function Name: app_gatts_req_read_handler
********************************************************************************
* Summary:
* This function handles the GATT read request events from the stack
*
* Parameters:
*  uint16_t conn_id: Connection ID
*  wiced_bt_gatt_read_t * p_read_data: Read data structure
*  wiced_bt_gatt_opcode_t opcode: GATT opcode
*  uint16_t len_requested: Length requested
* Return:
*  wiced_bt_gatt_status_t: GATT result
*
*******************************************************************************/
static wiced_bt_gatt_status_t app_gatts_req_read_handler(uint16_t conn_id,
                                                  wiced_bt_gatt_opcode_t opcode,
                                                  wiced_bt_gatt_read_t *p_read_data,
                                                  uint16_t len_requested);

/*******************************************************************************
* Function Name: app_gatt_read_by_type_handler
********************************************************************************
* Summary:
* This function handles the GATT read by type request events from the stack
*
* Parameters:
*  uint16_t conn_id: Connection ID
*  wiced_bt_gatt_opcode_t opcode: GATT opcode
*  wiced_bt_gatt_read_by_type_t * p_read_data: Read data structure
*  uint16_t len_requested: Length requested
*
* Return:
*  wiced_bt_gatt_status_t: GATT result
*
*******************************************************************************/
static wiced_bt_gatt_status_t app_gatt_read_by_type_handler(uint16_t conn_id,
                                         wiced_bt_gatt_opcode_t opcode,
                                         wiced_bt_gatt_read_by_type_t *p_read_data,
                                         uint16_t len_requested);

/*******************************************************************************
* Function Name: app_gatts_req_write_handler
********************************************************************************
* Summary:
* This function handles the GATT write request events from the stack
*
* Parameters:
*  uint16_t conn_id: Connection ID
*  wiced_bt_gatt_opcode_t opcode: GATT opcode
*  wiced_bt_gatt_write_t * p_data: Write data structure
*
* Return:
*  wiced_bt_gatt_status_t: GATT result
*
*******************************************************************************/
static wiced_bt_gatt_status_t app_gatts_req_write_handler(uint16_t conn_id,
                                                   wiced_bt_gatt_opcode_t opcode,
                                                   wiced_bt_gatt_write_req_t *p_data);
/*******************************************************************************
* Function Name: app_gatt_connect_handler
********************************************************************************
* Summary:
* This function handles the GATT connect request events from the stack
*
* Parameters:
*  wiced_bt_gatt_connection_status_t *p_conn_status: Connection or disconnection
*
* Return:
*  wiced_bt_gatt_status_t: GATT result
*
*******************************************************************************/
static wiced_bt_gatt_status_t app_gatt_connect_handler(
                       wiced_bt_gatt_connection_status_t *p_conn_status);

/*******************************************************************************
* Function Name: app_gatts_attr_req_handler
********************************************************************************
* Summary:
* This function redirects the GATT attribute requests to the appropriate
* functions
*
* Parameters:
*  wiced_bt_gatt_attribute_request_t *p_data: GATT request data structure
*
* Return:
*  wiced_bt_gatt_status_t: GATT result
*
*******************************************************************************/
static wiced_bt_gatt_status_t app_gatts_attr_req_handler(wiced_bt_gatt_attribute_request_t *p_data);

/*******************************************************************************
* Function Name: app_gatts_callback
********************************************************************************
* Summary:
* This function redirects the GATT requests to the appropriate functions
*
* Parameters:
*  wiced_bt_gatt_attribute_request_t *p_data: GATT request data structure
*  wiced_bt_gatt_event_data_t *p_data       : Pointer to BLE GATT event structures
* Return:
*  wiced_bt_gatt_status_t: GATT result
*
*******************************************************************************/
static wiced_bt_gatt_status_t app_gatts_callback(wiced_bt_gatt_evt_t event,
                                          wiced_bt_gatt_event_data_t *p_data);

/**
 * @brief BLE Event Handler Callback Function.
 *
 * This callback function is invoked whenever an event related to the BLE
 * operation occurs (e.g., connection, disconnection, advertisement, etc.).
 * It processes the BLE event and updates the system state or takes necessary
 * actions based on the event.
 *
 * The `app_management_callback` is typically registered with the BLE stack or
 * application and is called asynchronously to notify the application of BLE
 * events as they happen.
 *
 * @param event 		The type of the BLE event. This could be one of the
 *                   	predefined event types (e.g., connection, disconnection,
 *                   	advertising status update, etc.).
 * @param p_event_data 	Pointer to the data associated with the event. This could
 *                   	include connection details, error codes, or other context
 *                   	relevant to the event.
 *
 * @return response
 *
 * @note This function is designed to handle BLE events as part of the BLE
 *       application logic and should not block for extended periods.
 *       Long-running operations should be delegated to other functions.
 */
static wiced_result_t app_management_callback(wiced_bt_management_evt_t event,
                                       wiced_bt_management_evt_data_t *p_event_data);

/*******************************************************************************
* Function Name: application_init
********************************************************************************
* Summary:
* This function is called from the BTM enabled event
*    1. Initializes and registers the Generic ATTribute Data Base (GATT DB)
*    2. Configures the USER button 1 for interrupt
*    3. Sets pairable mode to true
*    4. Sets ADV data and starts advertising
*
*******************************************************************************/
static void application_init(void);

/*****************************************************************************
 * Static Function Definitions
 *****************************************************************************/
static gatt_db_lookup_table_t *app_get_attribute(uint16_t handle)
{
    /* Search for the given handle in the GATT DB and return the pointer to the
    correct attribute */
    uint8_t array_index = 0;

    for (array_index = 0; array_index < app_gatt_db_ext_attr_tbl_size; array_index++)
    {
        if (app_gatt_db_ext_attr_tbl[array_index].handle == handle)
        {
            return (&app_gatt_db_ext_attr_tbl[array_index]);
        }
    }
    return NULL;
}

static wiced_bt_gatt_status_t app_gatts_req_read_handler(uint16_t conn_id,
                                                  wiced_bt_gatt_opcode_t opcode,
                                                  wiced_bt_gatt_read_t *p_read_data,
                                                  uint16_t len_requested)
{

    gatt_db_lookup_table_t *puAttribute;
    int attr_len_to_copy;

    /* Get the right address for the handle in Gatt DB */
    if (NULL == (puAttribute = app_get_attribute(p_read_data->handle)))
    {
        printf("Read handle attribute not found. Handle:0x%X\n",
                p_read_data->handle);
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_read_data->handle,
                                            WICED_BT_GATT_INVALID_HANDLE);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    attr_len_to_copy = puAttribute->cur_len;

    printf("GATT Read handler: handle:0x%X, len:%d\n",
           p_read_data->handle, attr_len_to_copy);

    /* If the incoming offset is greater than the current length in the GATT DB
    then the data cannot be read back*/
    if (p_read_data->offset >= puAttribute->cur_len)
    {
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_read_data->handle,
                                            WICED_BT_GATT_INVALID_OFFSET);
        return (WICED_BT_GATT_INVALID_OFFSET);
    }

    int length_to_send = MIN(len_requested, attr_len_to_copy - p_read_data->offset);

    uint8_t *read_rsp = ((uint8_t *)puAttribute->p_data) + p_read_data->offset;

    wiced_bt_gatt_server_send_read_handle_rsp(conn_id, opcode, length_to_send, read_rsp, NULL);

    return WICED_BT_GATT_SUCCESS;
}

static wiced_bt_gatt_status_t app_gatt_read_by_type_handler(uint16_t conn_id,
                                         wiced_bt_gatt_opcode_t opcode,
                                         wiced_bt_gatt_read_by_type_t *p_read_data,
                                         uint16_t len_requested)
{
    gatt_db_lookup_table_t *puAttribute;
    uint16_t    attr_handle = p_read_data->s_handle;
    uint8_t     *p_rsp = wiced_bt_get_buffer(len_requested);
    uint8_t     pair_len = 0;
    int to_track_len_requested = 0;
    int len_filled;

    if (NULL == p_rsp)
    {
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, attr_handle,
                                            WICED_BT_GATT_INSUF_RESOURCE);
        return WICED_BT_GATT_INSUF_RESOURCE;
    }

    /* Read by type returns all attributes of the specified type,
     * between the start and end handles */
    while (WICED_TRUE)
    {
        attr_handle = wiced_bt_gatt_find_handle_by_type(attr_handle,
                p_read_data->e_handle, &p_read_data->uuid);

        if (false == attr_handle)
            break;

        if (NULL ==(puAttribute = app_get_attribute(attr_handle)))
        {
            wiced_bt_gatt_server_send_error_rsp(conn_id, opcode,
                    p_read_data->s_handle, WICED_BT_GATT_ERR_UNLIKELY);
            wiced_bt_free_buffer(p_rsp);
            return WICED_BT_GATT_ERR_UNLIKELY;
        }

        len_filled = wiced_bt_gatt_put_read_by_type_rsp_in_stream(
                p_rsp + to_track_len_requested, len_requested - to_track_len_requested, &pair_len, attr_handle,
                 puAttribute->cur_len, puAttribute->p_data);
        if (INITIAL_LEN_FILLED == len_filled)
        {
            break;
        }
        to_track_len_requested += len_filled;

        /* Increment starting handle for next search to one past current */
        attr_handle++;
    }

    if (INITIAL_LEN_FILLED == to_track_len_requested)
    {
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_read_data->s_handle,
                                            WICED_BT_GATT_INVALID_HANDLE);
        wiced_bt_free_buffer(p_rsp);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    /* Send the response */
    wiced_bt_gatt_server_send_read_by_type_rsp(conn_id, opcode, pair_len,
            to_track_len_requested, p_rsp, (wiced_bt_gatt_app_context_t)wiced_bt_free_buffer);

    return WICED_BT_GATT_SUCCESS;
}

static wiced_bt_gatt_status_t app_gatts_req_write_handler(uint16_t conn_id,
                                                   wiced_bt_gatt_opcode_t opcode,
                                                   wiced_bt_gatt_write_req_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_SUCCESS;
    uint8_t *p_attr = p_data->p_val;
    gatt_db_lookup_table_t *puAttribute;

    printf("GATT write handler: handle:0x%X len:%d, opcode:0x%X\n",
           p_data->handle, p_data->val_len, opcode);

    /* Get the right address for the handle in Gatt DB */
    if (NULL == (puAttribute = app_get_attribute(p_data->handle)))
    {
        printf("\nWrite Handle attr not found. Handle:0x%X\n", p_data->handle);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    switch (p_data->handle)
    {
        /* Write request for the WiFi SSID characteristic. Copy the incoming data
        to the WIFI SSID variable */
        case HDLC_CUSTOM_SERVICE_WIFI_SSID_VALUE:

            /* First copy the value to the GATT DB variable and then to the global
             * variable to be stored in NV memory
             */
            memset(app_custom_service_wifi_ssid, CRED_INIT_VALUE,
                   strlen((char *)app_custom_service_wifi_ssid));
            memcpy(app_custom_service_wifi_ssid, p_attr, p_data->val_len);
            puAttribute->cur_len = p_data->val_len;

            memcpy(&wifi_details.wifi_ssid[0], p_attr, p_data->val_len);
            wifi_details.ssid_len = p_data->val_len;

            printf("Wi-Fi SSID: %s\n", app_custom_service_wifi_ssid);
            break;

        /* Write request for the WiFi password characteristic. Accept the password.
        Copy the incoming data to the WIFI Password variable */
        case HDLC_CUSTOM_SERVICE_WIFI_PASSWORD_VALUE:

            /* First copy the value to the GATT DB variable and then to the global
             * variable to be stored in NV memory
             */
            memset(app_custom_service_wifi_password, CRED_INIT_VALUE,
                    strlen((char *)app_custom_service_wifi_password));
            memcpy(app_custom_service_wifi_password, p_attr, p_data->val_len);
            puAttribute->cur_len = p_data->val_len;

            memcpy(&wifi_details.wifi_password[0], p_attr, p_data->val_len);
            wifi_details.password_len = p_data->val_len;

            printf("Wi-Fi Password: %s\n", app_custom_service_wifi_password);
            break;

        /* Handle the CCCD values for WIFI NETWORKS characteristic */
        case HDLD_CUSTOM_SERVICE_WIFI_NETWORKS_CLIENT_CHAR_CONFIG:
            app_custom_service_wifi_networks_client_char_config[0] = p_attr[0];
            app_custom_service_wifi_networks_client_char_config[1] = p_attr[1];
            break;

        /* Write request for the control characteristic. Copy the incoming data
        to the WIFI control variable. Based on the value, start connect, disconnect
         or scan procedure */
        case HDLC_CUSTOM_SERVICE_WIFI_CONTROL_VALUE:
            app_custom_service_wifi_control[0] = p_attr[0];
            puAttribute->cur_len = p_data->val_len;

            /* Connect command */
            if (WIFI_CONTROL_CONNECT == app_custom_service_wifi_control[0])
            {
                if(0 != wifi_details.ssid_len)
                {
                    /* Set the WiFi Connection parameters structure to 0 before copying
                     * data */
                    memset(&wifi_conn_param, CRED_INIT_VALUE, sizeof(cy_wcm_connect_params_t));

                    /* Copy the WiFi credentials to the global variable */
                    memcpy(wifi_conn_param.ap_credentials.SSID,
                           &wifi_details.wifi_ssid[0], wifi_details.ssid_len);

                    memcpy(wifi_conn_param.ap_credentials.password,
                           &wifi_details.wifi_password[0], wifi_details.password_len);

                    update_conn_state(DEV_ST_WIFI_CONNECTING);

                    /* Send notification to the WiFi task to scan and connect with
                     * the given WiFi credentials
                     */
                    xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT),
                                eSetValueWithOverwrite);

                }
                else
                {
                    printf("WiFi Credentials not present\n");
                }
            }
            /* Scan command */
            else if(WIFI_CONTROL_SCAN == app_custom_service_wifi_control[0])
            {
                /* Check if notifications are enabled or not. If not then there is
                 * no point of starting the scan. If GATT notification is enabled
                 * then ask the WiFi task to start WiFi scan
                 */
                if ((false != conn_id) &&
                    (app_custom_service_wifi_networks_client_char_config[0] &
                    GATT_CLIENT_CONFIG_NOTIFICATION))
                {
                    xTaskNotify(wifi_task_handle, (NOTIF_SCAN), eSetValueWithOverwrite);
                }
                else
                {
                    printf("Notifications for WiFi Networks characteristic are "
                            "disabled. Cannot scan for networks\n");
                }
            }
            /* Disconnect command */
            else if(WIFI_CONTROL_DISCONNECT == app_custom_service_wifi_control[0])
            {
                xTaskNotify(wifi_task_handle, NOTIF_DISCONNECT,
                            eSetValueWithOverwrite);
            }
            else
            {
                printf("Invalid command\n");
            }
            break;

        /* Notification for control characteristic.
         */
        case HDLD_CUSTOM_SERVICE_WIFI_CONTROL_CLIENT_CHAR_CONFIG:
            app_custom_service_wifi_control_client_char_config[0] = p_attr[0];
            app_custom_service_wifi_control_client_char_config[1] = p_attr[1];
            break;

        case HDLC_CUSTOM_SERVICE_THERMOSTAT_DATA_VALUE:
        {
            parse_received_command((const char *)p_data->p_val, p_data->val_len);
            break;
        }

        default:
            printf("Write GATT Handle not found\n");
            result = WICED_BT_GATT_INVALID_HANDLE;
            break;
    }

    return result;
}

static wiced_bt_gatt_status_t app_gatt_connect_handler(
                       wiced_bt_gatt_connection_status_t *p_conn_status)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;
    wiced_result_t result;

    /* Check whether it is a connect event or disconnect event. If the device
    has been disconnected then restart advertisement */
    if (NULL != p_conn_status)
    {
        if (p_conn_status->connected)
        {
            /* Device got connected */
            printf("\nConnected: Peer BD Address: ");
            print_bd_address(p_conn_status->bd_addr);
            printf("\n");
            conn_id = p_conn_status->conn_id;

            memcpy(&peer_addr, &(p_conn_status->bd_addr), sizeof(wiced_bt_device_address_t));
            handle_connectivity_state(STATE_BLE_CONNECTED);
        }
        else /* Device got disconnected */
        {
            printf("\nDisconnected: Peer BD Address: ");
            print_bd_address(p_conn_status->bd_addr);
            printf("\n");

            printf("Reason for disconnection: %s\n",
                    get_bt_gatt_disconn_reason_name(p_conn_status->reason));

            conn_id = false;

            if(p_conn_status->reason != GATT_CONN_TERMINATE_LOCAL_HOST)
            {
				result = wiced_bt_ble_set_raw_advertisement_data(CY_BT_ADV_PACKET_DATA_SIZE,
																 cy_bt_adv_packet_data);
				if (WICED_SUCCESS != result)
				{
					printf("Set ADV data failed\n");
				}

				result = wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH,
													   false, NULL);
				if(WICED_SUCCESS != result)
				{
					printf("Start ADV failed");
				}
				update_conn_state(DEV_ST_BLE_ADVERTISING);
            }
        }

        status = WICED_BT_GATT_SUCCESS;
    }

    return status;
}

static wiced_bt_gatt_status_t app_gatts_attr_req_handler(wiced_bt_gatt_attribute_request_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;
    wiced_bt_gatt_write_req_t *p_write_request = &p_data->data.write_req;

    switch (p_data->opcode)
    {
        case GATT_REQ_READ:
        case GATT_REQ_READ_BLOB:
            result = app_gatts_req_read_handler(p_data->conn_id, p_data->opcode,
                                    &p_data->data.read_req, p_data->len_requested);
            break;

        case GATT_REQ_READ_BY_TYPE:
            result = app_gatt_read_by_type_handler(p_data->conn_id, p_data->opcode,
                                &p_data->data.read_by_type, p_data->len_requested);
            break;

        case GATT_REQ_WRITE:
        case GATT_CMD_WRITE:
        case GATT_CMD_SIGNED_WRITE:
            result = app_gatts_req_write_handler(p_data->conn_id, p_data->opcode,
                                                 &(p_data->data.write_req));
            if ((p_data->opcode == GATT_REQ_WRITE) && (result == WICED_BT_GATT_SUCCESS))
            {
                wiced_bt_gatt_server_send_write_rsp(p_data->conn_id, p_data->opcode,
                                                    p_write_request->handle);
            }
            else
            {
                wiced_bt_gatt_server_send_error_rsp(p_data->conn_id, p_data->opcode,
                                                    p_write_request->handle, result);
            }
            break;

        case GATT_REQ_MTU:
            printf("Exchanged MTU from client: %d\n", p_data->data.remote_mtu);
            wiced_bt_gatt_server_send_mtu_rsp(p_data->conn_id, p_data->data.remote_mtu,
            		wiced_bt_cfg_settings.p_ble_cfg->ble_max_rx_pdu_size);
            result = WICED_BT_GATT_SUCCESS;
            break;

        default:
            break;
    }

    return result;
}

static wiced_bt_gatt_status_t app_gatts_callback(wiced_bt_gatt_evt_t event,
                                          wiced_bt_gatt_event_data_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

    printf("GATTS event: %s\n", get_bt_gatt_evt_name(event));

    switch (event)
    {
        case GATT_CONNECTION_STATUS_EVT:
            result = app_gatt_connect_handler(&p_data->connection_status);
            break;

        case GATT_ATTRIBUTE_REQUEST_EVT:
            result = app_gatts_attr_req_handler(&p_data->attribute_request);
            break;

        case GATT_GET_RESPONSE_BUFFER_EVT:
            p_data->buffer_request.buffer.p_app_rsp_buffer = wiced_bt_get_buffer(
                                              p_data->buffer_request.len_requested);

            p_data->buffer_request.buffer.p_app_ctxt = (void *)wiced_bt_free_buffer;

            if(NULL == p_data->buffer_request.buffer.p_app_rsp_buffer)
            {
                printf("Insufficient resources\n");
                result = WICED_BT_GATT_INSUF_RESOURCE;
            }
            else
            {
                result = WICED_BT_GATT_SUCCESS;
            }
            break;

        case GATT_APP_BUFFER_TRANSMITTED_EVT:
        {
            pfn_free_buffer_t pfn_free = (pfn_free_buffer_t)p_data->
                                          buffer_xmitted.p_app_ctxt;

            /* If the buffer is dynamic, the context will point to a function to
             * free it.
             */
            if (pfn_free)
                pfn_free(p_data->buffer_xmitted.p_app_data);

            result = WICED_BT_GATT_SUCCESS;
        }
        break;

        default:
            printf("GATT event not handled\n");
    }
    return result;
}

static wiced_result_t app_management_callback(wiced_bt_management_evt_t event,
                                       wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_result_t result = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bda = {0};
    wiced_bt_dev_ble_io_caps_req_t *pairing_io_caps = &(p_event_data->
                                           pairing_io_capabilities_ble_request);

    printf("Bluetooth Management Event: %s\n", get_btm_event_name(event));

    switch (event)
    {
        case BTM_ENABLED_EVT:

            /* Bluetooth is enabled */
            wiced_bt_set_local_bdaddr((uint8_t *)cy_bt_device_address,
                                       BLE_ADDR_PUBLIC);

            /* Read and print the BD address */
            wiced_bt_dev_read_local_addr(bda);
            printf("Local Bluetooth Address: ");
            print_bd_address(bda);
            printf("\n");

            application_init();
            break;

        case BTM_DISABLED_EVT:
            break;
            /* Print passkey to the screen so that the user can enter it. */
        case BTM_PASSKEY_NOTIFICATION_EVT:
            printf( "********************************************************\r\n");
            printf( "Passkey Notification\r\n");
            printf("PassKey: %" PRIu32 "\r\n",
            p_event_data->user_passkey_notification.passkey );
            printf( "***********************************************************\r\n");
            display_ble_pin(p_event_data->user_passkey_notification.passkey);
            break;

        case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
            pairing_io_caps->local_io_cap = BTM_IO_CAPABILITIES_DISPLAY_ONLY;

            pairing_io_caps->oob_data = BTM_OOB_NONE;

            pairing_io_caps->auth_req = BTM_LE_AUTH_REQ_SC;

            pairing_io_caps->max_key_size = MAX_KEY_SIZE;

            pairing_io_caps->init_keys = BTM_LE_KEY_PENC | BTM_LE_KEY_PID |
                                        BTM_LE_KEY_PCSRK | BTM_LE_KEY_LENC;

            pairing_io_caps->resp_keys = BTM_LE_KEY_PENC | BTM_LE_KEY_PID |
                                        BTM_LE_KEY_PCSRK | BTM_LE_KEY_LENC;
            break;

        case BTM_PAIRING_COMPLETE_EVT:
            if (WICED_SUCCESS == p_event_data->
                                pairing_complete.pairing_complete_info.ble.status)
            {
                printf("Pairing Complete: SUCCESS\n");

                handle_connectivity_state(STATE_BLE_CONNECTED);
            }
            else /* Pairing Failed */
            {
                printf("Pairing Complete: FAILED\n");
            }
            break;

        case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT:
            /* Paired Device Link Keys update */
            result = WICED_SUCCESS;
            break;

        case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
            /* Paired Device Link Keys Request */
            result = WICED_BT_ERROR;
            break;

        case BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT:
            /* Local identity Keys Update */
            result = WICED_SUCCESS;
            break;

        case BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT:
            /* Local identity Keys Request */
            result = WICED_BT_ERROR;
            break;

        case BTM_ENCRYPTION_STATUS_EVT:
            if (WICED_SUCCESS == p_event_data->encryption_status.result)
            {
                printf("Encryption Status Event: SUCCESS\n");
            }
            else /* Encryption Failed */
            {
                printf("Encryption Status Event: FAILED\n");
            }
            break;

        case BTM_SECURITY_REQUEST_EVT:
            wiced_bt_ble_security_grant(p_event_data->security_request.bd_addr,
                                        WICED_BT_SUCCESS);
            break;

        case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
            printf("\n");
            printf("Advertisement state changed to %s\n", get_bt_advert_mode_name(
                                           p_event_data->ble_advert_state_changed));

            if(p_event_data->ble_advert_state_changed == BTM_BLE_ADVERT_OFF)
            {
                if(device_provisioned == false)
                {
                	printf("Device not provisioned. Device in un-provisioned state\n ");
                	if(!provision_inprogress)
                	{
                		update_conn_state(DEV_ST_UNPROVISIONED);
                	}
                }
                else
                {
                	printf("Device in provisioned state\n ");
                }

            }
            else if(p_event_data->ble_advert_state_changed == BTM_BLE_ADVERT_UNDIRECTED_LOW)
            {
            	if(is_manual_low_adv != true)
            	{
					handle_connectivity_state(STATE_BLE_ACTIVE_LOW_MODE);
            	}
            	else
            	{
            		is_manual_low_adv = false;
            	}
            }
            else if(p_event_data->ble_advert_state_changed == BTM_BLE_ADVERT_UNDIRECTED_HIGH)
            {
                handle_connectivity_state(STATE_BLE_ACTIVE_HIGH_MODE);
            }
            break;

        default:
            break;
        }

    return result;
}

static void application_init(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_bt_gatt_status_t gatt_status = WICED_BT_GATT_INVALID_CONNECTION_ID;

    /* Create a buffer heap, make it the default heap */
    app_heap_pointer = wiced_bt_create_heap("app", NULL, APP_HEAP_SIZE, NULL,
                                            WICED_TRUE);

    if(NULL == app_heap_pointer)
    {
        printf("Failed to create heap\n");
        handle_app_error();
    }

    /* Register with stack to receive GATT callback */
    gatt_status = wiced_bt_gatt_register(app_gatts_callback);

    if(WICED_BT_GATT_SUCCESS != gatt_status)
    {
        printf("\nGATT register failed. Status: %s\n", get_bt_gatt_status_name(
                                                       gatt_status));
    }

    /*  Inform the stack to use our GATT database */
    gatt_status = wiced_bt_gatt_db_init(gatt_database, gatt_database_len, NULL);

    if(WICED_BT_GATT_SUCCESS != gatt_status)
    {
        printf("\nGATT db init failed. Status :%s\n",get_bt_gatt_status_name(
                                                     gatt_status));
    }

    /* Allow peer to pair */
    wiced_bt_set_pairable_mode(WICED_TRUE, false);

    /* Set BLE advertisement data */
    result = wiced_bt_ble_set_raw_advertisement_data(CY_BT_ADV_PACKET_DATA_SIZE,
                                                     cy_bt_adv_packet_data);
    if (WICED_SUCCESS != result)
    {
        printf("Set ADV data failed\n");
    }

	/* Read data from NVM if present */
	result =  Cy_RRAM_NvmReadByteArray(RRAMC0,
			APP_RRAM_NVM_MAIN_NS_START + RRAM_NVM_MAIN_NS_DATA_OFFSET,
			( uint8_t *)&wifi_details.wifi_ssid[0], sizeof(wifi_details) );

	if ((!result) && (wifi_details.ssid_len))
	{
		printf("Device provisioned, WiFi credentials present in NVM\n");

		/* Set the WiFi Connection parameters structure to 0 before copying
		 * data */
		memset(&wifi_conn_param, 0, sizeof(cy_wcm_connect_params_t));

		/* Copy the WiFi credentials to the global variable */
		memcpy(wifi_conn_param.ap_credentials.SSID, &wifi_details.wifi_ssid[0],
			   wifi_details.ssid_len);

		memcpy(wifi_conn_param.ap_credentials.password,
			   &wifi_details.wifi_password[0], wifi_details.password_len);

		ble_start_advertising(BTM_BLE_ADVERT_UNDIRECTED_HIGH);

		handle_connectivity_state(STATE_PROVISIONED);
        vTaskDelay(1000);

		update_conn_state(DEV_ST_BLE_ADVERTISING);
	}
	else /* WiFi credentials not found in NVM */
	{
		printf("WiFi credentials not present in NVM\n");

		handle_connectivity_state(STATE_UNPROVISIONED);
        vTaskDelay(1000);

		update_conn_state(DEV_ST_UNPROVISIONED);
	}
}

/*****************************************************************************
 * Function Definitions
 *****************************************************************************/
cy_rslt_t wirelessdevice_init(void)
{
	cy_rslt_t result = CY_RSLT_SUCCESS;

	result = wifi_init();
	if(CY_RSLT_SUCCESS != result)
	{
		printf("\nUnable to init wifi module with error: %u\n", result);
		return result;
	}

	// result = ble_init();
	// if(CY_RSLT_SUCCESS != result)
	// {
	// 	printf("\nUnable to init BLE module with error: %u\n", result);
	// 	return result;
	// }

    // wifi_get_macaddr((uint8_t *)wifi_mac);

    // //Update the BLE name based on MAC address
    // snprintf((char *)ble_name, sizeof(ble_name),"Therm_%X%X%X", wifi_mac[3], wifi_mac[4], wifi_mac[5]);
    // memcpy(app_gap_device_name, ble_name, MAX_LEN_GAP_DEVICE_NAME);
    // cy_bt_adv_packet_data[1].p_data = (uint8_t *)ble_name;
    // cy_bt_scan_resp_packet_data[0].p_data = (uint8_t *)ble_name;

    // setuid(wifi_mac[3], wifi_mac[4], wifi_mac[5]);

    return result;
}

cy_rslt_t wifi_init(void)
{
	return wcm_init();
}

cy_rslt_t wifi_disconnect(void)
{
	return cy_wcm_disconnect_ap();
}

bool wifi_get_status(void)
{
	return cy_wcm_is_connected_to_ap();
}

void wifi_get_macaddr(uint8_t *mac)
{
	cy_wcm_get_mac_addr(CY_WCM_INTERFACE_TYPE_STA, (cy_wcm_mac_t *)mac);
}

void delete_wifi_credential(void)
{
	printf("Deleting Wi-Fi data from NVM\n");

	/* Set the data to 0*/
	memset(&wifi_details, 0, sizeof(wifi_details));
	memset(&wifi_conn_param, 0, sizeof(wifi_conn_param));

	Cy_RRAM_WriteByteArray(RRAMC0, APP_RRAM_NVM_MAIN_NS_START + RRAM_NVM_MAIN_NS_DATA_OFFSET , &wifi_details.wifi_ssid[0],sizeof(wifi_details));
}

// Function for BLE functionality
cy_rslt_t ble_init(void)
{
	cy_rslt_t result = CY_RSLT_SUCCESS;

	if(false == ble_stack_init_state)
	{
	    /* Initialize the Bluetooth stack with a callback function and stack
	     * configuration structure */
		result = wiced_bt_stack_init(app_management_callback, &wiced_bt_cfg_settings);
	    if (WICED_SUCCESS != result )
	    {
	        printf("Error in initializing the BT stack with error: %u\n", result);
	    }
	    else
	    {
	    	ble_stack_init_state = true;
	    }
	}

    return result;
}

cy_rslt_t ble_deinit(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    if(true == ble_stack_init_state)
    {
        /* Initialize the Bluetooth stack with a callback function and stack
         * configuration structure */
        result = wiced_bt_stack_deinit();
        if (WICED_SUCCESS != result )
        {
            printf("Error in deinitializing the BT stack with error: %u\n", result);
        }
        else
        {
            ble_stack_init_state = false;
        }
    }

    return result;
}


void ble_disconnect(void)
{
	if(true == ble_stack_init_state)
	{
        wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, false, NULL);
        vTaskDelay(100);

        wiced_bt_gatt_disconnect(conn_id);
        vTaskDelay(100);

        /* Delete the bonding information for the remote device */
        wiced_result_t result = wiced_bt_dev_delete_bonded_device(peer_addr);
        if (WICED_SUCCESS == result)
        {
            printf("Bonding information for device %02X:%02X:%02X:%02X:%02X:%02X removed successfully.\n", peer_addr[0],
                    peer_addr[1], peer_addr[2], peer_addr[3], peer_addr[4], peer_addr[5]);
        }
        else
        {
            printf("Failed to remove bonding information for device %02X:%02X:%02X:%02X:%02X:%02X\n", peer_addr[0],
                    peer_addr[1], peer_addr[2], peer_addr[3], peer_addr[4], peer_addr[5]);
        }
        vTaskDelay(100);
	}
}

wiced_result_t ble_start_advertising(wiced_bt_ble_advert_mode_t mode)
{
	update_conn_state(DEV_ST_BLE_ADVERTISING);
	return wiced_bt_start_advertisements(mode, false, NULL);
}

void ble_stop_advertising(void)
{
    wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF,
                                           false, NULL);
}

void app_bt_send_message(char *data, size_t data_len)
{
    if(conn_id)
    {
        wiced_bt_gatt_server_send_notification(conn_id,
        		HDLC_CUSTOM_SERVICE_THERMOSTAT_DATA_VALUE,data_len, (uint8_t *)data, NULL);
        printf("Send JSON Package Over BLE:\n%s\n", data);

        free(data);
    }
    else
    {
        printf("BLE Not Connected!\n");
    }
}

void handle_connectivity_state(connectivity_state_t state)
{
    switch (state)
    {
        case STATE_PROVISIONED:
        {
            if(!device_provisioned)
            {
                device_provisioned = true;
                update_provision_state(device_provisioned);
            }
            ble_retry_attempt = 0;
            break;
        }

        case STATE_UNPROVISIONED:
        {
            ble_retry_attempt = 0;
            update_conn_state(DEV_ST_UNPROVISIONED);
            update_comm_interface(CONNECTIVITY_NONE);
            if(device_provisioned)
            {
                device_provisioned = false;
                update_provision_state(device_provisioned);
            }
            break;
        }

        case STATE_START_PROVISIONING:
        {
        	provision_inprogress = true;
            ble_retry_attempt = 0;
			ble_start_advertising(BTM_BLE_ADVERT_UNDIRECTED_HIGH);
            break;
        }

        case STATE_BLE_ACTIVE_HIGH_MODE:
        {
        	//Do Nothing
            break;
        }

        case STATE_BLE_ACTIVE_LOW_MODE:
        {
            if((auto_reconnect_retry == BLE_CONN_RETRY_INFINITE) || (MAX_CONNECTION_RETRIES > ble_retry_attempt++))
            {
                ble_start_advertising(BTM_BLE_ADVERT_UNDIRECTED_HIGH);
            }
            else if(device_provisioned)
            {
                ble_stop_advertising();
                update_conn_state(DEV_ST_WIFI_CONNECTING);
                xTaskNotify(wifi_task_handle, (NOTIF_SCAN | NOTIF_CONNECT), eSetValueWithOverwrite);
                ble_retry_attempt = 0;
            }
            else
            {
                ble_stop_advertising();
            }
            break;
        }

        case STATE_BLE_ADV:
        {
            /* Don't advertise if already connected */
            if(conn_id != false)
            {
                printf("STATE_BLE_ADV Device already connected over BLE\n");
            }
            else
            {
                ble_start_advertising(BTM_BLE_ADVERT_UNDIRECTED_HIGH);
            }
            break;
        }

        case STATE_BLE_CONNECTED:
        {
            if(device_provisioned)
            {
                if(cy_wcm_is_connected_to_ap())
                {
                    mqtt_task_cmd_t mqtt_task_cmd = HANDLE_MANUAL_DISCONNECTION;
                    xQueueSend(mqtt_task_q, &mqtt_task_cmd, portMAX_DELAY);
                }
            }
            update_provision_state(device_provisioned);
            vTaskDelay(3000);
			update_comm_interface(CONNECTIVITY_BLE);
            update_conn_state(DEV_ST_BLE_CONNECTED);
            is_manual_low_adv = false;
            ble_retry_attempt = 0;
            break;
        }

        case STATE_WIFI_CONNECTED:
        {
            provision_inprogress = false;

            /* Set provision status as connected to Wi-Fi */
            if(!device_provisioned)
            {
                device_provisioned = true;
                update_provision_state(device_provisioned);
            }
        	break;
        }

        case STATE_WIFI_DISCONNECTED:
        {
        	if(!conn_id)
        	{
				is_manual_low_adv = true;
				update_conn_state(DEV_ST_WIFI_DISCONNECTED);
				ble_start_advertising(BTM_BLE_ADVERT_UNDIRECTED_HIGH);
        	}
        	break;
        }

        case STATE_CLOUD_CONNECT:
        {
            update_comm_interface(CONNECTIVITY_MQTT_CLOUD);
            update_conn_state(DEV_ST_CLOUD_CONNECTED);
            vTaskDelay(100);
            is_manual_low_adv = true;
            wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_LOW, false, NULL);
            if(!device_provisioned)
            {
                device_provisioned = true;
                update_provision_state(device_provisioned);
            }
            break;
        }

        case STATE_CLOUD_DISCONNECT:
        {
            update_conn_state(DEV_ST_CLOUD_DISCONNECTED);
            break;
        }

        default:
            // Handle unexpected states
            break;
    }
    current_state = state;
}

bool get_device_provision_state(void)
{
	return device_provisioned;
}

connection_retries_t get_retry_state(void)
{
	return auto_reconnect_retry;
}
