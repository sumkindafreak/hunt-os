# Base Node Hardware Specification

**Version:** 0.1 Genesis  
**Date:** 2026-06-29  
**Status:** Draft

---

## Purpose

The Base Node is a configurable world object for HUNT games.

It is not permanently one thing.

Depending on the scenario, the same Base Node may become a safe zone, generator, scanner, shop, capture point, puzzle terminal, bomb, treasure chest or extraction point.

---

## Current Planned Hardware

- ESP32-S3
- Rotary encoder
- Rotary encoder push button
- Button 1
- Button 2
- Expansion capability

---

## Core Responsibilities

The Base Node must support:

- Role selection
- Local configuration
- Player interaction
- Game event handling
- Wireless communication
- Local status feedback
- Optional hardware expansion

---

## Controls

### Rotary Encoder

Recommended behaviour:

- Rotate: scroll menu / change value
- Press: select / confirm

### Button 1

Recommended behaviour:

- Back / cancel
- Hold: return to main menu

### Button 2

Recommended behaviour:

- Action / admin function
- Hold: start or reset local role

These mappings may change after physical testing.

---

## Role Examples

Base Node roles may include:

- Safe Zone
- Respawn Point
- Medic Station
- Infection Scanner
- Objective Point
- Extraction Point
- Capture Point
- Generator
- Puzzle Terminal
- Shop / Vendor
- Bomb Site
- Treasure Chest
- Boss / Event Trigger

v0.1 should only implement a small number of roles.

Recommended first roles:

- Safe Zone
- Infection Scanner
- Objective Point
- Extraction Point

---

## Expansion Philosophy

The Base Node should be the main expansion platform.

Possible future modules:

- Relay output
- NeoPixel output
- Servo output
- PIR sensor
- RFID / NFC reader
- Keypad
- Audio module
- Solenoid lock
- Fog trigger
- Environmental sensors
- Long-range radio

Expansion should add capability without changing the core role system.

---

## User Interface

The Base Node should provide clear setup and status information.

If a screen is fitted, suggested menu structure:

```text
HUNT BASE
> Role
  Game
  Pairing
  Test
  Settings
```

If no screen is fitted initially, RGB LED / buzzer / serial debug should be used for prototype feedback.

---

## Minimum v0.1 Firmware Features

- Boot state
- Local role selection
- Basic button / encoder input
- Wireless communication test
- Send simple event to Player Node
- Receive acknowledgement
- Serial debug output

---

## Reliability Requirements

The Base Node must be more robust than a normal maker prototype because it may be used outdoors or in public games.

Important areas:

- Stable power
- Secure connectors
- Clear enclosure
- Protected buttons
- Reset recovery
- Simple diagnostics

---

## Future Features

Potential future additions:

- Built-in display
- SD card scenario loading
- OTA firmware updates
- Web configuration
- Local scenario cache
- Battery option
- Weather-resistant enclosure
- Auto-detection of expansion modules

---

## Design Rules

1. One Base Node should support many roles.
2. The organiser should not need to reprogram firmware for every game.
3. Advanced configuration should not make normal setup confusing.
4. Expansion should be modular.
5. The Base Node should be reliable enough for paid events.

---

## Open Questions

1. Exact ESP32-S3 board model?
2. Will the Base Node have an OLED or larger display?
3. What connector standard should expansion modules use?
4. Will Base Nodes be battery powered, USB powered or both?
5. What enclosure size is acceptable?
6. How many Base Nodes should a starter kit include?
7. Should each Base have a visible ID label or digital name only?
