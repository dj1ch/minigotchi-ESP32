/**
 * webui.h: header files for webui.cpp
 */

#ifndef WEBUI_H
#define WEBUI_H

#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> // using https://github.com/bmorcelli/ESPAsyncWebServer pirata is the GOAT!!!
#include <DNSServer.h>

// forward declaration of mood class
class Mood;

// this is the actual class we use
class WebUI {
public:
  WebUI();
  ~WebUI();
  static void setupServer();
  static const char html[] PROGMEM;
  static bool running;
private:
  static Mood &mood;
};

#endif // WEBUI_H
