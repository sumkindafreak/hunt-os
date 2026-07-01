# HUNT Base Node Firmware

**Version:** v0.1 Genesis

## Purpose

This sketch is the first bootable Base Node firmware spine.

It currently tests:

- Serial debug
- 1.3-inch OLED operator display (I2C)
- Rotary encoder
- Encoder push button
- Back button
- Action button
- Status LED blink
- Base role switching
- NeoPixel role feedback
- Web Command Center AP
- Shared HUNT protocol builder/parser
- ESP-NOW heartbeat and events

## Hardware Target

ESP32-S3 development board with 1.3-inch OLED + EC11 encoder module.

### OLED wiring (I2C)

| Signal | GPIO |
|--------|------|
| SDA | GPIO8 |
| SCL | GPIO9 |
| Address | 0x3C |

## Required Arduino Libraries

- **ESP32 Arduino core**
- **Adafruit NeoPixel**
- **Adafruit GFX Library**
- **Adafruit SH110X** (provides SH1106G driver for the 1.3" module)

If the OLED stays blank, install the **Adafruit SH110X** library from Library Manager (not just SSD1306).

## Current Controls

- Rotate encoder: change Base role (OLED flashes new role)
- Encoder press: confirm role / send HELLO
- Back button: refresh status screen
- Action button: send role event to player

## OLED Display

Shows role, scene, known node count, and web AP info. Updates on encoder change, node discovery, and back button.

If the OLED shows **TV static / snow**, the wrong driver was in use — base firmware now uses **SH1106** by default.

If the OLED stays blank after upload:

1. Install **Adafruit SH110X** + **Adafruit GFX** libraries
2. Confirm SDA/SCL wiring on GPIO8/GPIO9
3. Check serial for `Base SH1106 OLED ready`

If text is mirrored or offset, try address `0x3D` in `config.h` (auto-fallback is already built in).

## Upload Notes

Open `base-node.ino` in Arduino IDE.

Make sure the `firmware/shared` folder remains beside the base-node folder so the relative includes work.

## Next Firmware Step

Add menu navigation across multiple OLED screens via encoder.
