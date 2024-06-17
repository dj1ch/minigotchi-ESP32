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

#include "parasite.h"
#include "config.h"
#include "display.h"
#include <ArduinoJson.h>
#include <esp_wifi.h>
#include <sstream>
#include <string>
#include <vector>

class Frame {
public:
  static void pack();
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
  static bool sent;
  static size_t frameSize;
  static std::vector<uint8_t> beaconFrame;
  static size_t payloadSize;
  static const size_t chunkSize;

private:
  static void init();
  static void essid();
};

#endif // FRAME_H
