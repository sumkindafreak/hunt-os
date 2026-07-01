# HUNT Base Node PCB Components v0.1

## Purpose

This document lists the recommended PCB components for the next-generation HUNT Base Node.

The Base Node is the operator/mission hub. It hosts the Web Command Center, manages scenes and communicates with Player Nodes and future expansion nodes.

---

## Core Module

| Component | Purpose | Notes |
|---|---|---|
| ESP32-S3 development module/board | Main controller | Current Base target |
| Female headers or module footprint | Mount ESP32-S3 board | Allows easy replacement during prototyping |
| USB-C access | Programming/power | Often already on dev board |

---

## User Interface Components

| Component | Purpose | Notes |
|---|---|---|
| 1.3-inch OLED + EC11 module | Main Base interface | Current preferred Base UI module |
| EC11 rotary encoder | Menu navigation | Usually integrated into OLED module |
| Encoder push button | Confirm/select | Usually integrated into encoder |
| Action button | Trigger current role/scene | External momentary button |
| Back/cancel button | Menu back/cancel | External momentary button |
| Optional active buzzer | Local feedback | Useful even with DFPlayer installed |

---

## Lighting Components

| Component | Purpose | Suggested Part / Value |
|---|---|---|
| Status NeoPixel ring connector | Base role/status lighting | 3-pin JST: 5V, GND, DATA |
| Decor NeoPixel connector | Scene/prop lighting | 3-pin JST: 5V, GND, DATA |
| NeoPixel data resistors | Protect data lines | 330R in series per data line |
| NeoPixel bulk capacitor | Reduces voltage dips | 470uF to 1000uF across 5V/GND if using longer strips |

---

## Audio Components

| Component | Purpose | Notes |
|---|---|---|
| DFPlayer Mini module | Local audio playback | MP3 sounds from microSD |
| Speaker connector | Audio output | 2-pin JST or screw terminal |
| Speaker | Sound output | Small 4R/8R speaker depending on module use |
| DFPlayer RX/TX resistors | Serial protection/level friendliness | Optional 1kR series on TX/RX lines |
| DFPlayer power decoupling | Reduces resets/noise | 100uF + 100nF near module |

---

## Power Components

| Component | Purpose | Notes |
|---|---|---|
| Main power input | Base supply | USB-C, barrel jack or screw terminal |
| 5V rail | NeoPixels / DFPlayer / expansion | Needs enough current for LEDs |
| 3.3V rail | ESP32 and logic | Usually provided by dev board during prototype |
| Power switch | Main on/off | Rated for expected current |
| Fuse or polyfuse | Basic protection | Recommended for external power |
| Reverse polarity protection | Prevents damage | Diode or MOSFET protection later |
| Bulk capacitor | Stabilises power | 470uF+ on 5V rail recommended |
| Decoupling capacitors | Local filtering | 100nF near modules/connectors |

---

## Expansion Header

A future Base PCB should expose a HUNT expansion header.

Suggested signals:

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

Potential expansion modules:

- Relay board
- DMX interface
- Smoke/fog trigger
- NFC/RFID reader
- GPS module
- LoRa radio
- RS485 interface
- Extra display
- Keypad
- Sensor board

---

## Future Relay / Output Support

The main Base should avoid carrying large relay loads where possible.

Preferred architecture:

```text
Base Node -> ESP-NOW -> Relay Node -> Relays / locks / fog / beacons
```

However, a small onboard expansion output may be useful for simple prototypes.

Possible onboard output driver components:

| Component | Purpose |
|---|---|
| N-channel MOSFET module/footprint | Switch small DC load |
| Flyback diode | Protect against inductive load |
| Screw terminal | Output wiring |
| Status LED | Output state |

---

## Current Base v1 Reference Pin Map

| Function | GPIO |
|---|---|
| I2C SDA | 8 |
| I2C SCL | 9 |
| Encoder CLK | 4 |
| Encoder DT | 5 |
| Encoder SW | 6 |
| Back button | 7 |
| Action button | 10 |
| Status LED | 2 |
| Status NeoPixels | 11 |
| Decor NeoPixels | 12 |
| DFPlayer RX | 16 |
| DFPlayer TX | 17 |
| Buzzer | 18 |
| Battery sense | 1 |
| Expansion GPIO 1 | 13 |
| Expansion GPIO 2 | 14 |
| Expansion GPIO 3 | 15 |
| Expansion GPIO 4 | 21 |

---

## Minimum Base PCB BOM

- ESP32-S3 dev module or board
- 1.3-inch OLED + EC11 encoder module
- Action button
- Back button
- Optional active buzzer
- Status NeoPixel connector
- Decor NeoPixel connector
- 2x 330R NeoPixel data resistors
- DFPlayer Mini
- Speaker connector
- Power input connector
- Power switch
- 5V rail support
- Bulk capacitors
- Decoupling capacitors
- HUNT expansion header

---

## Design Notes

- Keep high-current LED power away from sensitive signal lines.
- Use wide tracks for 5V NeoPixel power.
- Label all connectors clearly on the silkscreen.
- Keep I2C pullups in mind; many OLED modules already include them.
- Use common ground between ESP32, NeoPixels, DFPlayer and expansion devices.
- Design the Base as a hub, not a high-current relay box.
