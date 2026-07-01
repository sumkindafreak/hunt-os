#include "HuntPlayerDisplay.h"
#include "HuntDebug.h"

#if defined(OLED_DRIVER_SH1106) && OLED_DRIVER_SH1106
#define HUNT_OLED_WHITE SH110X_WHITE
#define HUNT_OLED_BLACK SH110X_BLACK
#else
#define HUNT_OLED_WHITE SSD1306_WHITE
#define HUNT_OLED_BLACK SSD1306_BLACK
#endif

namespace {

const uint8_t BOOT_LETTERBOX_HEIGHT = 6;
const uint8_t BOOT_BAR_WIDTH = 68;
const uint8_t BOOT_BAR_HEIGHT = 6;

}  // namespace

HuntPlayerDisplay::HuntPlayerDisplay(
  int8_t sdaPin,
  int8_t sclPin,
  uint8_t i2cAddress,
  uint8_t width,
  uint8_t height,
  int16_t visibleX,
  int16_t visibleY,
  bool useSh1106,
  uint32_t i2cHz
)
  : sdaPin(sdaPin),
    sclPin(sclPin),
    i2cAddress(i2cAddress),
    i2cHz(i2cHz),
    visibleX(visibleX),
    visibleY(visibleY),
    useSh1106(useSh1106),
    ready(false),
    lastScreenIndex(0),
    lastState(nullptr),
    lastKnownNodes(0),
    alertActive(false),
    alertUntilMs(0),
    display(width, height, &Wire, -1) {}

bool HuntPlayerDisplay::begin() {
  Wire.begin(sdaPin, sclPin);
  Wire.setClock(i2cHz);

#if defined(OLED_DRIVER_SH1106) && OLED_DRIVER_SH1106
  ready = display.begin(i2cAddress, true);
#else
  ready = display.begin(SSD1306_SWITCHCAPVCC, i2cAddress, true, false);
#endif

  if (!ready) {
    huntLogErrorTag("OLED", "Display init failed on I2C 0x" + String(i2cAddress, HEX));
    return false;
  }

  (void)useSh1106;

  display.clearDisplay();
  display.setTextColor(HUNT_OLED_WHITE);
  display.setTextWrap(false);
  flush();

  huntLogService("OLED ready (Adafruit " + String(display.width()) + "x" + String(display.height()) + ")");
  return true;
}

void HuntPlayerDisplay::drawLine(int16_t x, int16_t y, const String &text, uint8_t textSize) {
  display.setTextSize(textSize);
  display.setCursor(visibleX + x, visibleY + y);
  display.print(text);
}

void HuntPlayerDisplay::flush() {
  display.display();
}

void HuntPlayerDisplay::showBootLogo(const String &firmwareVersion) {
  if (!ready) {
    return;
  }

  display.clearDisplay();
  drawLine(0, 0, "HUNT", 2);
  drawLine(0, 18, "PLAYER", 1);
  drawLine(0, 30, firmwareVersion, 1);
  flush();
}

void HuntPlayerDisplay::drawBootLetterbox(uint8_t barHeight) {
  if (barHeight == 0) {
    return;
  }

  display.fillRect(0, 0, display.width(), barHeight, HUNT_OLED_WHITE);
  display.fillRect(0, display.height() - barHeight, display.width(), barHeight, HUNT_OLED_WHITE);
}

void HuntPlayerDisplay::drawBootScanline(int16_t y) {
  if (y < 0 || y >= display.height()) {
    return;
  }

  display.drawFastHLine(0, y, display.width(), HUNT_OLED_WHITE);
}

void HuntPlayerDisplay::drawBootTitle(const char *text, uint8_t charsVisible) {
  display.setTextSize(2);
  display.setTextColor(HUNT_OLED_WHITE);
  display.setCursor(visibleX, visibleY - 2);
  for (uint8_t i = 0; i < charsVisible && text[i] != '\0'; i++) {
    display.print(text[i]);
  }
}

void HuntPlayerDisplay::drawBootProgressBar(uint8_t percent) {
  int16_t barX = visibleX;
  int16_t barY = visibleY + 28;

  display.drawRect(barX, barY, BOOT_BAR_WIDTH, BOOT_BAR_HEIGHT, HUNT_OLED_WHITE);

  int16_t fillWidth = (int16_t)((BOOT_BAR_WIDTH - 4) * percent / 100);
  if (fillWidth > 0) {
    display.fillRect(barX + 2, barY + 2, fillWidth, BOOT_BAR_HEIGHT - 4, HUNT_OLED_WHITE);
  }
}

void HuntPlayerDisplay::drawBootStatusLine(const char *label, bool done) {
  display.setTextSize(1);
  display.setTextColor(HUNT_OLED_WHITE);
  display.setCursor(visibleX, visibleY + 18);
  display.print(label);
  display.print(done ? " OK" : " ..");
}

void HuntPlayerDisplay::playCinematicBoot(const String &deviceId, const String &firmwareVersion) {
  if (!ready) {
    return;
  }

  huntLogInfo("UI", "Cinematic boot sequence start");

  // Phase 1 — letterbox bars slide in.
  for (uint8_t bar = 0; bar <= BOOT_LETTERBOX_HEIGHT; bar++) {
    display.clearDisplay();
    drawBootLetterbox(bar);
    flush();
    delay(35);
  }

  // Phase 2 — scanline sweep.
  for (int16_t y = 0; y < display.height(); y += 4) {
    display.clearDisplay();
    drawBootLetterbox(BOOT_LETTERBOX_HEIGHT);
    drawBootScanline(y);
    flush();
    delay(18);
  }

  // Phase 3 — title typewriter reveal.
  const char *title = "HUNT";
  for (uint8_t i = 0; i <= 4; i++) {
    display.clearDisplay();
    drawBootLetterbox(BOOT_LETTERBOX_HEIGHT);
    drawBootTitle(title, i);
    flush();
    delay(90);
  }

  // Phase 4 — subtitle + device id.
  display.clearDisplay();
  drawBootLetterbox(BOOT_LETTERBOX_HEIGHT);
  drawBootTitle(title, 4);
  drawLine(0, 16, "PLAYER NODE", 1);
  drawLine(0, 26, deviceId, 1);
  flush();
  delay(400);

  // Phase 5 — staged boot progress.
  struct BootStep {
    const char *label;
    uint8_t progress;
    uint16_t holdMs;
  };

  const BootStep steps[] = {
    {"CORE", 18, 220},
    {"RADIO", 46, 260},
    {"NET", 72, 280},
    {"SYNC", 100, 320},
  };

  for (uint8_t i = 0; i < 4; i++) {
    display.clearDisplay();
    drawBootLetterbox(BOOT_LETTERBOX_HEIGHT);
    drawBootTitle(title, 4);
    drawBootStatusLine(steps[i].label, steps[i].progress >= 100);
    drawBootProgressBar(steps[i].progress);
    drawLine(0, 36, firmwareVersion, 1);
    flush();
    delay(steps[i].holdMs);
  }

  // Phase 6 — ready flash.
  for (uint8_t pulse = 0; pulse < 3; pulse++) {
    display.clearDisplay();
    drawBootLetterbox(BOOT_LETTERBOX_HEIGHT);
    drawBootTitle(title, 4);
    drawLine(0, 18, "ONLINE", 1);
    drawLine(0, 28, "READY", 1);
    drawBootProgressBar(100);
    display.invertDisplay(pulse % 2 == 1);
    flush();
    delay(130);
  }

  display.invertDisplay(false);
  display.clearDisplay();
  flush();

  huntLogInfo("UI", "Cinematic boot sequence complete");
}

void HuntPlayerDisplay::drawStatusScreen(HuntPlayerStateManager *state) {
  if (state == nullptr) {
    return;
  }

  drawLine(0, 0, state->getId(), 1);
  drawLine(0, 10, state->getStateName(), 1);
  drawLine(0, 20, "HP " + String(state->getHealth()), 1);
  drawLine(0, 30, "Lives " + String(state->getLives()), 1);
}

void HuntPlayerDisplay::drawNetworkScreen(HuntPlayerStateManager *state, int knownNodes) {
  drawLine(0, 0, "NETWORK", 1);
  drawLine(0, 10, "Nodes " + String(knownNodes), 1);
  drawLine(0, 20, "ESP-NOW", 1);
  if (state != nullptr) {
    drawLine(0, 30, state->getStateName(), 1);
  }
}

void HuntPlayerDisplay::drawSystemScreen(int knownNodes, const String &firmwareVersion) {
  drawLine(0, 0, "SYSTEM", 1);
  drawLine(0, 10, firmwareVersion, 1);
  drawLine(0, 20, "Heap " + String(ESP.getFreeHeap()), 1);
  drawLine(0, 30, "Nodes " + String(knownNodes), 1);
}

void HuntPlayerDisplay::showScreen(int screenIndex, HuntPlayerStateManager *state, int knownNodes, const String &firmwareVersion) {
  if (!ready) {
    return;
  }

  lastScreenIndex = screenIndex;
  lastState = state;
  lastKnownNodes = knownNodes;
  lastFirmwareVersion = firmwareVersion;

  if (alertActive && millis() < alertUntilMs) {
    return;
  }

  display.clearDisplay();

  switch (screenIndex) {
    case 0:
      drawStatusScreen(state);
      break;
    case 1:
      drawNetworkScreen(state, knownNodes);
      break;
    default:
      drawSystemScreen(knownNodes, firmwareVersion);
      break;
  }

  flush();
}

void HuntPlayerDisplay::showAlert(const String &message, HuntNotificationType type) {
  if (!ready) {
    return;
  }

  display.clearDisplay();
  drawLine(0, 0, "ALERT", 1);

  if (type == NOTIFY_SUCCESS) {
    drawLine(0, 12, "OK", 1);
  } else if (type == NOTIFY_WARNING || type == NOTIFY_ALERT) {
    drawLine(0, 12, "!", 2);
  } else {
    drawLine(0, 12, "...", 1);
  }

  drawLine(0, 28, message, 1);
  flush();

  alertActive = true;
  alertUntilMs = millis() + 1800;
}

void HuntPlayerDisplay::update() {
  if (!ready || !alertActive) {
    return;
  }

  if (millis() >= alertUntilMs) {
    alertActive = false;
    showScreen(lastScreenIndex, lastState, lastKnownNodes, lastFirmwareVersion);
  }
}
