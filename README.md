# TFT-bitcoin-display
TFT (Thin Film Transistor) Bitcoin Display, LILYGO T-Display-S3 ESP32-S3.  

<img src="https://github.com/emuro2/TFT-bitcoin-display/assets/3782936/2ca29a34-58b5-4ba1-82fc-a4561aa8e648" width="200"/>

## Start Here:
https://github.com/Xinyuan-LilyGO/T-Display-S3


## Arduino Setup and Init:
1. In Arduino Preferences, on the Settings tab, enter the `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` URL in the `Additional boards manager URLs` input box. 
2. Click OK and the software will install. 
3. Search for ESP32 in Tools → Board Manager and install `esp32` (Arduino-ESP32)
4. Copy everything from the [T-Display-S3 repository](https://github.com/Xinyuan-LilyGO/T-Display-S3) lib folder to [Arduino library folder](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries#manual-installation) (e.g. `C:\Users\YourName\Documents\Arduino\libraries`)
    - Backup TFT_eSPI folder is copied in this repo's lib folder.
5. Select the correct settings in the Tools menu as shown below.

| Setting                  | Value                            |
| :----------------------- | :------------------------------- |
| USB CDC On Boot          | Enabled                          |
| CPU Frequency            | 240MHz (WiFi)                    |
| Core Debug Level         | None                             |
| USB DFU On Boot          | Enabled                          |
| Events Run On            | Core 1                           |
| Flash Mode               | QIO 80MHz                        |
| Flash Size               | 16MB (128Mb)                     |
| JTAG Adapter             | Integrated USB JTAG              |
| Arduino Runs On          | Core 1                           |
| USB Firmware MSC On Boot | Disabled                         |
| Partition Scheme         | Huge APP (3MB No OTA/1MB SPIFFS) |
| PSRAM                    | OPI PSRAM                        |
| USB Mode                 | Hardware CDC and JTAG            |
