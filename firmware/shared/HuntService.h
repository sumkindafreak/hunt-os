#ifndef HUNT_SERVICE_H
#define HUNT_SERVICE_H

#include <Arduino.h>

// =====================================================
// HUNT Service Interface v0.1
//
// Every major HUNT OS subsystem should behave like a
// service with begin() and update() methods.
// =====================================================

class HuntService {
public:
  virtual void begin() = 0;
  virtual void update() = 0;
  virtual const char *name() = 0;
};

#endif
