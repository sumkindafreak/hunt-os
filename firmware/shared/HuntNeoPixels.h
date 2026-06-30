#ifndef HUNT_NEOPIXELS_H
#define HUNT_NEOPIXELS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "HuntTypes.h"
#include "HuntRoleManager.h"

// =====================================================
// HUNT NeoPixel Manager v0.1
//
// Used by Base Nodes to display role, status and activity
// feedback using WS2812 / NeoPixel-compatible LEDs.
// =====================================================

class HuntNeoPixelManager {
public:
  HuntNeoPixelManager(uint8_t pin, uint16_t count, uint8_t brightness);

  void begin();
  void update();
  void setRoleColour(HuntBaseRole role);
  void flashActivation();
  void showError();
  void showIdle();

private:
  Adafruit_NeoPixel strip;
  uint8_t brightnessLevel;
  uint32_t currentColour;
  unsigned long lastAnimationMs;
  unsigned long flashUntilMs;
  bool flashActive;
  bool breathUp;
  uint8_t breathValue;

  uint32_t colourForRole(HuntBaseRole role);
  void fillAll(uint32_t colour);
};

#endif
