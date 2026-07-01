#ifndef HUNT_PLAYER_DISPLAY_H
#define HUNT_PLAYER_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "HuntPlayerState.h"
#include "HuntNotifications.h"

#if defined(OLED_DRIVER_SH1106) && OLED_DRIVER_SH1106
#include <Adafruit_SH1106G.h>
#else
#include <Adafruit_SSD1306.h>
#endif

// =====================================================
// HUNT Player OLED Display v0.1
//
// Small status screens for the ESP32-C3 player badge.
// Uses Adafruit GFX + SSD1306/SH1106 over I2C.
// =====================================================

class HuntPlayerDisplay {
public:
  HuntPlayerDisplay(
    int8_t sdaPin,
    int8_t sclPin,
    uint8_t i2cAddress,
    uint8_t width,
    uint8_t height,
    int16_t visibleX,
    int16_t visibleY,
    bool useSh1106,
    uint32_t i2cHz
  );

  bool begin();
  void showBootLogo(const String &firmwareVersion);
  void playCinematicBoot(const String &deviceId, const String &firmwareVersion);
  void showScreen(int screenIndex, HuntPlayerStateManager *state, int knownNodes, const String &firmwareVersion);
  void showAlert(const String &message, HuntNotificationType type);
  void update();

private:
  int8_t sdaPin;
  int8_t sclPin;
  uint8_t i2cAddress;
  uint32_t i2cHz;
  int16_t visibleX;
  int16_t visibleY;
  bool useSh1106;
  bool ready;

  int lastScreenIndex;
  HuntPlayerStateManager *lastState;
  int lastKnownNodes;
  String lastFirmwareVersion;

  bool alertActive;
  unsigned long alertUntilMs;

#if defined(OLED_DRIVER_SH1106) && OLED_DRIVER_SH1106
  Adafruit_SH1106G display;
#else
  Adafruit_SSD1306 display;
#endif

  void drawStatusScreen(HuntPlayerStateManager *state);
  void drawNetworkScreen(HuntPlayerStateManager *state, int knownNodes);
  void drawSystemScreen(int knownNodes, const String &firmwareVersion);

  void drawLine(int16_t x, int16_t y, const String &text, uint8_t textSize = 1);
  void drawBootLetterbox(uint8_t barHeight);
  void drawBootScanline(int16_t y);
  void drawBootProgressBar(uint8_t percent);
  void drawBootStatusLine(const char *label, bool done);
  void drawBootTitle(const char *text, uint8_t charsVisible);
  void flush();
};

#endif
