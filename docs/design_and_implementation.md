[Click here](../README.md) to view the README.

## Design and implementation

This project supports two displays -

   • [Waveshare 7-inch Raspberry Pi DSI LCD C Display](https://www.waveshare.com/7inch-dsi-lcd-c.htm): The LCD houses a Chipone ICN6211 display controller and uses the MIPI DSI interface.

   • 10.1 inch [WF101JTYAHMNB0](https://www.winstar.com.tw/products/tft-lcd/ips-tft/ips-touch.html) TFT LCD: The TFT LCD houses a [EK79007AD3](https://www.crystalfontz.com/controllers/Fitipower/EK79007AD3/505/) display controller and uses the MIPI DSI interface. This display is supported by default.

The design of this application is kept straightforward to help the user get started with code examples on PSOC&trade; Edge MCU devices. All PSOC&trade; Edge E84 MCU applications have a dual-CPU three-project structure to develop code for the CM33 and CM55 cores. The CM33 core has two separate projects for the Secure Processing Environment (SPE) and Non-secure Processing Environment (NSPE). A project folder consists of various subfolders – each denoting a specific aspect of the project. The three project folders are as follows:

**Table 1. Application Projects**

Project | Description
--------|------------------------
proj_cm33_s | Project for CM33 Secure Processing Environment (SPE)
proj_cm33_ns | Project for CM33 Non-secure Processing Environment (NSPE)
proj_cm55 | CM55 Project

In this code example, at device reset, the secured boot process starts from the ROM boot with the Secured Enclave as the Root of Trust. From the Secured Enclave, the boot flow is passed on to the System CPU Subsystem where the secure CM33 application is first started. After all necessary secure configurations, the flow is passed on to the non-secure CM33 application. Resource initialization for this example is performed by this CM33 non-secure project. It configures the system clocks, pins, clock to peripheral connections, and other platform resources. It then enables the CM55 core using the Cy_SysEnableCM55() function and enters to DeepSleep mode.

The CM55 application drives the LCD and renders the image using PSOC&trade; Edge graphics subsystem. The graphics subsystem of PSOC&trade; Edge MCU houses an independent 2.5D graphics processing unit (GPU), a display controller (DC), and a MIPI DSI host controller with MIPI D-PHY physical layer interface.

**cm55_gfx_task** initializes the Graphics subsystem and configures the DC and GPU interrupts. After that it initializes the LCD panel based on selection through _<application\>/proj_cm55/Makefile_. Once the panel is initialized, required amount of memory is allocated for VGLite draw/blit functions to be consumed by LVGL library.
The `lv_init()` function is used to initialize LVGL and set up the essential components required for LVGL to work correctly. The display and touch drivers are initialized using `lv_port_disp_init()` and `lv_port_indev_init()` functions respectively. The LVGL demo music player is displayed on the display by calling the LVGL demo widget API `lv_demo_music()`.
In order to switch to other available LVGL demos, user need to enable the `LV_USE_DEMO_<demo_name>` macro in `lv_conf.h` file and call the corresponding `lv_demo_<demo_name>()` API in place of `lv_demo_music()`.

This application allows user to evaluate the performance of PSOC&trade; Edge's graphics subsystem using built in system monitor component of LVGL. 
In order to enable the system monitor component, user needs to perform following steps:
   1. Set `LV_USE_SYSMON` in `lv_conf.h`
   2. Set `configGENERATE_RUN_TIME_STATS` in `FreeRTOSConfig.h`
   3. Post these 2 steps, build and program the application and observe the performance data (FPS, CPU usage) in botton-right corner of the display.

<br />