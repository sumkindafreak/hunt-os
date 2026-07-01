#ifndef HUNT_KERNEL_H
#define HUNT_KERNEL_H

#include <Arduino.h>
#include "HuntEventBus.h"
#include "HuntServiceManager.h"
#include "HuntDebug.h"

// =====================================================
// HUNT Kernel v0.1
//
// Central coordinator for HUNT OS services.
// =====================================================

class HuntKernel {
public:
  HuntKernel();

  void begin();
  void update();
  HuntEventBus *events();
  HuntServiceManager *services();

private:
  HuntEventBus eventBus;
  HuntServiceManager serviceManager;
  unsigned long bootMs;
};

#endif
