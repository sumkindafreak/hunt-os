#include "HuntEventBus.h"

HuntEventBus::HuntEventBus() {
  head = 0;
  tail = 0;
  queued = 0;
}

void HuntEventBus::begin() {
  head = 0;
  tail = 0;
  queued = 0;
}

bool HuntEventBus::publish(const HuntEvent &event) {
  if (queued >= HUNT_EVENT_BUS_QUEUE_SIZE) {
    return false;
  }

  queue[tail] = event;
  tail++;
  if (tail >= HUNT_EVENT_BUS_QUEUE_SIZE) tail = 0;
  queued++;
  return true;
}

bool HuntEventBus::available() {
  return queued > 0;
}

HuntEvent HuntEventBus::read() {
  HuntEvent event = huntCreateEvent(HUNT_EVENT_NONE, "SYSTEM", "SYSTEM", "EMPTY");

  if (queued <= 0) {
    return event;
  }

  event = queue[head];
  head++;
  if (head >= HUNT_EVENT_BUS_QUEUE_SIZE) head = 0;
  queued--;
  return event;
}

int HuntEventBus::count() {
  return queued;
}
