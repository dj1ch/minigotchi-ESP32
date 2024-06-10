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
 * minigotchi.ino: everything implemented here
*/

#include "config.h"
#include "minigotchi.h"

Config config;
Minigotchi minigotchi;

void setup() {
    Serial.begin(config.baud);
    minigotchi.boot();
}

/** developer note:
 *
 * everything should have been moved to minigotchi.cpp
 * as the program becomes more complicated, it gets easier to maintain all the main features in one file
 *
*/

void loop() {
    // cycle channels at start of loop
    minigotchi.cycle();
    delay(250);

    // the longer we are on this channel, the more likely we're gonna see a pwnagotchi on this channel
    // get local payload from local pwnagotchi, send raw frame if one is found
    minigotchi.detect();
    delay(250);

    // advertise our presence with the help of pwngrid compatible beacon frames (probably the most confusing part lmao)
    minigotchi.advertise();
    delay(250);

    // deauth random access point
    minigotchi.deauth();
    delay(250);
}
