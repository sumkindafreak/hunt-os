#ifndef HUNT_DEBUG_H
#define HUNT_DEBUG_H

#include <Arduino.h>
#include "HuntEvents.h"

// Master switch for all serial debug output.
#define HUNT_DEBUG true

// Minimum level printed: ERROR=0, WARN=1, INFO=2, DEBUG=3
#ifndef HUNT_LOG_LEVEL
#define HUNT_LOG_LEVEL 3
#endif

enum HuntLogLevel : uint8_t {
  HUNT_LOG_ERROR = 0,
  HUNT_LOG_WARN = 1,
  HUNT_LOG_INFO = 2,
  HUNT_LOG_DEBUG = 3,
};

inline const char *huntLogLevelLabel(HuntLogLevel level) {
  switch (level) {
    case HUNT_LOG_ERROR: return "ERROR";
    case HUNT_LOG_WARN: return "WARN ";
    case HUNT_LOG_INFO: return "INFO ";
    case HUNT_LOG_DEBUG: return "DEBUG";
    default: return "?????";
  }
}

inline void huntFormatUptime(char *buffer, size_t length) {
  unsigned long ms = millis();
  unsigned long seconds = ms / 1000UL;
  unsigned long millisPart = ms % 1000UL;
  snprintf(buffer, length, "%6lu.%03lus", seconds, millisPart);
}

inline void huntLogWrite(HuntLogLevel level, const char *tag, const String &message) {
  if (!HUNT_DEBUG || level > HUNT_LOG_LEVEL) {
    return;
  }

  char uptime[16];
  huntFormatUptime(uptime, sizeof(uptime));

  Serial.print('[');
  Serial.print(uptime);
  Serial.print("] [");
  Serial.print(huntLogLevelLabel(level));
  Serial.print("] [");
  Serial.print(tag);
  Serial.print("] ");
  Serial.println(message);
}

inline void huntLog(const String &message) {
  huntLogWrite(HUNT_LOG_INFO, "CORE", message);
}

inline void huntLogInfo(const char *tag, const String &message) {
  huntLogWrite(HUNT_LOG_INFO, tag, message);
}

inline void huntLogWarn(const String &message) {
  huntLogWrite(HUNT_LOG_WARN, "CORE", message);
}

inline void huntLogWarnTag(const char *tag, const String &message) {
  huntLogWrite(HUNT_LOG_WARN, tag, message);
}

inline void huntLogError(const String &message) {
  huntLogWrite(HUNT_LOG_ERROR, "CORE", message);
}

inline void huntLogErrorTag(const char *tag, const String &message) {
  huntLogWrite(HUNT_LOG_ERROR, tag, message);
}

inline void huntLogDebug(const String &message) {
  huntLogWrite(HUNT_LOG_DEBUG, "CORE", message);
}

inline void huntLogDebugTag(const char *tag, const String &message) {
  huntLogWrite(HUNT_LOG_DEBUG, tag, message);
}

inline void huntLogNet(const String &message) {
  huntLogWrite(HUNT_LOG_INFO, "NET ", message);
}

inline void huntLogService(const String &message) {
  huntLogWrite(HUNT_LOG_INFO, "SVC ", message);
}

inline void huntLogPacket(const String &direction, const String &packet) {
  if (!HUNT_DEBUG || HUNT_LOG_DEBUG > HUNT_LOG_LEVEL) {
    return;
  }

  char uptime[16];
  huntFormatUptime(uptime, sizeof(uptime));

  Serial.print('[');
  Serial.print(uptime);
  Serial.print("] [DEBUG] [PKT  ] ");
  Serial.print(direction);
  Serial.print(' ');
  if (direction == "TX") {
    Serial.print("--> ");
  } else if (direction == "RX") {
    Serial.print("<-- ");
  }
  Serial.println(packet);
}

inline void huntLogEvent(const HuntEvent &event) {
  huntLogWrite(
    HUNT_LOG_INFO,
    "EVT  ",
    event.name + " | src=" + event.source + " -> " + event.target + " | data=" + event.data
  );
}

inline void huntPrintRule(char fill = '-', int width = 38) {
  for (int i = 0; i < width; i++) {
    Serial.print(fill);
  }
  Serial.println();
}

inline void huntPrintStatusHeader(const char *title) {
  Serial.println();
  huntPrintRule('=');
  Serial.print("  ");
  Serial.println(title);
  huntPrintRule('-');
}

inline void huntPrintField(const char *label, const String &value, int labelWidth = 12) {
  Serial.print("  ");
  Serial.print(label);
  int padding = labelWidth - (int)strlen(label);
  if (padding < 1) {
    padding = 1;
  }
  for (int i = 0; i < padding; i++) {
    Serial.print(' ');
  }
  Serial.print(": ");
  Serial.println(value);
}

inline void huntPrintField(const String &label, const String &value, int labelWidth = 12) {
  huntPrintField(label.c_str(), value, labelWidth);
}

inline void huntPrintFieldInt(const char *label, int value, int labelWidth = 12) {
  huntPrintField(label, String(value), labelWidth);
}

inline void huntPrintStatusFooter() {
  huntPrintRule('-');
}

inline void huntPrintHeapStats() {
  huntPrintField("Heap free", String(ESP.getFreeHeap()) + " B");
#if defined(ESP32)
  if (ESP.getPsramSize() > 0) {
    huntPrintField("PSRAM free", String(ESP.getFreePsram()) + " B");
  }
#endif
}

inline void huntDebugBegin(const String &deviceName, const String &deviceId = "", const String &firmwareVersion = "") {
  Serial.begin(115200);
#if defined(ARDUINO_USB_CDC_ON_BOOT) && ARDUINO_USB_CDC_ON_BOOT
  // ESP32-C3 native USB needs a moment before the serial port appears on the PC.
  delay(1500);
#else
  delay(300);
#endif

  huntPrintStatusHeader("HUNT OS v0.1");
  huntPrintField("Device", deviceName);
  if (deviceId.length() > 0) {
    huntPrintField("ID", deviceId);
  }
  if (firmwareVersion.length() > 0) {
    huntPrintField("Firmware", firmwareVersion);
  }
  huntPrintField("Chip", String(ESP.getChipModel()) + " rev " + String(ESP.getChipRevision()));
  huntPrintField("CPU", String(ESP.getCpuFreqMHz()) + " MHz");
  huntPrintField("Flash", String(ESP.getFlashChipSize() / 1024) + " KB");
  huntPrintHeapStats();
  huntPrintStatusFooter();
  Serial.println();

  huntLogInfo("CORE", "Serial monitor ready @ 115200 baud");
}

#endif
