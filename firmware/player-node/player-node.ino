/*
  HUNT Player Node v0.1 Alpha

  ESP32-C3 Player firmware using the HUNT Kernel service model.

  Required libraries:
  - Arduino core for ESP32
  - Adafruit SSD1306
  - Adafruit GFX Library

  Arduino IDE board settings (ESP32-C3 Super Mini OLED):
  - Board: MakerGO ESP32 C3 SuperMini  (or ESP32C3 Dev Module)
  - USB CDC On Boot: Enabled
  - Flash Mode: DIO
  - Flash Size: 4MB
  - Upload Speed: 115200
  - Port: your USB Serial Device COM port (Espressif VID 303A)
  See README.md for full upload troubleshooting.
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
#include "../shared/HuntPlayerDisplay.h"
#include "PlayerSerial.h"

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
#include "../shared/HuntPlayerDisplay.cpp"

HuntKernel huntKernel;
HuntNodeRegistry nodeRegistry;
HuntNetworkService networkService(DEVICE_ID, huntKernel.events(), &nodeRegistry);
HuntPlayerService playerService(huntKernel.events(), DEVICE_ID);
HuntNotificationManager notificationManager;
HuntPlayerDisplay playerDisplay(
  OLED_SDA_PIN,
  OLED_SCL_PIN,
  OLED_I2C_ADDRESS,
  OLED_WIDTH,
  OLED_HEIGHT,
  OLED_VISIBLE_X,
  OLED_VISIBLE_Y,
  OLED_DRIVER_SH1106,
  OLED_I2C_HZ
);

unsigned long lastHelloMs = 0;
unsigned long lastButtonChangeMs = 0;
unsigned long buttonPressStartMs = 0;
bool lastButtonReading = HIGH;
bool buttonPressed = false;
int currentScreen = 0;
bool oledReady = false;
unsigned long heartbeatLedOffMs = 0;

PlayerSerialContext playerSerialContext() {
  PlayerSerialContext ctx;
  ctx.player = &playerService;
  ctx.registry = &nodeRegistry;
  ctx.events = huntKernel.events();
  ctx.kernel = &huntKernel;
  ctx.screenIndex = currentScreen;
  ctx.oledReady = oledReady;
  return ctx;
}

void heartbeatLedBegin() {
  pinMode(HEARTBEAT_LED_PIN, OUTPUT);
  digitalWrite(HEARTBEAT_LED_PIN, HEARTBEAT_LED_ACTIVE_HIGH ? LOW : HIGH);
}

void heartbeatLedPulse() {
  digitalWrite(HEARTBEAT_LED_PIN, HEARTBEAT_LED_ACTIVE_HIGH ? HIGH : LOW);
  heartbeatLedOffMs = millis() + HEARTBEAT_LED_PULSE_MS;
}

void heartbeatLedUpdate() {
  if (heartbeatLedOffMs > 0 && millis() >= heartbeatLedOffMs) {
    digitalWrite(HEARTBEAT_LED_PIN, HEARTBEAT_LED_ACTIVE_HIGH ? LOW : HIGH);
    heartbeatLedOffMs = 0;
  }
}

void setRgbRaw(bool redOn, bool blueOn) {
  if (RGB_COMMON_ANODE) {
    digitalWrite(RGB_RED_PIN, redOn ? LOW : HIGH);
    digitalWrite(RGB_BLUE_PIN, blueOn ? LOW : HIGH);
  } else {
    digitalWrite(RGB_RED_PIN, redOn ? HIGH : LOW);
    digitalWrite(RGB_BLUE_PIN, blueOn ? HIGH : LOW);
  }
}

void showPlayerStateOnLed() {
  HuntPlayerState state = playerService.state()->getState();
  if (state == PLAYER_ALIVE) setRgbRaw(false, true);
  if (state == PLAYER_INFECTED) setRgbRaw(true, true);
  if (state == PLAYER_SAFE) setRgbRaw(false, true);
  if (state == PLAYER_ELIMINATED) setRgbRaw(true, false);
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
#if PLAYER_SERIAL_FULL_DETAIL
  playerSerialPrintFullDashboard(playerSerialContext());
#else
  huntPrintStatusHeader("HUNT PLAYER ALPHA");
  huntPrintField("Device", DEVICE_ID);
  huntPrintField("Firmware", FIRMWARE_VERSION);
  huntPrintField("State", playerService.state()->getStateName());
  huntPrintFieldInt("Health", playerService.state()->getHealth());
  huntPrintFieldInt("Known nodes", nodeRegistry.count());
  huntPrintHeapStats();
  huntPrintStatusFooter();
#endif
}

void refreshPlayerScreen() {
  printCurrentScreen();
  if (oledReady) {
    playerDisplay.showScreen(currentScreen, playerService.state(), nodeRegistry.count(), FIRMWARE_VERSION);
  }
}

void applyNotification(const HuntEvent &event) {
  notificationManager.handleEvent(event);
  if (!notificationManager.hasNotification()) return;

  huntLogInfo("UI", "Notification: " + notificationManager.getMessage());
  HuntNotificationType type = notificationManager.getType();
  if (oledReady) {
    playerDisplay.showAlert(notificationManager.getMessage(), type);
  }
  if (type == NOTIFY_ALERT || type == NOTIFY_WARNING) beepAlert();
  else beepConfirm();
  notificationManager.clear();
}

void applyHuntEvent(const HuntEvent &event) {
  String previousState = playerService.state()->getStateName();
  playerService.applyEvent(event);
  if (previousState != playerService.state()->getStateName()) {
    playerSerialLogStateChange(previousState, playerService.state()->getStateName());
  }
  applyNotification(event);
  showPlayerStateOnLed();
  refreshPlayerScreen();
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
      playerSerialLogButtonPress();
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
        playerSerialLogButtonRelease(pressLength, "long-press state toggle");
        applyHuntEvent(event);
      } else {
        currentScreen++;
        if (currentScreen > 2) currentScreen = 0;
        playerSerialLogButtonRelease(pressLength, "short-press next screen");
        playerSerialLogScreenChange(currentScreen);
        beepConfirm();
        refreshPlayerScreen();
      }
    }
  }
}

void sendHelloIfDue() {
  if (millis() - lastHelloMs >= HUNT_HEARTBEAT_INTERVAL_MS) {
    lastHelloMs = millis();
    networkService.sendHello("PLAYER", FIRMWARE_VERSION, playerService.state()->getStateName());
    playerSerialLogHeartbeat();
    heartbeatLedPulse();
  }
}

void processAlphaEvents() {
  while (huntKernel.events()->available()) {
    HuntEvent event = huntKernel.events()->read();
    huntLogEvent(event);

    if (event.name == HUNT_EVENT_PLAYER_JOINED) {
      huntLogInfo("NET", "Discovered node " + event.source + " | " + event.data);
      heartbeatLedPulse();
    }

    if (event.target == DEVICE_ID || event.target == "ALL") {
      applyHuntEvent(event);
    }
  }
}

void setup() {
  huntDebugBegin(DEVICE_NAME, DEVICE_ID, FIRMWARE_VERSION);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  heartbeatLedBegin();

  setRgbRaw(false, true);
  beepConfirm();

  oledReady = playerDisplay.begin();
  if (oledReady) {
#if OLED_BOOT_CINEMATIC
    playerDisplay.playCinematicBoot(DEVICE_ID, FIRMWARE_VERSION);
#else
    playerDisplay.showBootLogo(FIRMWARE_VERSION);
    delay(900);
#endif
  }

  nodeRegistry.begin();
  notificationManager.begin();

  huntKernel.services()->registerService(&networkService);
  huntKernel.services()->registerService(&playerService);
  huntKernel.begin();

  playerSerialPrintPinMap();
  playerSerialPrintRadio();
  playerSerialPrintServices(&huntKernel);

  networkService.sendHello("PLAYER", FIRMWARE_VERSION, playerService.state()->getStateName());
  playerSerialLogHeartbeat();
  heartbeatLedPulse();
  showPlayerStateOnLed();
  refreshPlayerScreen();
  huntLogInfo("CORE", "Player Alpha ready — full serial detail enabled");
}

void loop() {
  handleButton();
  huntKernel.update();
  nodeRegistry.update();
  if (oledReady) {
    playerDisplay.update();
  }
  sendHelloIfDue();
  processAlphaEvents();
  heartbeatLedUpdate();
  playerSerialTick(playerSerialContext());
}
