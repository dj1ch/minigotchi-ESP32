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
 * frame.h: header files for frame.cpp
 */

#ifndef FRAME_H
#define FRAME_H

#include "config.h"
#include "display.h"
#include "parasite.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <sstream>
#include <string>
#include <vector>

// forward declaration of mood class
class Mood;

class Frame {
public:
  static uint8_t *pack();
  static uint8_t *packModified();
  static bool send();
  static void advertise();
  static const uint8_t header[];
  static const uint8_t IDWhisperPayload;
  static const uint8_t IDWhisperCompression;
  static const uint8_t IDWhisperIdentity;
  static const uint8_t IDWhisperSignature;
  static const uint8_t IDWhisperStreamHeader;
  static const uint8_t SignatureAddr[];
  static const uint8_t BroadcastAddr[];
  static const uint16_t wpaFlags;

  static const int pwngridHeaderLength;
  static size_t essidLength;
  static uint8_t headerLength;

  static size_t payloadSize;
  static const size_t chunkSize;

private:
  static Mood &mood;
};

#endif // FRAME_H
