# HUNT Base Node Firmware

**Version:** v0.1 Genesis

## Purpose

This sketch is the first bootable Base Node firmware spine.

It currently tests:

- Serial debug
- Rotary encoder
- Encoder push button
- Back button
- Action button
- Status LED blink
- Base role switching
- Shared HUNT protocol builder/parser
- Heartbeat packet logging

## Hardware Target

ESP32-S3 development board.

Pin values are prototype defaults and should be updated when the exact Base hardware is confirmed.

## Current Controls

- Rotate encoder: change Base role
- Encoder press: confirm/test selected role
- Back button: print debug screen
- Action button: build and log a test event packet

## Current Roles

- Safe Zone
- Scanner
- Objective
- Extraction

## Upload Notes

Open `base-node.ino` in Arduino IDE.

Make sure the `firmware/shared` folder remains beside the base-node folder so the relative includes work.

## Next Firmware Step

Add ESP-NOW send/receive support.