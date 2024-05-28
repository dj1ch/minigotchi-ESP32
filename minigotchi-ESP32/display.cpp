/**
 * display.cpp: handles display support
*/

#include "display.h"
#include <TFT_eSPI.h>
TFT_eSPI tft; // Define TFT_eSPI object



void Display::startScreen() {
  if (Config::display) {
    tft.begin(); // Initialize TFT_eSPI library
    tft.setRotation(1); // Set display rotation if needed
    tft.fillScreen(TFT_BLACK); // Fill screen with black color
    tft.setTextColor(TFT_WHITE); // Set text color to white
    tft.setTextSize(2); // Set text size
  }
}

void Display::cleanDisplayFace(String text) {
  if (Config::display) {
    tft.fillScreen(TFT_BLACK); // Clear screen with black color
    tft.setTextColor(TFT_GREEN); // Set text color to green
    tft.setCursor(110, 5); // Set cursor position
    tft.setTextSize(4); // Set text size
    tft.println(text); // Print text
  }
}

void Display::attachSmallText(String text) {
  if (Config::display) {
    tft.setTextColor(TFT_YELLOW); // Set text color to green
    tft.setCursor(50, 80); // Set cursor position
    tft.setTextSize(1); // Set text size
    tft.println(text); // Print text
  }
}