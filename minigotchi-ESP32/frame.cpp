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
 * frame.cpp: handles the sending of "pwnagotchi" beacon frames
 */

#include "frame.h"

/** developer note:
 *
 * when it comes to detecting a pwnagotchi, this is done with pwngrid/opwngrid.
 * essentially pwngrid looks for the numbers 222-226 in payloads, and if they
 * aren't there, it ignores it. these need to be put into the frames!!!
 *
 * note that these frames aren't just normal beacon frames, rather a modified
 * one with data, additional ids, etc. frames are dynamically constructed,
 * headers are included like a normal frame. by far this is the most memory
 * heaviest part of the minigotchi, the reason is
 *
 */

// initializing
size_t Frame::payloadSize = 255; // by default
const size_t Frame::chunkSize = 0xFF;

// beacon stuff
size_t Frame::essidLength = 0;
uint8_t Frame::headerLength = 0;

// payload ID's according to pwngrid
const uint8_t Frame::IDWhisperPayload = 0xDE;
const uint8_t Frame::IDWhisperCompression = 0xDF;
const uint8_t Frame::IDWhisperIdentity = 0xE0;
const uint8_t Frame::IDWhisperSignature = 0xE1;
const uint8_t Frame::IDWhisperStreamHeader = 0xE2;

// other addresses
const uint8_t Frame::SignatureAddr[] = {0xde, 0xad, 0xbe, 0xef, 0xde, 0xad};
const uint8_t Frame::BroadcastAddr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
const uint16_t Frame::wpaFlags = 0x0411;

/**
 * Gets first instance of mood class
 */
Mood &Frame::mood = Mood::getInstance();

// Don't even dare restyle!
const uint8_t Frame::header[]{
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0xad,
    0xbe, 0xef, 0xde, 0xad, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x11, 0x04,
};

// get header length
const int Frame::pwngridHeaderLength = sizeof(Frame::header);

/** developer note:
 *
 * frame structure based on how it was built here
 *
 * 1. header
 * 2. payload id's
 * 3. (chunked) pwnagotchi data
 *
 */

/** developer note:
 *
 * referenced the following for packing-related function:
 *
 * https://github.com/evilsocket/pwngrid/blob/master/wifi/pack.go
 *
 */

/**
 * Replicates pwngrid's pack() function from pack.go
 * https://github.com/evilsocket/pwngrid/blob/master/wifi/pack.go
 */
uint8_t *Frame::pack() {
  // make a json doc
  String jsonString = "";
  DynamicJsonDocument doc(2048);

  doc["epoch"] = Config::epoch;
  doc["face"] = Config::face;
  doc["identity"] = Config::identity;
  doc["name"] = Config::name;

  doc["policy"]["advertise"] = Config::advertise;
  doc["policy"]["ap_ttl"] = Config::ap_ttl;
  doc["policy"]["associate"] = Config::associate;
  doc["policy"]["bored_num_epochs"] = Config::bored_num_epochs;

  doc["policy"]["deauth"] = Config::deauth;
  doc["policy"]["excited_num_epochs"] = Config::excited_num_epochs;
  doc["policy"]["hop_recon_time"] = Config::hop_recon_time;
  doc["policy"]["max_inactive_scale"] = Config::max_inactive_scale;
  doc["policy"]["max_interactions"] = Config::max_interactions;
  doc["policy"]["max_misses_for_recon"] = Config::max_misses_for_recon;
  doc["policy"]["min_recon_time"] = Config::min_rssi;
  doc["policy"]["min_rssi"] = Config::min_rssi;
  doc["policy"]["recon_inactive_multiplier"] =
      Config::recon_inactive_multiplier;
  doc["policy"]["recon_time"] = Config::recon_time;
  doc["policy"]["sad_num_epochs"] = Config::sad_num_epochs;
  doc["policy"]["sta_ttl"] = Config::sta_ttl;

  doc["pwnd_run"] = Config::pwnd_run;
  doc["pwnd_tot"] = Config::pwnd_tot;
  doc["session_id"] = Config::session_id;
  doc["uptime"] = Config::uptime;
  doc["version"] = Config::version;

  // serialize then put into beacon frame
  serializeJson(doc, jsonString);
  Frame::essidLength = measureJson(doc);
  Frame::headerLength = 2 + ((uint8_t)(essidLength / 255) * 2);
  uint8_t *beaconFrame = new uint8_t[Frame::pwngridHeaderLength +
                                     Frame::essidLength + Frame::headerLength];
  memcpy(beaconFrame, Frame::header, Frame::pwngridHeaderLength);

  /** developer note:
   *
   * if you literally want to check the json everytime you send a packet(non
   * serialized ofc)
   *
   * Serial.println(jsonString);
   */

  int frameByte = pwngridHeaderLength;
  for (int i = 0; i < essidLength; i++) {
    if (i == 0 || i % 255 == 0) {
      beaconFrame[frameByte++] = Frame::IDWhisperPayload;
      uint8_t newPayloadLength = 255;
      if (essidLength - i < Frame::chunkSize) {
        newPayloadLength = essidLength - i;
      }
      beaconFrame[frameByte++] = newPayloadLength;
    }

    uint8_t nextByte = (uint8_t)'?';
    if (isAscii(jsonString[i])) {
      nextByte = (uint8_t)jsonString[i];
    }

    beaconFrame[frameByte++] = nextByte;
  }

  /* developer note: we can print the beacon frame like so...

  Serial.println("('-') Full Beacon Frame:");
  for (size_t i = 0; i < frameSize; ++i) {
    Serial.print(beaconFrame[i], HEX);
    Serial.print(" ");
  }

  Serial.println(" ");

  */

  return beaconFrame;
}

/**
 * Send a modified pwnagotchi packet,
 * except add minigotchi info as well.
 */
uint8_t *Frame::packModified() {
  // make a json doc
  String jsonString = "";
  DynamicJsonDocument doc(2048);

  doc["minigotchi"] = true;
  doc["epoch"] = Config::epoch;
  doc["face"] = Config::face;
  doc["identity"] = Config::identity;
  doc["name"] = Config::name;

  doc["policy"]["advertise"] = Config::advertise;
  doc["policy"]["ap_ttl"] = Config::ap_ttl;
  doc["policy"]["associate"] = Config::associate;
  doc["policy"]["bored_num_epochs"] = Config::bored_num_epochs;

  doc["policy"]["deauth"] = Config::deauth;
  doc["policy"]["excited_num_epochs"] = Config::excited_num_epochs;
  doc["policy"]["hop_recon_time"] = Config::hop_recon_time;
  doc["policy"]["max_inactive_scale"] = Config::max_inactive_scale;
  doc["policy"]["max_interactions"] = Config::max_interactions;
  doc["policy"]["max_misses_for_recon"] = Config::max_misses_for_recon;
  doc["policy"]["min_recon_time"] = Config::min_rssi;
  doc["policy"]["min_rssi"] = Config::min_rssi;
  doc["policy"]["recon_inactive_multiplier"] =
      Config::recon_inactive_multiplier;
  doc["policy"]["recon_time"] = Config::recon_time;
  doc["policy"]["sad_num_epochs"] = Config::sad_num_epochs;
  doc["policy"]["sta_ttl"] = Config::sta_ttl;

  doc["pwnd_run"] = Config::pwnd_run;
  doc["pwnd_tot"] = Config::pwnd_tot;
  doc["session_id"] = Config::session_id;
  doc["uptime"] = Config::uptime;
  doc["version"] = Config::version;

  // serialize then put into beacon frame
  serializeJson(doc, jsonString);
  Frame::essidLength = measureJson(doc);
  Frame::headerLength = 2 + ((uint8_t)(essidLength / 255) * 2);
  uint8_t *beaconFrame = new uint8_t[Frame::pwngridHeaderLength +
                                     Frame::essidLength + Frame::headerLength];
  memcpy(beaconFrame, Frame::header, Frame::pwngridHeaderLength);

  int frameByte = pwngridHeaderLength;
  for (int i = 0; i < essidLength; i++) {
    if (i == 0 || i % 255 == 0) {
      beaconFrame[frameByte++] = Frame::IDWhisperPayload;
      uint8_t newPayloadLength = 255;
      if (essidLength - i < Frame::chunkSize) {
        newPayloadLength = essidLength - i;
      }
      beaconFrame[frameByte++] = newPayloadLength;
    }
    uint8_t nextByte = (uint8_t)'?';
    if (isAscii(jsonString[i])) {
      nextByte = (uint8_t)jsonString[i];
    }

    beaconFrame[frameByte++] = nextByte;
  }

  return beaconFrame;
}

/**
 * Sends a pwnagotchi packet in AP mode
 */
bool Frame::send() {
  // convert to a pointer because esp-idf is a pain in the ass
  WiFi.mode(WIFI_AP);

  // create normal frame
  uint8_t *frame = Frame::pack();
  size_t frameSize = Frame::pwngridHeaderLength + Frame::essidLength +
                     Frame::headerLength; // actually disgusting but it works

  // send both frames if the first one is successful
  delay(102);
  esp_err_t err = esp_wifi_80211_tx(WIFI_IF_AP, frame, frameSize, false);
  if (err == ESP_OK) {
    delete[] frame;

    frame = Frame::packModified();
    frameSize =
        Frame::pwngridHeaderLength + Frame::essidLength +
        Frame::headerLength; // frameSize changes because essidLength changes
    err = esp_wifi_80211_tx(WIFI_IF_AP, frame, frameSize, false);
    if (err == ESP_OK) {
      delete[] frame;
    }
  }

  // might be redundant but just in case...
  return (err == ESP_OK);
}

/**
 * Full usage of Pwnagotchi's advertisments on the Minigotchi.
 */
void Frame::advertise() {
  int packets = 0;
  unsigned long startTime = millis();

  if (Config::advertise) {
    Serial.println(mood.getIntense() + " Starting advertisment...");
    Serial.println(" ");
    Display::updateDisplay(mood.getIntense(), "Starting advertisment...");
    Parasite::sendAdvertising();
    delay(Config::shortDelay);
    for (int i = 0; i < 150; ++i) {
      if (Frame::send()) {
        packets++;

        // calculate packets per second
        float pps = packets / (float)(millis() - startTime) * 1000;

        // show pps
        if (!isinf(pps)) {
          Serial.print(mood.getIntense() + " Packets per second: ");
          Serial.print(pps);
          Serial.print(" pkt/s (Channel: ");
          Serial.print(Channel::getChannel());
          Serial.println(")");
          Display::updateDisplay(
              mood.getIntense(),
              "Packets per second: " + (String)pps + " pkt/s" +
                  " (Channel: " + (String)Channel::getChannel() + ")");
        }
      } else {
        Serial.println(mood.getBroken() + " Advertisment failed to send!");
        Display::updateDisplay(mood.getBroken(),
                               "Advertisment failed to send!");
      }
    }

    Serial.println(" ");
    Serial.println(mood.getHappy() + " Advertisment finished!");
    Serial.println(" ");
    Display::updateDisplay(mood.getHappy(), "Advertisment finished!");
  } else {
    // do nothing but still idle
  }
}
