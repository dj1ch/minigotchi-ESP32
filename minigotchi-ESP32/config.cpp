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
 * config.cpp: configuration for minigotchi
 */

#include "config.h"

/** developer note:
 *
 * this is the very equivalent of the 'config.toml' for the pwnagotchi
 * variables are defined here which will be used by the minigotchi
 * whatever can be disabled/enabled can be enabled here
 *
 */

// define if features will be used
bool Config::deauth = true;
bool Config::advertise = true;
bool Config::scan = true;
bool Config::spam = true;

// define access point ssid and password
const char *Config::ssid = "minigotchi";
const char *Config::pass = "dj1ch-minigotchi";

// define universal delays
int Config::shortDelay = 500;
int Config::longDelay = 5000;

// Defines if this is running in parasite mode where it hooks up directly to a
// Pwnagotchi
bool Config::parasite = false;

// screen configuration
bool Config::display = false;
std::string Config::screen = "";

// define baud rate
int Config::baud = 115200;

// define init channel
int Config::channel = 1;

// define whitelist
std::vector<std::string> Config::whitelist = {"SSID", "SSID", "SSID"};

// define faces
String Config::happy = "(^-^)";
String Config::sad = "(;-;)";
String Config::broken = "(X-X)";
String Config::intense = "(>-<)";
String Config::looking1 = "(0-o)";
String Config::looking2 = "(o-0)";
String Config::neutral = "('-')";
String Config::sleeping = "(-.-)";

// json config
int Config::epoch = Minigotchi::currentEpoch;
std::string Config::face = "(^-^)";
std::string Config::identity =
    "b9210077f7c14c0651aa338c55e820e93f90110ef679648001b1cecdbffc0090";
std::string Config::name = "minigotchi";
int Config::ap_ttl = Config::random(30, 600);
bool Config::associate = true;
int Config::bored_num_epochs = Config::random(5, 30);

// define channels
int Config::channels[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// see https://github.com/evilsocket/pwnagotchi/blob/master/pwnagotchi/ai/gym.py
int Config::excited_num_epochs = Config::random(5, 30);
int Config::hop_recon_time = Config::random(5, 60);
int Config::max_inactive_scale = Config::random(3, 10);
int Config::max_interactions = Config::random(1, 25);
int Config::max_misses_for_recon = Config::random(3, 10);
int Config::min_recon_time = Config::random(1, 30);
int Config::min_rssi = Config::random(-200, -50);
int Config::recon_inactive_multiplier = Config::random(1, 3);
int Config::recon_time = Config::random(5, 60);
int Config::sad_num_epochs = Config::random(5, 30);
int Config::sta_ttl = Config::random(60, 300);
int Config::pwnd_run = 0;
int Config::pwnd_tot = 0;
std::string Config::session_id = "84:f3:eb:58:95:bd";
int Config::uptime = Config::time();

// wifi settings
wifi_init_config_t Config::wifiCfg = WIFI_INIT_CONFIG_DEFAULT();
wifi_country_t Config::ctryCfg = {.cc = "US", .schan = 1, .nchan = 13};

// configured flag which only the WebUI changes
bool Config::configured = false;

// define version(please do not change, this should not be changed)
std::string Config::version = "3.5.2-beta";

/**
 * Loads configuration values from NVS
 */
void Config::loadConfig() {
  nvs_handle_t cfgHandle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &cfgHandle);
  if (err == ESP_OK) {
    // load Config::configured
    uint8_t configured = 0;
    err = nvs_get_u8(cfgHandle, "configured", &configured);
    if (err == ESP_OK) {
      Config::configured = (configured == 1);
    }

    // load Config::whitelist
    size_t required_size = 0;
    err = nvs_get_str(cfgHandle, "whitelist", NULL, &required_size);
    if (err == ESP_OK && required_size > 0) {
      char *whitelistStr = (char *)malloc(required_size);
      err = nvs_get_str(cfgHandle, "whitelist", whitelistStr, &required_size);
      if (err == ESP_OK) {
        // convert back into a vector
        Config::whitelist.clear();
        std::stringstream ss(whitelistStr);
        std::string item;
        while (std::getline(ss, item, ',')) {
          Config::whitelist.push_back(item);
        }
      }
      free(whitelistStr);
    }

    nvs_close(cfgHandle);
  }
}

/**
 * Saves configuration to NVS
 */
void Config::saveConfig() {
  nvs_handle_t cfgHandle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &cfgHandle);
  if (err == ESP_OK) {
    // save Config::configured
    uint8_t configured = Config::configured ? 1 : 0;
    err = nvs_set_u8(cfgHandle, "configured", configured);
    ESP_ERROR_CHECK(err);

    // save Config::whitelist
    std::string whitelistStr;
    for (size_t i = 0; i < Config::whitelist.size(); ++i) {
      whitelistStr += Config::whitelist[i];
      if (i < Config::whitelist.size() - 1) {
        whitelistStr += ",";
      }
    }
    err = nvs_set_str(cfgHandle, "whitelist", whitelistStr.c_str());
    ESP_ERROR_CHECK(err);

    err = nvs_commit(cfgHandle);
    ESP_ERROR_CHECK(err);
    nvs_close(cfgHandle);
  }
}

/** developer note:
 *
 * these are meant to provide valid values for the frame's data to be almost
 * identical to a pwnagotchi's. they must be within a certain range to be valid.
 *
 */

/**
 * Generate a random integer based on a range of numbers
 * @param min Lowest number
 * @param max Highest number
 */
int Config::random(int min, int max) { return min + rand() % (max - min + 1); }

/**
 * Checks current uptime
 */
int Config::time() { return millis() / 1000; }
