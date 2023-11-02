#include <TFT_eSPI.h> // include display control library
#include <SPI.h>
#include <FT6236.h> //Capacitive touch panel controller IC FT6236 library
#include "Free_Fonts.h"  // include this header for custom fonts

#define TOUCH_THRESHOLD 80
#define I2C_SDA 26
#define I2C_SCL 27
#define STAR_WIDTH 70
#define STAR_HEIGHT 80

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

TFT_eSprite sprImg = TFT_eSprite(&tft);

FT6236 ts = FT6236();
unsigned long previousTouchTimestamp = 0;
unsigned long touchCheckInterval = 10;

#define TFT_BACK_LIGHT 5  // GPIO to which backlight of TFT connected

void setup(void) {
  pinMode( TFT_BACK_LIGHT, OUTPUT );    // set backlight pin as OUTPUT
  digitalWrite( TFT_BACK_LIGHT, HIGH ); // activate backlight

  Serial.begin(115200);
  
  if (!ts.begin(TOUCH_THRESHOLD, I2C_SDA, I2C_SCL)){
      Serial.println("Unable to start the capacitive touchscreen.");
  } 
  
  tft.init();  // initialize display
  tft.setRotation(3); // set suitable rotation for display
  tft.fillScreen(TFT_BLACK); // background color setting

  tft.setTextSize(1); // to normal Size of Text
  tft.fillScreen(TFT_BLACK); // background color
  tft.setTextColor(TFT_CYAN, TFT_BLACK); // set text color
  tft.setFreeFont(FF44); // set Font type
  tft.drawString("Makerpad Pro", 110, 40, GFXFF);

  tft.setFreeFont(FSI12);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);        
  tft.drawString("* Capacitive *", 175, 120, GFXFF);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // set text color
  tft.setFreeFont(FF18); // set Font type
  tft.drawString("touch, hold and move!", 120, 280, GFXFF);
  delay(2000);
  
  tft.fillScreen(TFT_BLACK); // background color
}

void loop() {
  getTouchEvent(); // Check for any Touch Event and responds to it!
  delay(1);
}


void getTouchEvent(){
  
  unsigned long currentTouchTimestamp = millis();
  bool needRefresh = true;

  if (currentTouchTimestamp - previousTouchTimestamp > touchCheckInterval) {
    previousTouchTimestamp = currentTouchTimestamp;
    
    for(int i=0; i< ts.touched(); i++){
      TS_Point p = ts.getPoint(i);
      int16_t x = tft.width() - p.y;
      int16_t y = p.x;
    
      drawStar(i, x, y, random(0x10000));
      needRefresh = false;
    }

    if (needRefresh) {
      tft.fillScreen(TFT_BLACK); // background color
    }
    
  }
}

void drawStar(int id, int x, int y, int star_color)
{
  // Create an 8 bit sprite 70x 80 pixels (uses 5600 bytes of RAM)
  sprImg.setColorDepth(8);
  sprImg.createSprite(STAR_WIDTH, STAR_HEIGHT);

  // Fill Sprite with a "transparent" colour
  // TFT_TRANSPARENT is already defined for convenience
  // We could also fill with any colour as "transparent" and later specify that
  // same colour when we push the Sprite onto the screen.
  sprImg.fillSprite(TFT_TRANSPARENT);

  // Draw 2 triangles to create a filled in star
  sprImg.fillTriangle(35, 0, 0,59, 69,59, star_color);
  sprImg.fillTriangle(35,79, 0,20, 69,20, star_color);

  // Punch a star shaped hole in the middle with a smaller transparent star
  sprImg.fillTriangle(35, 7, 6,56, 63,56, TFT_TRANSPARENT);
  sprImg.fillTriangle(35,73, 6,24, 63,24, TFT_TRANSPARENT);

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // Specify what colour is to be treated as transparent.
   
  //sprImg.drawString((String)id, 0, 0, 4); // Continue printing from new x position
  sprImg.pushSprite(x - STAR_WIDTH /2, y - STAR_HEIGHT/2, TFT_TRANSPARENT);

  // Delete it to free memory
  sprImg.deleteSprite(); 
}
