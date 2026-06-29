# HUNT Architecture Specification

**Version:** 0.1 Genesis  
**Date:** 2026-06-29  
**Status:** Draft

---

## Purpose

This document defines the first architecture direction for HUNT OS.

HUNT OS is the shared operating layer for HUNT live-action game devices.

The goal is to keep HUNT modular, reusable and commercially scalable.

---

## Platform Summary

HUNT is made from smart devices that run reusable firmware and take their behaviour from game scenarios.

The first platform contains two device classes:

1. Player Node
2. Base Node

Future devices should follow the same HUNT OS principles where possible.

---

## Core Loop

The first HUNT version must prove this loop:

1. Power on devices.
2. Assign or load a scenario.
3. Configure Base Node roles.
4. Register Player Nodes.
5. Start game.
6. Players interact with Bases.
7. Game state updates.
8. Game ends.
9. System resets for replay.

If this loop is not smooth, nothing else matters.

---

## Device Classes

### Player Node

The Player Node is carried by a player.

Responsibilities:

- Store player ID
- Show player status
- Show team colour
- Show objective prompts
- Receive game updates
- Send player interaction events
- Give LED and buzzer feedback

### Base Node

The Base Node is placed in the game world.

Responsibilities:

- Act as a configurable game object
- Run a selected role
- Interact with nearby Player Nodes
- Trigger local effects
- Display or indicate status
- Provide admin controls

---

## Core Firmware Modules

HUNT OS should be split into these internal modules:

1. Device Manager
2. Input Manager
3. Display Manager
4. LED Manager
5. Buzzer Manager
6. Communication Manager
7. Game State Manager
8. Role Manager
9. Scenario Manager
10. Diagnostics Manager
11. Storage Manager
12. Power Manager

Each module should be written so it can be reused across Player and Base firmware where sensible.

---

## Scenario-Driven Design

Game behaviour should not be hard-coded for one game where it can be expressed as scenario data.

A scenario should define:

- Game name
- Game mode
- Teams
- Player rules
- Base roles
- Objectives
- Timers
- Events
- Actions
- Win conditions
- Reset behaviour

---

## Event System

HUNT should be designed around events.

Example events:

- PLAYER_JOINED
- PLAYER_TAGGED
- PLAYER_INFECTED
- PLAYER_HEALED
- BASE_ACTIVATED
- OBJECTIVE_COMPLETE
- TIMER_EXPIRED
- GAME_STARTED
- GAME_ENDED

Events can trigger actions.

Example actions:

- Set player health
- Change team
- Play buzzer pattern
- Set LED colour
- Unlock objective
- Start cooldown
- Broadcast message

---

## First Prototype Architecture

The first prototype should be intentionally small.

Minimum hardware:

- 1 Player Node
- 1 Base Node

Minimum interaction:

1. Player Node starts as ALIVE.
2. Base Node is configured as SAFE_ZONE or INFECTION_SCANNER.
3. Player interacts with Base.
4. Base sends event.
5. Player updates OLED, RGB LED and buzzer.

This proves communication, game state and feedback.

---

## Recommended First Roles

Only a few roles should be implemented first:

- Safe Zone
- Infection Scanner
- Objective Point
- Extraction Point

These are enough to build a playable Zombie Outbreak scenario.

---

## Design Priorities

1. Reliability over cleverness
2. Simple setup over advanced features
3. Reusable systems over one-off code
4. Clear player feedback over hidden complexity
5. Fast replay over complex configuration

---

## Out of Scope for v0.1

These should not be built yet:

- Scenario marketplace
- Cloud login
- Full visual editor
- Complex inventory
- Achievements
- GPS
- Payments
- Mobile app
- Too many game modes

They may be future features, but they are distractions at Genesis stage.

---

## Success Criteria

HUNT v0.1 succeeds if:

- One Player Node can communicate with one Base Node.
- A Base role can change game state.
- The Player Node gives clear feedback.
- The interaction can be repeated reliably.
- The code structure can grow without being rewritten.

---

## Architecture Decision

HUNT will be designed as a platform-first system.

The first game will be Zombie Outbreak, but the firmware and documents must avoid locking HUNT into zombies only.
