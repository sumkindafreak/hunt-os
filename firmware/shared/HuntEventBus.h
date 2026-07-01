#ifndef HUNT_EVENT_BUS_H
#define HUNT_EVENT_BUS_H

#include <Arduino.h>
#include "HuntEvents.h"

#define HUNT_EVENT_BUS_QUEUE_SIZE 10

// =====================================================
// HUNT Event Bus v0.1
//
// Simple fixed-size event queue for Alpha.
// Later this can evolve into a listener/subscriber system.
// =====================================================

class HuntEventBus {
public:
  HuntEventBus();

  void begin();
  bool publish(const HuntEvent &event);
  bool available();
  HuntEvent read();
  int count();

private:
  HuntEvent queue[HUNT_EVENT_BUS_QUEUE_SIZE];
  int head;
  int tail;
  int queued;
};

#endif
