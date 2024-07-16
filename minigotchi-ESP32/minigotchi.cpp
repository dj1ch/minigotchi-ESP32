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
 * minigotchi.cpp: handles system usage info, etc
 */

#include "minigotchi.h"
#include "AXP192.h"

/** developer note:
 *
 * the functions here provide info on the minigotchi, such as memory,
 * temperature, etc. all this really does is print information to the serial
 * terminal this is meant to be ran on startup.
 *
 */

// this code is pretty disgusting and shitty but it makes minigotchi.ino less
// cluttered!!!

// current epoch val
int Minigotchi::currentEpoch = 0;

/**
 * Increment/increase current epoch by one
 */
int Minigotchi::addEpoch() {
  Minigotchi::currentEpoch++;
  return Minigotchi::currentEpoch;
}

/**
 * Show current Minigotchi epoch
 */
void Minigotchi::epoch() {
  Minigotchi::addEpoch();
  Parasite::readData();
  Serial.print("('-') Current Epoch: ");
  Serial.println(Minigotchi::currentEpoch);
  Serial.println(" ");
}

/**
 * Things to do on startup
 */
void Minigotchi::boot() {
  // StickC Plus 1.1 and 2 power management, to keep turned On after unplug USB
  // cable
  if (Config::screen == "M5StickCP") {
    AXP192 axp192;
    axp192.begin();           // Use the instance of AXP192
    axp192.ScreenBreath(100); // Use the instance of AXP192
  } else if (Config::screen == "M5StickCP2") {
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
  }

  Display::startScreen();
  Serial.println(" ");
  Serial.println("(^-^) Hi, I'm Minigotchi, your pwnagotchi's best friend!");
  Display::updateDisplay("(^-^)", "Hi,       I'm Minigotchi");
  Serial.println(" ");
  Serial.println(
      "('-') You can edit my configuration parameters in config.cpp!");
  Serial.println(" ");
  delay(250);
  Display::updateDisplay("('-')", "Edit my config.cpp!");
  delay(250);
  Serial.println("(>-<) Starting now...");
  Serial.println(" ");
  Display::updateDisplay("(>-<)", "Starting  now");
  delay(250);
  Serial.println("################################################");
  Serial.println("#                BOOTUP PROCESS                #");
  Serial.println("################################################");
  Serial.println(" ");
  ESP_ERROR_CHECK(esp_wifi_init(&Config::config));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
  Deauth::list();
  Channel::init(Config::channel);
  Minigotchi::info();
  Parasite::sendName();
  Minigotchi::finish();
}

/**
 * Show current Minigotchi info/stats
 */
void Minigotchi::info() {
  delay(250);
  Serial.println(" ");
  Serial.println("('-') Current Minigotchi Stats: ");
  Display::updateDisplay("('-')", "Current Minigotchi Stats:");
  version();
  mem();
  cpu();
  Serial.println(" ");
  delay(250);
}

/**
 * This is printed after everything is done in the bootup process
 */
void Minigotchi::finish() {
  Serial.println("################################################");
  Serial.println(" ");
  Serial.println("('-') Started successfully!");
  Serial.println(" ");
  Display::updateDisplay("('-')", "Started sucessfully");
  delay(250);
}

/**
 * Shows current Minigotchi version
 */
void Minigotchi::version() {
  Serial.print("('-') Version: ");
  Serial.println(Config::version.c_str());
  Display::updateDisplay("('-')",
                         "Version: " + (String)Config::version.c_str());
  delay(250);
}

/**
 * Shows current Minigotchi memory usage
 */
void Minigotchi::mem() {
  Serial.print("('-') Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Display::updateDisplay("('-')",
                         "Heap: " + (String)ESP.getFreeHeap() + " bytes");
  delay(250);
}

/**
 * Shows current Minigotchi Frequency
 */
void Minigotchi::cpu() {
  Serial.print("('-') CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
  Display::updateDisplay(
      "('-')", "CPU Frequency: " + (String)ESP.getCpuFreqMHz() + " MHz");
  delay(250);
}

/** developer note:
 *
 * these functions are much like the pwnagotchi's monstart and monstop which
 * start and stop monitor mode. neat!
 *
 * obviously i like to keep things consistent with the pwnagotchi, this is one
 * of those things.
 *
 */

/** developer note:
 *
 * to prevent issues we put the minigotchi back into client mode which is the
 * "default"
 *
 */

/**
 * Puts Minigotchi in promiscuous mode
 */
void Minigotchi::monStart() {
  // disconnect from WiFi if we were at all
  WiFi.disconnect();

  // revert to station mode
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
}

/**
 * Takes Minigotchi out of promiscuous mode
 */
void Minigotchi::monStop() {
  esp_wifi_set_promiscuous(false);

  // revert to station mode
  WiFi.mode(WIFI_STA);
}

/** developer note:
 *
 * when the minigotchi isn't cycling, detecting a pwnagotchi, or deauthing,
 * it is advertising it's own presence, hence the reason there being a constant
 * Frame::stop(); and Frame::start(); in each function
 *
 * when it comes to any of these features, you can't just call something and
 * expect it to run normally ex: calling Deauth::deauth(); because you're gonna
 * get the error:
 *
 * (X-X) No access point selected. Use select() first.
 * ('-') Told you so!
 *
 * the card is still busy in monitor mode on a certain channel(advertising), and
 * the AP's we're looking for could be on other channels hence we need to call
 * Frame::stop(); to stop this then we can do what we want...
 *
 */

/**
 * Channel cycling
 */
void Minigotchi::cycle() {
  Parasite::readData();
  Channel::cycle();
}

/**
 * Pwnagotchi detection
 */
void Minigotchi::detect() {
  Parasite::readData();
  Pwnagotchi::detect();
}

/**
 * Deauthing
 */
void Minigotchi::deauth() {
  Parasite::readData();
  Deauth::deauth();
}

/**
 * Advertising
 */
void Minigotchi::advertise() {
  Parasite::readData();
  Frame::advertise();
}
