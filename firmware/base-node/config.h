#ifndef BASE_NODE_CONFIG_H
#define BASE_NODE_CONFIG_H

#include "pins.h"

// =====================================================
// HUNT Base Node v0.1 Configuration
// Board target: ESP32-S3
// Pin assignments are kept in pins.h.
// =====================================================

#define DEVICE_ID "BASE_01"
#define DEVICE_NAME "HUNT Base Node"
#define FIRMWARE_VERSION "Base v0.1"

// I2C OLED / expansion bus.
#define OLED_SDA_PIN PIN_I2C_SDA
#define OLED_SCL_PIN PIN_I2C_SCL
#define OLED_I2C_ADDRESS 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Rotary encoder pins.
#define ENCODER_CLK_PIN PIN_ENCODER_CLK
#define ENCODER_DT_PIN PIN_ENCODER_DT
#define ENCODER_SW_PIN PIN_ENCODER_SW

// Local Base buttons.
#define BUTTON_BACK_PIN PIN_BUTTON_BACK
#define BUTTON_ACTION_PIN PIN_BUTTON_ACTION

// Optional onboard/status LED for prototype feedback.
#define STATUS_LED_PIN PIN_STATUS_LED

// NeoPixel status ring / strip.
#define BASE_NEOPIXEL_PIN PIN_STATUS_NEOPIXELS
#define BASE_NEOPIXEL_COUNT 12
#define BASE_NEOPIXEL_BRIGHTNESS 40

// NeoPixel decor / expansion output.
#define DECOR_NEOPIXEL_PIN PIN_DECOR_NEOPIXELS
#define DECOR_NEOPIXEL_COUNT 30
#define DECOR_NEOPIXEL_BRIGHTNESS 60

// DFPlayer Mini serial.
#define DFPLAYER_RX_PIN PIN_DFPLAYER_RX
#define DFPLAYER_TX_PIN PIN_DFPLAYER_TX
#define DFPLAYER_VOLUME 25

// Optional buzzer.
#define BUZZER_PIN PIN_BUZZER

// Button timing.
#define BUTTON_DEBOUNCE_MS 40
#define ENCODER_DEBOUNCE_MS 3

#endif
