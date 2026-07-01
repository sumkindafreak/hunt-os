#include "HuntKernel.h"

HuntKernel::HuntKernel() {
  bootMs = 0;
}

void HuntKernel::begin() {
  bootMs = millis();
  huntLog("HUNT Kernel begin");
  eventBus.begin();
  serviceManager.beginAll();
  huntLog("HUNT Kernel ready");
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
