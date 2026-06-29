/*
  HUNT Base Node v0.1

  Purpose:
  - Boots an ESP32-S3 Base Node.
  - Tests rotary encoder, encoder button and two extra buttons.
  - Shows clear Serial debug messages.
  - Builds and parses HUNT protocol packets.

  Required libraries:
  - Arduino core for ESP32
*/

#include <Arduino.h>
#include "config.h"
#include "../shared/HuntTypes.h"
#include "../shared/HuntDebug.h"
#include "../shared/HuntProtocol.h"

// =====================================================
// Base state variables
// =====================================================
HuntBaseRole currentRole = ROLE_SAFE_ZONE;
unsigned long lastHeartbeatMs = 0;
unsigned long lastStatusBlinkMs = 0;
bool statusLedState = false;

// Encoder state.
int lastEncoderClk = HIGH;
int roleIndex = 0;

// Button state.
bool lastBackButton = HIGH;
bool lastActionButton = HIGH;
bool lastEncoderButton = HIGH;
unsigned long lastButtonChangeMs = 0;

// =====================================================
// Role helpers
// =====================================================
String getRoleName(HuntBaseRole role) {
  switch (role) {
    case ROLE_SAFE_ZONE:
      return "SAFE_ZONE";
    case ROLE_SCANNER:
      return "SCANNER";
    case ROLE_OBJECTIVE:
      return "OBJECTIVE";
    case ROLE_EXTRACTION:
      return "EXTRACTION";
  }

  return "UNKNOWN";
}

void setRoleByIndex(int index) {
  if (index < 0) index = 3;
  if (index > 3) index = 0;
  roleIndex = index;

  switch (roleIndex) {
    case 0:
      currentRole = ROLE_SAFE_ZONE;
      break;
    case 1:
      currentRole = ROLE_SCANNER;
      break;
    case 2:
      currentRole = ROLE_OBJECTIVE;
      break;
    case 3:
      currentRole = ROLE_EXTRACTION;
      break;
  }

  huntLog("Role changed to " + getRoleName(currentRole));
}

void printBaseScreen() {
  Serial.println();
  Serial.println("------ BASE SCREEN ------");
  Serial.print("Device: ");
  Serial.println(DEVICE_ID);
  Serial.print("Firmware: ");
  Serial.println(FIRMWARE_VERSION);
  Serial.print("Role: ");
  Serial.println(getRoleName(currentRole));
  Serial.println("Rotate encoder to change role");
  Serial.println("Encoder press = confirm/test role");
  Serial.println("Back button = debug back");
  Serial.println("Action button = send test event");
  Serial.println("-------------------------");
}

// =====================================================
// Encoder handling
// =====================================================
void handleEncoder() {
  int clkState = digitalRead(ENCODER_CLK_PIN);

  if (clkState != lastEncoderClk) {
    int dtState = digitalRead(ENCODER_DT_PIN);

    if (dtState != clkState) {
      setRoleByIndex(roleIndex + 1);
    } else {
      setRoleByIndex(roleIndex - 1);
    }

    printBaseScreen();
  }

  lastEncoderClk = clkState;
}

// =====================================================
// Button handling
// =====================================================
void handleButtons() {
  if (millis() - lastButtonChangeMs < BUTTON_DEBOUNCE_MS) {
    return;
  }

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
    huntLog("Action button pressed - building test event");
    String packet = huntBuildPacket("EVENT", DEVICE_ID, "PLAYER_01", "BASE_ROLE:" + getRoleName(currentRole));
    huntLogPacket("TX", packet);
  }

  if (encoderReading == LOW && lastEncoderButton == HIGH) {
    lastButtonChangeMs = millis();
    huntLog("Encoder button pressed - role confirmed: " + getRoleName(currentRole));
  }

  lastBackButton = backReading;
  lastActionButton = actionReading;
  lastEncoderButton = encoderReading;
}

// =====================================================
// Status output
// =====================================================
void updateStatusLed() {
  if (millis() - lastStatusBlinkMs >= 500) {
    lastStatusBlinkMs = millis();
    statusLedState = !statusLedState;
    digitalWrite(STATUS_LED_PIN, statusLedState ? HIGH : LOW);
  }
}

// =====================================================
// Protocol self-test and heartbeat
// =====================================================
void runProtocolSelfTest() {
  String packet = huntBuildPacket("HELLO", DEVICE_ID, "ALL", "ROLE:BASE;BASE_ROLE:" + getRoleName(currentRole));
  huntLogPacket("BUILT", packet);

  HuntPacket parsed = huntParsePacket(packet);
  if (parsed.valid) {
    huntLog("Protocol parser self-test passed");
  } else {
    huntLog("Protocol parser self-test FAILED");
  }
}

void sendHeartbeatIfDue() {
  if (millis() - lastHeartbeatMs >= HUNT_HEARTBEAT_INTERVAL_MS) {
    lastHeartbeatMs = millis();
    String packet = huntBuildPacket("HEARTBEAT", DEVICE_ID, "ALL", "ROLE:" + getRoleName(currentRole));
    huntLogPacket("TX", packet);
  }
}

// =====================================================
// Arduino setup and loop
// =====================================================
void setup() {
  huntDebugBegin(DEVICE_NAME);

  pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
  pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
  pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
  pinMode(BUTTON_ACTION_PIN, INPUT_PULLUP);
  pinMode(STATUS_LED_PIN, OUTPUT);

  lastEncoderClk = digitalRead(ENCODER_CLK_PIN);

  huntLog("Booting Base Node");
  setRoleByIndex(0);
  runProtocolSelfTest();
  printBaseScreen();
  huntLog("Base Node ready");
}

void loop() {
  handleEncoder();
  handleButtons();
  updateStatusLed();
  sendHeartbeatIfDue();
}
