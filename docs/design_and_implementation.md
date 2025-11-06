[Click here](../README.md) to view the README.

## Design and implementation

The design of this application is minimalistic to get started with code examples on PSOC&trade; Edge MCU devices. All PSOC&trade; Edge E84 MCU applications have a dual-CPU three-project structure to develop code for the CM33 and CM55 cores. The CM33 core has two separate projects for the secure processing environment (SPE) and non-secure processing environment (NSPE). A project folder consists of various subfolders, each denoting a specific aspect of the project. The three project folders are as follows:

**Table 1. Application projects**

Project | Description
--------|------------------------
*proj_cm33_s* | Project for CM33 secure processing environment (SPE)
*proj_cm33_ns* | Project for CM33 non-secure processing environment (NSPE)
*proj_cm55* | CM55 project

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