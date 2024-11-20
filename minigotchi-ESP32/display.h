/*
 * Minigotchi: An even smaller Pwnagotchi
 * Copyright (C) 2024 dj1ch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * display.h: header files for display.cpp
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include "mood.h"

// words cannot describe how much space this has saved me
#if disp
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Defines the TFT_eSPI library for CYD
#include <U8g2lib.h>
#include <Wire.h>
#endif

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

#define SH1106_SCREEN_WIDTH 128
#define SH1106_SCREEN_HEIGHT 64
#define SH1106_OLED_RESET -1

#define SH1106_SCL 5
#define SH1106_SDA 4

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
  static void printU8G2Data(int x, int y, const char *data);
  static String storedFace;
  static String previousFace;
  static String storedText;
  static String previousText;
  ~Display();

private:
#if disp
  static Adafruit_SSD1306 *ssd1306_adafruit_display;
  static Adafruit_SSD1305 *ssd1305_adafruit_display;
  static U8G2_SSD1306_128X64_NONAME_F_SW_I2C *ssd1306_ideaspark_display;
  static U8G2_SH1106_128X64_NONAME_F_SW_I2C *sh1106_adafruit_display;
  static TFT_eSPI *tft_display;
#endif
};

#endif // DISPLAY_H
