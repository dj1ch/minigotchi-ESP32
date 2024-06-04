/**
 * display.cpp: handles display support
*/

#include "display.h"

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
    } else if (Config::screen == "T_DISPLAY_T3") {
      tft.begin();
      tft.setRotation(1);
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(2);
    } else {
      // use wemos shield by default
      adafruit_display = new Adafruit_SSD1306(WEMOS_OLED_SHIELD_OLED_RESET);
      if (!adafruit_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // initialize with the I2C addr 0x3C
        Serial.println(F("SSD1306 allocation failed"));
        return;
      }
      delay(5); // initialize w/ delays to prevent crash
      adafruit_display->display();
      delay(5);
      adafruit_display->clearDisplay();
	    delay(5);
      adafruit_display->setTextColor(WHITE);
	    delay(5);
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
    } else if (Config::screen == "T_DISPLAY_T3") {
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(0, 5);
      tft.setTextSize(4);
      tft.println(text);
    } else {
      adafruit_display->clearDisplay();
	    delay(5);
      adafruit_display->setTextSize(2);
	    delay(5);
      adafruit_display->setCursor(0, 0);
	    delay(5);
      adafruit_display->println(text);
	    delay(5);
      adafruit_display->display();
	    delay(5);
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
    } else if (Config::screen == "T_DISPLAY_T3") {
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(0, 40);
      tft.setTextSize(1);
      tft.println(text);
    } else {
      adafruit_display->setCursor(0, 20);
	    delay(5);
      adafruit_display->setTextSize(1);
	    delay(5);
      adafruit_display->println(text);
	    delay(5);
      adafruit_display->display();
	    delay(5);
    }
  }
}
