/*
  HUNT Base Node v0.1 Alpha

  ESP32-S3 Base firmware using the HUNT Kernel service model.

  Required libraries:
  - Arduino core for ESP32
  - Adafruit NeoPixel
*/

#include <Arduino.h>
#include "config.h"
#include "../shared/HuntTypes.h"
#include "../shared/HuntDebug.h"
#include "../shared/HuntEvents.h"
#include "../shared/HuntKernel.h"
#include "../shared/HuntNodeRegistry.h"
#include "../shared/HuntNetworkService.h"
#include "../shared/HuntRoleManager.h"
#include "../shared/HuntNeoPixels.h"
#include "../shared/HuntSceneService.h"
#include "../shared/HuntLightingService.h"
#include "../shared/HuntSoundService.h"
#include "../shared/HuntWebService.h"

// Arduino IDE compatibility: include shared implementations directly.
#include "../shared/HuntProtocol.cpp"
#include "../shared/HuntEspNow.cpp"
#include "../shared/HuntEventBus.cpp"
#include "../shared/HuntServiceManager.cpp"
#include "../shared/HuntKernel.cpp"
#include "../shared/HuntNodeRegistry.cpp"
#include "../shared/HuntNetworkService.cpp"
#include "../shared/HuntRoleManager.cpp"
#include "../shared/HuntNeoPixels.cpp"
#include "../shared/HuntSceneService.cpp"
#include "../shared/HuntLightingService.cpp"
#include "../shared/HuntSoundService.cpp"
#include "../shared/HuntWebServer.cpp"
#include "../shared/HuntWebService.cpp"

HuntKernel huntKernel;
HuntNodeRegistry nodeRegistry;
HuntRoleManager roleManager;

HuntNeoPixelManager statusPixels(BASE_NEOPIXEL_PIN, BASE_NEOPIXEL_COUNT, BASE_NEOPIXEL_BRIGHTNESS);
HuntNeoPixelManager decorPixels(DECOR_NEOPIXEL_PIN, DECOR_NEOPIXEL_COUNT, DECOR_NEOPIXEL_BRIGHTNESS);

HuntNetworkService networkService(DEVICE_ID, huntKernel.events(), &nodeRegistry);
HuntSceneService sceneService(huntKernel.events());
HuntLightingService lightingService(huntKernel.events(), &statusPixels, &decorPixels);
HuntSoundService soundService(huntKernel.events());
HuntWebService webService("HUNT_BASE_01", "huntbase01");

unsigned long lastHelloMs = 0;
unsigned long lastStatusBlinkMs = 0;
unsigned long lastButtonChangeMs = 0;
bool statusLedState = false;
bool lastBackButton = HIGH;
bool lastActionButton = HIGH;
bool lastEncoderButton = HIGH;
int lastEncoderClk = HIGH;

void printBaseScreen() {
  Serial.println();
  Serial.println("------ HUNT BASE ALPHA ------");
  Serial.print("Device: "); Serial.println(DEVICE_ID);
  Serial.print("Firmware: "); Serial.println(FIRMWARE_VERSION);
  Serial.print("Role: "); Serial.println(roleManager.getRoleName());
  Serial.print("Scene: "); Serial.println(sceneService.currentScene());
  Serial.print("Known nodes: "); Serial.println(nodeRegistry.count());
  Serial.println("Web AP: HUNT_BASE_01 / huntbase01");
  Serial.println("Open: http://192.168.4.1");
  Serial.println("-----------------------------");
}

void handleEncoder() {
  int clkState = digitalRead(ENCODER_CLK_PIN);
  if (clkState != lastEncoderClk) {
    int dtState = digitalRead(ENCODER_DT_PIN);
    if (dtState != clkState) roleManager.nextRole();
    else roleManager.previousRole();

    lightingService.setRole(roleManager.getRole());
    sceneService.loadScene(roleManager.getRoleName());
    soundService.playTrack(3);
    huntLog("Role changed: " + roleManager.getRoleName());
    printBaseScreen();
  }
  lastEncoderClk = clkState;
}

void sendRoleEventToPlayer() {
  HuntEvent event = roleManager.buildActivationEvent(DEVICE_ID, "PLAYER_01");
  networkService.sendEvent("PLAYER_01", event);
  lightingService.flash();
  soundService.playTrack(6);
  huntKernel.events()->publish(event);
}

void handleButtons() {
  if (millis() - lastButtonChangeMs < BUTTON_DEBOUNCE_MS) return;

  bool backReading = digitalRead(BUTTON_BACK_PIN);
  bool actionReading = digitalRead(BUTTON_ACTION_PIN);
  bool encoderReading = digitalRead(ENCODER_SW_PIN);

  if (backReading == LOW && lastBackButton == HIGH) {
    lastButtonChangeMs = millis();
    soundService.playTrack(3);
    printBaseScreen();
  }

  if (actionReading == LOW && lastActionButton == HIGH) {
    lastButtonChangeMs = millis();
    huntLog("Action button: send role event");
    sendRoleEventToPlayer();
  }

  if (encoderReading == LOW && lastEncoderButton == HIGH) {
    lastButtonChangeMs = millis();
    huntLog("Role confirmed: " + roleManager.getRoleName());
    lightingService.flash();
    soundService.playTrack(4);
    networkService.sendHello("BASE", FIRMWARE_VERSION, roleManager.getRoleName());
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

void sendHelloIfDue() {
  if (millis() - lastHelloMs >= HUNT_HEARTBEAT_INTERVAL_MS) {
    lastHelloMs = millis();
    networkService.sendHello("BASE", FIRMWARE_VERSION, roleManager.getRoleName());
  }
}

void processAlphaEvents() {
  while (huntKernel.events()->available()) {
    HuntEvent event = huntKernel.events()->read();
    huntLog("Event: " + event.name + " from " + event.source + " data " + event.data);
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
  nodeRegistry.begin();

  huntKernel.services()->registerService(&networkService);
  huntKernel.services()->registerService(&sceneService);
  huntKernel.services()->registerService(&lightingService);
  huntKernel.services()->registerService(&soundService);
  huntKernel.services()->registerService(&webService);

  huntKernel.begin();
  lightingService.setRole(roleManager.getRole());
  soundService.playTrack(1);
  networkService.sendHello("BASE", FIRMWARE_VERSION, roleManager.getRoleName());

  printBaseScreen();
  huntLog("Base Alpha ready");
}

void loop() {
  handleEncoder();
  handleButtons();
  huntKernel.update();
  nodeRegistry.update();
  updateStatusLed();
  sendHelloIfDue();
  processAlphaEvents();
}
