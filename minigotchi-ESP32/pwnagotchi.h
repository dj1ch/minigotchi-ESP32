/**
 * pwnagotchi.h: header files for pwnagotchi.cpp
*/

#ifndef PWNAGOTCHI_H
#define PWNAGOTCHI_H

#include "frame.h"
#include "minigotchi.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <Arduino.h>
#include <stdint.h>
#include <string>

class Pwnagotchi {
public:
    static void detect();
    static void pwnagotchiCallback(void *buf, wifi_promiscuous_pkt_type_t type);
    static void stopCallback();

private:
    static std::string extractMAC(const unsigned char *buff);
    static void getMAC(char* addr, const unsigned char* buff, int offset);
    static std::string essid;
    static bool pwnagotchiDetected;

    // source: https://github.com/justcallmekoko/ESP32Marauder/blob/c0554b95ceb379d29b9a8925d27cc2c0377764a9/esp32_marauder/WiFiScan.h#L213
    typedef struct {
      int16_t fctl;
      int16_t duration;
      uint8_t da;
      uint8_t sa;
      uint8_t bssid;
      int16_t seqctl;
      unsigned char payload[];
    } __attribute__((packed)) WifiMgmtHdr;
    
    typedef struct {
      uint8_t payload[0];
      WifiMgmtHdr hdr;
    } wifi_ieee80211_packet_t;
};

#endif // PWNAGOTCHI_H
