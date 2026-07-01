#ifndef BASE_NODE_PINS_H
#define BASE_NODE_PINS_H

// =====================================================
// HUNT Base Node v1 Pin Map
//
// All Base firmware should use these named pins instead
// of raw GPIO numbers. Wire the prototype to this map
// where practical, or edit this file if the hardware
// changes later.
// =====================================================

// I2C bus for 1.3-inch OLED + future I2C expansion.
#define PIN_I2C_SDA 8
#define PIN_I2C_SCL 9

// EC11 rotary encoder module.
#define PIN_ENCODER_CLK 4
#define PIN_ENCODER_DT 5
#define PIN_ENCODER_SW 6

// Local Base buttons.
#define PIN_BUTTON_BACK 7
#define PIN_BUTTON_ACTION 10

// Local status LED, optional onboard LED.
#define PIN_STATUS_LED 2

// NeoPixel outputs.
#define PIN_STATUS_NEOPIXELS 11
#define PIN_DECOR_NEOPIXELS 12

// DFPlayer Mini serial pins.
#define PIN_DFPLAYER_RX 16
#define PIN_DFPLAYER_TX 17

// Optional simple buzzer.
#define PIN_BUZZER 18

// Future battery voltage sense input.
#define PIN_BATTERY_SENSE 1

// Reserved expansion GPIO.
#define PIN_EXPANSION_GPIO_1 13
#define PIN_EXPANSION_GPIO_2 14
#define PIN_EXPANSION_GPIO_3 15
#define PIN_EXPANSION_GPIO_4 21

#endif
