#ifndef HUNT_DEBUG_H
#define HUNT_DEBUG_H

#include <Arduino.h>

#define HUNT_DEBUG true

inline void huntDebugBegin(const String &deviceName) {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("================================");
  Serial.println("HUNT OS v0.1");
  Serial.print("Device: ");
  Serial.println(deviceName);
  Serial.println("================================");
}

inline void huntLog(const String &message) {
  if (HUNT_DEBUG) {
    Serial.print("[HUNT] ");
    Serial.println(message);
  }
}

inline void huntLogPacket(const String &prefix, const String &packet) {
  if (HUNT_DEBUG) {
    Serial.print("[HUNT PACKET] ");
    Serial.print(prefix);
    Serial.print(": ");
    Serial.println(packet);
  }
}

#endif
