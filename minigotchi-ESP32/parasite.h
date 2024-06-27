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
 * parasite.h: header files for parasite.cpp
 */

#ifndef PARASITE_H
#define PARASITE_H

#include "channel.h"
#include "config.h"
#include "deauth.h"
#include "frame.h"
#include "pwnagotchi.h"
#include <Arduino.h>
#include <ArduinoJson.h>

typedef enum {
  SCANNING = 200,
  FRIEND_FOUND = 201,
  NO_FRIEND_FOUND = 202,
  FRIEND_SCAN_ERROR = 250
} parasite_pwnagotchi_scan_type_t;

typedef enum {
  SYNCED_CHANNEL = 200,
  RANDOM_CHANNEL = 201,
} parasite_channel_status_type_t;

typedef enum {
  START_SCAN = 200,
  PICKED_AP = 201,
  NO_APS = 202,
  START_DEAUTH = 203,
  SKIPPING_WHITELIST = 210,
  SKIPPING_UNENCRYPTED = 211,
  DEAUTH_SCAN_ERROR = 250
} parasite_deauth_status_type_t;

class Parasite {
public:
  static void readData();
  static void sendChannelStatus(parasite_channel_status_type_t status);
  static void sendName();
  static void sendAdvertising();
  static void sendPwnagotchiStatus(parasite_pwnagotchi_scan_type_t status);
  static void sendPwnagotchiStatus(parasite_pwnagotchi_scan_type_t status,
                                   const char *frd);
  static void sendDeauthStatus(parasite_deauth_status_type_t status);
  static void sendDeauthStatus(parasite_deauth_status_type_t status,
                               const char *target, int channel);
  static int channel;

private:
  static void sendData(const char *command, uint8_t status, const char *data);
  static void formatData(char *buf, const char *data, size_t bufSize);
};

#endif // PARASITE_H
