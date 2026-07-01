#include "HuntBaseDisplay.h"
#include "HuntDebug.h"

namespace {

bool i2cDevicePresent(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

}  // namespace

HuntBaseDisplay::HuntBaseDisplay(
  int8_t sdaPin,
  int8_t sclPin,
  uint8_t i2cAddress,
  uint8_t width,
  uint8_t height,
  uint32_t i2cHz
)
  : sdaPin(sdaPin),
    sclPin(sclPin),
    i2cAddress(i2cAddress),
    width(width),
    height(height),
    i2cHz(i2cHz),
    ready(false),
    flashActive(false),
    flashUntilMs(0),
    lastKnownNodes(0),
    display(width, height, &Wire, -1) {}

bool HuntBaseDisplay::begin() {
  Wire.begin(sdaPin, sclPin);
  delay(20);
  Wire.setClock(i2cHz);

  uint8_t activeAddress = i2cAddress;
  if (!i2cDevicePresent(activeAddress) && i2cDevicePresent(0x3D)) {
    activeAddress = 0x3D;
    huntLogWarnTag("OLED", "Using alternate I2C address 0x3D");
  }

  ready = display.begin(activeAddress, true);

  if (!ready) {
    huntLogErrorTag(
      "OLED",
      "SH1106 init failed SDA=" + String(sdaPin) + " SCL=" + String(sclPin) + " addr=0x" + String(activeAddress, HEX)
    );
    return false;
  }

  display.invertDisplay(false);
  display.setContrast(0x8F);
  prepareDrawContext();
  display.clearDisplay();
  flush();
  delay(30);
  display.clearDisplay();
  flush();

  huntLogService("Base SH1106 OLED ready @ 0x" + String(activeAddress, HEX) + " GPIO" + String(sdaPin) + "/" + String(sclPin));
  return true;
}

bool HuntBaseDisplay::isReady() {
  return ready;
}

void HuntBaseDisplay::prepareDrawContext() {
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setTextWrap(false);
}

void HuntBaseDisplay::drawLine(int16_t x, int16_t y, const String &text, uint8_t textSize) {
  display.setTextSize(textSize);
  display.setCursor(x, y);
  display.print(text);
}

void HuntBaseDisplay::drawDivider(int16_t y) {
  display.drawFastHLine(0, y, width, SH110X_WHITE);
}

void HuntBaseDisplay::flush() {
  display.display();
}

void HuntBaseDisplay::showBootLogo(const String &deviceId, const String &firmwareVersion) {
  if (!ready) {
    return;
  }

  prepareDrawContext();
  display.clearDisplay();
  drawLine(16, 8, "HUNT", 2);
  drawLine(24, 30, "BASE NODE", 1);
  drawLine(20, 42, deviceId, 1);
  drawLine(12, 54, firmwareVersion, 1);
  flush();
  delay(800);
}

void HuntBaseDisplay::showDashboard(
  const String &roleName,
  const String &sceneName,
  int knownNodes,
  const String &apName,
  const String &apIp
) {
  if (!ready) {
    return;
  }

  lastRoleName = roleName;
  lastSceneName = sceneName;
  lastKnownNodes = knownNodes;
  lastApName = apName;
  lastApIp = apIp;

  if (flashActive && millis() < flashUntilMs) {
    return;
  }

  prepareDrawContext();
  display.clearDisplay();
  drawLine(0, 0, "HUNT BASE", 1);
  drawDivider(10);
  drawLine(0, 14, "ROLE", 1);
  drawLine(42, 14, roleName, 1);
  drawLine(0, 26, "SCENE", 1);
  drawLine(42, 26, sceneName, 1);
  drawLine(0, 38, "NODES " + String(knownNodes), 1);
  drawLine(0, 50, apName, 1);
  drawLine(0, 58, apIp, 1);
  flush();
}

void HuntBaseDisplay::showRoleFlash(const String &roleName) {
  if (!ready) {
    return;
  }

  prepareDrawContext();
  display.clearDisplay();
  drawLine(0, 0, "ROLE SET", 1);
  drawDivider(10);
  drawLine(0, 22, roleName, 2);
  flush();

  flashActive = true;
  flashUntilMs = millis() + 600;
}

void HuntBaseDisplay::update() {
  if (!ready || !flashActive) {
    return;
  }

  if (millis() >= flashUntilMs) {
    flashActive = false;
    showDashboard(lastRoleName, lastSceneName, lastKnownNodes, lastApName, lastApIp);
  }
}
