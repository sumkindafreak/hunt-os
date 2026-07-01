# HUNT Alpha Build and Test Checklist

## Purpose

This checklist confirms whether HUNT Alpha is ready for real hardware testing.

Alpha target:

- 1 Base Node
- 1 Player Node
- ESP-NOW communication
- Command Center WebUI
- Scene/role event flow

---

## Required Arduino Libraries

Install these before compiling:

- Arduino core for ESP32
- Adafruit NeoPixel

Future libraries likely needed:

- OLED display library
- DFPlayer library

---

## Base Node Upload Test

Open:

```text
firmware/base-node/base-node.ino
```

Expected serial output:

- HUNT OS boot banner
- Service registration messages
- Network service start
- Web service start
- AP IP address
- Base Alpha ready

Expected Wi-Fi:

```text
SSID: HUNT_BASE_01
Password: huntbase01
URL: http://192.168.4.1
```

Expected hardware:

- Status NeoPixels show Safe Zone colour
- Decor NeoPixels show Safe Zone colour
- Status LED blinks
- Encoder changes role
- Action button sends event to PLAYER_01

---

## Player Node Upload Test

Open:

```text
firmware/player-node/player-node.ino
```

Expected serial output:

- HUNT OS boot banner
- Service registration messages
- Network service start
- Player Alpha ready

Expected hardware:

- RGB LED shows player state
- Buzzer beeps on boot
- Short button press changes screen/debug output
- Long press toggles infected/safe test state

---

## ESP-NOW Alpha Test

1. Power Base Node.
2. Power Player Node.
3. Wait for HELLO packets.
4. Base should discover Player.
5. Player should discover Base.
6. Press Base action button.
7. Player should receive event.
8. Player should change state/LED/buzzer.
9. Serial logs should show event flow.

---

## WebUI Alpha Test

1. Connect phone to `HUNT_BASE_01`.
2. Open `http://192.168.4.1`.
3. Confirm HUNT Command Center loads.
4. Press Start / Pause / End buttons.
5. Confirm API responses in page log.

---

## Known Alpha Limitations

- OLED rendering is not yet implemented.
- DFPlayer real serial control is not yet implemented.
- Command Center API routes are starter/demo routes.
- WebUI does not yet show real node registry data.
- ESP-NOW has not yet been physically range-tested.
- Pin map must be verified on real hardware.

---

## Alpha Complete When

- Both sketches compile.
- Both boards boot.
- Base WebUI loads from phone.
- Base and Player discover each other.
- Base sends event.
- Player reacts.
- Serial logs confirm the full path.
