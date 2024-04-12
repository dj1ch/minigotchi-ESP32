/**
 * source: https://github.com/tht/80211Raw/blob/master/structs.h
*/

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

/**
 * WiFi (802.11) and SDK structures to use with WiFi.
 * Quite a few are not used currently. Still keeping them for reference.
 */

#ifdef ESP32
    #include <WiFi.h>
    #include <esp_wifi.h>
#else
    #include <ESP8266WiFi.h>
#endif

// SDK structures -----------------------------------

#ifndef ESP32
typedef struct {
    signed rssi:8;            //< signal intensity of packet
    unsigned rate:4;          //< data rate
    unsigned is_group:1;
    unsigned :1;              //< reserve
    unsigned sig_mode:2;      //< 0:is not 11n packet; 1:is 11n packet
    unsigned legacy_length:12;
    unsigned damatch0:1;
    unsigned damatch1:1;
    unsigned bssidmatch0:1;
    unsigned bssidmatch1:1;
    unsigned mcs:7;           //< if is 11n packet, shows the modulation(range from 0 to 76)
    unsigned cwb:1;           //< if is 11n packet, shows if is HT40 packet or not
    unsigned HT_length:16;    //< reserve
    unsigned smoothing:1;     //< reserve
    unsigned not_sounding:1;  //< reserve
    unsigned :1;              //< reserve
    unsigned aggregation:1;   //< Aggregation
    unsigned stbc:2;          //< STBC
    unsigned fec_coding:1;    //< Flag is set for 11n packets which are LDPC
    unsigned sgi:1;           //< SGI
    unsigned rxend_state:8;
    unsigned ampdu_cnt:8;     //< ampdu cnt
    unsigned channel:4;       //< which channel this packet in
    unsigned :4;              //< reserve
    signed noise_floor:8;
} wifi_pkt_rx_ctrl_t;

typedef struct {
    wifi_pkt_rx_ctrl_t rx_ctrl; //< metadata header
    uint8_t payload[0];       //< Data or management payload. Length of payload is described by rx_ctrl.sig_len. Type of content determined by packet type argument of callback.
} wifi_promiscuous_pkt_t;
#endif

typedef struct { // Size: 128
    wifi_pkt_rx_ctrl_t rx_ctrl;
    uint8_t buf[112];
    uint16_t cnt;
    uint16_t len; //length of packet
} wifi_pkt_mgmt_t;

typedef struct {
    uint16_t length;
    uint16_t seq;
    uint8_t  address3[6];
} wifi_pkt_lenseq_t;

typedef struct {
    wifi_pkt_rx_ctrl_t rx_ctrl;
    uint8_t  buf[36];
    uint16_t cnt;
    wifi_pkt_lenseq_t lenseq[1];
} wifi_pkt_data_t;


// IEEE802.11 data structures ---------------------

#ifndef ESP32
typedef enum {
    WIFI_PKT_MGMT,
    WIFI_PKT_CTRL,
    WIFI_PKT_DATA,
    WIFI_PKT_MISC,
} wifi_promiscuous_pkt_type_t;
#endif

typedef enum {
    ASSOCIATION_REQ,
    ASSOCIATION_RES,
    REASSOCIATION_REQ,
    REASSOCIATION_RES,
    PROBE_REQ,
    PROBE_RES,
    NU1,  /* ......................*/
    NU2,  /* 0110, 0111 not used */
    BEACON,
    ATIM,
    DISASSOCIATION,
    AUTHENTICATION,
    DEAUTHENTICATION,
    ACTION,
    ACTION_NACK,
} wifi_mgmt_subtypes_t;

typedef struct {
    unsigned interval:16;
    unsigned capability:16;
    unsigned tag_number:8;
    unsigned tag_length:8;
    char ssid[0];
    uint8_t rates[1];
} wifi_mgmt_beacon_t;

typedef struct {
    unsigned protocol:2;
    unsigned type:2;
    unsigned subtype:4;
    unsigned to_ds:1;
    unsigned from_ds:1;
    unsigned more_frag:1;
    unsigned retry:1;
    unsigned pwr_mgmt:1;
    unsigned more_data:1;
    unsigned wep:1;
    unsigned strict:1;
} wifi_header_frame_control_t;

/**
 * Ref: https://github.com/lpodkalicki/blog/blob/master/esp32/016_wifi_sniffer/main/main.c
 */
typedef struct {
    wifi_header_frame_control_t frame_ctrl;
    //unsigned duration_id:16; /* !!!! ugly hack */
    uint8_t addr1[6]; /* receiver address */
    uint8_t addr2[6]; /* sender address */
    uint8_t addr3[6]; /* filtering address */
    unsigned sequence_ctrl:16;
    uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[2]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

#endif
