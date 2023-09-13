/* Demo code to test Camera, Touch and SDcard feature combine of Makerpad Pro */

// include library files 
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <esp_camera.h>
#include <LovyanGFX.hpp>

#include <FT6236.h> //Capacitive touch panel controller IC FT6236 library
#define TOUCH_THRESHOLD 80
#define I2C_SDA 26
#define I2C_SCL 27
#define STAR_WIDTH 70
#define STAR_HEIGHT 80

//Common SPI pins for Display and SD card
#define HSPI_MOSI 13
#define HSPI_MISO 12
#define HSPI_SCK 14
#define HSPI_CS 15

//ILI9488 display related pins
#define TFT_CS 15
#define TFT_RST 26
#define TFT_DC 33
#define TFT_BL -1

#define TFT_WIDTH 320
#define TFT_HEIGHT 480
#define TFT_ROTATION 3

//SDcard chip select pin
#define SD_CS 4


// create instance for Touch control
FT6236 ts = FT6236();

struct LGFX_SPIConfig
{
    static constexpr spi_host_device_t spi_host = VSPI_HOST;
    static constexpr int dma_channel = 1;
    static constexpr int spi_sclk = HSPI_SCK;
    static constexpr int spi_mosi = HSPI_MOSI;
    static constexpr int spi_miso = HSPI_MISO;
};

static lgfx::LGFX_SPI<LGFX_SPIConfig> tft;
static lgfx::Panel_ILI9488 lgfx_panel;

String sdImageName = "";
int img_count = 0;
int touchXY[2] = {0, 0};  // Holds touch co-ordinates

#define IMG_ARRAY_LENGTH 320 * 240 * 3 

const uint8_t img_rgb888_320_240_head[54] = {
    0x42, 0x4d, 0x36, 0x84, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x36, 0x0, 0x0, 0x0, 0x28, 0x0,
    0x0, 0x0, 0x40, 0x1, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x18, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x84, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

void setup()
{   
    Serial.begin(115200); 

    if (!ts.begin(TOUCH_THRESHOLD, I2C_SDA, I2C_SCL)){
      Serial.println("Unable to start the capacitive touchscreen.");
    } 
  
    Serial.println("Configure and Setup SPI pins!");
    pinMode(SD_CS, OUTPUT); // set SD chip select pin as OUTPUT
    pinMode(TFT_CS, OUTPUT);  // set display chip select pin as OUTPUT

    // Make sure to deactivate one while using other, Low - to activate and High - to deactivate
    digitalWrite(TFT_CS, HIGH);  // logic HIGH will deactivate, SPI display de-selected 
    digitalWrite(SD_CS, HIGH);   // logic HIGH will deactivate, SPI SDcard de-selected 
    
    SPI.begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI); // initialize and configure SPI pins 

    int sd_check = 0;
    // Check configure and check if SDcard available
    digitalWrite(TFT_CS, LOW);  // Turn on SD card SPI bus
    if (!SD.begin(SD_CS, SPI, 40000000))
    {
        Serial.println("Card Mount Failed");
    }
    else
    {
        Serial.println("Card Found!");
        sd_check = 1;
    }
    digitalWrite(SD_CS, HIGH);   // turn off SD card SPI bus
    
    // Select SPI display, perform operation and de-select
    digitalWrite(TFT_CS, LOW);  // logic low will activate, SPI display selected 
    lgfx_TFTsetup();
    tft.begin();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(30, 30);
    tft.println("Hello..!");
    delay(500);
    tft.fillScreen(TFT_BLACK);

    // message for SD card detection on display
    if (sd_check == 1)
    {
       msg_info(4);
    }
    else
    {
       msg_info(3);
    }
    
    tft.fillCircle(440, 160, 30, TFT_RED); // Draw image click button
    digitalWrite(TFT_CS, HIGH);   // logic HIGH will deactivate, SPI display de-selected 

    Serial.println("Setting up Camera!");
    // configure and setup camera 
    camera_setup();
}

void loop()
{
    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();

    tft.pushImage(20, 20, fb->width, fb->height, (lgfx::swap565_t *)fb->buf);
    if (ts.touched()){
       
      TS_Point p = ts.getPoint();
      int16_t x = tft.width() - p.y;
      int16_t y = p.x;

      Serial.print("\n X Y: ");
      Serial.print(x);
      Serial.print(", ");
      Serial.println(y);
      if (420 < x && x < 470)
      {
        if (140 < y && y < 180)
        {
            void *ptrVal = NULL;
            ptrVal = heap_caps_malloc(IMG_ARRAY_LENGTH, MALLOC_CAP_SPIRAM);
            uint8_t *rgb = (uint8_t *)ptrVal;
            fmt2rgb888(fb->buf, fb->len, PIXFORMAT_RGB565, rgb);
            if (store_image(SD, rgb) == -1)
            {
                msg_info(2);
            }
            else{
                msg_info(1);
            }
                
            heap_caps_free(ptrVal);
            rgb = NULL;
         }
      }
      
    }
    
    esp_camera_fb_return(fb);
}

// Initialize and setup Camera
void camera_setup()
{
    // camera config
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 5;
    config.pin_d1 = 18;
    config.pin_d2 = 19;
    config.pin_d3 = 21;
    config.pin_d4 = 36;
    config.pin_d5 = 39;
    config.pin_d6 = 34;
    config.pin_d7 = 35;
    config.pin_xclk = 32;
    config.pin_pclk = 22;
    config.pin_vsync = 25;
    config.pin_href = 23;
    config.pin_sscb_sda = 26;
    config.pin_sscb_scl = 27;
    config.pin_pwdn = -1;
    config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;

    config.pixel_format = PIXFORMAT_RGB565;
    config.frame_size = FRAMESIZE_QVGA; 
    config.jpeg_quality = 10;
    config.fb_count = 1;
   
    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera failed to initialize, error 0x%x", err);
        while (1)
            ;
    }

    sensor_t *s = esp_camera_sensor_get();
    // initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV2640_PID)
    {
        s->set_vflip(s, 0);   // vertical flip
        s->set_hmirror(s, 0); // Horizontal mirror

        s->set_brightness(s, 0); // up the blightness just a bit
        s->set_saturation(s, 1); // lower the saturation
    }
    // drop down frame size for higher initial frame rate
    s->set_framesize(s, FRAMESIZE_QVGA); 
    Serial.print("Done..!");
}


void lgfx_TFTsetup()
{
    lgfx_panel.freq_write = 40000000;
    lgfx_panel.freq_fill = 40000000;
    lgfx_panel.freq_read = 16000000;

    lgfx_panel.spi_cs = TFT_CS;
    lgfx_panel.spi_dc = TFT_DC;
    lgfx_panel.gpio_rst = TFT_RST;
    lgfx_panel.gpio_bl = TFT_BL;

    tft.setPanel(&lgfx_panel);
}

void displayText(String text){
  tft.setTextSize(2);
  tft.setCursor(40, 280);
  tft.print(text);
}

void msg_info(int choice)
{
    digitalWrite(TFT_CS, LOW);  // logic low will activate, SPI display selected
    tft.fillRect(0, 310, 480, 10, TFT_BLACK);
    
    switch (choice)
    {
    case 1:
        tft.setTextColor(TFT_YELLOW);
        displayText("Saved as " + sdImageName);
        delay(1000);
        
        tft.setTextColor(TFT_BLACK);
        displayText("Saved as " + sdImageName);
        break;

    case 2:
        tft.setTextColor(TFT_RED);
        displayText("No sdcard, save issue..!");
        delay(1000);

        tft.setTextColor(TFT_BLACK);
        displayText("No sdcard, save issue..!");
        break;

    case 3:
        tft.setTextColor(TFT_RED);
        displayText("SD card not Found..!");
        delay(1000);
        
        tft.setTextColor(TFT_BLACK);
        displayText("SD card not Found..!");
        break;

    case 4:
        tft.setTextColor(TFT_GREEN);
        displayText("Ready..!");
        delay(1000);
        
        tft.setTextColor(TFT_BLACK);
        displayText("Ready..!");
        break;
            
    default:
        break;
    }
  digitalWrite(TFT_CS, HIGH);   // logic HIGH will deactivate, SPI display de-selected 
}

// Store clicked image to SDcard if present
int store_image(fs::FS &fs, uint8_t *rgb)
{
    digitalWrite(SD_CS, LOW);   // turn on SDcard SPI bus
    
    sdImageName = "/image" + String(img_count) + ".bmp";
    img_count++;

    Serial.println("Image name：" + sdImageName);

    File imgFile = fs.open(sdImageName, "w");
    if (!imgFile)
    {
        Serial.println("Failed to open file for writing");
        return -1;
    }

    imgFile.write(img_rgb888_320_240_head, 54);
    
    imgFile.write(rgb, 230400);

    imgFile.close();
    
    digitalWrite(SD_CS, HIGH);   // turn off SD card SPI bus
    return 0;
}
