#ifndef PLAYER_SERIAL_H
#define PLAYER_SERIAL_H

#include <Arduino.h>
#include "../shared/HuntPlayerService.h"
#include "../shared/HuntNodeRegistry.h"
#include "../shared/HuntEventBus.h"
#include "../shared/HuntKernel.h"

// Full serial diagnostics for the ESP32-C3 player badge.

struct PlayerSerialContext {
  HuntPlayerService *player;
  HuntNodeRegistry *registry;
  HuntEventBus *events;
  HuntKernel *kernel;
  int screenIndex;
  bool oledReady;
};

void playerSerialPrintPinMap();
void playerSerialPrintRadio();
void playerSerialPrintRegistry(HuntNodeRegistry *registry);
void playerSerialPrintPlayer(HuntPlayerService *player, int screenIndex, bool oledReady);
void playerSerialPrintServices(HuntKernel *kernel);
void playerSerialPrintEventBus(HuntEventBus *events);
void playerSerialPrintFullDashboard(const PlayerSerialContext &ctx);
void playerSerialLogButtonPress();
void playerSerialLogButtonRelease(unsigned long pressMs, const char *action);
void playerSerialLogStateChange(const String &fromState, const String &toState);
void playerSerialLogHeartbeat();
void playerSerialLogScreenChange(int screenIndex);
void playerSerialTick(const PlayerSerialContext &ctx);

#endif
