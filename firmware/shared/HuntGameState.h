#ifndef HUNT_GAME_STATE_H
#define HUNT_GAME_STATE_H

#include <Arduino.h>
#include "HuntEvents.h"

// =====================================================
// HUNT Game State Manager v0.1
//
// Tracks the overall state of the current game.
// =====================================================

enum HuntGamePhase {
  GAME_LOBBY,
  GAME_RUNNING,
  GAME_PAUSED,
  GAME_ENDED
};

class HuntGameStateManager {
public:
  HuntGameStateManager();

  void begin();
  void handleEvent(const HuntEvent &event);
  HuntGamePhase getPhase();
  String getPhaseName();
  unsigned long getGameStartMs();

private:
  HuntGamePhase phase;
  unsigned long gameStartMs;
};

#endif
