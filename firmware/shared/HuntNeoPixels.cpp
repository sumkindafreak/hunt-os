#include "HuntNeoPixels.h"

HuntNeoPixelManager::HuntNeoPixelManager(uint8_t pin, uint16_t count, uint8_t brightness)
  : strip(count, pin, NEO_GRB + NEO_KHZ800) {
  brightnessLevel = brightness;
  currentColour = 0;
  lastAnimationMs = 0;
  flashUntilMs = 0;
  flashActive = false;
  breathUp = true;
  breathValue = 10;
}

void HuntNeoPixelManager::begin() {
  strip.begin();
  strip.setBrightness(brightnessLevel);
  strip.clear();
  strip.show();
  showIdle();
}

void HuntNeoPixelManager::update() {
  if (flashActive) {
    if (millis() < flashUntilMs) {
      fillAll(strip.Color(255, 255, 255));
      return;
    }

    flashActive = false;
    fillAll(currentColour);
  }

  if (millis() - lastAnimationMs < 35) {
    return;
  }

  lastAnimationMs = millis();

  if (breathUp) {
    breathValue++;
    if (breathValue >= brightnessLevel) breathUp = false;
  } else {
    if (breathValue > 8) breathValue--;
    else breathUp = true;
  }

  strip.setBrightness(breathValue);
  fillAll(currentColour);
}

void HuntNeoPixelManager::setRoleColour(HuntBaseRole role) {
  currentColour = colourForRole(role);
  strip.setBrightness(brightnessLevel);
  fillAll(currentColour);
}

void HuntNeoPixelManager::flashActivation() {
  flashActive = true;
  flashUntilMs = millis() + 250;
}

void HuntNeoPixelManager::showError() {
  currentColour = strip.Color(255, 0, 0);
  strip.setBrightness(brightnessLevel);
  fillAll(currentColour);
}

void HuntNeoPixelManager::showIdle() {
  currentColour = strip.Color(20, 20, 20);
  strip.setBrightness(15);
  fillAll(currentColour);
}

uint32_t HuntNeoPixelManager::colourForRole(HuntBaseRole role) {
  if (role == ROLE_SAFE_ZONE) return strip.Color(0, 255, 0);
  if (role == ROLE_SCANNER) return strip.Color(180, 0, 255);
  if (role == ROLE_OBJECTIVE) return strip.Color(0, 80, 255);
  if (role == ROLE_EXTRACTION) return strip.Color(255, 180, 0);
  return strip.Color(255, 255, 255);
}

void HuntNeoPixelManager::fillAll(uint32_t colour) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, colour);
  }
  strip.show();
}
