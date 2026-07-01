#include "HuntNotifications.h"

HuntNotificationManager::HuntNotificationManager() {
  pending = false;
  type = NOTIFY_NONE;
  message = "";
}

void HuntNotificationManager::begin() {
  clear();
}

void HuntNotificationManager::handleEvent(const HuntEvent &event) {
  pending = true;

  if (event.name == HUNT_EVENT_PLAYER_HEALED || event.name == HUNT_EVENT_PLAYER_SAFE) {
    type = NOTIFY_SUCCESS;
    message = "Player safe";
  } else if (event.name == HUNT_EVENT_PLAYER_INFECTED) {
    type = NOTIFY_ALERT;
    message = "Infected";
  } else if (event.name == HUNT_EVENT_OBJECTIVE_COMPLETE) {
    type = NOTIFY_SUCCESS;
    message = "Objective complete";
  } else if (event.name == HUNT_EVENT_EXTRACTION_OPEN) {
    type = NOTIFY_SUCCESS;
    message = "Extraction open";
  } else if (event.name == HUNT_EVENT_GAME_START) {
    type = NOTIFY_CONFIRM;
    message = "Game started";
  } else {
    type = NOTIFY_CONFIRM;
    message = event.name;
  }
}

bool HuntNotificationManager::hasNotification() {
  return pending;
}

HuntNotificationType HuntNotificationManager::getType() {
  return type;
}

String HuntNotificationManager::getMessage() {
  return message;
}

void HuntNotificationManager::clear() {
  pending = false;
  type = NOTIFY_NONE;
  message = "";
}
