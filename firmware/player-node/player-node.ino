/*
  HUNT Player Node v0.1
  Bootable ESP32-C3 Player firmware spine with ESP-NOW, HUNT protocol,
  player state management and notifications.
*/

#include <Arduino.h>
#include "config.h"
#include "../shared/HuntTypes.h"
#include "../shared/HuntDebug.h"
#include "../shared/HuntProtocol.h"
#include "../shared/HuntEspNow.h"
#include "../shared/HuntEvents.h"
#include "../shared/HuntPlayerState.h"
#include "../shared/HuntNotifications.h"

// Arduino IDE compatibility: include shared implementations directly for now.
#include "../shared/HuntProtocol.cpp"
#include "../shared/HuntEspNow.cpp"
#include "../shared/HuntPlayerState.cpp"
#include "../shared/HuntNotifications.cpp"

HuntPlayerStateManager playerManager;
HuntNotificationManager notificationManager;

unsigned long lastHeartbeatMs = 0;
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
  HuntPlayerState state = playerManager.getState();
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
}\n
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
    Serial.println(playerManager.getStateName());
    Serial.print("Health: ");
    Serial.println(playerManager.getHealth());
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

void applyNotification() {
  if (!notificationManager.hasNotification()) return;
  huntLog("Notification: " + notificationManager.getMessage());
  HuntNotificationType type = notificationManager.getType();
  if (type == NOTIFY_ALERT || type == NOTIFY_WARNING) beepAlert();
  else beepConfirm();
  notificationManager.clear();
}

void applyHuntEvent(const HuntEvent &event) {
  playerManager.handleEvent(event);
  notificationManager.handleEvent(event);
  showPlayerStateOnLed();
  applyNotification();
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
        HuntEvent testEvent;
        if (playerManager.getState() == PLAYER_INFECTED) {
          testEvent = huntCreateEvent(HUNT_EVENT_PLAYER_SAFE, DEVICE_ID, DEVICE_ID, "0");
        } else {
          testEvent = huntCreateEvent(HUNT_EVENT_PLAYER_INFECTED, DEVICE_ID, DEVICE_ID, "0");
        }
        applyHuntEvent(testEvent);
      } else {
        currentScreen++;
        if (currentScreen > 2) currentScreen = 0;
        beepConfirm();
        printCurrentScreen();
      }
    }
  }
}

HuntEvent eventFromPayload(const HuntPacket &packet) {
  if (packet.payload.indexOf("SAFE_ZONE") >= 0 || packet.payload.indexOf("HEAL") >= 0) {
    return huntCreateEvent(HUNT_EVENT_PLAYER_SAFE, packet.source, DEVICE_ID, "25");
  }
  if (packet.payload.indexOf("SCANNER") >= 0 || packet.payload.indexOf("INFECT") >= 0) {
    return huntCreateEvent(HUNT_EVENT_PLAYER_INFECTED, packet.source, DEVICE_ID, "0");
  }
  if (packet.payload.indexOf("ELIMINATE") >= 0) {
    return huntCreateEvent(HUNT_EVENT_PLAYER_ELIMINATED, packet.source, DEVICE_ID, "0");
  }
  return huntCreateEvent(HUNT_EVENT_BASE_ACTIVATED, packet.source, DEVICE_ID, packet.payload);
}

void sendAckToSource(const String &source, const String &ackPayload) {
  String ack = huntBuildPacket("ACK", DEVICE_ID, source, ackPayload);
  huntEspNowSendBroadcast(ack);
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

  if (packet.type == "EVENT") {
    HuntEvent event = eventFromPayload(packet);
    applyHuntEvent(event);
    sendAckToSource(packet.source, "EVENT_RECEIVED");
  } else if (packet.type == "HEARTBEAT") {
    huntLog("Heartbeat from " + packet.source + ": " + packet.payload);
  } else if (packet.type == "COMMAND") {
    huntLog("Command received: " + packet.payload);
    sendAckToSource(packet.source, "COMMAND_RECEIVED");
  }
}

void runProtocolSelfTest() {
  String packet = huntBuildPacket("HELLO", DEVICE_ID, "ALL", "ROLE:PLAYER");
  huntLogPacket("BUILT", packet);
  HuntPacket parsed = huntParsePacket(packet);
  huntLog(parsed.valid ? "Protocol parser self-test passed" : "Protocol parser self-test FAILED");
}

void sendHeartbeatIfDue() {
  if (millis() - lastHeartbeatMs >= HUNT_HEARTBEAT_INTERVAL_MS) {
    lastHeartbeatMs = millis();
    String packet = huntBuildPacket("HEARTBEAT", DEVICE_ID, "ALL", "STATE:" + playerManager.getStateName());
    huntEspNowSendBroadcast(packet);
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
  huntLog("Booting Player Node");
  beepConfirm();

  playerManager.begin(DEVICE_ID);
  notificationManager.begin();
  runProtocolSelfTest();

  if (!huntEspNowBegin()) {
    huntLog("ESP-NOW failed to start");
    setRgbRaw(true, false, false);
  }

  String hello = huntBuildPacket("HELLO", DEVICE_ID, "ALL", "ROLE:PLAYER");
  huntEspNowSendBroadcast(hello);

  showPlayerStateOnLed();
  printCurrentScreen();
  huntLog("Player Node ready");
}

void loop() {
  handleButton();
  handleIncomingPackets();
  sendHeartbeatIfDue();
}
