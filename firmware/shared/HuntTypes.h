#ifndef HUNT_TYPES_H
#define HUNT_TYPES_H

#include <Arduino.h>

#define HUNT_PROTOCOL_VERSION "0.1"
#define HUNT_MAX_PACKET_LENGTH 180
#define HUNT_HEARTBEAT_INTERVAL_MS 5000

enum HuntDeviceType {
  DEVICE_PLAYER,
  DEVICE_BASE,
  DEVICE_ADMIN,
  DEVICE_UNKNOWN
};

enum HuntPlayerState {
  PLAYER_ALIVE,
  PLAYER_INFECTED,
  PLAYER_SAFE,
  PLAYER_ELIMINATED
};

enum HuntBaseRole {
  ROLE_SAFE_ZONE,
  ROLE_SCANNER,
  ROLE_OBJECTIVE,
  ROLE_EXTRACTION
};

struct HuntPacket {
  bool valid;
  String version;
  String type;
  String source;
  String target;
  String payload;
};

#endif
