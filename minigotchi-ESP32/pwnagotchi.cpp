/**
 * pwnagotchi.cpp: sniffs for pwnagotchi beacon frames            
 * source: https://github.com/justcallmekoko/ESP32Marauder 
*/

#include "pwnagotchi.h"

/** developer note: 
 * 
 * essentially the pwnagotchi sends out a frame(with JSON) while associated to a network
 * if the minigotchi listens for a while it should find something
 * this is under the assumption that we put the minigotchi on the same channel as the pwnagotchi
 * or one of the channels that the pwnagotchi listens on
 * the JSON frame it sends out should have some magic id attached to it (222 or 223) so it is identified by pwngrid
 * however we don't need to search for such things
 *
*/

// start off false
bool Pwnagotchi::pwnagotchiDetected = false;

void Pwnagotchi::getMAC(char* addr, const unsigned char* buff, int offset) {
    snprintf(addr, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
             buff[offset], buff[offset + 1], buff[offset + 2],
             buff[offset + 3], buff[offset + 4], buff[offset + 5]);
}

std::string Pwnagotchi::extractMAC(const unsigned char *buff) {
    char addr[] = "00:00:00:00:00:00";
    getMAC(addr, buff, 10);
    return std::string(addr);
}

void Pwnagotchi::detect() {
    // cool animation
    for (int i = 0; i < 5; ++i) {
        Serial.println("(0-o) Scanning for Pwnagotchi.");
        delay(500);
        Serial.println("(o-0) Scanning for Pwnagotchi..");
        delay(500);
        Serial.println("(0-o) Scanning for Pwnagotchi...");
        delay(500);
        Serial.println(" ");
        delay(500);
    }

    // delay for scanning
    delay(5000);

    // set mode and callback
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(true);
    wifi_set_promiscuous_rx_cb(&pwnagotchiCallback);

    if (pwnagotchiDetected) {
        // send the advertisement if it is found
        Serial.println(" ");
        Serial.println("(^-^) Starting advertisement...");
        Serial.println(" ");
        delay(5000);
        Frame::start();
    }

    // check if the pwnagotchiCallback wasn't triggered during scanning
    if (!pwnagotchiDetected) {
        // only searches on your current channel and such afaik, 
        // so this only applies for the current searching area
        Serial.println("(;-;) No Pwnagotchi found.");
        Serial.println(" ");
    }
}

void Pwnagotchi::pwnagotchiCallback(unsigned char *buf, short unsigned int type) {
    wifi_promiscuous_pkt_t* snifferPacket = (wifi_promiscuous_pkt_t*)buf;

    // check if it is a beacon frame
    if (snifferPacket->payload[0] == 0x80) {
        // extract mac
        char addr[] = "00:00:00:00:00:00";
        getMAC(addr, snifferPacket->payload, 10);
        String src = addr;

        // Check if the source MAC matches the target
        if (src == "de:ad:be:ef:de:ad") {
            pwnagotchiDetected = true;
            Serial.println(" ");
            Serial.println("(^-^) Pwnagotchi detected!");
            Serial.println(" ");

            // extract the ESSID from the beacon frame
            String essid(reinterpret_cast<const char*>(&snifferPacket->payload[36]));

            Serial.print("ESSID: ");
            Serial.println(essid);
            Serial.println(" ");

            // load json from the ESSID
            DynamicJsonDocument jsonBuffer(1024);
            DeserializationError error = deserializeJson(jsonBuffer, essid);

            // check if json parsing is successful
            if (error) {
                Serial.println(F("(X-X) Could not parse Pwnagotchi json: "));
                Serial.println(error.c_str());
                Serial.println(" ");
            } else {
                Serial.println("(^-^) Successfully parsed json");
                Serial.println(" ");

                // find out some stats
                String name = jsonBuffer["name"].as<String>();
                String pwndTot = jsonBuffer["pwnd_tot"].as<String>();

                // print the info
                Serial.print("(^-^) Pwnagotchi name: ");
                Serial.println(name);
                Serial.print("(^-^) Pwned Networks: ");
                Serial.println(pwndTot);
                Serial.print(" ");
            }
        }
    } 
}