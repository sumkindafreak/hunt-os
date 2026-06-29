# HUNT Communication Protocol

**Version:** 0.1 Genesis  
**Date:** 2026-06-29  
**Status:** Draft

---

## Purpose

This document defines the first communication direction for HUNT devices.

The goal is to allow Player Nodes and Base Nodes to exchange simple, reliable game messages.

---

## Design Goals

The protocol should be:

- Simple
- Human-readable during debugging
- Small enough for ESP32 devices
- Reliable enough for live games
- Expandable for future features
- Independent from any single game mode

---

## v0.1 Communication Model

The first prototype only needs:

- Player Node to Base Node messages
- Base Node to Player Node messages
- Basic discovery / pairing
- Basic event transfer
- Basic acknowledgement

Cloud communication is out of scope for v0.1.

---

## Candidate Transports

### ESP-NOW

Pros:

- Fast local device-to-device communication
- No router required
- Good fit for outdoor games

Cons:

- Requires careful peer management
- Payload size limits
- Debugging can be harder than UDP

### Wi-Fi UDP

Pros:

- Easy to debug
- Human-readable packets possible
- Works well with a local access point

Cons:

- Requires network setup or AP mode
- May be less direct than ESP-NOW

### BLE

Pros:

- Good for phones and short-range interaction
- Useful for future app integration

Cons:

- More complexity
- May not be ideal for fast multi-device game messaging

---

## Recommendation

For early prototyping, test both:

1. ESP-NOW for direct game events
2. Wi-Fi UDP for debugging and possible admin tools

Do not commit to the final transport until real-world testing is complete.

---

## Message Format

The first message format should be simple text.

Recommended format:

```text
HUNT|VERSION|TYPE|SOURCE|TARGET|PAYLOAD
```

Example:

```text
HUNT|0.1|EVENT|BASE_01|PLAYER_04|PLAYER_HEALED:25
```

---

## Message Fields

### Prefix

Always:

```text
HUNT
```

### Version

Protocol version.

Example:

```text
0.1
```

### Type

Message category.

Examples:

- HELLO
- ACK
- EVENT
- STATE
- COMMAND
- ERROR
- HEARTBEAT

### Source

Device sending the message.

Examples:

- PLAYER_01
- BASE_03
- ADMIN_01

### Target

Device intended to receive the message.

Examples:

- PLAYER_01
- BASE_03
- ALL
- TEAM_BLUE

### Payload

Command, event or data.

Examples:

- PLAYER_HEALED:25
- PLAYER_INFECTED
- SET_LED:GREEN
- GAME_START

---

## Core Message Types

### HELLO

Used when a device announces itself.

Example:

```text
HUNT|0.1|HELLO|PLAYER_04|ALL|ROLE:PLAYER;NAME:Toby
```

### HEARTBEAT

Used to show a device is alive.

Example:

```text
HUNT|0.1|HEARTBEAT|BASE_01|ALL|ROLE:SAFE_ZONE;STATE:READY
```

### EVENT

Used for game events.

Example:

```text
HUNT|0.1|EVENT|BASE_01|PLAYER_04|PLAYER_HEALED:25
```

### COMMAND

Used to instruct a device.

Example:

```text
HUNT|0.1|COMMAND|ADMIN_01|BASE_01|SET_ROLE:SAFE_ZONE
```

### STATE

Used to report current state.

Example:

```text
HUNT|0.1|STATE|PLAYER_04|BASE_01|HP:75;STATUS:ALIVE;TEAM:BLUE
```

### ACK

Used to confirm message receipt.

Example:

```text
HUNT|0.1|ACK|PLAYER_04|BASE_01|MSG:1234
```

### ERROR

Used to report problems.

Example:

```text
HUNT|0.1|ERROR|BASE_01|ADMIN_01|LOW_BATTERY
```

---

## Device IDs

Temporary v0.1 IDs may be manually assigned.

Examples:

- PLAYER_01
- PLAYER_02
- BASE_01
- BASE_02

Future versions should support persistent IDs stored in device memory.

---

## Basic Event List

Initial supported events:

- GAME_START
- GAME_END
- PLAYER_JOINED
- PLAYER_HEALED
- PLAYER_INFECTED
- PLAYER_ELIMINATED
- PLAYER_SAFE
- BASE_ACTIVATED
- OBJECTIVE_COMPLETE
- EXTRACTION_OPEN

---

## Reliability Notes

Live games cannot rely on perfect communication.

Firmware should assume messages may be missed.

Recommended strategies:

- Repeat important broadcasts
- Use acknowledgement for critical events
- Use heartbeats
- Fail safely if connection is lost
- Display communication status on devices

---

## Security Notes

v0.1 security is basic and should focus on preventing accidental interference.

Future versions should consider:

- Pairing keys
- Scenario IDs
- Message checksums
- Signed scenario files
- Admin lockout
- Anti-cheat handling

---

## Out of Scope for v0.1

- Cloud sync
- User accounts
- Marketplace downloads
- Payment systems
- Encrypted multiplayer infrastructure
- Mobile app integration

---

## Next Steps

1. Build a simple parser for the text format.
2. Send HELLO messages between Player and Base.
3. Send one EVENT message from Base to Player.
4. Confirm ACK response.
5. Test range and reliability.
6. Decide whether ESP-NOW or UDP is better for v1.
