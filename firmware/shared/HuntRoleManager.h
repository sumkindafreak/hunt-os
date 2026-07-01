#ifndef HUNT_ROLE_MANAGER_H
#define HUNT_ROLE_MANAGER_H

#include <Arduino.h>
#include "HuntTypes.h"
#include "HuntEvents.h"

class HuntRoleManager {
public:
  HuntRoleManager();

  void begin(HuntBaseRole startingRole);
  void setRole(HuntBaseRole newRole);
  void nextRole();
  void previousRole();

  HuntBaseRole getRole();
  String getRoleName();
  HuntEvent buildActivationEvent(const String &sourceDevice, const String &targetDevice);

private:
  HuntBaseRole role;
  int roleIndex;
};

#endif
