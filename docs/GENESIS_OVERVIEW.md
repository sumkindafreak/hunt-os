# HUNT OS Genesis Overview

**Version:** 0.1 Genesis  
**Date:** 2026-06-29  
**Status:** Foundation document

---

## What This Repository Is

This repository is the starting point for **HUNT OS**.

HUNT OS is the operating system, design language and technical foundation for the HUNT live-action gaming platform.

HUNT is a modular real-world game system built around reusable smart devices. A device can become a player handheld, safe zone, mission terminal, capture point, puzzle, shop, generator, scanner, respawn point or any other game object defined by a scenario.

This repository should become the single source of truth for HUNT's architecture, design decisions, firmware direction, scenario engine and product documentation.

---

## What HUNT Is

HUNT is not just a zombie game.

HUNT is a platform for creating live-action adventures using reusable hardware and downloadable game scenarios.

The goal is simple:

> Build the hardware once. Create endless games with software.

---

## Core Devices

### Player Node

A small wearable or handheld device carried by each player.

Current planned hardware:

- ESP32-C3 Super Mini with OLED
- Single button
- RGB LED
- Buzzer
- Battery power

Primary responsibilities:

- Player identity
- Health, lives and status
- Team colour
- Objectives
- Inventory
- Alerts and feedback
- Communication with Base Nodes

---

### Base Node

A configurable game station used as an objective, checkpoint, safe zone, puzzle terminal or world object.

Current planned hardware:

- ESP32-S3
- Rotary encoder with push button
- Two extra buttons
- Expansion capability

Primary responsibilities:

- Game object role selection
- Objective logic
- Interaction with Player Nodes
- Timers and cooldowns
- Local admin controls
- Optional external hardware control

---

## Proposed Repository Structure

```text
hunt-os/
├── README.md
├── docs/
│   ├── manifesto/
│   ├── architecture/
│   ├── product/
│   ├── business/
│   ├── hdp/
│   └── reports/
├── firmware/
│   ├── player-node/
│   ├── base-node/
│   └── shared/
├── scenarios/
│   ├── zombie-outbreak/
│   └── examples/
├── hardware/
│   ├── player-node/
│   ├── base-node/
│   └── expansion-modules/
├── software/
│   ├── hunt-studio/
│   └── tools/
└── assets/
    ├── branding/
    ├── ui/
    └── diagrams/
```

Some folders may begin as documentation-only while the platform is being designed.

---

## Development Philosophy

HUNT will be designed as a commercial product from day one.

Every feature should support at least one of the following:

- Better player experience
- Better organiser experience
- Greater replayability
- Stronger platform flexibility
- Easier manufacturing or support
- Clearer product identity

---

## Current Priority

The immediate priority is not to build every feature.

The immediate priority is to prove the core platform loop:

1. Configure a scenario.
2. Assign Base Node roles.
3. Give players Player Nodes.
4. Start the game.
5. Play successfully.
6. Reset and play again.

If that loop feels good, HUNT has a strong foundation.

---

## Recommended First Playable Scenario

**Zombie Outbreak** should be the first complete prototype game.

Reasons:

- It connects directly to the original idea.
- It is easy for players to understand.
- It naturally uses health, infection, safe zones and objectives.
- It clearly demonstrates why Player Nodes and Base Nodes are useful.

---

## Next Documents To Create

Recommended next documents:

1. HUNT Architecture Specification v0.1
2. Player Node Hardware Specification v0.1
3. Base Node Hardware Specification v0.1
4. Communication Protocol v0.1
5. Zombie Outbreak Scenario Specification v0.1
6. HUNT Design Proposal template

---

## Current Status

HUNT is at the Genesis stage.

This means:

- The vision is being defined.
- The platform direction is being documented.
- Hardware assumptions are being recorded.
- No final technical decision should be treated as locked until tested.

The next major milestone is a working two-device prototype:

- One Player Node
- One Base Node
- One simple game interaction
