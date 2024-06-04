#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h>  // Defines the TFT_eSPI library for CYD
#include "config.h"

// SSD1306 screen
#define SSD1306_SCREEN_WIDTH 128
#define SSD1306_SCREEN_HEIGHT 64

// Init screen
#define SSD1306_OLED_RESET -1 
#define WEMOS_OLED_SHIELD_OLED_RESET 0 // GPIO0

/** developer note:
 * 
 * the TFT_eSPI library may not require this, but these will be here regardless
 * 
*/

// Define display parameters for CYD
#define CYD_SCREEN_WIDTH 240
#define CYD_SCREEN_HEIGHT 320

// lilygo t-display-t3
#define T_DISPLAY_T3_WIDTH 320
#define T_DISPLAY_T3_HEIGHT 170

class Display {
public:
  static void startScreen();
  static void cleanDisplayFace(String text);
  static void attachSmallText(String text);
  ~Display();

private:
  static Adafruit_SSD1306* adafruit_display;
  static TFT_eSPI* tft_display;  // Declare a static pointer to a TFT_eSPI object to manage the TFT display.
};

#endif // DISPLAY_H
