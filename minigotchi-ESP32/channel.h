/**
 * channel.h: header files for channel.cpp
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include "config.h"
#include "display.h"
#include "minigotchi.h"
#include <WiFi.h>
#include <esp_wifi.h>

class Channel {
public:
  static void init(int initChannel);
  static void cycle();
  static void switchChannel(int newChannel);
  static int getChannel();
  static void checkChannel(int channel);
  static int channelList[13]; // 13 channels

private:
  static int randomIndex;
  static int numChannels;
  static int currentChannel;
  static int newChannel;
};

#endif // CHANNEL_H
