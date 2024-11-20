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
 * ble.h: header files for ble.cpp
 */

#ifndef BLE_H
#define BLE_H

#include "config.h"
#include "mood.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// forward declaration of mood class
class Mood;

class Ble {
public:
  static void spam();

private:
  static int random(int min, int max);
  static void init();
  static void start();
  static void stop();

  static Mood &mood;
  static BLEAdvertising *pAdvertising;
  static int deviceType;
  static int delaySeconds;
  static int advType;
};

#endif // BLE_H
