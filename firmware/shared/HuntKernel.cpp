#include "HuntKernel.h"

HuntKernel::HuntKernel() {
  bootMs = 0;
}

void HuntKernel::begin() {
  bootMs = millis();
  huntLogInfo("CORE", "HUNT kernel boot");
  eventBus.begin();
  serviceManager.beginAll();
  huntLogInfo("CORE", "HUNT kernel ready");
}

void HuntKernel::update() {
  serviceManager.updateAll();
}

HuntEventBus *HuntKernel::events() {
  return &eventBus;
}

HuntServiceManager *HuntKernel::services() {
  return &serviceManager;
}
