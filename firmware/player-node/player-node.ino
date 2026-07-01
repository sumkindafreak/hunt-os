/*
  HUNT Player Node v0.1 Alpha

  ESP32-C3 Player firmware using the HUNT Kernel service model.

  Required libraries:
  - Arduino core for ESP32
*/

#include <Arduino.h>
#include "config.h"
#include "../shared/HuntTypes.h"
#include "../shared/HuntDebug.h"
#include "../shared/HuntEvents.h"
#include "../shared/HuntKernel.h"
#include "../shared/HuntNodeRegistry.h"
#include "../shared/HuntNetworkService.h"
#include "../shared/HuntPlayerService.h"
#include "../shared/HuntNotifications.h"

// Arduino IDE compatibility: include shared implementations directly.
#include "../shared/HuntProtocol.cpp"
#include "../shared/HuntEspNow.cpp"
#include "../shared/HuntEventBus.cpp"
#include "../shared/HuntServiceManager.cpp"
#include "../shared/HuntKernel.cpp"
#include "../shared/HuntNodeRegistry.cpp"
#include "../shared/HuntNetworkService.cpp"
#include "../shared/HuntPlayerState.cpp"
#include "../shared/HuntPlayerService.cpp"
#include "../shared/HuntNotifications.cpp"

HuntKernel huntKernel;
HuntNodeRegistry nodeRegistry;
HuntNetworkService networkService(DEVICE_ID, huntKernel.events(), &nodeRegistry);
HuntPlayerService playerService(huntKernel.events(), DEVICE_ID);
HuntNotificationManager notificationManager;

unsigned long lastHelloMs = 0;
unsigned long lastButtonChangeMs = 0;
unsigned long buttonPressStartMs = 0;
bool lastButtonReading = HIGH;
bool buttonPressed = false;
int currentScreen = 0;

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
  HuntPlayerState state = playerService.state()->getState();
  if (state == PLAYER_ALIVE) setRgbRaw(false, true, false);
  if (state == PLAYER_INFECTED) setRgbRaw(true, false, true);
  if (state == PLAYER_SAFE) setRgbRaw(false, false, true);
  if (state == PLAYER_ELIMINATED) setRgbRaw(true, false, false);
}

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

void printCurrentScreen() {
  Serial.println();
  Serial.println("----- HUNT PLAYER ALPHA -----");
  Serial.print("Device: "); Serial.println(DEVICE_ID);
  Serial.print("Firmware: "); Serial.println(FIRMWARE_VERSION);
  Serial.print("State: "); Serial.println(playerService.state()->getStateName());
  Serial.print("Health: "); Serial.println(playerService.state()->getHealth());
  Serial.print("Known nodes: "); Serial.println(nodeRegistry.count());
  Serial.println("-----------------------------");
}

void applyNotification(const HuntEvent &event) {
  notificationManager.handleEvent(event);
  if (!notificationManager.hasNotification()) return;

  huntLog("Notification: " + notificationManager.getMessage());
  HuntNotificationType type = notificationManager.getType();
  if (type == NOTIFY_ALERT || type == NOTIFY_WARNING) beepAlert();
  else beepConfirm();
  notificationManager.clear();
}

void applyHuntEvent(const HuntEvent &event) {
  playerService.applyEvent(event);
  applyNotification(event);
  showPlayerStateOnLed();
  printCurrentScreen();
}

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
        HuntEvent event;
        if (playerService.state()->getState() == PLAYER_INFECTED) {
          event = huntCreateEvent(HUNT_EVENT_PLAYER_SAFE, DEVICE_ID, DEVICE_ID, "0");
        } else {
          event = huntCreateEvent(HUNT_EVENT_PLAYER_INFECTED, DEVICE_ID, DEVICE_ID, "0");
        }
        applyHuntEvent(event);
      } else {
        currentScreen++;
        if (currentScreen > 2) currentScreen = 0;
        beepConfirm();
        printCurrentScreen();
      }
    }
  }
}

void sendHelloIfDue() {
  if (millis() - lastHelloMs >= HUNT_HEARTBEAT_INTERVAL_MS) {
    lastHelloMs = millis();
    networkService.sendHello("PLAYER", FIRMWARE_VERSION, playerService.state()->getStateName());
  }
}

void processAlphaEvents() {
  while (huntKernel.events()->available()) {
    HuntEvent event = huntKernel.events()->read();
    huntLog("Event: " + event.name + " from " + event.source + " data " + event.data);

    if (event.target == DEVICE_ID || event.target == "ALL") {
      applyHuntEvent(event);
    }
  }
}

void setup() {
  huntDebugBegin(DEVICE_NAME);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  setRgbRaw(false, false, true);
  beepConfirm();
  nodeRegistry.begin();
  notificationManager.begin();

  huntKernel.services()->registerService(&networkService);
  huntKernel.services()->registerService(&playerService);
  huntKernel.begin();

  networkService.sendHello("PLAYER", FIRMWARE_VERSION, playerService.state()->getStateName());
  showPlayerStateOnLed();
  printCurrentScreen();
  huntLog("Player Alpha ready");
}

void loop() {
  handleButton();
  huntKernel.update();
  nodeRegistry.update();
  sendHelloIfDue();
  processAlphaEvents();
}
