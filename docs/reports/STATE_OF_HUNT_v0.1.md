# State of HUNT

**Version:** 0.1 Genesis  
**Date:** 2026-06-29  
**Project:** HUNT OS  
**Status:** Initial architecture review

---

## Executive Summary

HUNT is being positioned as a modular live-action gaming platform rather than a single zombie game.

This is the correct strategic direction.

The strongest commercial opportunity is not selling one fixed game, but selling a reusable system that can support many games, scenarios and expansions over time.

The current concept has two primary device classes:

1. **Player Node** - carried by players.
2. **Base Node** - placed in the world and configured as a game object.

The biggest priority now is to avoid feature creep and build a focused Version 1 that proves the core loop:

> Configure a scenario. Assign Base roles. Give players devices. Start game. Play successfully.

---

## Current Known Hardware

### Player Node

Current planned hardware:

- ESP32-C3 Super Mini with OLED
- RGB LED
- Buzzer
- Single button
- Battery power

Likely role:

- Player identity
- Health / lives / infection status
- Team colour
- Objective display
- Alerts
- Basic menu navigation
- Communication with Base Nodes

### Base Node

Current planned hardware:

- ESP32-S3
- Rotary encoder
- Rotary encoder push button
- Two additional buttons
- Expansion capability

Likely role:

- Configurable game object
- Role selection
- Objective logic
- Admin control
- Interaction with Player Nodes
- Optional external hardware control

---

## Strategic Strengths

### 1. Platform Thinking

HUNT is being designed as a platform instead of a one-off game.

This creates long-term value because new scenarios can be released without changing the hardware.

### 2. Reusable Base Nodes

The Base Node idea is strong because it allows one physical product to become many different world objects.

Examples:

- Safe Zone
- Generator
- Capture Point
- Medic Station
- Ammo Station
- Scanner
- Bomb
- Treasure Chest
- Puzzle Terminal
- Extraction Point

### 3. Low-Cost Hardware Foundation

ESP32 hardware keeps prototype and early manufacturing costs low.

This may allow starter kits to be priced accessibly compared with specialist live-action gaming systems.

### 4. Strong Expansion Potential

The system can grow into:

- Scenario marketplace
- Creator tools
- Expansion modules
- Educational games
- Corporate team-building
- Scare attraction products
- Escape room modules

---

## Main Risks

### 1. Feature Creep

The biggest danger is trying to build every possible game mode before proving one excellent experience.

Recommendation:

Build one flagship game first, but architect it as a platform.

### 2. Hardware Fragmentation

Too many device types too early will increase cost, support burden and confusion.

Recommendation:

Keep Version 1 to Player Node and Base Node only.

### 3. Weak First-Time User Experience

If setup is confusing, the platform will feel like a hobby kit rather than a product.

Recommendation:

Design the unboxing, setup and first game flow before adding advanced features.

### 4. Connectivity Reliability

Live-action games need reliable communication in messy real-world environments.

Recommendation:

Prototype communication early and test outdoors, through walls and around people.

### 5. Commercial Focus

The project could drift into technical cleverness instead of solving customer problems.

Recommendation:

Every feature should be evaluated against customer value.

---

## Version 1 Scope Recommendation

HUNT v1 should prove the system with one complete flagship scenario.

Recommended flagship scenario:

**Zombie Outbreak**

Reason:

- It matches the original project roots.
- It is easy for players to understand.
- It supports health, infection, safe zones, objectives and timed escape.
- It demonstrates both Player Nodes and Base Nodes well.

---

## Minimum Viable Product

### Player Node MVP

Must support:

- Boot screen
- Device ID
- Player status
- Health or infection state
- Team colour
- Buzzer alerts
- RGB status LED
- Single-button menu navigation
- Wireless communication

Should avoid in v1:

- Complex inventory
- Accounts
- Cloud login
- Advanced achievements
- Too many game modes

### Base Node MVP

Must support:

- Role selection
- Local menu using encoder and buttons
- Broadcast role and status
- Interact with Player Nodes
- Trigger simple game actions
- Show admin feedback

Suggested initial roles:

- Safe Zone
- Infection Scanner
- Objective Point
- Extraction Point

Should avoid in v1:

- Too many role types
- Complex scripting on-device
- Marketplace dependency
- Overcomplicated admin menus

---

## Proposed HUNT OS Modules

HUNT OS should be split into clear internal systems:

1. Device Manager
2. Display Manager
3. Input Manager
4. LED Manager
5. Buzzer Manager
6. Communication Manager
7. Game State Manager
8. Role Manager
9. Scenario Manager
10. Diagnostics Manager

This structure will keep firmware understandable and reusable.

---

## Proposed Repository Priorities

1. Document the vision.
2. Document the hardware.
3. Define the communication protocol.
4. Define the scenario format.
5. Build Player Node test firmware.
6. Build Base Node test firmware.
7. Build the Zombie Outbreak prototype scenario.
8. Run real playtests.
9. Refine based on player experience.

---

## Immediate Next Decisions

The next design decisions should be:

1. What exact ESP32-C3 OLED board is being used?
2. What exact ESP32-S3 board is being used for the Base?
3. What wireless method will v1 use? Wi-Fi, ESP-NOW, BLE, or hybrid?
4. How will devices identify each other?
5. What is the first complete playable scenario?
6. What does a 10-minute first play experience look like?

---

## Current Recommendation

Do not start with a full scenario editor.

Start with hand-authored scenario files and firmware support for a few strong roles.

Once the format works, then build HUNT Studio around it.

This avoids spending time building a visual editor before the underlying game engine is proven.

---

## Final Assessment

HUNT has strong commercial potential if it remains focused on the platform vision.

The correct next move is to build a disciplined foundation:

- Manifesto
- Architecture
- Hardware spec
- Communication protocol
- Scenario format
- One excellent prototype game

If the first game is fun and the hardware feels reusable, HUNT becomes more than a project.

It becomes a product ecosystem.
