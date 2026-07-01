#ifndef PLAYER_NODE_CONFIG_H
#define PLAYER_NODE_CONFIG_H

// =====================================================
// HUNT Player Node v0.1 Configuration
// Board target: ESP32-C3 Mini OLED board
// Known from board diagram:
//   OLED SDA = GPIO8
//   OLED SCL = GPIO9
// =====================================================

#define DEVICE_ID "PLAYER_01"
#define DEVICE_NAME "HUNT Player Node"
#define FIRMWARE_VERSION "Player v0.1"

// Single player button.
// Prototype default until final enclosure wiring is confirmed.
#define BUTTON_PIN 3

// RGB LED pins. These defaults assume a simple 3-pin RGB LED.
// May later be replaced with a single NeoPixel output.
#define RGB_RED_PIN 4
#define RGB_GREEN_PIN 5
#define RGB_BLUE_PIN 6
#define RGB_COMMON_ANODE false

// Buzzer pin. Works best with an active buzzer for first tests.
#define BUZZER_PIN 7

// Onboard OLED I2C pins.
#define OLED_SDA_PIN 8
#define OLED_SCL_PIN 9
#define OLED_I2C_ADDRESS 0x3C

// OLED driver is still to be confirmed by test sketch.
#define OLED_DRIVER_UNKNOWN 1
#define OLED_WIDTH 72
#define OLED_HEIGHT 40

// Button timing.
#define BUTTON_DEBOUNCE_MS 40
#define BUTTON_LONG_PRESS_MS 800

#endif
