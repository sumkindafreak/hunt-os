# HDP-0004: HUNT Relay Node

**Status:** Planned

## Purpose

The Relay Node is an ESP-NOW controlled expansion module that allows any HUNT Base to control real-world outputs without carrying high-current hardware inside the Base.

## Hardware

- ESP32-C3 or ESP32
- 4 or 8 relay outputs
- Status NeoPixel
- Optional push button
- Screw terminals
- 5V/12V power input

## Communication

Uses the existing HUNT ESP-NOW protocol.

Example commands:

RELAY:1:ON
RELAY:1:OFF
RELAY:2:PULSE:3000
RELAY:ALL:OFF
STATUS

## Typical Uses

- Door maglocks
- Solenoid latches
- Fog machines
- Sirens
- Beacons
- UV lighting
- Generator props
- Treasure chests
- Air cannons
- DMX trigger interfaces

## Future Expansion

Relay Nodes should automatically advertise themselves to HUNT OS during boot, allowing Base units to discover nearby nodes and assign them to scenes or objectives.

Each Relay Node will have a unique device ID so multiple nodes can operate simultaneously.

## Long-term Goal

A single Base should be able to orchestrate multiple Relay Nodes, audio nodes and sensor nodes, forming a distributed attraction controlled entirely over ESP-NOW.