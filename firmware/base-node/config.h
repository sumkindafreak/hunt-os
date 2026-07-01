#ifndef BASE_NODE_CONFIG_H
#define BASE_NODE_CONFIG_H

// =====================================================
// HUNT Base Node v0.1 Configuration
// Board target: ESP32-S3
// NOTE: Pin values are prototype defaults and should be
// updated once the exact Base hardware is confirmed.
// =====================================================

#define DEVICE_ID "BASE_01"
#define DEVICE_NAME "HUNT Base Node"
#define FIRMWARE_VERSION "Base v0.1"

// Rotary encoder pins.
#define ENCODER_CLK_PIN 4
#define ENCODER_DT_PIN 5
#define ENCODER_SW_PIN 6

// Two extra buttons.
#define BUTTON_BACK_PIN 7
#define BUTTON_ACTION_PIN 8

// Optional onboard/status LED for prototype feedback.
#define STATUS_LED_PIN 2

// NeoPixel status ring / strip.
#define BASE_NEOPIXEL_PIN 9
#define BASE_NEOPIXEL_COUNT 12
#define BASE_NEOPIXEL_BRIGHTNESS 40

// NeoPixel decor / expansion output.
#define DECOR_NEOPIXEL_PIN 10
#define DECOR_NEOPIXEL_COUNT 30
#define DECOR_NEOPIXEL_BRIGHTNESS 60

#define BUTTON_DEBOUNCE_MS 40
#define ENCODER_DEBOUNCE_MS 3

#endif
