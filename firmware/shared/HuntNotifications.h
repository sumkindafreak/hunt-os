#ifndef HUNT_NOTIFICATIONS_H
#define HUNT_NOTIFICATIONS_H

#include <Arduino.h>
#include "HuntEvents.h"

// =====================================================
// HUNT Notification Manager v0.1
//
// Converts game events into simple feedback requests.
// Device sketches decide how to turn these into OLED,
// LED and buzzer behaviour.
// =====================================================

enum HuntNotificationType {
  NOTIFY_NONE,
  NOTIFY_CONFIRM,
  NOTIFY_ALERT,
  NOTIFY_WARNING,
  NOTIFY_SUCCESS
};

class HuntNotificationManager {
public:
  HuntNotificationManager();

  void begin();
  void handleEvent(const HuntEvent &event);
  bool hasNotification();
  HuntNotificationType getType();
  String getMessage();
  void clear();

private:
  bool pending;
  HuntNotificationType type;
  String message;
};

#endif
