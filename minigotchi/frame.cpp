/**
 * frame.cpp: handles the sending of "pwnagotchi" beacon frames
*/

#include "frame.h"

/** developer note: 
 *
 * when it comes to detecting a pwnagotchi, this is done with pwngrid/opwngrid.
 * essentially pwngrid looks for the numbers 222-226 in payloads, and if they aren't there, it ignores it.
 * these need to be put into the frames!!!
 *
 * note that these frames aren't just normal beacon frames, rather a modified one with data, additional ids, etc.
 * frames are dynamically constructed, headers are included like a normal frame.
 * by far this is the most memory heaviest part of the minigotchi, the reason is 
 * 
*/

// initializing
bool Frame::running = false;

// payload ID's according to pwngrid
const uint8_t Frame::IDWhisperPayload = 0xDE;
const uint8_t Frame::IDWhisperCompression = 0xDF;
const uint8_t Frame::IDWhisperIdentity = 0xE0;
const uint8_t Frame::IDWhisperSignature = 0xE1;
const uint8_t Frame::IDWhisperStreamHeader = 0xE2;

// other addresses
const uint8_t Frame::SignatureAddr[] = {0xde, 0xad, 0xbe, 0xef, 0xde, 0xad};
const uint8_t Frame::BroadcastAddr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
const int Frame::wpaFlags = 1041;

// frame control, etc
const uint8_t Frame::header[] = {
    0x80, 0x00,                         // Frame Control: Version 0, Type: Management, Subtype: Beacon
    0x00, 0x00,                         // Duration/ID (will be overwritten)
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination address: Broadcast
    0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, // Source address: Set in "init"
    0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, // BSSID: Set in "init"
    0x00, 0x00                          // Sequence/Fragment number
};

/** developer note:
 * 
 * according to pack.go:
 * we build the frame like so
 * 
 * func PackOneOf(from, to net.HardwareAddr, peerID []byte, signature []byte, streamID uint64, seqNum uint64, seqTot uint64, payload []byte, compress bool) (error, []byte) {
 * 	stack := []gopacket.SerializableLayer{
 *		&layers.RadioTap{},
 *		&layers.Dot11{
 *			Address1: to,
 *			Address2: SignatureAddr,
 *			Address3: from,
 *			Type:     layers.Dot11TypeMgmtBeacon,
 *		},
 * 		&layers.Dot11MgmtBeacon{
 *			Flags:    uint16(wpaFlags),
 *			Interval: 100,
 *		},
 *	}
 *
 *	if peerID != nil {
 *		stack = append(stack, Info(IDWhisperIdentity, peerID))
 *	}
 *
 *	if signature != nil {
 *		stack = append(stack, Info(IDWhisperSignature, signature))
 *	}
 *
 *	if streamID > 0 {
 *		streamBuf := new(bytes.Buffer)
 *		if err := binary.Write(streamBuf, binary.LittleEndian, streamID); err != nil {
 *			return err, nil
 *		} else if err = binary.Write(streamBuf, binary.LittleEndian, seqNum); err != nil {
 *			return err, nil
 *		} else if err = binary.Write(streamBuf, binary.LittleEndian, seqTot); err != nil {
 *			return err, nil
 *		}
 *		stack = append(stack, Info(IDWhisperStreamHeader, streamBuf.Bytes()))
 *	}
 *
 *	if compress {
 *		if didCompress, compressed, err := Compress(payload); err != nil {
 *			return err, nil
 *		} else if didCompress {
 *			stack = append(stack, Info(IDWhisperCompression, []byte{1}))
 *			payload = compressed
 *		}
 *	}
 *
 *	dataSize := len(payload)
 *	dataLeft := dataSize
 *	dataOff := 0
 *	chunkSize := 0xff
 *
 *	for dataLeft > 0 {
 *		sz := chunkSize
 *		if dataLeft < chunkSize {
 *			sz = dataLeft
 *		}
 *
 *		chunk := payload[dataOff : dataOff+sz]
 *		stack = append(stack, Info(IDWhisperPayload, chunk))
 *
 *		dataOff += sz
 *		dataLeft -= sz
 *	}
 *
 *	return Serialize(stack...)
 * }
 *
 * ofc, when it comes to any new programming language such as Go, i am pretty clueless as how to interpret it
 * so this is all my best try
 *  
*/

void Frame::pack() {
    // clear frame before constructing
    frameControl.clear();
    beaconFrame.clear();

    // copy pre-defined header to beaconFrame
    beaconFrame.insert(beaconFrame.end(), Frame::header, Frame::header + sizeof(Frame::header));

    // parse and set the BSSID (to)
    const char* bssidStr = Config::bssid;
    uint8_t bssidBytes[6];

    char *token = strtok((char*)bssidStr, ":");
    int i = 0;
    while (token != NULL && i < 6) {
        bssidBytes[i++] = strtol(token, NULL, 16);
        token = strtok(NULL, ":");
    }

    // set the BSSID in the frame header
    std::copy(bssidBytes, bssidBytes + 6, beaconFrame.begin() + 10);

    // set signature address
    beaconFrame[10] = Frame::SignatureAddr[0];
    beaconFrame[11] = Frame::SignatureAddr[1];
    beaconFrame[12] = Frame::SignatureAddr[2];
    beaconFrame[13] = Frame::SignatureAddr[3];
    beaconFrame[14] = Frame::SignatureAddr[4];
    beaconFrame[15] = Frame::SignatureAddr[5];

    // get mac addr (from)
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char macStr[18];
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // dynamic construction
    size_t offset = 0;

    // id's
    beaconFrame.push_back(Frame::IDWhisperIdentity);
    beaconFrame.push_back(Frame::IDWhisperSignature);
    beaconFrame.push_back(Frame::IDWhisperStreamHeader);

    // other payload data
    beaconFrame.push_back(Config::epoch);
    
    // all chars
    for (size_t i = 0; i < sizeof(Config::face); ++i) {
        beaconFrame.push_back(Config::face[i]);
    }

    for (size_t i = 0; i < sizeof(Config::identity); ++i) {
        beaconFrame.push_back(Config::identity[i]);
    }

    for (size_t i = 0; i < sizeof(Config::name); ++i) {
        beaconFrame.push_back(Config::name[i]);
    }

    beaconFrame.push_back(Config::associate);
    
    beaconFrame.push_back(Config::bored_num_epochs);
    beaconFrame.push_back(Config::excited_num_epochs);
    beaconFrame.push_back(Config::hop_recon_time);
    beaconFrame.push_back(Config::max_inactive_scale);
    beaconFrame.push_back(Config::max_interactions);
    beaconFrame.push_back(Config::max_misses_for_recon);
    beaconFrame.push_back(Config::min_recon_time);
    beaconFrame.push_back(Config::min_rssi);
    beaconFrame.push_back(Config::recon_inactive_multiplier);
    beaconFrame.push_back(Config::recon_time);
    beaconFrame.push_back(Config::sad_num_epochs);
    beaconFrame.push_back(Config::sta_ttl);
    beaconFrame.push_back(Config::pwnd_run);
    beaconFrame.push_back(Config::pwnd_tot);

    for (size_t i = 0; i < sizeof(Config::session_id); ++i) {
        beaconFrame.push_back(Config::session_id[i]);
    }

    beaconFrame.push_back(Config::uptime);

    for (size_t i = 0; i < sizeof(Config::version); ++i) {
        beaconFrame.push_back(Config::version[i]);
    }

    // payload size
    const size_t payloadSize = beaconFrame.size();

    // full frame size
    frameSize = beaconFrame.size();

    // add IDWhisperPayload for every chunk
    const size_t chunkSize = 0xff;

    for (size_t i = 0; i < frameSize; i += chunkSize) {
    beaconFrame.push_back(IDWhisperPayload);

    size_t chunkEnd = std::min(i + chunkSize, payloadSize);
    for (size_t j = i; j < chunkEnd; ++j) {
        beaconFrame.push_back(beaconFrame[j]);
        }
    }
}

void Frame::send() {
    // build frame
    Frame::pack();

    // send full frame
    // we dont use raw80211 since it sends a header(which we don't need), although we do use it for monitoring, etc.
    WiFi.rawPacket(beaconFrame.data(), frameSize);
}


void Frame::advertise() {
    if (Config::advertise && Frame::running) {
        send();
        delay(100);
    } else {
        // do nothing but still idle
    }
}

void Frame::start() {
    Frame::running = true;
}

void Frame::stop() {
    Frame::running = false;
}
