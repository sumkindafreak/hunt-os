/*
  HUNT Base Node v0.1 Alpha

  ESP32-S3 Base firmware using the HUNT Kernel service model.

  Required libraries:
  - Arduino core for ESP32
  - Adafruit NeoPixel
  - Adafruit SSD1306
  - Adafruit GFX Library
  - Adafruit SH110X (SH1106 driver for 1.3" base OLED)
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
#include "../shared/HuntBaseDisplay.h"

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
#include "../shared/HuntBaseDisplay.cpp"

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
HuntBaseDisplay baseDisplay(
  OLED_SDA_PIN,
  OLED_SCL_PIN,
  OLED_I2C_ADDRESS,
  OLED_WIDTH,
  OLED_HEIGHT,
  OLED_I2C_HZ
);

unsigned long lastHelloMs = 0;
unsigned long lastStatusBlinkMs = 0;
unsigned long lastButtonChangeMs = 0;
bool statusLedState = false;
bool oledReady = false;
bool lastBackButton = HIGH;
bool lastActionButton = HIGH;
bool lastEncoderButton = HIGH;
int lastEncoderClk = HIGH;

String baseApIp() {
  if (webService.serverInstance()->isRunning()) {
    return webService.serverInstance()->getIpAddress();
  }
  return "192.168.4.1";
}

void refreshBaseScreen(bool roleFlash = false) {
  printBaseScreen();
  if (!oledReady) {
    return;
  }

  if (roleFlash) {
    baseDisplay.showRoleFlash(roleManager.getRoleName());
    return;
  }

  baseDisplay.showDashboard(
    roleManager.getRoleName(),
    sceneService.currentScene(),
    nodeRegistry.count(),
    "AP HUNT_BASE_01",
    baseApIp()
  );
}

void printBaseScreen() {
  huntPrintStatusHeader("HUNT BASE ALPHA");
  huntPrintField("Device", DEVICE_ID);
  huntPrintField("Firmware", FIRMWARE_VERSION);
  huntPrintField("Role", roleManager.getRoleName());
  huntPrintField("Scene", sceneService.currentScene());
  huntPrintFieldInt("Known nodes", nodeRegistry.count());
  huntPrintField("Web AP", "HUNT_BASE_01 / huntbase01");
  huntPrintField("Dashboard", "http://192.168.4.1");
  huntPrintHeapStats();
  huntPrintStatusFooter();
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
    huntLogInfo("INPUT", "Role changed: " + roleManager.getRoleName());
    refreshBaseScreen(true);
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
    refreshBaseScreen();
  }

  if (actionReading == LOW && lastActionButton == HIGH) {
    lastButtonChangeMs = millis();
    huntLogInfo("INPUT", "Action button: send role event");
    sendRoleEventToPlayer();
  }

  if (encoderReading == LOW && lastEncoderButton == HIGH) {
    lastButtonChangeMs = millis();
    huntLogInfo("INPUT", "Role confirmed: " + roleManager.getRoleName());
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
    huntLogEvent(event);

    if (event.name == HUNT_EVENT_PLAYER_JOINED) {
      refreshBaseScreen();
    }
  }
}

void setup() {
  huntDebugBegin(DEVICE_NAME, DEVICE_ID, FIRMWARE_VERSION);

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

  oledReady = baseDisplay.begin();
  if (oledReady) {
    baseDisplay.showBootLogo(DEVICE_ID, FIRMWARE_VERSION);
  }

  networkService.sendHello("BASE", FIRMWARE_VERSION, roleManager.getRoleName());

  refreshBaseScreen();
  huntLogInfo("CORE", "Base Alpha ready");
}

void loop() {
  handleEncoder();
  handleButtons();
  huntKernel.update();
  nodeRegistry.update();
  updateStatusLed();
  sendHelloIfDue();
  processAlphaEvents();
  if (oledReady) {
    baseDisplay.update();
  }
}
