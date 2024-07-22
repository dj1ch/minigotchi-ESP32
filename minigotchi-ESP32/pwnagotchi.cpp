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
 * pwnagotchi.cpp: sniffs for pwnagotchi beacon frames
 * source: https://github.com/justcallmekoko/ESP32Marauder
 */

#include "pwnagotchi.h"

/** developer note:
 *
 * essentially the pwnagotchi sends out a frame(with JSON) while associated to a
 * network if the minigotchi listens for a while it should find something this
 * is under the assumption that we put the minigotchi on the same channel as the
 * pwnagotchi or one of the channels that the pwnagotchi listens on the JSON
 * frame it sends out should have some magic id attached to it (numbers 222-226)
 * so it is identified by pwngrid however we don't need to search for such
 * things
 *
 */

// start off false
bool Pwnagotchi::pwnagotchiDetected = false;

/**
 * Get's the mac based on source address
 * @param addr Address to use
 * @param buff Buffer to use
 * @param offset Data offset
 */
void Pwnagotchi::getMAC(char *addr, const unsigned char *buff, int offset) {
  snprintf(addr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", buff[offset],
           buff[offset + 1], buff[offset + 2], buff[offset + 3],
           buff[offset + 4], buff[offset + 5]);
}

/**
 * Extract Mac Address using getMac()
 * @param buff Buffer to use
 */
std::string Pwnagotchi::extractMAC(const unsigned char *buff) {
  char addr[] = "00:00:00:00:00:00";
  getMAC(addr, buff, 10);
  return std::string(addr);
}

/**
 * Detect a Pwnagotchi
 */
void Pwnagotchi::detect() {
  if (Config::scan) {
    // set mode and callback
    Minigotchi::monStart();
    esp_wifi_set_promiscuous_rx_cb(pwnagotchiCallback);

    // cool animation
    for (int i = 0; i < 5; ++i) {
      Serial.println("(0-o) Scanning for Pwnagotchi.");
      Display::updateDisplay("(0-o)", "Scanning  for Pwnagotchi.");
      delay(Config::shortDelay);
      Serial.println("(o-0) Scanning for Pwnagotchi..");
      Display::updateDisplay("(o-0)", "Scanning  for Pwnagotchi..");
      delay(Config::shortDelay);
      Serial.println("(0-o) Scanning for Pwnagotchi...");
      Display::updateDisplay("(0-o)", "Scanning  for Pwnagotchi...");
      delay(Config::shortDelay);
      Serial.println(" ");
      delay(Config::shortDelay);
    }

    // delay for scanning
    delay(Config::longDelay);

    // check if the pwnagotchiCallback wasn't triggered during scanning
    if (!pwnagotchiDetected) {
      // only searches on your current channel and such afaik,
      // so this only applies for the current searching area
      Minigotchi::monStop();
      Pwnagotchi::stopCallback();
      Serial.println("(;-;) No Pwnagotchi found");
      Display::updateDisplay("(;-;)", "No Pwnagotchi found.");
      Serial.println(" ");
      Parasite::sendPwnagotchiStatus(NO_FRIEND_FOUND);
    } else if (pwnagotchiDetected) {
      Minigotchi::monStop();
      Pwnagotchi::stopCallback();
    } else {
      Minigotchi::monStop();
      Pwnagotchi::stopCallback();
      Serial.println("(X-X) How did this happen?");
      Display::updateDisplay("(X-X)", "How did this happen?");
      Parasite::sendPwnagotchiStatus(FRIEND_SCAN_ERROR);
    }
  } else {
  }
}

/**
 * Stops Pwnagotchi scan
 */
void Pwnagotchi::stopCallback() { esp_wifi_set_promiscuous_rx_cb(nullptr); }

/**
 * Pwnagotchi Scanning callback
 * Source:
 * https://github.com/justcallmekoko/ESP32Marauder/blob/master/esp32_marauder/WiFiScan.cpp#L2439
 * @param buf Packet recieved to use as a buffer
 * @param len Length of the buffer
 */
void Pwnagotchi::pwnagotchiCallback(void *buf,
                                    wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t *snifferPacket = (wifi_promiscuous_pkt_t *)buf;
  WifiMgmtHdr *frameControl = (WifiMgmtHdr *)snifferPacket->payload;
  wifi_pkt_rx_ctrl_t ctrl = (wifi_pkt_rx_ctrl_t)snifferPacket->rx_ctrl;
  int len = snifferPacket->rx_ctrl.sig_len;

  if (type == WIFI_PKT_MGMT) {
    len -= 4;
    int fctl = ntohs(frameControl->fctl);
    const wifi_ieee80211_packet_t *ipkt =
        (wifi_ieee80211_packet_t *)snifferPacket->payload;
    const WifiMgmtHdr *hdr = &ipkt->hdr;

    // check if it is a beacon frame
    if (snifferPacket->payload[0] == 0x80) {
      // extract mac
      char addr[] = "00:00:00:00:00:00";
      getMAC(addr, snifferPacket->payload, 10);
      String src = addr;
      // Serial.println("'" + src + "'");

      // check if the source MAC matches the target
      if (src == "de:ad:be:ef:de:ad") {
        pwnagotchiDetected = true;
        Serial.println("(^-^) Pwnagotchi detected!");
        Serial.println(" ");
        Display::updateDisplay("(^-^)", "Pwnagotchi detected!");
        // delay(Config::shortDelay);

        // extract the ESSID from the beacon frame
        String essid = "";

        // "borrowed" from ESP32 Marauder
        for (int i = 38; i < len; i++) {
          if (isAscii(snifferPacket->payload[i])) {
            essid.concat((char)snifferPacket->payload[i]);
          } else {
            essid.concat("?");
          }
        }

        // give it a sec
        // delay(Config::shortDelay);

        // network related info
        Serial.print("(^-^) RSSI: ");
        Serial.println(snifferPacket->rx_ctrl.rssi);
        Serial.print("(^-^) Channel: ");
        Serial.println(snifferPacket->rx_ctrl.channel);
        Serial.print("(^-^) BSSID: ");
        Serial.println(addr);
        Serial.print("(^-^) ESSID: ");
        Serial.println(essid);
        Serial.println(" ");

        // parse the ESSID as JSON
        DynamicJsonDocument jsonBuffer(2048);
        DeserializationError error = deserializeJson(jsonBuffer, essid);
        // delay(Config::shortDelay);

        // check if json parsing is successful
        if (error) {
          Serial.println(F("(X-X) Could not parse Pwnagotchi json: "));
          Serial.print("(X-X) ");
          Serial.println(error.c_str());
          Display::updateDisplay("(^-^)", "Could not parse Pwnagotchi json: " +
                                              (String)error.c_str());
          Serial.println(" ");
        } else {
          Serial.println("(^-^) Successfully parsed json!");
          Serial.println(" ");
          Display::updateDisplay("(^-^)", "Successfully parsed json!");
          // find out some stats
          String name = jsonBuffer["name"].as<String>();
          delay(Config::shortDelay);
          String pwndTot = jsonBuffer["pwnd_tot"].as<String>();
          delay(Config::shortDelay);

          if (name == "null") {
            name = "N/A";
          }

          if (pwndTot == "null") {
            pwndTot = "N/A";
          }

          // print the info
          Serial.print("(^-^) Pwnagotchi name: ");
          Serial.println(name);
          Serial.print("(^-^) Pwned Networks: ");
          Serial.println(pwndTot);
          Serial.print(" ");
          Display::updateDisplay("(^-^)", "Pwnagotchi name: " + (String)name);
          delay(Config::shortDelay);
          Display::updateDisplay("(^-^)", "Pwned Networks: " + (String)pwndTot);
          delay(Config::shortDelay);
          Parasite::sendPwnagotchiStatus(FRIEND_FOUND, name.c_str());
        }
      }
    }
  }
}
