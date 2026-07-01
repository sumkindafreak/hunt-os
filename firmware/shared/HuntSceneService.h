#ifndef HUNT_SCENE_SERVICE_H
#define HUNT_SCENE_SERVICE_H

#include <Arduino.h>
#include "HuntService.h"
#include "HuntEventBus.h"
#include "HuntEvents.h"
#include "HuntTypes.h"

class HuntSceneService : public HuntService {
public:
  HuntSceneService(HuntEventBus *eventBus);

  void begin() override;
  void update() override;
  const char *name() override;

  void loadScene(const String &sceneName);
  String currentScene();

private:
  HuntEventBus *events;
  String activeScene;
};

#endif
