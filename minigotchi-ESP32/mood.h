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
 * mood.cpp: header files for mood.cpp
 */

#ifndef MOOD_CPP
#define MOOD_CPP

#include "config.h"
#include "display.h"
#include <Arduino.h>
#include <string>

class Mood {
public:
    static String happy;
    static String sad;
    static String broken;
    static String intense;
    static String looking1;
    static String looking2;
    static String neutral;
    static String sleeping;

    static String getCurrentFace();
    static String getCurrentMood();
    static String getMood(String face);
    static String getFace(String mood);
    static bool checkMood(String previous, String current);
private:
    static String currentMood;
    static String currentFace;
};

#endif // MOOD_CPP