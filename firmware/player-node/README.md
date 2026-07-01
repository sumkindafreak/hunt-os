# HUNT Player Node Firmware

**Version:** v0.1 Genesis

## Purpose

This sketch is the first bootable Player Node firmware spine.

It currently tests:

- Serial debug
- Onboard OLED (128x64 SSD1306 via I2C)
- RGB LED
- Buzzer
- Single button
- Player state changes
- Shared HUNT protocol builder/parser
- ESP-NOW heartbeat and events

## Hardware Target

ESP32-C3 Super Mini with OLED.

OLED pins (working reference):

- SDA: GPIO5
- SCL: GPIO6
- I2C address: 0x3C

## Required Arduino Libraries

Install from **Arduino Library Manager**:

- **Adafruit SSD1306**
- **Adafruit GFX Library**

Also requires the **ESP32 Arduino core** (3.x).

---

## Upload Settings (Arduino IDE)

These settings matter for the **ESP32-C3 Super Mini OLED** board. Wrong board = compile may work but upload fails or serial stays blank.

### Recommended board profile

| Setting | Value |
|---|---|
| **Board** | **MakerGO ESP32 C3 SuperMini** |
| USB CDC On Boot | **Enabled** |
| Flash Mode | **DIO** |
| Flash Size | **4MB (32Mb)** |
| Flash Frequency | **80MHz** |
| Partition Scheme | **Default 4MB with spiffs** |
| Upload Speed | **115200** |
| CPU Frequency | **160MHz (WiFi)** |

If you do not see *MakerGO ESP32 C3 SuperMini*, use **ESP32C3 Dev Module** with:

| Setting | Value |
|---|---|
| JTAG Adapter | **Integrated USB JTAG** |
| USB CDC On Boot | **Enabled** |
| Flash Mode | **DIO** |
| Upload Speed | **115200** |

### Port selection

- Pick the **USB Serial Device** COM port (Espressif `VID_303A`).
- On Windows this often appears as `COM18` or similar.
- Do **not** pick a Bluetooth or duplicate port if two appear.

### Before you click Upload

1. **Close the Serial Monitor** — an open monitor locks the COM port and upload fails with *"port is busy"*.
2. Use a **data-capable USB-C cable** (not charge-only).
3. Plug directly into the PC if possible (avoid flaky hubs).

### If upload still fails

1. **Manual boot mode**
   - Hold **BOOT**
   - Tap **RESET**
   - Release **BOOT**
   - Click **Upload** immediately

2. **Lower upload speed** to `115200` (or `256000` on Windows).

3. **Erase flash once** — set *Erase All Flash Before Sketch Upload* to **Enabled**, upload once, then set back to Disabled.

4. **Install USB driver** — Espressif devices use native USB; install [CP210x](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) only if your board uses an external UART chip instead.

### After upload

1. Open **Serial Monitor** at **115200 baud**.
2. Wait ~1.5 s for USB CDC to connect (boot banner should appear).
3. Confirm `[SVC ] OLED ready` in the log.

---

## Current Button Behaviour

- Short press: cycle OLED / serial status screens (status, network, system)
- Long press: toggle test infected / safe state

## Current OLED Screens

1. **Status** — player ID, state, HP, lives
2. **Network** — known nodes, ESP-NOW link
3. **System** — firmware version, heap, node count

Event alerts flash on the OLED for ~1.8 seconds.

## Serial Monitor (full detail)

At **115200 baud** the player prints structured diagnostics:

- Boot: hardware pin map, radio/MAC, service list
- Live: player state, known nodes (online/stale/age), heap, event queue
- Input: button press/release with duration and action
- Network: HELLO heartbeats, packet TX/RX (DEBUG), node discovery
- Periodic full dashboard every **15 s** (`PLAYER_SERIAL_TICK_MS` in `config.h`)

Set `PLAYER_SERIAL_FULL_DETAIL false` for compact status only.  
Set `PLAYER_SERIAL_TICK_MS 0` to disable periodic dumps.

## Current LED Behaviour

**GPIO8 — Blue heartbeat LED**
- Short blink (~140 ms) on every HELLO heartbeat TX (every 5 s)
- Also blinks when a new node is discovered on ESP-NOW
- Off between pulses

**GPIO7 / GPIO3 — Status RGB (red + blue channels)**
- Blue: alive / safe
- Red + blue: infected
- Red: eliminated

## Arduino CLI (optional)

From this folder:

```bash
arduino-cli compile --profile player-node
arduino-cli upload   --profile player-node -p COM18
```

Replace `COM18` with your port. Close Serial Monitor first.

## Sketch layout

Open `player-node.ino` in Arduino IDE.

Keep `firmware/shared` beside `player-node` so the relative `#include "../shared/..."` paths work.

## Troubleshooting OLED

If serial shows `OLED ready` but the screen is blank, set `OLED_DRIVER_SH1106` to `true` in `config.h` and re-upload.

## Next Firmware Step

Add objective text and team colour to the OLED status screens.
