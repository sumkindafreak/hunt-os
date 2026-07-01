#include "HuntLightingService.h"

HuntLightingService::HuntLightingService(HuntEventBus *eventBus, HuntNeoPixelManager *status, HuntNeoPixelManager *decor) {
  events = eventBus;
  statusPixels = status;
  decorPixels = decor;
}

void HuntLightingService::begin() {
  if (statusPixels != nullptr) statusPixels->begin();
  if (decorPixels != nullptr) decorPixels->begin();
  setRole(ROLE_SAFE_ZONE);
}

void HuntLightingService::update() {
  if (statusPixels != nullptr) statusPixels->update();
  if (decorPixels != nullptr) decorPixels->update();
}

const char *HuntLightingService::name() { return "Lighting"; }

void HuntLightingService::setRole(HuntBaseRole role) {
  if (statusPixels != nullptr) statusPixels->setRoleColour(role);
  if (decorPixels != nullptr) decorPixels->setRoleColour(role);
}

void HuntLightingService::flash() {
  if (statusPixels != nullptr) statusPixels->flashActivation();
  if (decorPixels != nullptr) decorPixels->flashActivation();
}

void HuntLightingService::error() {
  if (statusPixels != nullptr) statusPixels->showError();
  if (decorPixels != nullptr) decorPixels->showError();
}
