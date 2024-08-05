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
 * channel.h: header files for channel.cpp
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include "config.h"
#include "display.h"
#include "minigotchi.h"
#include "mood.h"
#include "parasite.h"
#include <WiFi.h>
#include <esp_wifi.h>

// forward declaration of mood class
class Mood;

class Channel {
public:
  static void init(int initChannel);
  static void cycle();
  static void switchChannel(int newChannel);
  static int getChannel();
  static void checkChannel(int channel);
  static bool isValidChannel(int channel);
  static int channelList[13]; // 13 channels

private:
  static Mood &mood;
  static int randomIndex;
  static int numChannels;
  static int currentChannel;
  static int newChannel;
};

#endif // CHANNEL_H
