#include "HuntSoundService.h"
#include "HuntDebug.h"

HuntSoundService::HuntSoundService(HuntEventBus *eventBus) {
  events = eventBus;
  volumeLevel = 25;
}

void HuntSoundService::begin() {
  huntLog("Sound service ready");
}

void HuntSoundService::update() {
  // DFPlayer command handling will be added after hardware wiring test.
}

const char *HuntSoundService::name() { return "Sound"; }

void HuntSoundService::playTrack(uint16_t trackNumber) {
  huntLog("Sound play track " + String(trackNumber));
}

void HuntSoundService::stopAll() {
  huntLog("Sound stop");
}

void HuntSoundService::volume(uint8_t level) {
  volumeLevel = constrain(level, 0, 30);
  huntLog("Sound volume " + String(volumeLevel));
}
