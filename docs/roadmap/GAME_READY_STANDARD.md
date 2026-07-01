# HUNT Game-Ready Standard

## Purpose

This document defines what HUNT must achieve before it can be considered game-ready.

Game-ready does not mean commercially finished. It means the system is reliable enough to run a real test game with players, one organiser and working hardware.

---

## Game-Ready Definition

HUNT is game-ready when one organiser can run a complete small game using:

- 1 Base Node
- 2 or more Player Nodes
- Command Center WebUI
- ESP-NOW communication
- Scene/role control
- Player status changes
- Lighting feedback
- Audio/haptic feedback
- Clear event logging

---

## Required Player Node Features

- Boots reliably
- Sends HELLO/heartbeat packets
- Appears in Base node registry
- Shows player state on OLED or serial debug during early builds
- Has button input
- Has RGB/status LED feedback
- Has buzzer feedback
- Has rumble/haptic feedback
- Can become Alive, Safe, Infected or Eliminated
- Can receive events from Base
- Can send player action events back to Base

---

## Required Base Node Features

- Boots reliably
- Hosts Command Center Wi-Fi AP
- Serves WebUI from flash/PROGMEM
- Sends HELLO/heartbeat packets
- Tracks discovered nodes
- Sends events to Player Nodes
- Changes role/scene locally
- Controls status NeoPixels
- Controls decor NeoPixels
- Has audio service ready for DFPlayer integration
- Logs game events

---

## Required Command Center Features

- Loads from phone
- Shows system status
- Shows connected players
- Shows connected nodes
- Can start/pause/end game
- Can trigger emergency stop
- Can trigger basic scene changes
- Can trigger player state changes
- Shows live or refreshable event log

---

## Required Game Features

Minimum playable game mode:

- Lobby phase
- Game running phase
- Game ended phase
- Player alive state
- Player infected state
- Player safe/healed state
- Player eliminated state
- Safe Zone scene
- Scanner/Infection scene
- Objective scene
- Extraction scene

---

## Required Feedback Patterns

### Player Node

- Boot: short beep / blue LED
- Alive: green LED
- Infected: purple/red feedback
- Safe: blue/green feedback
- Eliminated: red feedback
- Event received: beep and/or rumble

### Base Node

- Safe Zone: green status
- Scanner: purple status
- Objective: blue status
- Extraction: yellow status
- Error: red status
- Activation: white flash

---

## Required Tests

- Base boots without crash
- Player boots without crash
- Base WebUI loads on phone
- Base discovers Player
- Player discovers Base or receives Base events
- Base action button affects Player
- Player button changes local state/event
- NeoPixels update by scene
- Event log shows activity
- System survives 30 minutes powered on

---

## Not Required For Game-Ready

These can wait until later:

- OTA updates
- Full scenario package import
- HUNT Studio
- Advanced mission editor
- Commercial enclosure
- Custom PCB
- Multiple Base Nodes
- Relay Node support
- Marketplace/distribution

---

## Final Rule

A feature is not game-ready until it can be controlled from at least one of these:

- Physical hardware
- Command Center WebUI
- HUNT packet/API command

For v1.0, important features should eventually support all three.
