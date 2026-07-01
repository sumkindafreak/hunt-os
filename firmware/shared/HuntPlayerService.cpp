#include "HuntPlayerService.h"
#include "HuntDebug.h"

HuntPlayerService::HuntPlayerService(HuntEventBus *eventBus, const String &playerId) {
  events = eventBus;
  id = playerId;
}

void HuntPlayerService::begin() {
  playerState.begin(id);
  huntLog("Player service ready: " + id);
}

void HuntPlayerService::update() {
  // Alpha uses direct applyEvent calls. Bus subscription comes later.
}

const char *HuntPlayerService::name() { return "Player"; }

HuntPlayerStateManager *HuntPlayerService::state() {
  return &playerState;
}

void HuntPlayerService::applyEvent(const HuntEvent &event) {
  playerState.handleEvent(event);
}
