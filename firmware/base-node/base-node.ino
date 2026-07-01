/*
  HUNT Base Node v0.1
  Bootable ESP32-S3 Base firmware spine with ESP-NOW, HUNT protocol,
  shared Role Manager, NeoPixel status ring and decor/expansion NeoPixel output.

  Required libraries:
  - Arduino core for ESP32
  - Adafruit NeoPixel
*/

#include <Arduino.h>
#include "config.h"
#include "../shared/HuntTypes.h"
#include "../shared/HuntDebug.h"
#include "../shared/HuntProtocol.h"
#include "../shared/HuntEspNow.h"
#include "../shared/HuntEvents.h"
#include "../shared/HuntRoleManager.h"
#include "../shared/HuntNeoPixels.h"

// Arduino IDE compatibility: include shared implementations directly for now.
#include "../shared/HuntProtocol.cpp"
#include "../shared/HuntEspNow.cpp"
#include "../shared/HuntRoleManager.cpp"
#include "../shared/HuntNeoPixels.cpp"

HuntRoleManager roleManager;

// Status ring: communicates what the Base is doing.
HuntNeoPixelManager statusPixels(BASE_NEOPIXEL_PIN, BASE_NEOPIXEL_COUNT, BASE_NEOPIXEL_BRIGHTNESS);

// Decor strip: used for atmosphere, prop lighting and expansion effects.
HuntNeoPixelManager decorPixels(DECOR_NEOPIXEL_PIN, DECOR_NEOPIXEL_COUNT, DECOR_NEOPIXEL_BRIGHTNESS);

unsigned long lastHeartbeatMs = 0;
unsigned long lastStatusBlinkMs = 0;
bool statusLedState = false;
int lastEncoderClk = HIGH;
bool lastBackButton = HIGH;
bool lastActionButton = HIGH;
bool lastEncoderButton = HIGH;
unsigned long lastButtonChangeMs = 0;

void printBaseScreen() {
  Serial.println();
  Serial.println("------ BASE SCREEN ------");
  Serial.print("Device: ");
  Serial.println(DEVICE_ID);
  Serial.print("Firmware: ");
  Serial.println(FIRMWARE_VERSION);
  Serial.print("Role: ");
  Serial.println(roleManager.getRoleName());
  Serial.print("Status NeoPixels: ");
  Serial.print(BASE_NEOPIXEL_COUNT);
  Serial.print(" on pin ");
  Serial.println(BASE_NEOPIXEL_PIN);
  Serial.print("Decor NeoPixels: ");
  Serial.print(DECOR_NEOPIXEL_COUNT);
  Serial.print(" on pin ");
  Serial.println(DECOR_NEOPIXEL_PIN);
  Serial.println("Rotate encoder to change role");
  Serial.println("Encoder press = confirm role");
  Serial.println("Back button = print screen");
  Serial.println("Action button = send role event");
  Serial.println("-------------------------");
}

void applyRoleLighting() {
  statusPixels.setRoleColour(roleManager.getRole());
  decorPixels.setRoleColour(roleManager.getRole());
}

void handleEncoder() {
  int clkState = digitalRead(ENCODER_CLK_PIN);

  if (clkState != lastEncoderClk) {
    int dtState = digitalRead(ENCODER_DT_PIN);

    if (dtState != clkState) roleManager.nextRole();
    else roleManager.previousRole();

    huntLog("Role changed to " + roleManager.getRoleName());
    applyRoleLighting();
    printBaseScreen();
  }

  lastEncoderClk = clkState;
}

void sendHello() {
  String packet = huntBuildPacket("HELLO", DEVICE_ID, "ALL", "ROLE:BASE;BASE_ROLE:" + roleManager.getRoleName());
  huntEspNowSendBroadcast(packet);
}

void sendRoleEventToPlayer() {
  HuntEvent event = roleManager.buildActivationEvent(DEVICE_ID, "PLAYER_01");
  String payload = "EVENT:" + event.name + ";DATA:" + event.data + ";ROLE:" + roleManager.getRoleName();
  String packet = huntBuildPacket("EVENT", DEVICE_ID, "PLAYER_01", payload);
  huntEspNowSendBroadcast(packet);
  statusPixels.flashActivation();
  decorPixels.flashActivation();
}

void handleIncomingPackets() {
  String rawPacket = huntEspNowReadPacket();
  if (rawPacket.length() == 0) return;

  HuntPacket packet = huntParsePacket(rawPacket);
  if (!packet.valid) {
    huntLog("Ignored invalid packet");
    return;
  }

  if (!huntIsPacketForDevice(packet, DEVICE_ID)) {
    huntLog("Ignored packet for another target: " + packet.target);
    return;
  }

  huntLog("Accepted packet type: " + packet.type + " from " + packet.source);

  if (packet.type == "ACK") {
    huntLog("ACK received: " + packet.payload);
    statusPixels.flashActivation();
  } else if (packet.type == "HELLO") {
    huntLog("HELLO received: " + packet.payload);
  } else if (packet.type == "HEARTBEAT") {
    huntLog("Heartbeat from " + packet.source + ": " + packet.payload);
  }
}

void handleButtons() {
  if (millis() - lastButtonChangeMs < BUTTON_DEBOUNCE_MS) return;

  bool backReading = digitalRead(BUTTON_BACK_PIN);
  bool actionReading = digitalRead(BUTTON_ACTION_PIN);
  bool encoderReading = digitalRead(ENCODER_SW_PIN);

  if (backReading == LOW && lastBackButton == HIGH) {
    lastButtonChangeMs = millis();
    huntLog("Back button pressed");
    printBaseScreen();
  }

  if (actionReading == LOW && lastActionButton == HIGH) {
    lastButtonChangeMs = millis();
    huntLog("Action button pressed - sending role event");
    sendRoleEventToPlayer();
  }

  if (encoderReading == LOW && lastEncoderButton == HIGH) {
    lastButtonChangeMs = millis();
    huntLog("Encoder button pressed - role confirmed: " + roleManager.getRoleName());
    statusPixels.flashActivation();
    decorPixels.flashActivation();
    sendHello();
  }

  lastBackButton = backReading;
  lastActionButton = actionReading;
  lastEncoderButton = encoderReading;
}

void updateStatusLed() {
  if (millis() - lastStatusBlinkMs >= 500) {
    lastStatusBlinkMs = millis();
    statusLedState = !statusLedState;
    digitalWrite(STATUS_LED_PIN, statusLedState ? HIGH : LOW);
  }
}

void runProtocolSelfTest() {
  String packet = huntBuildPacket("HELLO", DEVICE_ID, "ALL", "ROLE:BASE;BASE_ROLE:" + roleManager.getRoleName());
  huntLogPacket("BUILT", packet);
  HuntPacket parsed = huntParsePacket(packet);
  huntLog(parsed.valid ? "Protocol parser self-test passed" : "Protocol parser self-test FAILED");
}

void sendHeartbeatIfDue() {
  if (millis() - lastHeartbeatMs >= HUNT_HEARTBEAT_INTERVAL_MS) {
    lastHeartbeatMs = millis();
    String packet = huntBuildPacket("HEARTBEAT", DEVICE_ID, "ALL", "ROLE:" + roleManager.getRoleName());
    huntEspNowSendBroadcast(packet);
  }
}

void setup() {
  huntDebugBegin(DEVICE_NAME);

  pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
  pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
  pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
  pinMode(BUTTON_ACTION_PIN, INPUT_PULLUP);
  pinMode(STATUS_LED_PIN, OUTPUT);

  lastEncoderClk = digitalRead(ENCODER_CLK_PIN);
  roleManager.begin(ROLE_SAFE_ZONE);

  statusPixels.begin();
  decorPixels.begin();
  applyRoleLighting();

  huntLog("Booting Base Node");
  runProtocolSelfTest();

  if (!huntEspNowBegin()) {
    huntLog("ESP-NOW failed to start");
    statusPixels.showError();
    decorPixels.showError();
  }

  sendHello();
  printBaseScreen();
  huntLog("Base Node ready");
}

void loop() {
  handleEncoder();
  handleButtons();
  handleIncomingPackets();
  updateStatusLed();
  statusPixels.update();
  decorPixels.update();
  sendHeartbeatIfDue();
}
