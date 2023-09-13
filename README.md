# Makerpad_Pro_Software
MakerPad Pro is an ESP32-powered SmartScreen Hub with a 3.5-inch touchscreen display offering vivid visuals and a 2 MP camera for image related applications. Ideal for IoT projects and creative DIY electronics.

This GitHub provides getting started guides for Makerpad Pro.

### Features :
- ESP32-WROVER-B module which generic Wi-Fi + Bluetooth® + Bluetooth LE MCU modules that target a wide variety of applications
- Camera for photography related applications
- Capacitive Touch Display 3.5” for user interaction
- Onboard micro SD card storage support for Data logging
- Buzzer for adding audio alert for project
- Type C interface for programming/power 
- Optional GPIO pins breakout for interfacing additional peripherals if camera not used
- Boot and Reset Button 
- Open source LVGL is supported for designing rich GUI for embedded applications.
  
### Specifications :
- ESP32-WROVER-B contains low-power Xtensa® 32-bit LX6 microprocessors, 448 KB of ROM and 520 KB of on-chip SRAM, with 8MB flash and 8MB PSRAM
- WiFi protocols 802.11 b/g/n and Bluetooth v4.2 BR/EDR and Bluetooth LE 
- OV2640 camera built-in having 2M pixel with lens 1/4"
- Camera Pixel Size 2.2 µm x 2.2 µm
- Camera Supports image sizes: UXGA, SXGA, SVGA, and any size scaling down from SXGA to 40x30
- Display Resolution is 320x480 pixel 
- TFT LCD driver is ILI9488
- FT6236 Series capacitive touch panel controller IC
- Board Supply voltage 5V
- Operating voltage of GPIO pins 3.3V
- Operating Temperature: -30°C to 70°C
- Stable Image : 0°C to 50°C
  
### Pinouts :
<img src="https://github.com/sbcshop/Makerpad_Pro_Software/blob/main/images/Makerpad_Pro_pinout.jpg">

- (1) Camera 
- (2) 3.5” Touch Display
- (3) Reset Button
- (4) Boot Button
- (5) Power Status LED
- (6) Type C 
- (7) TF card slot
- (8) GPIOs breakout
- (9) Buzzer
- (10) ESP32-WROVER-B module
  
### Interfacing Details :
- Display interfacing with ESP32
    | ESP32 | Display | Function |
    |---|---|---|
    | IO12 | LCD_CLK | Clock pin of SPI interface for Display|
    | IO11 | LCD_DIN | MOSI (Master OUT Slave IN) pin of SPI interface|
    | IO10 | LCD_CS | Chip Select pin of SPI interface|
    | IO13 | LCD_DC| Data/Command (MISO) pin of SPI interface|
    | IO14 | LCD_RST | Display Reset pin |
    | IO5  | BL | Backlight of display|

- ESP32 and FT6236 Touch controller interfacing
    | ESP32| FT6236 Touch | Function |
    |---|---|---|
    | IO26 | SDA | Serial Data pin of I2C communication |
    | IO27 | SCL | Serial Clock of I2C communication |
  
- Buttons, Buzzer and LED Interfacing with ESP32
    | ESP32 | Hardware | Function |
    |---|---|---|
    |IO0 | BOOT |Boot button |
    |IO7 | SCAN | Scan Button |
  
  
   

### 1. Configure and Setup Development Environment
   - Download Arduino IDE from [official site](https://www.arduino.cc/en/software) and install into your system. We have use Arduino IDE 1.8.19
   - Once installation done will add ESP32 board support into IDE, for this first you need to add below link into preference:
     
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
     
     Select File > Preference, and add link as show in below image,
      <img src= "https://github.com/sbcshop/3.2_Touchsy_ESP-32_Resistive_Software/blob/main/images/preference_board.gif" />
      
   - Now will install ESP32 based different boards as shown in below image, you may get new version option but select _**1.0.5 version**_ which is compatible for Makerpad Pro 

     <img src= "https://github.com/sbcshop/Makerpad_Pro_Software/blob/main/images/install_esp32_board.gif" />
     
   - Once done, keeping default settings select the _**ESP32 WROVER Module**_ with suitable com port (may be different in your case) as shown below, 

     <img src="https://github.com/sbcshop/Makerpad_Pro_Software/blob/main/images/ESP32_board_selection.gif">
     
     
### 2. Installing Libraries
   - Download [library zip file]() provided here in github.
   - Extract and copy files obtain in previous step inside your Document > Arduino > Libraries folder. Make sure to restart Arduino IDE whenever you update or add any libraries.

     <img src= "https://github.com/sbcshop/3.2_Touchsy_ESP-32_Resistive_Software/blob/main/images/library_files_path.png" />
     
### 3. Testing First Code
   - At this step you are all set to test codes, for easy getting started we have provided various demo [example codes]() in github which you can download and try. 
   - Open one example code in Arduino and make sure you have selected correct board with suitable com port, click on upload button to transfer code on ESP32 of Makerpad Pro. Make sure to _press Boot button_ to begin upload process (in terminal you see message -> Connecting....____ ) as shown in below image.
     
     <img src="https://github.com/sbcshop/Makerpad_Pro_Software/blob/main/images/code_upload_process.gif">
   - Checkout other examples below and build your own custom program codes using those references.

### Example Codes
   - [Example 1]() : Display demo code
   - [Example 2]() : Buzzer testing with display code
   - and [Many more...]()

   Now you are ready to try out your own codes, **_Happy Coding!_**
   
## Resources
  * [Schematic](https://github.com/sbcshop/Makerpad_Pro_Hardware/blob/main/Design%20Data/Sch_Scangenie.pdf)
  * [Hardware Files](https://github.com/sbcshop/Makerpad_Pro_Hardware/tree/main)
  * [Step File](https://github.com/sbcshop/Makerpad_Pro_Hardware/blob/main/Mechanical%20Data/Makerpad%20Pro%203d%20step%20file.zip)
  * [Getting Started with ESP32 in Arduino](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
  * [ILI9488_display_driver Datasheet](https://github.com/sbcshop/Makerpad_Pro_Software/blob/main/documents/ILI9488_Data_Sheet.pdf)
  * [OV2640 Camera Datasheet](https://github.com/sbcshop/Makerpad_Pro_Software/blob/main/documents/OV2640_Datasheet.pdf)
  * [ESP32-WROVER-B Datasheet](https://github.com/sbcshop/Makerpad_Pro_Software/blob/main/documents/esp32-wrover-b_datasheet_en.pdf)
  * [Arduino IDE 1 overview](https://docs.arduino.cc/software/ide-v1/tutorials/Environment)



## Related Products
  * [Touchsy - 3.2" Touch LCD Display Based on ESP32 MCU](https://shop.sb-components.co.uk/products/touchsy-3-2-touch-lcd-display-based-on-esp32-mcu) 
   
     ![Touchsy_3_2_with_ESP32](https://shop.sb-components.co.uk/cdn/shop/files/esp.jpg?v=1686900424&width=300)   

  * [Touchsy - 3.2" Touch LCD Display Based on Pico W](https://shop.sb-components.co.uk/products/touchsy-3-2-touch-lcd-display-based-on-pico-w) 
   
     ![Touchsy_3_2_with_PicoW](https://shop.sb-components.co.uk/cdn/shop/files/touchsypicowresitive.jpg?v=1686903806&width=300) 

  * [Touchsy - 3.2" Touch LCD Display Breakout Board](https://shop.sb-components.co.uk/products/touchsy-3-2-touch-lcd-display-breakout-board) 
   
     ![Touchsy_3_2_Breakout](https://shop.sb-components.co.uk/cdn/shop/files/resistivetouchsy.jpg?v=1686908046&width=300)
    
  * [Touchsy - 3.2" Touch LCD Display for Raspberry Pi](https://shop.sb-components.co.uk/products/touchsy-3-2-touch-lcd-display-for-raspberry-pi) 
   
     ![Touchsy_3_2_HAT](https://shop.sb-components.co.uk/cdn/shop/files/hat.jpg?v=1686898617&width=300) 

 
## Product License

This is ***open source*** product. Kindly check LICENSE.md file for more information.

Please contact support@sb-components.co.uk for technical support.
<p align="center">
  <img width="360" height="100" src="https://cdn.shopify.com/s/files/1/1217/2104/files/Logo_sb_component_3.png?v=1666086771&width=300">
</p>
