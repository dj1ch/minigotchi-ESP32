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
SemaphoreHandle_t Minigotchi::taskSemaphore = xSemaphoreCreateBinary();

int Minigotchi::addEpoch() {
  Minigotchi::currentEpoch++;
  return Minigotchi::currentEpoch;
}

void Minigotchi::epoch() {
  Minigotchi::addEpoch();
  Parasite::readData();
  Serial.print("('-') Current Epoch: ");
  Serial.println(Minigotchi::currentEpoch);
  Serial.println(" ");
}

// things to do when starting up
void Minigotchi::boot() {
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
  Deauth::list();
  Channel::init(Config::channel);
  Minigotchi::info();
  Parasite::sendName();
  Minigotchi::finish();
  xSemaphoreGive(taskSemaphore);
}

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

// if this can be printed, everything should have gone right...
void Minigotchi::finish() {
  Serial.println("################################################");
  Serial.println(" ");
  Serial.println("('-') Started successfully!");
  Serial.println(" ");
  Display::updateDisplay("('-')", "Started sucessfully");
  delay(250);
}

void Minigotchi::version() {
  Serial.print("('-') Version: ");
  Serial.println(Config::version.c_str());
  Display::updateDisplay("('-')",
                         "Version: " + (String)Config::version.c_str());
  delay(250);
}

void Minigotchi::mem() {
  Serial.print("('-') Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Display::updateDisplay("('-')",
                         "Heap: " + (String)ESP.getFreeHeap() + " bytes");
  delay(250);
}

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

void Minigotchi::monStart() {
  // disconnect from WiFi if we were at all
  WiFi.disconnect();

  // revert to station mode
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
}

void Minigotchi::monStop() {
  esp_wifi_set_promiscuous(false);

  // revert to station mode
  WiFi.mode(WIFI_STA);
}

/** developer note:
 * 
 * for freeRTOS, which allows us to "pin tasks" to specific cores on the ESP32, we will need to create the following
 * 
 * - a task
 * - a function to run that task using xTaskCreatePinnedToCore();
 * 
 * in this case, we use Minigotchi::<whichever function we're using>();
 * and Minigotchi::<whichever function we're using>Task();
 * to allow specific tasks defined in the Minigotchi::<whichever function we're using>Task();
 * in the regular functions called Minigotchi::<whichever function we're using>();
 * 
 */

void Minigotchi::cycleTask(void* param) {
  Channel::cycle();
  xSemaphoreGive(taskSemaphore);
  vTaskDelete(NULL);
}

void Minigotchi::detectTask(void* param) {
  Pwnagotchi::detect();
  xSemaphoreGive(taskSemaphore);
  vTaskDelete(NULL);
}

void Minigotchi::deauthTask(void* param) {
  Deauth::deauth();
  xSemaphoreGive(taskSemaphore);
  vTaskDelete(NULL);
}

void Minigotchi::advertiseTask(void* param) {
  Frame::advertise();
  xSemaphoreGive(taskSemaphore);
  vTaskDelete(NULL);
}

void Minigotchi::cycle() {
  xSemaphoreTake(taskSemaphore, portMAX_DELAY);
  xTaskCreatePinnedToCore(Minigotchi::cycleTask, "cycleTask", 4096, NULL, 1, NULL, 0);
}

void Minigotchi::detect() {
  xSemaphoreTake(taskSemaphore, portMAX_DELAY);
  xTaskCreatePinnedToCore(Minigotchi::detectTask, "detectTask", 4096, NULL, 1, NULL, 0);
}

void Minigotchi::deauth() {
  xSemaphoreTake(taskSemaphore, portMAX_DELAY);
  xTaskCreatePinnedToCore(Minigotchi::deauthTask, "deauthTask", 4096, NULL, 1, NULL, 0);
}

void Minigotchi::advertise() {
  xSemaphoreTake(taskSemaphore, portMAX_DELAY);
  xTaskCreatePinnedToCore(Minigotchi::advertiseTask, "advertiseTask", 4096, NULL, 1, NULL, 0);
}
