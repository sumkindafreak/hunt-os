#include "HuntSceneService.h"

HuntSceneService::HuntSceneService(HuntEventBus *eventBus) {
  events = eventBus;
  activeScene = "SAFE_ZONE";
}

void HuntSceneService::begin() {
  activeScene = "SAFE_ZONE";
  if (events != nullptr) events->publish(huntCreateEvent(HUNT_EVENT_BASE_ACTIVATED, "SCENE", "ALL", activeScene));
}

void HuntSceneService::update() {
  // Alpha scene service is command-driven.
}

const char *HuntSceneService::name() { return "Scene"; }

void HuntSceneService::loadScene(const String &sceneName) {
  activeScene = sceneName;
  if (events != nullptr) events->publish(huntCreateEvent(HUNT_EVENT_BASE_ACTIVATED, "SCENE", "ALL", activeScene));
}

String HuntSceneService::currentScene() { return activeScene; }
