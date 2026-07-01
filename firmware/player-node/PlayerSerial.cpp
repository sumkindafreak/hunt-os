#include "PlayerSerial.h"
#include "config.h"
#include "../shared/HuntTypes.h"
#include "../shared/HuntDebug.h"
#include <WiFi.h>

namespace {

const char *playerScreenName(int screenIndex) {
  switch (screenIndex) {
    case 0: return "STATUS";
    case 1: return "NETWORK";
    default: return "SYSTEM";
  }
}

String playerFormatAge(unsigned long lastSeenMs) {
  if (lastSeenMs == 0) {
    return "never";
  }

  unsigned long ageMs = millis() - lastSeenMs;
  if (ageMs < 1000) {
    return String(ageMs) + "ms";
  }
  return String(ageMs / 1000) + "s";
}

}  // namespace

void playerSerialPrintPinMap() {
  huntPrintStatusHeader("PLAYER HARDWARE MAP");
  huntPrintField("Button", "GPIO" + String(BUTTON_PIN));
  huntPrintField("Status RGB", String(RGB_RED_PIN) + "R / " + String(RGB_BLUE_PIN) + "B");
  huntPrintField("Heartbeat LED", "GPIO" + String(HEARTBEAT_LED_PIN) + " blue");
  huntPrintField("Buzzer", "GPIO" + String(BUZZER_PIN));
  if (RUMBLE_ENABLED) {
    huntPrintField("Rumble", "GPIO" + String(RUMBLE_PIN));
  }
  huntPrintField("OLED SDA", "GPIO" + String(OLED_SDA_PIN));
  huntPrintField("OLED SCL", "GPIO" + String(OLED_SCL_PIN));
  huntPrintField("OLED addr", "0x" + String(OLED_I2C_ADDRESS, HEX));
  huntPrintField("OLED driver", OLED_DRIVER_SH1106 ? "SH1106" : "SSD1306");
  huntPrintField("OLED viewport", String(OLED_VISIBLE_X) + "," + String(OLED_VISIBLE_Y));
  huntPrintField("Debounce", String(BUTTON_DEBOUNCE_MS) + "ms");
  huntPrintField("Long press", String(BUTTON_LONG_PRESS_MS) + "ms");
  huntPrintStatusFooter();
}

void playerSerialPrintRadio() {
  huntPrintStatusHeader("PLAYER RADIO");
  huntPrintField("Protocol", "HUNT v" + String(HUNT_PROTOCOL_VERSION));
  huntPrintField("Transport", "ESP-NOW broadcast");
  huntPrintField("Heartbeat", String(HUNT_HEARTBEAT_INTERVAL_MS) + "ms");
  huntPrintField("Wi-Fi MAC", WiFi.macAddress());
  huntPrintField("Max packet", String(HUNT_MAX_PACKET_LENGTH) + " B");
  huntPrintStatusFooter();
}

void playerSerialPrintRegistry(HuntNodeRegistry *registry) {
  huntPrintStatusHeader("KNOWN NODES");

  if (registry == nullptr || registry->count() == 0) {
    huntPrintField("Nodes", "none yet");
    huntPrintStatusFooter();
    return;
  }

  huntPrintFieldInt("Count", registry->count());

  for (int i = 0; i < registry->count(); i++) {
    HuntNodeRecord node = registry->getNode(i);
    String line = node.id + " [" + node.type + "] " + node.state;
    line += node.online ? " ONLINE" : " STALE";
    line += " age=" + playerFormatAge(node.lastSeenMs);
    line += " fw=" + node.firmware;

    char label[12];
    snprintf(label, sizeof(label), "Node %d", i + 1);
    huntPrintField(label, line);
  }

  huntPrintStatusFooter();
}

void playerSerialPrintPlayer(HuntPlayerService *player, int screenIndex, bool oledReady) {
  if (player == nullptr) {
    return;
  }

  huntPrintStatusHeader("PLAYER STATE");
  huntPrintField("ID", player->state()->getId());
  huntPrintField("State", player->state()->getStateName());
  huntPrintFieldInt("Health", player->state()->getHealth());
  huntPrintFieldInt("Lives", player->state()->getLives());
  huntPrintField("OLED", oledReady ? "ready" : "offline");
  huntPrintField("Screen", String(screenIndex + 1) + "/3 " + playerScreenName(screenIndex));
  huntPrintStatusFooter();
}

void playerSerialPrintServices(HuntKernel *kernel) {
  if (kernel == nullptr) {
    return;
  }

  huntPrintStatusHeader("SERVICES");
  huntPrintFieldInt("Registered", kernel->services()->serviceCount());
  huntPrintField("Kernel", "running");
  huntPrintStatusFooter();
}

void playerSerialPrintEventBus(HuntEventBus *events) {
  if (events == nullptr) {
    return;
  }

  huntPrintFieldInt("Event queue", events->count());
}

void playerSerialPrintFullDashboard(const PlayerSerialContext &ctx) {
  playerSerialPrintPlayer(ctx.player, ctx.screenIndex, ctx.oledReady);
  playerSerialPrintRegistry(ctx.registry);
  playerSerialPrintRadio();

  huntPrintStatusHeader("RUNTIME");
  huntPrintField("Uptime", String(millis() / 1000) + "s");
  huntPrintField("Firmware", FIRMWARE_VERSION);
  huntPrintHeapStats();
  if (ctx.events != nullptr) {
    playerSerialPrintEventBus(ctx.events);
  }
  huntPrintStatusFooter();
}

void playerSerialLogButtonPress() {
  huntLogDebugTag("INPUT", "Button down (GPIO" + String(BUTTON_PIN) + ")");
}

void playerSerialLogButtonRelease(unsigned long pressMs, const char *action) {
  huntLogInfo("INPUT", String("Button up ") + pressMs + "ms -> " + action);
}

void playerSerialLogStateChange(const String &fromState, const String &toState) {
  huntLogInfo("STATE", fromState + " -> " + toState);
}

void playerSerialLogHeartbeat() {
  huntLogDebugTag("NET", "HELLO heartbeat TX");
}

void playerSerialLogScreenChange(int screenIndex) {
  huntLogInfo("UI", String("Screen ") + (screenIndex + 1) + "/3 " + playerScreenName(screenIndex));
}

void playerSerialTick(const PlayerSerialContext &ctx) {
#if PLAYER_SERIAL_TICK_MS > 0
  static unsigned long lastTickMs = 0;
  unsigned long now = millis();

  if (now - lastTickMs < PLAYER_SERIAL_TICK_MS) {
    return;
  }

  lastTickMs = now;
  huntLogInfo("TICK", "Periodic player status dump");
  playerSerialPrintFullDashboard(ctx);
#endif
}
