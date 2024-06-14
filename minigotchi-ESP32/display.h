#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <TFT_eSPI.h> // Defines the TFT_eSPI library for CYD
#include <Wire.h>

#define SSD1306_SCREEN_WIDTH 128
#define SSD1306_SCREEN_HEIGHT 64

#define SSD1306_OLED_RESET -1
#define WEMOS_OLED_SHIELD_OLED_RESET 0 // GPIO0

#define SSD1305_SCREEN_WIDTH 128
#define SSD1305_SCREEN_HEIGHT 32

#define SSD1305_OLED_CLK 14
#define SSD1305_OLED_MOSI 13
#define SSD1305_OLED_CS 15
#define SSD1305_OLED_DC 4
#define SSD1305_OLED_RESET 5

#define IDEASPARK_SSD1306_SCL 14
#define IDEASPARK_SSD1306_SDA 12

/** developer note:
 *
 * the TFT_eSPI library may not require this, but these will be here regardless
 *
 */

// Define display parameters for CYD
#define CYD_SCREEN_WIDTH 240
#define CYD_SCREEN_HEIGHT 320

// lilygo t-display-t3
#define T_DISPLAY_S3_WIDTH 320
#define T_DISPLAY_S3_HEIGHT 170

class Display {
public:
  static void startScreen();
  static void updateDisplay(String face);
  static void updateDisplay(String face, String text);
  ~Display();

private:
  static Adafruit_SSD1306 *ssd1306_adafruit_display;
  static Adafruit_SSD1305 *ssd1305_adafruit_display;
  static U8G2_SSD1306_128X64_NONAME_F_SW_I2C *ssd1306_ideaspark_display;
  static TFT_eSPI *tft_display;
  static void clearTft();
  static void printU8G2Data(int x, int y, const char *data);
};

#endif // DISPLAY_H
