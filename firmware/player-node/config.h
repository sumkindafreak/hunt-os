#ifndef PLAYER_NODE_CONFIG_H
#define PLAYER_NODE_CONFIG_H

// =====================================================
// HUNT Player Node v0.1 Configuration
// Board target: ESP32-C3 Super Mini with OLED
// NOTE: Pin values are prototype defaults and may need
// updating once the exact OLED board pinout is confirmed.
// =====================================================

#define DEVICE_ID "PLAYER_01"
#define DEVICE_NAME "HUNT Player Node"
#define FIRMWARE_VERSION "Player v0.1"

// Single player button.
#define BUTTON_PIN 3

// RGB LED pins. These defaults assume a simple 3-pin RGB LED.
#define RGB_RED_PIN 4
#define RGB_GREEN_PIN 5
#define RGB_BLUE_PIN 6
#define RGB_COMMON_ANODE false

// Buzzer pin. Works best with an active buzzer for first tests.
#define BUZZER_PIN 7

// OLED I2C pins. Many ESP32-C3 OLED boards use fixed onboard pins.
#define OLED_SDA_PIN 8
#define OLED_SCL_PIN 9
#define OLED_I2C_ADDRESS 0x3C

// Button timing.
#define BUTTON_DEBOUNCE_MS 40
#define BUTTON_LONG_PRESS_MS 800

#endif
