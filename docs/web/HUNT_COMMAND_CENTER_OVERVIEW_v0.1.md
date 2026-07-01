# HUNT Command Center v0.1

## Purpose

HUNT Command Center is the phone-friendly WebUI served by the Base Node.

It gives the organiser full control over a HUNT game without needing a laptop, cloud service or internet connection.

## Hosting Model

The ESP32-S3 Base Node hosts the interface from PROGMEM.

The Base may run as:

- Wi-Fi Access Point
- Local network station
- Hybrid mode later

Default local access target:

- http://192.168.4.1
- http://hunt.local when supported

## Required Sections

- Dashboard
- Game Control
- Players
- Base Nodes
- Relay Nodes
- Audio
- Lighting
- Sensors
- Scenes
- Scenarios
- Missions
- Network
- Event Log
- Diagnostics
- Settings
- Developer Mode

## Design Principle

Every major HUNT feature should be controllable from:

1. The physical device controls.
2. The HUNT Command Center WebUI.
3. The HUNT API.

This keeps the platform consistent and scalable.
