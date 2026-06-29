# Player Node Hardware Specification

**Version:** 0.1 Genesis  
**Date:** 2026-06-29  
**Status:** Draft

---

## Purpose

The Player Node is the device carried by each player during a HUNT game.

It should be small, simple, reliable and cheap enough to build in quantity.

---

## Current Planned Hardware

- ESP32-C3 Super Mini with OLED
- Single user button
- RGB LED
- Buzzer
- Battery power
- USB programming / charging depending on board and battery circuit

---

## Core Responsibilities

The Player Node must support:

- Player ID
- Player status
- Team colour
- Health or lives
- Infection state
- Current objective
- Buzzer alerts
- RGB status feedback
- Basic menu navigation
- Wireless communication with Base Nodes

---

## User Interface

### OLED

The OLED should display short, readable status screens.

Suggested screens:

1. Boot / logo screen
2. Player status
3. Objective
4. Team / game state
5. Diagnostics

Example:

```text
HUNT
PLAYER 04
TEAM: BLUE
HP: 80
STATUS: ALIVE
```

### Button

Recommended behaviour:

- Short press: next screen / acknowledge alert
- Long press: action / select
- Hold on boot: setup or recovery mode

Avoid complicated button patterns in v1.

### RGB LED

Suggested default meanings:

- Blue: booting / pairing
- Green: alive / ready
- Yellow: warning / objective active
- Red: eliminated / critical
- Purple: infected / special state
- White: setup mode

### Buzzer

Suggested default sounds:

- Short beep: confirm
- Double beep: alert
- Fast chirp: objective update
- Low tone: error
- Long tone: eliminated / game over

---

## Power Requirements

The Player Node should be suitable for battery operation.

v0.1 assumptions:

- Battery type not finalised
- Battery monitoring desirable but not mandatory for first prototype
- Low-power mode should be considered later

---

## Wireless Requirements

The Player Node must communicate with Base Nodes.

The exact protocol is not finalised.

Options to test:

- ESP-NOW
- Wi-Fi UDP
- BLE
- Hybrid mode

The first prototype should prioritise reliable local communication over internet features.

---

## Minimum v0.1 Firmware Features

- Boot screen
- Device ID
- Player state display
- Button screen switching
- RGB status LED
- Buzzer feedback
- Receive a basic message from Base Node
- Update displayed player state

---

## Future Features

Potential future additions:

- Battery percentage
- Player profiles
- Inventory
- Achievements
- OTA firmware updates
- Charging dock support
- Wearable enclosure
- Vibration motor
- Haptic feedback

---

## Design Rules

1. The Player Node must be simple to use.
2. The screen must show only useful information during play.
3. Feedback must be obvious without needing to read a manual.
4. It must survive real outdoor handling.
5. It should not require a phone to function during a game.

---

## Open Questions

1. Exact ESP32-C3 OLED board model?
2. Exact OLED size and driver?
3. RGB LED type: common cathode, common anode or addressable?
4. Buzzer type: active or passive?
5. Battery type and charging method?
6. Preferred enclosure size?
7. Is the Player Node handheld, wearable or both?
