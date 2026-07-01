# Base Node Interface Profile: 1.3-inch OLED + EC11 Encoder Module

**Version:** v0.1  
**Role:** HUNT Base Node primary user interface  
**Status:** Prototype target

---

## Module Summary

The Base Node will use a combined front-panel module containing:

- 1.3-inch OLED display
- EC11 rotary encoder
- Encoder push button
- I2C interface for the OLED
- Extra onboard button if provided by module variant

---

## Why This Module Is Suitable

This module simplifies Base assembly because the display and rotary encoder are already positioned together.

It is better suited to the Base than a small OLED because it can show:

- Current role
- Scene name
- Network status
- Player count
- Timer
- Menu options
- Diagnostics
- Error messages

---

## HUNT Base UI Standard

The Base Node should use this module for:

- Local menu navigation
- Role selection
- Scene selection
- Network status
- Diagnostics
- Emergency / warning messages

---

## Firmware Requirements

The firmware should support:

- I2C OLED display driver
- Encoder clockwise / counter-clockwise input
- Encoder push button
- Menu system
- Confirmation sounds
- NeoPixel role feedback
- WebUI status mirroring

---

## Open Questions

These must be confirmed with the test sketch or module documentation:

- OLED driver: SSD1306 or SH1106
- OLED resolution: likely 128x64
- I2C address: likely 0x3C or 0x3D
- Encoder CLK pin
- Encoder DT pin
- Encoder SW pin
- Additional button pin if present
- Module voltage tolerance

---

## Design Rule

The Base display is for operators.

It should show concise operational information, not player-facing story text unless the Base is being used as a visible prop terminal.
