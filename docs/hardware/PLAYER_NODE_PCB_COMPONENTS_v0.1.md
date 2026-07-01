# HUNT Player Node PCB Components v0.1

## Purpose

This document lists the recommended PCB components for the next-generation HUNT Player Node.

The Player Node is the small handheld/wearable player device. It should be simple, rugged and cheap enough to build in multiples.

---

## Core Module

| Component | Purpose | Notes |
|---|---|---|
| ESP32-C3 Mini OLED board | Main controller, radio and display | Current prototype target |
| Female header sockets or castellated footprint | Mount ESP32-C3 board | Allows replacement during prototyping |

---

## User Feedback Components

| Component | Purpose | Suggested Value / Type |
|---|---|---|
| Main push button | Player input | Momentary tactile / panel button |
| RGB LED or 3 discrete LEDs | Player status feedback | Red, green, blue |
| LED resistors | Current limiting | 220R to 330R per colour |
| Active buzzer | Simple audio alerts | 3.3V active buzzer preferred |
| 3V coin vibration motor | Haptic feedback | Small penny/coin style motor |

---

## Rumble Motor Driver

The vibration motor must not be driven directly from the ESP32 GPIO.

### Recommended Transistor Driver

| Component | Purpose | Suggested Part |
|---|---|---|
| NPN transistor | Switches vibration motor | 2N2222, PN2222A, BC337, S8050 |
| Base resistor | Limits base current | 1kR |
| Pull-down resistor | Keeps motor off during boot | 10kR |
| Flyback diode | Protects against motor voltage spike | 1N4148 or 1N5819 |
| Motor connector | Connects vibration motor | 2-pin JST / pads |

### Rumble Wiring

```text
GPIO4 -> 1kR -> transistor base
Base -> 10kR -> GND
Emitter -> GND
Collector -> Motor -
Motor + -> 3.3V or battery +
Diode across motor, stripe to Motor +
```

### Notes

- PWM can be used on the GPIO to control vibration strength.
- Use short vibration pulses first: 80ms to 200ms.
- If the motor is powered from LiPo+, make sure all grounds are common.

---

## Power Components

| Component | Purpose | Notes |
|---|---|---|
| LiPo battery connector | Battery input | JST-PH 2-pin recommended |
| Power switch | Main on/off | Small slide switch |
| Charging module or charge header | Battery charging | TP4056 module during prototype, charger IC later |
| 3.3V regulator if needed | Stable logic supply | Depends on final battery/board arrangement |
| Bulk capacitor | Power smoothing | 100uF near power input suggested |
| Decoupling capacitors | Noise reduction | 100nF near active parts |

---

## Optional Expansion

| Component | Purpose |
|---|---|
| 4-pin expansion header | 3V3, GND, GPIO, GPIO |
| I2C test pads | SDA, SCL, GND |
| Battery voltage divider | Future battery monitoring |
| Programming/test pads | Debug and manufacturing |

---

## Current Reference Pin Map

| Function | GPIO |
|---|---|
| OLED SDA | 5 |
| OLED SCL | 6 |
| Main button | 9 |
| Buzzer | 10 |
| Rumble motor drive | 4 |
| RGB Red | 7 |
| RGB Green | 8 |
| RGB Blue | 3 |

---

## Minimum Player PCB BOM

- ESP32-C3 Mini OLED board
- Main button
- RGB LED or 3 discrete LEDs
- 3 LED resistors
- Active buzzer
- 3V coin vibration motor
- NPN transistor or small MOSFET
- 1kR resistor
- 10kR resistor
- Flyback diode
- Battery connector
- Power switch
- Charging connector/module

---

## Design Notes

- Keep the Player Node simple.
- Avoid unnecessary connectors or fragile parts.
- Label every connector on the silkscreen.
- Keep the motor driver close to the motor connector.
- Keep power tracks wider than signal tracks.
- Add mounting holes if the PCB is going into a printed case.
