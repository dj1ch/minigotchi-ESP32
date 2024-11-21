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
 * config.h: header files for config.cpp
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "minigotchi.h"
#include "parasite.h"
#include "webui.h"
#include <Arduino.h>
#include <esp_wifi.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// quick and dirty way to save space if you're not using a display
#define disp 1

class Config {
public:
  static bool deauth;
  static bool advertise;
  static bool scan;
  static bool spam;
  static const char *ssid;
  static const char *pass;
  static int shortDelay;
  static int longDelay;
  static bool parasite;
  static bool display;
  static std::string screen;
  static int baud;
  static int channel;
  static std::vector<std::string> whitelist;
  static String happy;
  static String sad;
  static String broken;
  static String intense;
  static String looking1;
  static String looking2;
  static String neutral;
  static String sleeping;
  static int epoch;
  static std::string face;
  static std::string identity;
  static std::string name;
  static int ap_ttl;
  static bool associate;
  static int bored_num_epochs;
  static int channels[13];
  static int excited_num_epochs;
  static int hop_recon_time;
  static int max_inactive_scale;
  static int max_interactions;
  static int max_misses_for_recon;
  static int min_recon_time;
  static int min_rssi;
  static int recon_inactive_multiplier;
  static int recon_time;
  static int sad_num_epochs;
  static int sta_ttl;
  static int pwnd_run;
  static int pwnd_tot;
  static std::string session_id;
  static int uptime;
  static std::string version;
  static wifi_init_config_t wifiCfg;
  static wifi_country_t ctryCfg;
  static bool configured;
  static void loadConfig();
  static void saveConfig();

private:
  static int random(int min, int max);
  static int time();
};

#endif // CONFIG_H
