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
 * parasite.cpp: handles running parasite mode related tasks
 */

#include "parasite.h"

int Parasite::channel = 0;

/**
 * Reads data from Parasite mode on the Minigotchi
 */
void Parasite::readData() {
  if (Config::parasite) {
    int curChan = Parasite::channel;
    while (Serial.available() > 0) {
      String line = Serial.readStringUntil('\n');
      if (line.startsWith("chn:::")) {
        int chn = atoi(line.substring(6).c_str());
        if (Channel::isValidChannel(chn)) {
          Parasite::channel = chn;
        } else {
          Parasite::channel = 0;
        }
      } else if (line.startsWith("nme:::")) {
        Parasite::sendName();
      }
    }

    // If parasite channel is set and is different than what was there before,
    // notify that we're synced Otherwise if parasite channel is not set but was
    // before, notify we've unsynced
    if (Parasite::channel > 0 && Parasite::channel != curChan) {
      Parasite::sendChannelStatus(SYNCED_CHANNEL);
    } else if (Parasite::channel == 0 && curChan > 0) {
      Parasite::sendChannelStatus(RANDOM_CHANNEL);
    }
  }
}

/**
 * Shows current channel
 * @param status Channel, either synced or unsynced
 */
void Parasite::sendChannelStatus(parasite_channel_status_type_t status) {
  if (Config::parasite) {
    char chnBuf[4];
    snprintf(chnBuf, sizeof(chnBuf), "%d", Parasite::channel);
    Parasite::sendData("chn", static_cast<uint8_t>(status), chnBuf);
  }
}

/**
 * Send the Minigotchi's name to serial
 */
void Parasite::sendName() {
  if (Config::parasite) {
    if (Config::name.length() > 25) {
      // Pwnagotchi names can be 25 characters max, so want to limit our
      // Minigotchi's name to that as well Will truncate it to 22 characters +
      // "..." if it exceeds 25
      char buf[26];
      Parasite::formatData(buf, Config::name.c_str(), sizeof(buf));
      Parasite::sendData("nme", 200, buf);
    } else {
      Parasite::sendData("nme", 200, Config::name.c_str());
    }
  }
}

/**
 * Send current status (Advertising)
 */
void Parasite::sendAdvertising() {
  if (Config::parasite) {
    Parasite::sendData("adv", 200, nullptr);
  }
}

/**
 * Send current Pwnagotchi scanning status, in this case no friend
 * @param status Current scanning status
 */
void Parasite::sendPwnagotchiStatus(parasite_pwnagotchi_scan_type_t status) {
  Parasite::sendPwnagotchiStatus(status, nullptr);
}

/**
 * Send current Pwnagotchi scanning status
 * @param status Current scanning status
 * @param frd Found Pwnagotchi's name
 */
void Parasite::sendPwnagotchiStatus(parasite_pwnagotchi_scan_type_t status,
                                    const char *frd) {
  if (Config::parasite) {
    if (frd != nullptr && strlen(frd) > 25) {
      // frd is another Pwnagotchi's name, which should be 25 characters max
      // Will truncate it to 22 characters + "..." if it exceeds 25 somehow
      char buf[26];
      Parasite::formatData(buf, frd, sizeof(buf));
      Parasite::sendData("pwn", static_cast<uint8_t>(status), buf);
    } else {
      Parasite::sendData("pwn", static_cast<uint8_t>(status), frd);
    }
  }
}

/**
 * Sends current deauthing status, blank for the most part
 * @param status Current deauthing status
 */
void Parasite::sendDeauthStatus(parasite_deauth_status_type_t status) {
  Parasite::sendDeauthStatus(status, nullptr, 0);
}

/**
 * Sends deauth status
 * @param status Current status
 * @param target AP that is being deauthed
 * @param channel Current channel
 */
void Parasite::sendDeauthStatus(parasite_deauth_status_type_t status,
                                const char *target, int channel) {
  if (Config::parasite) {
    if (target != nullptr && channel > 0) {
      JsonDocument doc;
      char chnBuf[4];
      char buf[65];

      snprintf(chnBuf, sizeof(chnBuf), "%d", channel);
      // target is an SSID, which should only be 32 characters at most
      // Unlikely scenario but will truncate to 29 characters + "..." in case
      // that gets disrespected by someone
      if (strlen(target) > 32) {
        char targetBuf[33];
        Parasite::formatData(targetBuf, target, sizeof(targetBuf));
        doc["ssid"] = targetBuf;
      } else {
        doc["ssid"] = target;
      }
      doc["channel"] = chnBuf;
      serializeJson(doc, buf);
      Parasite::sendData("atk", static_cast<uint8_t>(status), buf);
    } else {
      Parasite::sendData("atk", static_cast<uint8_t>(status), nullptr);
    }
  }
}

/**
 * Algorithm to send serial data
 * @param command Current command
 * @param status Current status
 * @param data Data to use
 */
void Parasite::sendData(const char *command, uint8_t status, const char *data) {
  JsonDocument doc;
  char nBuf[4];  // Up to 3 digits + null terminator
  char buf[129]; // Up to 128 characters + null terminator
  char fullCmd[135] = {
      0}; // Data buffer (128) + command (3) + delimiter (3) + null terminator
  snprintf(nBuf, sizeof(nBuf), "%d", status);
  doc["status"] = nBuf;
  if (data != nullptr) {
    doc["data"] = data;
  }
  serializeJson(doc, buf);
  strncat(fullCmd, command, sizeof(fullCmd) - 1);
  strncat(fullCmd, ":::", sizeof(fullCmd) - strlen(fullCmd) - 1);
  strncat(fullCmd, buf, sizeof(fullCmd) - strlen(fullCmd) - 1);
  Serial.println(fullCmd);
}

/**
 * Formats data to be sent over serial
 * @param buf Buffer to use
 * @param data Full data that's used
 * @param bufSize The size of the buffer
 */
void Parasite::formatData(char *buf, const char *data, size_t bufSize) {
  buf[0] = '\0';
  strncat(buf, data, bufSize - 4);
  strncat(buf, "...", bufSize - strlen(buf) - 1);
}
