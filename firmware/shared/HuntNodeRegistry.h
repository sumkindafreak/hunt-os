#ifndef HUNT_NODE_REGISTRY_H
#define HUNT_NODE_REGISTRY_H

#include <Arduino.h>

#define HUNT_MAX_NODES 24

struct HuntNodeRecord {
  String id;
  String type;
  String firmware;
  String state;
  unsigned long lastSeenMs;
  bool online;
};

class HuntNodeRegistry {
public:
  HuntNodeRegistry();

  void begin();
  void update();
  bool upsertNode(const String &id, const String &type, const String &firmware, const String &state);
  int count();
  HuntNodeRecord getNode(int index);
  String toJson();

private:
  HuntNodeRecord nodes[HUNT_MAX_NODES];
  int nodeCount;
  int findIndex(const String &id);
};

#endif
