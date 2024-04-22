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
    delay(5000);

    // the longer we are on this channel, the more likely we're gonna see a pwnagotchi on this channel
    // get local payload from local pwnagotchi, send raw frame if one is found
    minigotchi.detect();
    delay(5000);

    // ugly hack: remove all these lines containing the words "delay(5000);" or comment them out with a "//" slash.
    // doing so will make the loop a lot faster. plus this might overheat the board and stuff but its worth a try.

    // deauth random access point
    minigotchi.deauth();
    delay(5000);
}