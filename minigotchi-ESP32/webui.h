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
 * webui.h: header files for webui.cpp
 */

#ifndef WEBUI_H
#define WEBUI_H

#include "config.h"
#include "minigotchi.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h> // using https://github.com/bmorcelli/ESPAsyncWebServer pirata is the GOAT!!!
#include <WiFi.h>

// forward declaration of mood class
class Mood;

// this is the actual class we use
class WebUI {
public:
  WebUI();
  ~WebUI();
  static void setupServer();
  static void updateWhitelist(String newWhitelist);
  static const char html[] PROGMEM;
  static bool running;

private:
  static Mood &mood;
};

#endif // WEBUI_H
