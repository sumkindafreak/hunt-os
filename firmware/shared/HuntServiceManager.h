#ifndef HUNT_SERVICE_MANAGER_H
#define HUNT_SERVICE_MANAGER_H

#include <Arduino.h>
#include "HuntService.h"
#include "HuntDebug.h"

#define HUNT_MAX_SERVICES 12

class HuntServiceManager {
public:
  HuntServiceManager();

  bool registerService(HuntService *service);
  void beginAll();
  void updateAll();
  int serviceCount();

private:
  HuntService *services[HUNT_MAX_SERVICES];
  int count;
};

#endif
