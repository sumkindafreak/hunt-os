#ifndef HUNT_BASE_DISPLAY_H
#define HUNT_BASE_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// =====================================================
// HUNT Base OLED Display v0.1
//
// 1.3-inch SH1106 operator display for the ESP32-S3 Base.
// Most 1.3" 128x64 modules use SH1106 — SSD1306 driver causes TV static.
// =====================================================

class HuntBaseDisplay {
public:
  HuntBaseDisplay(
    int8_t sdaPin,
    int8_t sclPin,
    uint8_t i2cAddress,
    uint8_t width,
    uint8_t height,
    uint32_t i2cHz
  );

  bool begin();
  void showBootLogo(const String &deviceId, const String &firmwareVersion);
  void showDashboard(
    const String &roleName,
    const String &sceneName,
    int knownNodes,
    const String &apName,
    const String &apIp
  );
  void showRoleFlash(const String &roleName);
  void update();
  bool isReady();

private:
  int8_t sdaPin;
  int8_t sclPin;
  uint8_t i2cAddress;
  uint8_t width;
  uint8_t height;
  uint32_t i2cHz;
  bool ready;

  bool flashActive;
  unsigned long flashUntilMs;
  String lastRoleName;
  String lastSceneName;
  int lastKnownNodes;
  String lastApName;
  String lastApIp;

  Adafruit_SH1106G display;

  void prepareDrawContext();
  void drawLine(int16_t x, int16_t y, const String &text, uint8_t textSize = 1);
  void drawDivider(int16_t y);
  void flush();
};

#endif
