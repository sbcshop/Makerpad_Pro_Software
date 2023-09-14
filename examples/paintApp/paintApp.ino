/* Paint App on Makerpad Pro */

// include library files 
#include <SPI.h>
#include <SD.h>
#include <FS.h>
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

int touchXY[2] = {0, 0};  // Holds touch co-ordinates

int paint_color = TFT_WHITE;

void setup()
{   
    Serial.begin(115200); 

    if (!ts.begin(TOUCH_THRESHOLD, I2C_SDA, I2C_SCL)){
      Serial.println("Unable to start the capacitive touchscreen.");
    } 
  
    Serial.println("Configure and Setup SPI pins!");
    pinMode(TFT_CS, OUTPUT);  // set display chip select pin as OUTPUT
    
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
     
    createColorPanel(); // Draw Color Panel

}

void loop()
{
    if (ts.touched()){
       
      TS_Point p = ts.getPoint();
      int16_t x = tft.width() - p.y;
      int16_t y = p.x;

      Serial.print("\n X Y: ");
      Serial.print(x);
      Serial.print(", ");
      Serial.println(y);
      if (0 < y && y < 60){
        if (0 < x && x < 80)
          {
            paint_color = TFT_RED ; 
          }
        else if (80 < x && x < 160)
          {
            paint_color = TFT_GREEN ; 
          }
         else if (160 < x && x < 240)
          {
            paint_color = TFT_BLUE ;
          }
         else if (240 < x && x < 320)
          {
             paint_color = TFT_YELLOW ;
           }
         else if (320 < x && x < 400)
          {
            paint_color = TFT_CYAN ;
          }
         else if (400 < x && x < 480)
          {
            paint_color = TFT_WHITE ;
          }
      }
      else {
        tft.fillCircle(x, y, 2, paint_color);
      }

    }
   
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

void createColorPanel(){
  tft.fillRect(0, 0, 80,60, TFT_RED);
  tft.fillRect(80, 0, 80,60, TFT_GREEN);
  tft.fillRect(160, 0, 80,60, TFT_BLUE);
  tft.fillRect(240, 0, 80,60, TFT_YELLOW);
  tft.fillRect(320, 0, 80,60, TFT_CYAN);
  tft.fillRect(400, 0, 80,60, TFT_WHITE);
}
