#ifndef HUNT_EVENTS_H
#define HUNT_EVENTS_H

#include <Arduino.h>

// =====================================================
// HUNT Event System v0.1
//
// This file defines the common event names used inside
// HUNT OS. Keeping names in one place prevents random
// strings being scattered through the firmware.
// =====================================================

#define HUNT_EVENT_NONE "NONE"
#define HUNT_EVENT_GAME_START "GAME_START"
#define HUNT_EVENT_GAME_END "GAME_END"
#define HUNT_EVENT_PLAYER_JOINED "PLAYER_JOINED"
#define HUNT_EVENT_PLAYER_HEALED "PLAYER_HEALED"
#define HUNT_EVENT_PLAYER_INFECTED "PLAYER_INFECTED"
#define HUNT_EVENT_PLAYER_SAFE "PLAYER_SAFE"
#define HUNT_EVENT_PLAYER_ELIMINATED "PLAYER_ELIMINATED"
#define HUNT_EVENT_BASE_ACTIVATED "BASE_ACTIVATED"
#define HUNT_EVENT_OBJECTIVE_COMPLETE "OBJECTIVE_COMPLETE"
#define HUNT_EVENT_EXTRACTION_OPEN "EXTRACTION_OPEN"

struct HuntEvent {
  String name;
  String source;
  String target;
  String data;
  unsigned long timestampMs;
};

inline HuntEvent huntCreateEvent(const String &name, const String &source, const String &target, const String &data) {
  HuntEvent event;
  event.name = name;
  event.source = source;
  event.target = target;
  event.data = data;
  event.timestampMs = millis();
  return event;
}

#endif
