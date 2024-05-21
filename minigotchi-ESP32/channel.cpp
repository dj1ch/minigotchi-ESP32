/**
 * channel.cpp: handles channel switching
*/

#include "channel.h"

/** developer note: 
 *
 * i am using the ideal 2.4 GHz channels, which are 1, 6, and 11.
 * the reason for that is so we don't interfere with other devices on our frequency.
 * there are probably plenty more reasons but this is a good practice for such iot devices.
 *
 */ 

// same channels in config
int Channel::channelList[3] = {Config::channels[0], Config::channels[1], Config::channels[2]};

void Channel::init(int initChannel) {
    // start on user specified channel
    delay(250);
    Serial.println(" ");
    Serial.print("(-.-) Initializing on channel ");
    Serial.println(initChannel);
    Serial.println(" ");
    delay(250);
    
    // switch channel
    Minigotchi::monStop();
    esp_wifi_set_channel(initChannel, WIFI_SECOND_CHAN_NONE);
    Minigotchi::monStart();

    if (initChannel == getChannel()) {
        Serial.print("('-') Successfully initialized on channel ");
        Serial.println(getChannel());
        delay(250);
    } else {
        Serial.print("(X-X) Channel initialization failed, try again?");
        delay(250);
    }
}

void Channel::cycle() { 
    // get channels
    int numChannels = sizeof(channelList) / sizeof(channelList[0]);

    // select a random one
    int randomIndex = random(numChannels);
    int newChannel = channelList[randomIndex];

    // switch here
    switchC(newChannel);
}

void Channel::switchC(int newChannel) {
    // switch to channel
    delay(250);
    Serial.print("(-.-) Switching to channel ");
    Serial.println(newChannel);
    Serial.println(" ");
    delay(250);

    // monitor this one channel
    Minigotchi::monStop();
    esp_wifi_set_channel(newChannel, WIFI_SECOND_CHAN_NONE);   
    Minigotchi::monStart();

    // switched channel
    Serial.print("('-') Currently on channel ");
    Serial.println(getChannel());
    Serial.println(" ");
    delay(250);
}

int Channel::getChannel() {
    wifi_country_t country;
    memset(&country, 0, sizeof(country));
    esp_wifi_get_country(&country);

    wifi_promiscuous_channel_info_t info;
    esp_wifi_get_channel(&info);
    return info.primary;
}

int Channel::list() {
    return channelList[3];
}