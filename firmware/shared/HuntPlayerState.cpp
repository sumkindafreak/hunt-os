#include "HuntPlayerState.h"

HuntPlayerStateManager::HuntPlayerStateManager() {
  id = "PLAYER_UNKNOWN";
  state = PLAYER_ALIVE;
  health = 100;
  lives = 1;
}

void HuntPlayerStateManager::begin(const String &playerId) {
  id = playerId;
  state = PLAYER_ALIVE;
  health = 100;
  lives = 1;
}

void HuntPlayerStateManager::handleEvent(const HuntEvent &event) {
  if (event.name == HUNT_EVENT_PLAYER_HEALED) {
    heal(event.data.toInt());
  } else if (event.name == HUNT_EVENT_PLAYER_INFECTED) {
    state = PLAYER_INFECTED;
  } else if (event.name == HUNT_EVENT_PLAYER_SAFE) {
    state = PLAYER_SAFE;
  } else if (event.name == HUNT_EVENT_PLAYER_ELIMINATED) {
    state = PLAYER_ELIMINATED;
    health = 0;
  }
}

String HuntPlayerStateManager::getId() {
  return id;
}

HuntPlayerState HuntPlayerStateManager::getState() {
  return state;
}

String HuntPlayerStateManager::getStateName() {
  switch (state) {
    case PLAYER_ALIVE: return "ALIVE";
    case PLAYER_INFECTED: return "INFECTED";
    case PLAYER_SAFE: return "SAFE";
    case PLAYER_ELIMINATED: return "ELIMINATED";
  }
  return "UNKNOWN";
}

int HuntPlayerStateManager::getHealth() {
  return health;
}

int HuntPlayerStateManager::getLives() {
  return lives;
}

void HuntPlayerStateManager::setState(HuntPlayerState newState) {
  state = newState;
}

void HuntPlayerStateManager::setHealth(int newHealth) {
  health = constrain(newHealth, 0, 100);
}

void HuntPlayerStateManager::heal(int amount) {
  health = constrain(health + amount, 0, 100);
  if (health > 0 && state == PLAYER_ELIMINATED) {
    state = PLAYER_ALIVE;
  }
}

void HuntPlayerStateManager::damage(int amount) {
  health = constrain(health - amount, 0, 100);
  if (health <= 0) {
    state = PLAYER_ELIMINATED;
  }
}
