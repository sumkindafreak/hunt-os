#ifndef HUNT_NETWORK_SERVICE_H
#define HUNT_NETWORK_SERVICE_H

#include <Arduino.h>
#include "HuntService.h"
#include "HuntEspNow.h"
#include "HuntProtocol.h"
#include "HuntEventBus.h"
#include "HuntNodeRegistry.h"
#include "HuntEvents.h"

class HuntNetworkService : public HuntService {
public:
  HuntNetworkService(const String &deviceId, HuntEventBus *eventBus, HuntNodeRegistry *registry);

  void begin() override;
  void update() override;
  const char *name() override;

  void sendHello(const String &deviceType, const String &firmware, const String &state);
  void sendEvent(const String &target, const HuntEvent &event);
  void sendCommand(const String &target, const String &payload);

private:
  String id;
  HuntEventBus *events;
  HuntNodeRegistry *nodes;
  unsigned long lastHelloMs;

  void handlePacket(const HuntPacket &packet);
  String payloadValue(const String &payload, const String &key);
};

#endif
