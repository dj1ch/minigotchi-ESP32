/**
 * channel.h: header files for channel.cpp
*/

#ifndef CHANNEL_H
#define CHANNEL_H

#include "minigotchi.h"
#include "config.h"
#include <WiFi.h>
#include <esp_wifi.h>

class Channel {
public:
    static void init(int initChannel);
    static void cycle();
    static void switchC(int newChannel);
    static int getChannel();
    static int list();
    static int channelList[3]; // 3 channels 

private:
    static int randomIndex; 
    static int numChannels; 
    static int currentChannel;
    static int newChannel;
};

#endif // CHANNEL_H
