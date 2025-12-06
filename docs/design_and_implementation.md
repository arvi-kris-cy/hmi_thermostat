[Click here](../README.md) to view the README.

<<<<<<< HEAD
## Design and implementation <TODO>

The design of this application is kept straightforward to help the user get started with code examples on PSOC&trade; Edge MCU devices. All PSOC&trade; Edge E84 MCU applications have a dual-CPU three-project structure to develop code for the CM33 and CM55 cores. The CM33 core has two separate projects for the Secure Processing Environment (SPE) and Non-secure Processing Environment (NSPE). A project folder consists of various subfolders – each denoting a specific aspect of the project. The three project folders are as follows:

**Table 1. Application Projects**
=======
## Design and implementation

The design of this application is minimalistic to get started with code examples on PSOC&trade; Edge MCU devices. All PSOC&trade; Edge E84 MCU applications have a dual-CPU three-project structure to develop code for the CM33 and CM55 cores. The CM33 core has two separate projects for the secure processing environment (SPE) and non-secure processing environment (NSPE). A project folder consists of various subfolders, each denoting a specific aspect of the project. The three project folders are as follows:

**Table 1. Application projects**
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97

Project | Description
--------|------------------------
*proj_cm33_s* | Project for CM33 secure processing environment (SPE)
*proj_cm33_ns* | Project for CM33 non-secure processing environment (NSPE)
*proj_cm55* | CM55 project

<<<<<<< HEAD
In this code example, at device reset, the secured boot process starts from the ROM boot with the Secured Enclave as the Root of Trust. From the Secured Enclave, the boot flow is passed on to the System CPU Subsystem where the secure CM33 application is first started. After all necessary secure configurations, the flow is passed on to the non-secure CM33 application. Resource initialization for this example is performed by this CM33 non-secure project. It configures the system clocks, pins, clock to peripheral connections, and other platform resources. It then enables the CM55 core using the Cy_SysEnableCM55() function and the CM55 core is subsequently put to deepsleep.

This example implements three RTOS tasks: MQTT client, publisher, and subscriber. The main function initializes the BSP and the retarget-io library, and creates the MQTT client task.

The MQTT client task initializes the Wi-Fi connection manager (WCM) and connects to a Wi-Fi access point (AP) using the Wi-Fi network credentials that are configured in *wifi_config.h*. Upon a successful Wi-Fi connection, the task initializes the MQTT library and establishes a connection with the MQTT broker/server.

The MQTT connection is configured to be secure by default; the secure connection requires a client certificate, a private key, and the Root CA certificate of the MQTT broker that are configured in *mqtt_client_config.h*.

After a successful MQTT connection, the subscriber and publisher tasks are created. The MQTT client task then waits for commands from the other two tasks and callbacks to handle events like unexpected disconnections.

The subscriber task initializes the user LED GPIO and subscribes to messages on the topic specified by the `MQTT_SUB_TOPIC` macro that can be configured in *mqtt_client_config.h*. When the subscriber task receives a message from the broker, it turns the user LED ON or OFF depending on whether the received message is "TURN ON" or "TURN OFF" (configured using the `MQTT_DEVICE_ON_MESSAGE` and `MQTT_DEVICE_OFF_MESSAGE` macros).

The publisher task sets up the user button GPIO and configures an interrupt for the button. The ISR notifies the Publisher task upon a button press. The publisher task then publishes messages (*TURN ON* / *TURN OFF*) on the topic specified by the `MQTT_PUB_TOPIC` macro. When the publish operation fails, a message is sent over a queue to the MQTT client task.

An MQTT event callback function `mqtt_event_callback()` invoked by the MQTT library for events like MQTT disconnection and incoming MQTT subscription messages from the MQTT broker. In the case of an MQTT disconnection, the MQTT client task is informed about the disconnection using a message queue. When an MQTT subscription message is received, the subscriber callback function implemented in *subscriber_task.c* is invoked to handle the incoming MQTT message.

The MQTT client task handles unexpected disconnections in the MQTT or Wi-Fi connections by initiating reconnection to restore the Wi-Fi and/or MQTT connections. Upon failure, the publisher and subscriber tasks are deleted, cleanup operations of various libraries are performed, and then the MQTT client task is terminated.
=======
<br>

The proj_cm33_ns folder contains the firmware code for the CM33 non-secure core. It primarily handles communication, WiFi, MQTT client tasks, Sensors task and inter-processor communication (IPC) with the CM55 core. Key components include:
- Initialization of board support package, IPC communication setup, and low power timer for tickless idle mode.
- Tasks for WiFi management, MQTT client, HTTP Client, Sensor fusion and UI message receiving.
- IPC message callback to handle messages from the CM55 core.
- Use of FreeRTOS for task scheduling and management.

The proj_cm55 folder contains the firmware code for the CM55 core, focused on the graphical user interface (GUI), display, touch input, and thermostat event handling. Key components include:
- Initialization of device peripherals, low power timer, and IPC communication with CM33.
- A FreeRTOS graphics task that initializes the graphics subsystem, display controller, GPU interrupts, I2C for touch and display drivers, and the LVGL graphics library.
- Management of display brightness, device connection state, fan speed, thermostat mode, and temperature updates via IPC messages from CM33.
- Use of LVGL for UI rendering and interaction, including QR code display and various UI screens.
- Handling of GPU and display controller interrupts for smooth graphics rendering.
- Together, these two projects form a dual-core firmware system where CM33 handles communication and network tasks, while CM55 manages the user interface and display, communicating via IPC.

This application allows user to evaluate the performance of PSOC&trade; Edge's graphics subsystem using the built-in system monitor component of LVGL. In order to enable the system monitor component, user needs to perform the following steps:
   1. Set `LV_USE_SYSMON` in `lv_conf.h`
   2. Set `configGENERATE_RUN_TIME_STATS` in `FreeRTOSConfig.h`
   3. Build and program the application and observe the performance data (FPS, CPU usage) in bottom-right corner of the display

## Features
1. Graphical user interface using LVGL.
2. Thermostat application with (simulated) temperature, fan speed and operation modes.
3. BLE based Wi-Fi on-boarding through on-screen keyboard and mobile app.
4. Temperature, Humidity, CO2 & Radar based sensor fusion demonstration.
5. Voice Assistance feature.
6. AWS Cloud connectivity & Firmware updates on Over-The-Air (OTA).
7. HTTP Client based demonstartion for fetching real-time weather data and timezone data.
>>>>>>> e976160882b41277609efcbb0f01c52860d8cd97
