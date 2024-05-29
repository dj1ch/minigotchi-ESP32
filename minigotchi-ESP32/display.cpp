/**
 * display.cpp: handles display support
*/

#include "display.h"
#include <TFT_eSPI.h>  // Include the TFT_eSPI library to enable control of TFT displays using the ESP32 and CYD
TFT_eSPI tft; // Define TFT_eSPI object

Adafruit_SSD1306* Display::adafruit_display = nullptr;
TFT_eSPI* Display::tft_display = nullptr;   // Initialize the static pointer tft_display in the Display class to null

Display::~Display() {
  if (adafruit_display) {
    delete adafruit_display;
  }
  if (tft_display) {   // Check if tft_display is not null (i.e., if a TFT display object exists) and delete it to free up memory
    delete tft_display;
  }
}

void Display::startScreen() {
  if (Config::display) {
    if (Config::screen == "SSD1306") {
      adafruit_display = new Adafruit_SSD1306(SSD1306_SCREEN_WIDTH, SSD1306_SCREEN_HEIGHT, &Wire, SSD1306_OLED_RESET);
      if (!adafruit_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // initialize with the I2C addr 0x3C
        Serial.println(F("SSD1306 allocation failed"));
        return;
      }
      delay(100); // initialize w/ delays to prevent crash
      adafruit_display->display();
      delay(100);
      adafruit_display->clearDisplay();
      adafruit_display->setTextColor(WHITE);
    } else if (Config::screen == "WEMOS_OLED_SHIELD") {
      adafruit_display = new Adafruit_SSD1306(WEMOS_OLED_SHIELD_OLED_RESET);
      if (!adafruit_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // initialize with the I2C addr 0x3C
        Serial.println(F("SSD1306 allocation failed"));
        return;
      }
      delay(100); // initialize w/ delays to prevent crash
      adafruit_display->display();
      delay(100);
      adafruit_display->clearDisplay();
      adafruit_display->setTextColor(WHITE);
    } else if (Config::screen == "CYD") {   // Check if the screen configuration is set to "CYD" and execute the corresponding code
      tft.begin();  // Initialize TFT_eSPI library
      tft.setRotation(1);  // Set display rotation if needed
      tft.fillScreen(TFT_BLACK);  // Fill screen with black color
      tft.setTextColor(TFT_WHITE);  // Set text color to white
      tft.setTextSize(2);  // Set text size
    } else {
      // use wemos shield by default
      adafruit_display = new Adafruit_SSD1306(WEMOS_OLED_SHIELD_OLED_RESET);
      if (!adafruit_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // initialize with the I2C addr 0x3C
        Serial.println(F("SSD1306 allocation failed"));
        return;
      }
      delay(100); // initialize w/ delays to prevent crash
      adafruit_display->display();
      delay(100);
      adafruit_display->clearDisplay();
      adafruit_display->setTextColor(WHITE);
    }
  }
}

void Display::cleanDisplayFace(String text) {
  if (Config::display) {
    if (Config::screen == "CYD") {   // Check if the screen configuration is set to "CYD" and execute the corresponding code
      tft.fillScreen(TFT_BLACK);  // Clear screen with black color
      tft.setTextColor(TFT_WHITE);  // Set text color to WHITE
      tft.setCursor(0, 5);  // Set cursor position
      tft.setTextSize(4);  // Set text size
      tft.println(text);  // Print text
    } else {
      adafruit_display->clearDisplay();
      adafruit_display->setTextSize(2);
      adafruit_display->setCursor(0, 0);
      adafruit_display->println(text);
      adafruit_display->display();
    }
  }
}

void Display::attachSmallText(String text) {
  if (Config::display) {
    if (Config::screen == "CYD") {   // Check if the screen configuration is set to "CYD" and execute the corresponding code
      tft.setTextColor(TFT_WHITE);  // Set text color to WHITE
      tft.setCursor(0, 40);  // Set cursor position
      tft.setTextSize(1);  // Set text size
      tft.println(text);  // Print text
    } else {
      adafruit_display->setCursor(0, 20);
      adafruit_display->setTextSize(1);
      adafruit_display->println(text);
      adafruit_display->display();
    }
  }
}
