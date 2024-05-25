/**
 * pwnagotchi.h: header files for pwnagotchi.cpp
*/

#ifndef PWNAGOTCHI_H
#define PWNAGOTCHI_H

#include "frame.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Arduino.h>
#include <stdint.h>
#include <string>

class Pwnagotchi {
public:
    static void detect();
    static void pwnagotchiCallback(unsigned char *buf, short unsigned int type);

private:
    static std::string extractMAC(const unsigned char *buff);
    static void getMAC(char* addr, const unsigned char* buff, int offset);
    static std::string essid;
    static bool pwnagotchiDetected;
};

#endif // PWNAGOTCHI_H