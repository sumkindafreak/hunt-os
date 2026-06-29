#include "HuntRoleManager.h"

HuntRoleManager::HuntRoleManager() {
  role = ROLE_SAFE_ZONE;
  roleIndex = 0;
}

void HuntRoleManager::begin(HuntBaseRole startingRole) {
  setRole(startingRole);
}

void HuntRoleManager::setRole(HuntBaseRole newRole) {
  role = newRole;

  switch (role) {
    case ROLE_SAFE_ZONE: roleIndex = 0; break;
    case ROLE_SCANNER: roleIndex = 1; break;
    case ROLE_OBJECTIVE: roleIndex = 2; break;
    case ROLE_EXTRACTION: roleIndex = 3; break;
  }
}

void HuntRoleManager::nextRole() {
  roleIndex++;
  if (roleIndex > 3) roleIndex = 0;
  setRole((HuntBaseRole)roleIndex);
}

void HuntRoleManager::previousRole() {
  roleIndex--;
  if (roleIndex < 0) roleIndex = 3;
  setRole((HuntBaseRole)roleIndex);
}

HuntBaseRole HuntRoleManager::getRole() {
  return role;
}

String HuntRoleManager::getRoleName() {
  switch (role) {
    case ROLE_SAFE_ZONE: return "SAFE_ZONE";
    case ROLE_SCANNER: return "SCANNER";
    case ROLE_OBJECTIVE: return "OBJECTIVE";
    case ROLE_EXTRACTION: return "EXTRACTION";
  }
  return "UNKNOWN";
}

HuntEvent HuntRoleManager::buildActivationEvent(const String &sourceDevice, const String &targetDevice) {
  if (role == ROLE_SAFE_ZONE) {
    return huntCreateEvent(HUNT_EVENT_PLAYER_SAFE, sourceDevice, targetDevice, "25");
  }

  if (role == ROLE_SCANNER) {
    return huntCreateEvent(HUNT_EVENT_PLAYER_INFECTED, sourceDevice, targetDevice, "0");
  }

  if (role == ROLE_OBJECTIVE) {
    return huntCreateEvent(HUNT_EVENT_OBJECTIVE_COMPLETE, sourceDevice, targetDevice, getRoleName());
  }

  if (role == ROLE_EXTRACTION) {
    return huntCreateEvent(HUNT_EVENT_EXTRACTION_OPEN, sourceDevice, targetDevice, getRoleName());
  }

  return huntCreateEvent(HUNT_EVENT_BASE_ACTIVATED, sourceDevice, targetDevice, getRoleName());
}
