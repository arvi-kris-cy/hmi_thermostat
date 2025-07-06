/******************************************************************************
* File Name:   mqtt_client_config.h
*
* Description: This file contains all the configuration macros used by the
*              MQTT client in this example.
*
* Related Document: See README.md
*
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

#ifndef MQTT_CLIENT_CONFIG_H_
#define MQTT_CLIENT_CONFIG_H_

#include "cy_mqtt_api.h"

/*******************************************************************************
* Macros
********************************************************************************/

/***************** MQTT CLIENT CONNECTION CONFIGURATION MACROS *****************/
/* MQTT Broker/Server address and port used for the MQTT connection. */
//#define MQTT_BROKER_ADDRESS               "a3t25hrcseg2ba-ats.iot.eu-north-1.amazonaws.com"
#define MQTT_BROKER_ADDRESS				  "a2pmd6m35psott-ats.iot.eu-north-1.amazonaws.com"
#define MQTT_PORT                         8883

/* Set this macro to 1 if a secure (TLS) connection to the MQTT Broker is
 * required to be established, else 0.
 */
#define MQTT_SECURE_CONNECTION            ( 1 )

/* Configure the user credentials to be sent as part of MQTT CONNECT packet */
#define MQTT_USERNAME                     "User"
#define MQTT_PASSWORD                     ""


/********************* MQTT MESSAGE CONFIGURATION MACROS **********************/
/* The MQTT topics to be used by the publisher and subscriber. */
#define MQTT_PUB_TOPIC                    "ledstatus"

/* Set the QoS that is associated with the MQTT publish, and subscribe messages.
 * Valid choices are 0, 1, and 2. Other values should not be used in this macro.
 */
#define MQTT_MESSAGES_QOS                 ( 1 )

/* Configuration for the 'Last Will and Testament (LWT)'. It is an MQTT message
 * that will be published by the MQTT broker if the MQTT connection is
 * unexpectedly closed. This configuration is sent to the MQTT broker during
 * MQTT connect operation and the MQTT broker will publish the Will message on
 * the Will topic when it recognizes an unexpected disconnection from the client.
 *
 * If you want to use the last will message, set this macro to 1 and configure
 * the topic and will message, else 0.
 */
#define ENABLE_LWT_MESSAGE                ( 0 )
#if ENABLE_LWT_MESSAGE
    #define MQTT_WILL_TOPIC_NAME          MQTT_PUB_TOPIC "/will"
    #define MQTT_WILL_MESSAGE             ("MQTT client unexpectedly disconnected!")
#endif

/* MQTT messages which are published on the MQTT_PUB_TOPIC that controls the
 * device (user LED in this example) state in this code example.
 */
#define MQTT_DEVICE_ON_MESSAGE            "TURN ON"
#define MQTT_DEVICE_OFF_MESSAGE           "TURN OFF"


/******************* OTHER MQTT CLIENT CONFIGURATION MACROS *******************/
/* A unique client identifier to be used for every MQTT connection. */
#define MQTT_CLIENT_IDENTIFIER            "psocedge-mqtt-client"

/* The timeout in milliseconds for MQTT operations in this example. */
#define MQTT_TIMEOUT_MS                   ( 5000 )

/* The keep-alive interval in seconds used for MQTT ping request. */
#define MQTT_KEEP_ALIVE_SECONDS           ( 60 )

/* Every active MQTT connection must have a unique client identifier. If you
 * are using the above 'MQTT_CLIENT_IDENTIFIER' as client ID for multiple MQTT
 * connections simultaneously, set this macro to 1. The device will then
 * generate a unique client identifier by appending a timestamp to the
 * 'MQTT_CLIENT_IDENTIFIER' string. Example: 'psoc6-mqtt-client5927'
 */
#define GENERATE_UNIQUE_CLIENT_ID         ( 1 )

/* The longest client identifier that an MQTT server must accept (as defined
 * by the MQTT 3.1.1 spec) is 23 characters. However some MQTT brokers support
 * longer client IDs. Configure this macro as per the MQTT broker specification.
 */
#define MQTT_CLIENT_IDENTIFIER_MAX_LEN    ( 23 )

/* As per Internet Assigned Numbers Authority (IANA) the port numbers assigned
 * for MQTT protocol are 1883 for non-secure connections and 8883 for secure
 * connections. In some cases there is a need to use other ports for MQTT like
 * port 443 (which is reserved for HTTPS). Application Layer Protocol
 * Negotiation (ALPN) is an extension to TLS that allows many protocols to be
 * used over a secure connection. The ALPN ProtocolNameList specifies the
 * protocols that the client would like to use to communicate over TLS.
 *
 * This macro specifies the ALPN Protocol Name to be used that is supported
 * by the MQTT broker in use.
 * Note: For AWS IoT, currently "x-amzn-mqtt-ca" is the only supported ALPN
 *       ProtocolName and it is only supported on port 443.
 *
 * Uncomment the below line and specify the ALPN Protocol Name to use this
 * feature.
 */
// #define MQTT_ALPN_PROTOCOL_NAME           "x-amzn-mqtt-ca"

/* Server Name Indication (SNI) is extension to the Transport Layer Security
 * (TLS) protocol. As required by some MQTT Brokers, SNI typically includes the
 * hostname in the Client Hello message sent during TLS handshake.
 *
 * Uncomment the below line and specify the SNI Host Name to use this extension
 * as specified by the MQTT Broker.
 */
// #define MQTT_SNI_HOSTNAME                 "SNI_HOST_NAME"

/* A Network buffer is allocated for sending and receiving MQTT packets over
 * the network. Specify the size of this buffer using this macro.
 *
 * Note: The minimum buffer size is defined by 'CY_MQTT_MIN_NETWORK_BUFFER_SIZE'
 * macro in the MQTT library. Please ensure this macro value is larger than
 * 'CY_MQTT_MIN_NETWORK_BUFFER_SIZE'.
 */
#define MQTT_NETWORK_BUFFER_SIZE          ( 2 * CY_MQTT_MIN_NETWORK_BUFFER_SIZE )

/* Maximum MQTT connection re-connection limit. */
#define MAX_MQTT_CONN_RETRIES            (150u)

/* MQTT re-connection time interval in milliseconds. */
#define MQTT_CONN_RETRY_INTERVAL_MS      (2000)


/**************** MQTT CLIENT CERTIFICATE CONFIGURATION MACROS ****************/

/* Configure the below credentials in case of a secure MQTT connection. */
/* PEM-encoded client certificate */
#define CLIENT_CERTIFICATE      \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkGgAwIBAgIUY9pmEyrfZOPvf4d/eYvtALXwzfUwDQYJKoZIhvcNAQEL\n" \
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n" \
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MDYxMTA2MDkw\n" \
"MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n" \
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKu4tP9Ql3cAkwC9Ry16\n" \
"KLRI5JageuGFHdPv90AHhjXF5HqLIjlJMIcVZ1OxLY5eR2JNL9o0BFkYZBfBH8Uh\n" \
"BNsvjJ0oKuTJUGUq5JO/+e56n81RWRII7aFQxDLG/yHGdqh/XqeYamh2hCbrjx4c\n" \
"bxphrRsUR8YZcIRkA66yqNRMSUoNhp79nipmWon18eLu+ue6a8JzsSR7lP84pW5O\n" \
"Ejd5PuGvg/aNWnhPqGRtAgu2x1HjXdqoNgNAa3CJj9M2UaAp+c5g48GV7VmxoR6Q\n" \
"/ayXNGcXVSYmGUVMqDZJ0FH4Xi9kwQHMoTzTfC+RydORvUGfNDPFr+LwxYGFpNRN\n" \
"/RkCAwEAAaNgMF4wHwYDVR0jBBgwFoAUusgV+YY+KxLa8gmlxM2lOvN1bc0wHQYD\n" \
"VR0OBBYEFIeoMb8pl6HJz0EAdJ2qeALVOKJTMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n" \
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQA6ACaasarzVQ3BT8xqeyKgV74O\n" \
"ZX1dmWOnqIKOUnxRZchc0Xs4I1frm9EeJoLgneR49uC5ya/biDCpPTRykMBXDSic\n" \
"cgoELROv3gQ3+ukHKyaOO6EZ0g+ZgNK8uU4VlJdE1YxLOTHwWr+qfKMHQOTOdIxt\n" \
"cDWD7RkzbCoWqsX9YZnzQqRWfAYPFF7rOdUmAbrJx7E6uhtrH+YsKK3dvGiLs8MG\n" \
"rVa2A9ZeUyXuDyP06WKCqX+VmGIyXXvxyMK+Gll3jbmt4vLUb46UelcvQwwZhiE9\n" \
"ayWfYzST3GJHfIv4Qd7KujIqRziu66E2KYRYa5tCPhd4v1W1o8GlNSwgIjNP\n" \
"-----END CERTIFICATE-----"

/* PEM-encoded client private key */
#define CLIENT_PRIVATE_KEY          \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEAq7i0/1CXdwCTAL1HLXootEjklqB64YUd0+/3QAeGNcXkeosi\n" \
"OUkwhxVnU7Etjl5HYk0v2jQEWRhkF8EfxSEE2y+MnSgq5MlQZSrkk7/57nqfzVFZ\n" \
"EgjtoVDEMsb/IcZ2qH9ep5hqaHaEJuuPHhxvGmGtGxRHxhlwhGQDrrKo1ExJSg2G\n" \
"nv2eKmZaifXx4u7657prwnOxJHuU/zilbk4SN3k+4a+D9o1aeE+oZG0CC7bHUeNd\n" \
"2qg2A0BrcImP0zZRoCn5zmDjwZXtWbGhHpD9rJc0ZxdVJiYZRUyoNknQUfheL2TB\n" \
"AcyhPNN8L5HJ05G9QZ80M8Wv4vDFgYWk1E39GQIDAQABAoIBABWn+KOOPVvTpbZd\n" \
"KIHSuxlpa/KXEIgqaoWU6MCZclKLv3G45DsHQOh4SYyjdpRSzXvXMia5kqbNzam1\n" \
"QFVzAZLG31veefJadRodG7CKcHGj401Yafw9RgKnFec1c22GAubjEUPfk+PStn3W\n" \
"DTmF6nkQQm181ERmZus7Vb+NZn9HXrjvR4PqzeSAPyZymW0fKy/XpcRdq3iHKB35\n" \
"kjOB1/EA3SLMqWKtpGx1Sd1QtNP632AcpO7G5CunbsgLPEl+G1ZuRD7bf67uOPtG\n" \
"CfHyRLJfhsiLPbVOU2s1aGpiGgAHjY3uEOXfwP8lmGGMi4mJ4kNi7KFxZEHuV5c6\n" \
"svDjfwECgYEA2MW4UWnNRdCGbbV6fpliyEyBU6avvUD0neKmvCV8KxWwvtaK9xy7\n" \
"xuEsU7Q3i7aM/Xe0aXtq3E5962fdiLcO5gBN1w851Z9Aj2qSkFtQOXhN9slpXSty\n" \
"C51DPhVpAD/wr9VGloZ5qggGyJ3rnmgGAPbO4iweYOb2sLha5fO1OvkCgYEAysvx\n" \
"1FCnIuFpnYPdXO7Ggxt9znudzDxnMmw2t+jUhZGTCOWjuUHMJyW4Xh3euwrtiHd3\n" \
"/WdEcfEcAGAUfibED6Visl6FikO58TMzmC5yhY4SoPKEPSiDRyCnpC4eeFkZId66\n" \
"rs3xRxdbImudhnM233hm0o+BOEU2N22RY1yqOyECgYBqlc1rOnqUOVPf3bu9Q+4u\n" \
"Tm/Ikc0XYTjl6OvS1xuWk7O0IglyN86cm1sQTSyCpd/tQU6UDvscF/wSI5/p+Rh6\n" \
"PuwHMpVdVFCKM/ycvklT+LNdBOupxBLvYwQNIrneRZIy4ssyeCyaThgHzJ5t5PgO\n" \
"wUw4KTlGrEnf2sXXC12xAQKBgQCAimyqKsUFsuMC2EZDVYW2LIK9klUe01qF91ln\n" \
"kMjEMNWF2ijAkBga6CnIXh6DaBXPXgpMMFyN7EnXYw8aNvAnCqlYbdkvHmaJn+6g\n" \
"EzC7vqXIJm/zY+5x8xzPT/w4RCFN+qNRkW/Ro9g8JQIf6n9pfiOOay94v0XyNBzn\n" \
"YiD0gQKBgQC7RoRxjjgjkW3y6zl7pTe7zwV6wSbjxhtEtyS7Yl7IKLsX9RosLtkq\n" \
"SuJaC7AOiB99IW50pe4+lv909RVquOtWpx+T0Q16XlOB/isyYHktP9yJtwAEHkCe\n" \
"sqJ0U+63XSO6ujqCAmLr/flByVX6JWyhAhL95cIxsKKSvrpx69Uf3Q==\n" \
"-----END RSA PRIVATE KEY-----"

/* PEM-encoded Root CA certificate */
#define ROOT_CA_CERTIFICATE     \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----"

/******************************************************************************
* Global Variables
*******************************************************************************/
extern cy_mqtt_broker_info_t broker_info;
extern cy_awsport_ssl_credentials_t  *security_info;
extern cy_mqtt_connect_info_t connection_info;


#endif /* MQTT_CLIENT_CONFIG_H_ */
