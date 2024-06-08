/**
 * frame.h: header files for frame.cpp
*/

#ifndef FRAME_H
#define FRAME_H

#include "display.h"
#include "config.h"
#include <sstream>
#include <vector>
#include <string>
#include <esp_wifi.h>
#include <ArduinoJson.h>

class Frame {
public:
    static void pack();
    static bool send();
    static void advertise();
    static const uint8_t header[];
    static const uint8_t IDWhisperPayload;
    static const uint8_t IDWhisperCompression;
    static const uint8_t IDWhisperIdentity;
    static const uint8_t IDWhisperSignature;
    static const uint8_t IDWhisperStreamHeader;
    static const uint8_t SignatureAddr[];
    static const uint8_t BroadcastAddr[];
    static const uint16_t wpaFlags;
    static bool sent;
    static size_t frameSize;
    static std::vector<uint8_t> beaconFrame;
    static size_t payloadSize;
    static const size_t chunkSize;

private:
    static void init();
    static void essid();

};

#endif // FRAME_H