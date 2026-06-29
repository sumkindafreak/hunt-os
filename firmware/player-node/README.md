# HUNT Player Node Firmware

**Version:** v0.1 Genesis

## Purpose

This sketch is the first bootable Player Node firmware spine.

It currently tests:

- Serial debug
- RGB LED
- Buzzer
- Single button
- Player state changes
- Shared HUNT protocol builder/parser
- Heartbeat packet logging

## Hardware Target

ESP32-C3 Super Mini with OLED.

The OLED is not enabled yet because the exact board pinout and display driver still need confirming.

## Current Button Behaviour

- Short press: changes debug screen
- Long press: toggles test infected state

## Current LED Behaviour

- Blue: booting
- Green: alive
- Purple: infected
- Red: eliminated

## Upload Notes

Open `player-node.ino` in Arduino IDE.

Make sure the `firmware/shared` folder remains beside the player-node folder so the relative includes work.

## Next Firmware Step

Add ESP-NOW send/receive support.