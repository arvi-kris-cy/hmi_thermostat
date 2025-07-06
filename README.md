# PSOC&trade; Edge HMI Kit: Smart Thermostat

## Thermostat

This demo application implements a smart thermostat panel user interface that allows a user to interact, monitor and control room temperature, either directly via the touchscreen, using voice commands or remotely via mobile app over a cloud-connected platform. The system also responds to radar-based presence detection to automatically adjust operating modes.

## Design and Implementation
### *Currently Implemented Features*
1. Graphical user interface using LVGL
2. Thermostat application with (simulated) temperature, fan speed and operation modes
3. BLE based Wi-Fi on-boarding through on-screen keyboard and mobile app


### Tasks executing on each CPU

The proj_cm33_ns folder contains the firmware code for the CM33 non-secure core. It primarily handles communication, WiFi, MQTT client tasks, and inter-processor communication (IPC) with the CM55 core. Key components include:
- Initialization of board support package, IPC communication setup, and low power timer for tickless idle mode.
- Tasks for WiFi management, MQTT client, and UI message receiving.
- IPC message callback to handle messages from the CM55 core.
- Use of FreeRTOS for task scheduling and management.

The proj_cm55 folder contains the firmware code for the CM55 core, focused on the graphical user interface (GUI), display, touch input, and thermostat event handling. Key components include:
- Initialization of device peripherals, low power timer, and IPC communication with CM33.
- A FreeRTOS graphics task that initializes the graphics subsystem, display controller, GPU interrupts, I2C for touch and display drivers, and the LVGL graphics library.
- Management of display brightness, device connection state, fan speed, thermostat mode, and temperature updates via IPC messages from CM33.
- Use of LVGL for UI rendering and interaction, including QR code display and various UI screens.
- Handling of GPU and display controller interrupts for smooth graphics rendering.
- Together, these two projects form a dual-core firmware system where CM33 handles communication and network tasks, while CM55 manages the user interface and display, communicating via IPC.


## Document history


Document title: *PSOC&trade; Edge HMI Kit: Smart Thermostat*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | 
 1.1.0   | 

All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.

---------------------------------------------------------

© Cypress Semiconductor Corporation, 2024-2025. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress's patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress's published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, ModusToolbox, PSOC, CAPSENSE, EZ-USB, F-RAM, and TRAVEO are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit www.infineon.com. Other names and brands may be claimed as property of their respective owners.


