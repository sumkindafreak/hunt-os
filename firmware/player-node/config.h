#ifndef PLAYER_NODE_CONFIG_H
#define PLAYER_NODE_CONFIG_H

// =====================================================
// HUNT Player Node v0.1 Configuration
// Board target: ESP32-C3 Mini OLED board
// Reference source: original HUNT / ZombieTag badge config
// This is next-gen HUNT firmware; old code is reference only.
// =====================================================

#define DEVICE_ID "PLAYER_01"
#define DEVICE_NAME "HUNT Player Node"
#define FIRMWARE_VERSION "Player v0.1"

// Single player button.
#define BUTTON_PIN 9

// Status RGB (red + blue channels on GPIO7 / GPIO3).
#define RGB_RED_PIN 7
#define RGB_BLUE_PIN 3
#define RGB_COMMON_ANODE false
#define RGB_LED_MAX_BRIGHTNESS 96

// Dedicated blue heartbeat LED — blinks on radio activity (GPIO8).
#define HEARTBEAT_LED_PIN 8
#define HEARTBEAT_LED_ACTIVE_HIGH true
#define HEARTBEAT_LED_PULSE_MS 140

// Buzzer pin from original working reference.
#define BUZZER_PIN 10

// Optional rumble / vibration motor output from original reference.
#define RUMBLE_PIN 4
#define RUMBLE_ENABLED true

// Onboard OLED I2C pins from original working reference.
// Note: this overrides the earlier board-diagram assumption of 8/9.
#define OLED_SDA_PIN 5
#define OLED_SCL_PIN 6
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_HZ 400000UL

// OLED settings from original reference.
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_VISIBLE_X 28
#define OLED_VISIBLE_Y 24
#define OLED_DRIVER_SH1106 false

// Boot experience — set false for instant static logo.
#define OLED_BOOT_CINEMATIC true

// Button timing.
#define BUTTON_DEBOUNCE_MS 45
#define BUTTON_LONG_PRESS_MS 650

// Serial diagnostics — set TICK to 0 to disable periodic status dumps.
#define PLAYER_SERIAL_FULL_DETAIL true
#define PLAYER_SERIAL_TICK_MS 15000

#endif
