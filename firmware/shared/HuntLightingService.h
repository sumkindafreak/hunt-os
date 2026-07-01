#ifndef HUNT_LIGHTING_SERVICE_H
#define HUNT_LIGHTING_SERVICE_H

#include <Arduino.h>
#include "HuntService.h"
#include "HuntEventBus.h"
#include "HuntNeoPixels.h"
#include "HuntEvents.h"
#include "HuntTypes.h"

class HuntLightingService : public HuntService {
public:
  HuntLightingService(HuntEventBus *eventBus, HuntNeoPixelManager *status, HuntNeoPixelManager *decor);

  void begin() override;
  void update() override;
  const char *name() override;

  void setRole(HuntBaseRole role);
  void flash();
  void error();

private:
  HuntEventBus *events;
  HuntNeoPixelManager *statusPixels;
  HuntNeoPixelManager *decorPixels;
};

#endif
