/**
 * display.h: header files for display.cpp
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
//#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>

class Display {
public:
  static void startScreen();
  static void cleanDisplayFace(String text);
  static void attachSmallText(String text);
};

#endif // DISPLAY_H