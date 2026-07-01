#include "HuntNetworkService.h"

HuntNetworkService::HuntNetworkService(const String &deviceId, HuntEventBus *eventBus, HuntNodeRegistry *registry) {
  id = deviceId;
  events = eventBus;
  nodes = registry;
  lastHelloMs = 0;
}

void HuntNetworkService::begin() {
  huntLog("Network Service starting");
  huntEspNowBegin();
  lastHelloMs = 0;
}

void HuntNetworkService::update() {
  String raw = huntEspNowReadPacket();
  if (raw.length() > 0) {
    HuntPacket packet = huntParsePacket(raw);
    if (packet.valid && huntIsPacketForDevice(packet, id)) handlePacket(packet);
  }
}

const char *HuntNetworkService::name() { return "Network"; }

void HuntNetworkService::sendHello(const String &deviceType, const String &firmware, const String &state) {
  String payload = "TYPE:" + deviceType + ";FW:" + firmware + ";STATE:" + state;
  String packet = huntBuildPacket("HELLO", id, "ALL", payload);
  huntEspNowSendBroadcast(packet);
}

void HuntNetworkService::sendEvent(const String &target, const HuntEvent &event) {
  String payload = "EVENT:" + event.name + ";DATA:" + event.data;
  String packet = huntBuildPacket("EVENT", id, target, payload);
  huntEspNowSendBroadcast(packet);
}

void HuntNetworkService::sendCommand(const String &target, const String &payload) {
  String packet = huntBuildPacket("COMMAND", id, target, payload);
  huntEspNowSendBroadcast(packet);
}

void HuntNetworkService::handlePacket(const HuntPacket &packet) {
  if (packet.type == "HELLO" && nodes != nullptr) {
    nodes->upsertNode(packet.source, payloadValue(packet.payload, "TYPE"), payloadValue(packet.payload, "FW"), payloadValue(packet.payload, "STATE"));
    if (events != nullptr) events->publish(huntCreateEvent(HUNT_EVENT_PLAYER_JOINED, packet.source, id, packet.payload));
  }

  if (packet.type == "EVENT" && events != nullptr) {
    events->publish(huntCreateEvent(payloadValue(packet.payload, "EVENT"), packet.source, packet.target, payloadValue(packet.payload, "DATA")));
  }
}

String HuntNetworkService::payloadValue(const String &payload, const String &key) {
  String prefix = key + ":";
  int start = payload.indexOf(prefix);
  if (start < 0) return "";
  start += prefix.length();
  int end = payload.indexOf(';', start);
  if (end < 0) end = payload.length();
  return payload.substring(start, end);
}
