/**
 * parasite.h: header files for parasite.cpp
 */

#ifndef PARASITE_H
#define PARASITE_H

#include "channel.h"
#include "config.h"
#include "deauth.h"
#include "frame.h"
#include "pwnagotchi.h"
#include <Arduino.h>
#include <ArduinoJson.h>

typedef enum {
  SCANNING = 200,
  FRIEND_FOUND = 201,
  NO_FRIEND_FOUND = 202,
  FRIEND_SCAN_ERROR = 250
} parasite_pwnagotchi_scan_type_t;

typedef enum {
  SYNCED_CHANNEL = 200,
  RANDOM_CHANNEL = 201,
} parasite_channel_status_type_t;

typedef enum {
  START_SCAN = 200,
  PICKED_AP = 201,
  NO_APS = 202,
  START_DEAUTH = 203,
  SKIPPING_WHITELIST = 210,
  SKIPPING_UNENCRYPTED = 211,
  DEAUTH_SCAN_ERROR = 250
} parasite_deauth_status_type_t;

class Parasite {
public:
  static void readData();
  static void sendChannelStatus(parasite_channel_status_type_t status);
  static void sendName();
  static void sendAdvertising();
  static void sendPwnagotchiStatus(parasite_pwnagotchi_scan_type_t status);
  static void sendPwnagotchiStatus(parasite_pwnagotchi_scan_type_t status,
                                   const char *frd);
  static void sendDeauthStatus(parasite_deauth_status_type_t status);
  static void sendDeauthStatus(parasite_deauth_status_type_t status,
                               const char *target, int channel);
  static int channel;

private:
  static void sendData(const char *cmd, uint8 status, const char *data);
  static void formatData(char *buf, const char *data, size_t bufSize);
};

#endif // PARASITE_H