#ifndef HUNT_PLAYER_SERVICE_H
#define HUNT_PLAYER_SERVICE_H

#include <Arduino.h>
#include "HuntService.h"
#include "HuntEventBus.h"
#include "HuntPlayerState.h"

class HuntPlayerService : public HuntService {
public:
  HuntPlayerService(HuntEventBus *eventBus, const String &playerId);

  void begin() override;
  void update() override;
  const char *name() override;

  HuntPlayerStateManager *state();
  void applyEvent(const HuntEvent &event);

private:
  HuntEventBus *events;
  HuntPlayerStateManager playerState;
  String id;
};

#endif
