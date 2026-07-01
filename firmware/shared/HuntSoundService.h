#ifndef HUNT_SOUND_SERVICE_H
#define HUNT_SOUND_SERVICE_H

#include <Arduino.h>
#include "HuntService.h"
#include "HuntEventBus.h"

class HuntSoundService : public HuntService {
public:
  HuntSoundService(HuntEventBus *eventBus);

  void begin() override;
  void update() override;
  const char *name() override;

  void playTrack(uint16_t trackNumber);
  void stopAll();
  void volume(uint8_t level);

private:
  HuntEventBus *events;
  uint8_t volumeLevel;
};

#endif
