# Player Node Board Profile: ESP32-C3 Mini OLED

**Version:** v0.1  
**Role:** HUNT Player Node / lightweight expansion node  
**Status:** Prototype target

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

## Confirmed From Board Diagram

### I2C OLED Pins

- GPIO8 = I2C SDA
- GPIO9 = I2C SCL

### Serial Pins

- GPIO20 = RX
- GPIO21 = TX

### ADC-labelled Pins

- GPIO0 = A0
- GPIO1 = A1
- GPIO2 = A2
- GPIO3 = A3
- GPIO4 = A4
- GPIO5 = A5

### SPI-labelled Pins

- GPIO5 = MISO
- GPIO6 = MOSI
- GPIO7 = SS

---

## HUNT Player Node Suggested Use

Recommended Player Node hardware:

- Onboard OLED for player status
- One external game button
- Small buzzer
- RGB LED or single NeoPixel
- Battery power
- ESP-NOW communication

---

## Notes

The onboard OLED driver is not yet confirmed in firmware.

Likely candidates:

- SSD1306
- SH1106

The display driver must be confirmed with a test sketch before final firmware lock-in.

---

## Design Rule

The Player Node should remain simple.

The display should show only the information a player needs during play:

- ID
- Team
- Alive / infected / safe / eliminated
- Health
- Objective prompt
- Alerts
