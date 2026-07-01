# HUNT Device Standard

## Purpose

This document defines the naming and capability standard for all HUNT devices.

---

## Device Families

| Code | Device | Purpose |
|---|---|---|
| HN-01 | Player Node | Player badge / handheld unit |
| HB-01 | Base Node | Main hub / scene controller |
| HR-01 | Relay Node | Remote relay/output controller |
| HL-01 | Lighting Node | NeoPixel / lighting controller |
| HA-01 | Audio Node | Remote audio playback |
| HS-01 | Sensor Node | Motion, RFID, NFC, environment sensors |
| HD-01 | Display Node | Remote display / prop terminal |
| HG-01 | Gateway Node | Bridge to Wi-Fi, web, or future cloud tools |

---

## Device IDs

Device IDs should be human-readable.

Examples:

```text
BASE-001
PLAYER-001
PLAYER-002
RELAY-001
AUDIO-001
SENSOR-001
```

Development builds may still use:

```text
BASE_01
PLAYER_01
```

---

## Device Capabilities

Each node should advertise what it can do.

Example Player capabilities:

```text
OLED,BUTTON,BUZZER,RGB,RUMBLE
```

Example Base capabilities:

```text
WEB,OLED,ENCODER,STATUS_PIXELS,DECOR_PIXELS,AUDIO
```

Example Relay capabilities:

```text
RELAY8,INPUT4,12V
```

---

## Standard Capability Names

- OLED
- BUTTON
- ENCODER
- BUZZER
- RGB
- RUMBLE
- STATUS_PIXELS
- DECOR_PIXELS
- AUDIO
- WEB
- RELAY4
- RELAY8
- INPUT4
- SENSOR
- BATTERY
- USB_POWER
- LIPO_POWER

---

## Discovery Rule

Every HUNT node should announce:

- Device ID
- Device type
- Firmware version
- Current state
- Capabilities

---

## Design Rule

The Command Center should not assume hardware exists.

It should show controls based on reported capabilities wherever possible.
