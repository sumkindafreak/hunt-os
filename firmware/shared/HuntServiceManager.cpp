#include "HuntServiceManager.h"

HuntServiceManager::HuntServiceManager() {
  count = 0;
  for (int i = 0; i < HUNT_MAX_SERVICES; i++) {
    services[i] = nullptr;
  }
}

bool HuntServiceManager::registerService(HuntService *service) {
  if (service == nullptr) {
    return false;
  }

  if (count >= HUNT_MAX_SERVICES) {
    huntLog("Service manager full");
    return false;
  }

  services[count] = service;
  count++;
  huntLog(String("Registered service: ") + service->name());
  return true;
}

void HuntServiceManager::beginAll() {
  for (int i = 0; i < count; i++) {
    if (services[i] != nullptr) {
      huntLog(String("Starting service: ") + services[i]->name());
      services[i]->begin();
    }
  }
}

void HuntServiceManager::updateAll() {
  for (int i = 0; i < count; i++) {
    if (services[i] != nullptr) {
      services[i]->update();
    }
  }
}

int HuntServiceManager::serviceCount() {
  return count;
}
