# PSOC™ Edge HMI Kit: Smart Thermostat

This demo application implements a smart thermostat panel user interface that allows a user to interact, monitor and control room temperature, either directly via the touchscreen, using voice commands or remotely via mobile app over a cloud-connected platform. The system also responds to radar-based presence detection to automatically adjust operating modes.

See [Design and Implementation](docs/design_and_implementation.md) for the functional description of this code example. <br>

## Requirements

- [ModusToolbox&trade; software](https://www.infineon.com/modustoolbox) v3.5 or later (tested with v3.5)
- ModusToolbox&trade; PSOC&trade; Edge E84 Early Access Pack. See [Software setup](#software-setup) for more details.
- Board support package (BSP) minimum required version for:
   - KIT_PSE84_HMI: v0.0.1
- Programming language: C
- Associated parts: All PSOC&trade; Edge E84 MCU parts

## Supported toolchains (make variable 'TOOLCHAIN')

- Arm&reg; Compiler v6.22 (`ARM`)

## Supported kits (make variable 'TARGET')

- **PSOC&trade; Edge E84 Evaluation Kit** (Minimum required revision: Rev *D) <br>
   - `KIT_PSE84_HMI` - Default value of `TARGET` <br> 

## Software setup

See the [ModusToolbox&trade; tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package. <br>

<details><summary><b>ModusToolbox&trade; PSOC&trade; Edge E84 Early Access Pack</b></summary>

See _**AN235935** – Getting started with PSOC&trade; Edge E84 MCU on ModusToolbox&trade; software_ for detailed instructions on using the ModusToolbox&trade; Setup Program to install the PSOC&trade; Edge E84 Early Access Pack. A brief overview of the instructions is provided below.
 
1. Download and install the [ModusToolbox&trade; Setup Program](https://softwaretools.infineon.com/tools/com.ifx.tb.tool.modustoolboxsetup)
2. Run the ModusToolbox&trade; Setup Program and log in using your Infineon credentials. 
3. Select the “ModusToolbox&trade; PSOC&trade; Edge E84 Early Access Pack” under additional packages by clicking on the corresponding check box and click on `Next`. 
4. On the next dialog box, the packages which are ready to be installed are listed. You may access the 'Release Notes' and 'License' by clicking on the respective links. You may also choose a custom installation directory for Early Access Pack by clicking on 'Customize Installation'.
    > **Note:** The default installation directory of the Early Access pack is the root "ModusToolbox" installation directory of the respective operating system.<br>
    > **Note:** Do not exceed 120 characters for installation path of Early Access Pack in a non-default
    location in Windows operating system<br>
5. Select the check box to agree to the license and click on `Start`. The ModusToolbox&trade; PSOC&trade; Edge E84 Early Access Pack will be downloaded and installed. 
6. After installing the Early Access Pack, you can use the ModusToolbox&trade; Settings tool to enable the early access environment. The Settings tool (available from ModusToolbox&trade; v3.5 onwards) is a stand-alone tool and can be launched from other ModusToolbox&trade; tools such as Dashboard, BSP Assistant, Project Creator or Library Manager.  
Alternatively, you may also choose to add the following system variable manually. 
    > Variable name: <code>MTB_ENABLE_EARLY_ACCESS</code> <br>
    Variable value: <code>com.ifx.tb.tool.modustoolboxpackpsoce84</code>
7. Save the Environment variables and restart ModusToolbox&trade; software. 

</details>

## Sequence of Operation:

### Through HMI
1. Power the HMI kit. 
2. UIs interfaces
<br>
![](Imgs/Home.png)
</br>
    - <b>Controlled UIs</b>

        1. Circular bar  : - To control room temperature.
        2. Fan icon : - To control room cooling based on fan speed.
        3. Setting icon : - To navigate settings panel.
            - Display settings : - To change screen brightness, ....
            - Audio Settings : - To control notification volume, ....
            - Sensor Settings : - To ....
            - Performance Settings : - To .... 
            - System Settings : - To change screen timeout, To change temperature unit, To factory reset of unit, To check firmware updates, To know about device version, .... 
        4. Presence icon : - To navigate list of radar commands.
        5. Mode icon : - To change thermostat mode. 
        6. Command icon : - To navigate list of voice assistance commands available. 
        7. Bluetooth icon : - To establish cloud connectivity over MAPP (Mobile app)

    - <b>Monitor UIs</b>

        8. Time and Date : - To monitor current time and date (Based on Timezone).
        9. CO2 : - To monitor current CO2 concerntration of the room.
        10. Temperature : -  To monitor current Temperature of the room.
        11. Location and Weather : - To monitor current temperature & weather forcast of the location.

### Through Mobile Application (MAPP)
1. Power the HMI kit.
2. Tap on the Bluetooth icon in thermostat app, HMI kit.
3. Open the MAPP in mobile and tap on the plus icon from home. 
<br>
![](Imgs/OpenMAPP_and_PlusIcon.png)
</br>
4. Select thermostat and scan nearby thermostat device.
<br>
![](Imgs/SelectThermostat_and_Scan.png)
</br>
5. Select the thermostat from device list & Pair the thermostat via bluetooth.
<br>
![](Imgs/BLE_Connection.png)
</br>
6. Select the Wi-Fi network you want your thermostat to connect with. Enter the passkey and tap on connect.
<br>
![](Imgs/WiFi_Connection.png)
</br>
7. Monitor <b>Wi-Fi Connected & Cloud Connected</b> in HMI kit. Monitor <b>Device is Online</b> in MAPP.
<br>
![](Imgs/Device_Is_Online.png)
</br>
8. Now, User can control thermostat parameters over MAPP.
<br>
![](Imgs/Device_Control.png)
</br>

## Related resources

Resources  | Links
-----------|----------------------------------
Application notes  | **
Code examples  | **
Device documentation | **
Development kits | **
Libraries  | **
Tools  | **

## Other resources

Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

## Document history

Document title: *PSOC&trade; Edge HMI Kit: Smart Thermostat*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example

All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.

---------------------------------------------------------

© Cypress Semiconductor Corporation, 2023-2025. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress's patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress's published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, ModusToolbox, PSOC, CAPSENSE, EZ-USB, F-RAM, and TRAVEO are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit www.infineon.com. Other names and brands may be claimed as property of their respective owners.