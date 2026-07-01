# Alpha Build and Test Checklist

## Goal

Prepare HUNT OS for the first real Base + Player bench test.

## Firmware Modules Present

- HuntKernel
- HuntService
- HuntServiceManager
- HuntEventBus
- HuntNodeRegistry
- HuntNetworkService
- HuntPlayerService
- HuntSceneService
- HuntLightingService
- HuntSoundService
- HuntWebServer
- HuntWebService

## Required Arduino Libraries

Install these before compiling:

- ESP32 Arduino core
- Adafruit NeoPixel

Likely later:

- OLED display library
- DFPlayer library

## Base Node Alpha Test

1. Open `firmware/base-node/base-node.ino`.
2. Select ESP32-S3 board.
3. Install Adafruit NeoPixel library.
4. Compile.
5. Upload.
6. Open Serial Monitor at 115200.
7. Confirm boot messages.
8. Confirm NeoPixels initialise.
9. Confirm ESP-NOW starts.
10. Connect phone to Base Wi-Fi when Web Service is enabled.
11. Open `192.168.4.1`.

## Player Node Alpha Test

1. Open `firmware/player-node/player-node.ino`.
2. Select ESP32-C3 board.
3. Compile.
4. Upload.
5. Open Serial Monitor at 115200.
6. Confirm boot messages.
7. Confirm Player sends HELLO.
8. Confirm Base receives Player.

## Alpha Success Criteria

Alpha is successful when:

- Base boots.
- Player boots.
- Both start ESP-NOW.
- Player broadcasts HELLO.
- Base receives Player packet.
- Base can send event to Player.
- Player changes state.
- Base Command Center loads from phone.
- Serial logs show complete flow.

## Known Not-Yet-Hardware-Tested Areas

- OLED display driver
- DFPlayer wiring and library
- Exact ESP32-S3 pin behaviour
- Web server running alongside ESP-NOW under real load
- NeoPixel current draw

## Rule

After this checklist, the next phase is not more architecture.

The next phase is compile, upload, wiring fixes and real bench testing.
