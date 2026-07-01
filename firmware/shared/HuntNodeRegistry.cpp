#include "HuntNodeRegistry.h"

HuntNodeRegistry::HuntNodeRegistry() { nodeCount = 0; }

void HuntNodeRegistry::begin() {
  nodeCount = 0;
  for (int i = 0; i < HUNT_MAX_NODES; i++) {
    nodes[i].id = "";
    nodes[i].type = "UNKNOWN";
    nodes[i].firmware = "";
    nodes[i].state = "";
    nodes[i].lastSeenMs = 0;
    nodes[i].online = false;
  }
}

void HuntNodeRegistry::update() {
  unsigned long now = millis();
  for (int i = 0; i < nodeCount; i++) nodes[i].online = (now - nodes[i].lastSeenMs) < 15000;
}

bool HuntNodeRegistry::upsertNode(const String &id, const String &type, const String &firmware, const String &state) {
  if (id.length() == 0) return false;
  int index = findIndex(id);
  if (index < 0) {
    if (nodeCount >= HUNT_MAX_NODES) return false;
    index = nodeCount++;
  }
  nodes[index].id = id;
  nodes[index].type = type;
  nodes[index].firmware = firmware;
  nodes[index].state = state;
  nodes[index].lastSeenMs = millis();
  nodes[index].online = true;
  return true;
}

int HuntNodeRegistry::count() { return nodeCount; }

HuntNodeRecord HuntNodeRegistry::getNode(int index) {
  if (index < 0 || index >= nodeCount) return HuntNodeRecord();
  return nodes[index];
}

String HuntNodeRegistry::toJson() {
  String json = "[";
  for (int i = 0; i < nodeCount; i++) {
    if (i > 0) json += ",";
    json += "{\"id\":\"" + nodes[i].id + "\",\"type\":\"" + nodes[i].type + "\",\"firmware\":\"" + nodes[i].firmware + "\",\"state\":\"" + nodes[i].state + "\",\"online\":" + String(nodes[i].online ? "true" : "false") + "}";
  }
  json += "]";
  return json;
}

int HuntNodeRegistry::findIndex(const String &id) {
  for (int i = 0; i < nodeCount; i++) if (nodes[i].id == id) return i;
  return -1;
}
