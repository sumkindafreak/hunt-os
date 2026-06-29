#ifndef HUNT_PROTOCOL_H
#define HUNT_PROTOCOL_H

#include <Arduino.h>
#include "HuntTypes.h"

// Builds a standard HUNT packet in this format:
// HUNT|VERSION|TYPE|SOURCE|TARGET|PAYLOAD
String huntBuildPacket(const String &type, const String &source, const String &target, const String &payload);

// Parses a received packet into a HuntPacket struct.
HuntPacket huntParsePacket(const String &rawPacket);

// Returns true if the packet is valid and intended for this device or ALL.
bool huntIsPacketForDevice(const HuntPacket &packet, const String &deviceId);

#endif
