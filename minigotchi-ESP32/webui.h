/**
 * webui.h: header files for webui.cpp
 */

#ifndef WEBUI_H
#define WEBUI_H

#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <AsyncWebSocket.h>
#include "ESPAsyncWebServer.h"
#include <DNSServer.h>

class WebUI : public AsyncWebHandler {
public:
  WebUI();
  virtual ~WebUI();
  static void setupServer();
  static bool running;
private:
  static const char html[] PROGMEM;
  static bool canHandle(AsyncWebServerRequest *request);
  static void handleRequest(AsyncWebServerRequest *request);
};

#endif // WEBUI_H
