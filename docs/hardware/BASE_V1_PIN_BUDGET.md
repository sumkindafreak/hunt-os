# HUNT Base v1 Pin Budget

**Status:** Draft  
**Target:** ESP32-S3 Base Node  
**Purpose:** Reserve GPIO before final wiring or PCB design

---

## Principle

HUNT firmware should use named pins only.

No feature should use raw GPIO numbers directly inside the main sketch.

All wiring decisions should live in:

```text
firmware/base-node/pins.h
```

This allows prototype wiring, PCB revisions and future hardware changes without rewriting game logic.

---

## Reserved Base Functions

| Function | Purpose | Notes |
|---|---|---|
| I2C SDA | OLED / expansion bus | Shared I2C bus |
| I2C SCL | OLED / expansion bus | Shared I2C bus |
| Encoder CLK | EC11 rotary input | Base menu navigation |
| Encoder DT | EC11 rotary input | Base menu navigation |
| Encoder SW | EC11 push button | Select / confirm |
| Action Button | Extra physical control | Trigger / activate |
| Back Button | Extra physical control | Back / cancel |
| Status NeoPixels | Base role/status ring | UI feedback |
| Decor NeoPixels | Expansion / atmosphere strip | Scene feedback |
| DFPlayer RX | Audio serial input | Base sound output |
| DFPlayer TX | Audio serial output | Base sound output |
| Buzzer | Simple local feedback | Optional if DFPlayer fitted |
| Battery Sense | Power monitoring | Future ADC input |
| Expansion GPIO 1 | Future module | Reserved |
| Expansion GPIO 2 | Future module | Reserved |
| Expansion GPIO 3 | Future module | Reserved |
| Expansion GPIO 4 | Future module | Reserved |

---

## Recommended Expansion Header

A future Base PCB should expose a standard HUNT expansion header.

Suggested pins:

```text
5V
3V3
GND
I2C SDA
I2C SCL
UART TX
UART RX
GPIO 1
GPIO 2
GPIO 3
GPIO 4
```

---

## Expansion Header Uses

Future plug-in modules may include:

- Relay board
- DMX board
- Smoke machine trigger
- NFC / RFID reader
- GPS module
- LoRa radio
- RS485 interface
- Sensor board
- Extra display
- Keypad

---

## Design Rule

Do not wire prototypes randomly if a named reserved function already exists.

Wire the hardware to match the firmware plan where practical.

If a pin must change, update `pins.h` first, not the main sketch.

---

## Next Step

Create:

```text
firmware/base-node/pins.h
```

Then update:

```text
firmware/base-node/config.h
```

to include named hardware definitions from `pins.h`.
