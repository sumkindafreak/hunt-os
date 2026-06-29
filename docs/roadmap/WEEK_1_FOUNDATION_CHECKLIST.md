# Week 1 Foundation Checklist

**Phase:** Genesis  
**Target:** Prepare HUNT OS for first firmware skeleton  
**Status:** Active

---

## Goal

Finish enough foundation planning to begin coding the first Player Node and Base Node firmware with a clear structure.

---

## Completed

- Manifesto v0.1 created
- Genesis Overview created
- State of HUNT report created
- Architecture specification v0.1 created
- Player Node hardware specification v0.1 created
- Base Node hardware specification v0.1 created
- Communication Protocol v0.1 created
- HUNT Design Proposal template created
- HDP-0001 communications decision created
- Firmware Structure v0.1 created
- Scenarios directory started

---

## Decisions Made

### Communications

ESP-NOW will be used first for v0.1 game-event communication.

Wi-Fi UDP remains a future option for debugging, admin tools and companion software.

### Product Scope

HUNT v0.1 focuses on:

- One Player Node
- One Base Node
- One simple interaction
- Clear OLED / LED / buzzer feedback
- Serial debug

### First Game Direction

Zombie Outbreak remains the recommended flagship scenario, but v0.1 firmware should stay platform-neutral.

---

## Still Needed Before Coding

- Confirm exact ESP32-C3 OLED board pinout.
- Confirm exact ESP32-S3 Base board.
- Confirm RGB LED type.
- Confirm buzzer type.
- Confirm button pin choices.
- Confirm encoder pin choices.
- Confirm OLED driver and I2C pins.

---

## Firmware Skeleton Target

The next commit should create:

```text
firmware/player-node/player-node.ino
firmware/player-node/config.h
firmware/player-node/README.md
firmware/base-node/base-node.ino
firmware/base-node/config.h
firmware/base-node/README.md
firmware/shared/HuntProtocol.h
firmware/shared/HuntProtocol.cpp
firmware/shared/HuntTypes.h
firmware/shared/HuntDebug.h
```

---

## First Functional Test

The first code test should prove:

1. Player Node boots.
2. OLED displays HUNT Player.
3. RGB LED shows boot / ready state.
4. Buzzer beeps on startup.
5. Button press changes screen.
6. Base Node boots.
7. Encoder and buttons print debug messages.
8. Both firmware sketches compile independently.

---

## Definition of Week 1 Done

Week 1 is complete when:

- Core docs exist.
- Core hardware assumptions are recorded.
- Communications direction is chosen.
- Firmware structure is defined.
- First firmware skeleton is ready to implement.

---

## Next Phase

Week 2: HUNT OS Firmware Skeleton

Primary goal:

> Get one Player Node and one Base Node booting with visible feedback, serial debug and a shared protocol parser.
