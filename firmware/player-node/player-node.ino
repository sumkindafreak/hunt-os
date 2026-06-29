/*
  HUNT Player Node v0.1

  Purpose:
  - Boots an ESP32-C3 Player Node.
  - Shows clear serial debug messages.
  - Tests button, RGB LED and buzzer.
  - Builds and parses HUNT protocol packets.

  Required libraries:
  - Arduino core for ESP32

  Optional OLED support will be added once the exact ESP32-C3 OLED board is confirmed.
*/

#include <Arduino.h>
#include "config.h"
#include "../shared/HuntTypes.h"
#include "../shared/HuntDebug.h"
#include "../shared/HuntProtocol.h"

// =====================================================
// Player state variables
// =====================================================
HuntPlayerState playerState = PLAYER_ALIVE;
unsigned long lastHeartbeatMs = 0;
unsigned long lastButtonChangeMs = 0;
unsigned long buttonPressStartMs = 0;
bool lastButtonReading = HIGH;
bool buttonPressed = false;
int currentScreen = 0;

// =====================================================
// RGB LED helper
// =====================================================
void setRgbRaw(bool redOn, bool greenOn, bool blueOn) {
  if (RGB_COMMON_ANODE) {
    digitalWrite(RGB_RED_PIN, redOn ? LOW : HIGH);
    digitalWrite(RGB_GREEN_PIN, greenOn ? LOW : HIGH);
    digitalWrite(RGB_BLUE_PIN, blueOn ? LOW : HIGH);
  } else {
    digitalWrite(RGB_RED_PIN, redOn ? HIGH : LOW);
    digitalWrite(RGB_GREEN_PIN, greenOn ? HIGH : LOW);
    digitalWrite(RGB_BLUE_PIN, blueOn ? HIGH : LOW);
  }
}

void showPlayerStateOnLed() {
  switch (playerState) {
    case PLAYER_ALIVE:
      setRgbRaw(false, true, false); // Green
      break;
    case PLAYER_INFECTED:
      setRgbRaw(true, false, true); // Purple
      break;
    case PLAYER_SAFE:
      setRgbRaw(false, false, true); // Blue
      break;
    case PLAYER_ELIMINATED:
      setRgbRaw(true, false, false); // Red
      break;
  }
}

// =====================================================
// Buzzer helpers
// =====================================================
void beepConfirm() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(70);
  digitalWrite(BUZZER_PIN, LOW);
}

void beepAlert() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(60);
    digitalWrite(BUZZER_PIN, LOW);
    delay(60);
  }
}

// =====================================================
// Display placeholder
// =====================================================
void updateDisplay() {
  // OLED support will be added after confirming the exact onboard OLED driver and pins.
  // For now, the Serial Monitor is the official v0.1 display output.
}

void printCurrentScreen() {
  Serial.println();
  Serial.println("----- PLAYER SCREEN -----");
  Serial.print("Device: ");
  Serial.println(DEVICE_ID);
  Serial.print("Firmware: ");
  Serial.println(FIRMWARE_VERSION);

  if (currentScreen == 0) {
    Serial.println("Screen: Status");
    Serial.print("State: ");
    if (playerState == PLAYER_ALIVE) Serial.println("ALIVE");
    if (playerState == PLAYER_INFECTED) Serial.println("INFECTED");
    if (playerState == PLAYER_SAFE) Serial.println("SAFE");
    if (playerState == PLAYER_ELIMINATED) Serial.println("ELIMINATED");
  } else if (currentScreen == 1) {
    Serial.println("Screen: Objective");
    Serial.println("Objective: Awaiting game start");
  } else {
    Serial.println("Screen: Diagnostics");
    Serial.print("Millis: ");
    Serial.println(millis());
  }
  Serial.println("-------------------------");
}

// =====================================================
// Button handling
// =====================================================
void handleButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonChangeMs = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastButtonChangeMs) > BUTTON_DEBOUNCE_MS) {
    if (reading == LOW && !buttonPressed) {
      buttonPressed = true;
      buttonPressStartMs = millis();
      huntLog("Button pressed");
    }

    if (reading == HIGH && buttonPressed) {
      buttonPressed = false;
      unsigned long pressLength = millis() - buttonPressStartMs;

      if (pressLength >= BUTTON_LONG_PRESS_MS) {
        huntLog("Long press detected - toggling infected test state");
        playerState = (playerState == PLAYER_INFECTED) ? PLAYER_ALIVE : PLAYER_INFECTED;
        beepAlert();
      } else {
        huntLog("Short press detected - changing screen");
        currentScreen++;
        if (currentScreen > 2) currentScreen = 0;
        beepConfirm();
      }

      showPlayerStateOnLed();
      printCurrentScreen();
    }
  }
}

// =====================================================
// Protocol test
// =====================================================
void runProtocolSelfTest() {
  String packet = huntBuildPacket("HELLO", DEVICE_ID, "ALL", "ROLE:PLAYER");
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
    String packet = huntBuildPacket("HEARTBEAT", DEVICE_ID, "ALL", "STATE:READY");
    huntLogPacket("TX", packet);
  }
}

// =====================================================
// Arduino setup and loop
// =====================================================
void setup() {
  huntDebugBegin(DEVICE_NAME);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  setRgbRaw(false, false, true); // Blue during boot.
  huntLog("Booting Player Node");
  beepConfirm();

  runProtocolSelfTest();

  playerState = PLAYER_ALIVE;
  showPlayerStateOnLed();
  printCurrentScreen();

  huntLog("Player Node ready");
}

void loop() {
  handleButton();
  updateDisplay();
  sendHeartbeatIfDue();
}
