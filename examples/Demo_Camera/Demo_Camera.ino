/* Demo code to test Onboard Camera module of Makerpad Pro */

// include library files 
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <esp_camera.h>
#include <LovyanGFX.hpp>

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

void setup()
{   
    Serial.begin(115200); 
    Serial.println("Configure and Setup SPI pins!");
    SPI.begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI); // initialize and configure SPI pins 

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
    digitalWrite(TFT_CS, HIGH);   // logic HIGH will deactivate, SPI display de-selected 

    Serial.println("Setting up Camera!");
    // configure and setup camera 
    camera_setup();
}

void loop()
{
    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();

    tft.pushImage(40, 10, fb->width, fb->height, (lgfx::swap565_t *)fb->buf);
    
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
    config.frame_size = FRAMESIZE_CIF; // 400x296, 
    config.jpeg_quality = 10;
    config.fb_count = 1;
    
    /* Other Frame Options available: 
      FRAMESIZE_96X96,    // 96x96
      FRAMESIZE_QQVGA,    // 160x120
      FRAMESIZE_QCIF,     // 176x144
      FRAMESIZE_HQVGA,    // 240x176
      FRAMESIZE_240X240,  // 240x240
      FRAMESIZE_QVGA,     // 320x240
      FRAMESIZE_CIF,      // 400x296
      FRAMESIZE_HVGA,     // 480x320
     */
    
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
    s->set_framesize(s, FRAMESIZE_CIF); // 400x296
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
