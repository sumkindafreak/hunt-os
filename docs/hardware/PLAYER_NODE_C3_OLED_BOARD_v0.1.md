# Player Node Board Profile: ESP32-C3 Mini OLED

**Version:** v0.1  
**Role:** HUNT Player Node / lightweight expansion node  
**Status:** Prototype target  
**Reference:** Original HUNT / ZombieTag badge firmware

---

## Board Summary

This board is the current preferred Player Node target.

It includes:

- ESP32-C3
- USB-C connector
- Small onboard OLED
- BOOT button
- RESET button
- Exposed GPIO headers
- 5V, 3.3V and GND pins

---

## Important Note

The printed board diagram suggested OLED I2C on GPIO8/GPIO9, but the original working HUNT reference firmware used GPIO5/GPIO6.

For next-gen HUNT, the working reference takes priority until hardware testing proves otherwise.

---

## Working Reference Pin Map

### OLED

- SDA: GPIO5
- SCL: GPIO6
- Address: 0x3C
- I2C speed: 400kHz
- Width: 128
- Height: 64
- Visible X offset: 28
- Visible Y offset: 24
- Driver reference: SSD1306-style path in old badge build

### Button

- Main button: GPIO9
- Debounce: 45ms
- Long press: 650ms

### Buzzer / Haptics

- Buzzer: GPIO10
- Rumble motor: GPIO4

### RGB LED

- Red: GPIO7
- Green: GPIO8
- Blue: GPIO3
- Common anode: false
- Suggested brightness cap: 96 / 255

---

## HUNT Player Node Suggested Use

Recommended Player Node hardware:

- Onboard OLED for player status
- One external game button
- Small buzzer
- Optional rumble motor
- RGB LED or single NeoPixel
- Battery power
- ESP-NOW communication

---

## Design Rule

This reference firmware should not be merged into next-gen HUNT.

It is only used to confirm hardware behaviour, pins and working display assumptions.

The Player Node should remain simple and show only the information a player needs during play:

- ID
- Team
- Alive / infected / safe / eliminated
- Health
- Objective prompt
- Alerts
