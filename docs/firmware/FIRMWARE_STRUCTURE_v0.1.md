# HUNT Firmware Structure

**Version:** 0.1 Genesis  
**Date:** 2026-06-29  
**Status:** Draft

---

## Purpose

This document defines the first main firmware structure for HUNT OS.

The goal is to make the firmware modular from the start so Player Node and Base Node code can share common systems.

---

## Firmware Goals

HUNT firmware should be:

- Easy to understand
- Modular
- Debuggable
- Reusable between devices
- Scenario-driven where possible
- Safe from feature creep
- Suitable for Arduino IDE / PlatformIO later

---

## Repository Firmware Layout

```text
firmware/
├── player-node/
│   ├── player-node.ino
│   ├── config.h
│   └── README.md
├── base-node/
│   ├── base-node.ino
│   ├── config.h
│   └── README.md
└── shared/
    ├── HuntProtocol.h
    ├── HuntProtocol.cpp
    ├── HuntDevice.h
    ├── HuntDevice.cpp
    ├── HuntMessages.h
    ├── HuntTypes.h
    ├── HuntDebug.h
    └── HuntConfig.h
```

---

## Shared Modules

### HuntProtocol

Responsible for:

- Building HUNT text packets
- Parsing HUNT text packets
- Validating protocol version
- Extracting type, source, target and payload

### HuntDevice

Responsible for:

- Device ID
- Device type
- Firmware version
- Current game state
- Heartbeat timing

### HuntMessages

Contains standard message names:

- HELLO
- HEARTBEAT
- EVENT
- COMMAND
- STATE
- ACK
- ERROR

### HuntTypes

Contains enums and shared data types:

- DeviceType
- PlayerState
- BaseRole
- MessageType
- TeamColour

### HuntDebug

Responsible for:

- Serial debug helpers
- Debug banners
- Packet logging
- Error logging

### HuntConfig

Shared compile-time settings:

- Protocol version
- Default heartbeat interval
- Debug mode
- Maximum packet length

---

## Player Node Firmware Modules

Initial Player Node firmware should include:

1. Boot manager
2. OLED display manager
3. Button manager
4. RGB LED manager
5. Buzzer manager
6. ESP-NOW communication manager
7. Player state manager
8. Packet handler

---

## Base Node Firmware Modules

Initial Base Node firmware should include:

1. Boot manager
2. Encoder input manager
3. Button manager
4. Role manager
5. ESP-NOW communication manager
6. Base state manager
7. Packet handler
8. Serial admin/debug interface

---

## Main Loop Rules

The main loop must stay non-blocking.

Avoid delay() except tiny startup effects where absolutely harmless.

Recommended loop pattern:

```cpp
void loop() {
  updateInputs();
  updateCommunication();
  updateGameState();
  updateDisplay();
  updateOutputs();
  sendHeartbeatIfDue();
}
```

---

## First Firmware Milestone

The first firmware milestone is not a full game.

It is:

1. Player Node boots.
2. Base Node boots.
3. Both broadcast HELLO.
4. Base sends EVENT.
5. Player receives event.
6. Player changes LED, buzzer and OLED state.
7. Player sends ACK.
8. Serial monitor shows the full exchange.

---

## Coding Standards

- Define pins clearly at the top of config files.
- Use readable function names.
- Add beginner-friendly comments.
- Print useful Serial debug messages.
- Keep device-specific logic out of shared protocol files.
- Keep game logic separate from hardware output logic.
- Prefer enums over magic strings inside firmware where practical.

---

## Versioning

Initial firmware labels:

- HUNT OS v0.1
- Player Node Firmware v0.1
- Base Node Firmware v0.1
- Protocol v0.1

---

## Out of Scope for First Firmware

- OTA updates
- Scenario files on SD
- Full game editor
- Cloud connection
- Mobile app
- Encryption
- Marketplace
- Multiple game modes

---

## Next Step

Create the first compile-ready Player Node and Base Node firmware skeletons.

The skeletons should include:

- All includes
- Pin definitions
- setup()
- loop()
- Debug output
- Basic hardware test functions
- Placeholder-free structure with real functions

The first code should compile and give visible feedback even before wireless interaction is complete.
