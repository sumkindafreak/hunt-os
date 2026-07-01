#ifndef HUNT_PLAYER_STATE_H
#define HUNT_PLAYER_STATE_H

#include <Arduino.h>
#include "HuntTypes.h"
#include "HuntEvents.h"

class HuntPlayerStateManager {
public:
  HuntPlayerStateManager();

  void begin(const String &playerId);
  void handleEvent(const HuntEvent &event);

  String getId();
  HuntPlayerState getState();
  String getStateName();
  int getHealth();
  int getLives();

  void setState(HuntPlayerState newState);
  void setHealth(int newHealth);
  void heal(int amount);
  void damage(int amount);

private:
  String id;
  HuntPlayerState state;
  int health;
  int lives;
};

#endif
