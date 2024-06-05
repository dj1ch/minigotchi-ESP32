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

// define whether or not the deauthing or advertising is turned on 
bool Config::deauth = true;
bool Config::advertise = true;

// screen configuration
bool Config::display = false;  
std::string Config::screen = "";  

// define baud rate
int Config::baud = 115200;

// define init channel
int Config::channel = 1;

// define whitelist 
std::vector<std::string> Config::whitelist = {"SSID", "SSID", "SSID"}; 

// json config
int Config::epoch = 1;
std::string Config::face = "(^-^)";
std::string Config::identity = "b9210077f7c14c0651aa338c55e820e93f90110ef679648001b1cecdbffc0090";
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

// define version(please do not change, this should not be changed)
std::string Config::version = "3.2.2-beta";

/** developer note:
 * 
 * these are meant to provide valid values for the frame's data to be almost identical to a pwnagotchi's.
 * they must be within a certain range to be valid.
 * 
*/

// randomize config values
int Config::random(int min, int max) {
    return min + rand() % (max - min + 1);
}

int Config::time() {
    return millis() / 1000; // convert to seconds
}