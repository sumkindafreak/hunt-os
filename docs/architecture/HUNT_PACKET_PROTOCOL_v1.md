# HUNT Packet Protocol v1

## Purpose

The HUNT Packet Protocol defines the standard message format used between HUNT devices.

The aim is to keep Player Nodes, Base Nodes, Relay Nodes, Audio Nodes and future devices speaking the same language.

---

## Current Alpha Format

Alpha currently uses a readable pipe-separated packet format.

Example:

```text
HUNT|0.1|EVENT|BASE_01|PLAYER_01|EVENT:PLAYER_INFECTED;DATA:0
```

---

## Required Packet Fields

Every packet should contain:

| Field | Purpose |
|---|---|
| Protocol name | Confirms this is a HUNT packet |
| Version | Protocol version |
| Packet type | HELLO, HEARTBEAT, EVENT, COMMAND, ACK, ERROR |
| Source ID | Sending device |
| Target ID | Destination device or ALL |
| Payload | Packet-specific data |

---

## Packet Types

### HELLO

Announces a device.

Payload example:

```text
TYPE:PLAYER;FW:Player v0.1;STATE:ALIVE
```

### HEARTBEAT

Keeps a device visible as online.

Payload example:

```text
STATE:ALIVE;BAT:87
```

### EVENT

Reports or triggers a game event.

Payload example:

```text
EVENT:PLAYER_INFECTED;DATA:0
```

### COMMAND

Orders a device to do something.

Payload example:

```text
RELAY:1:ON
```

### ACK

Confirms a packet was received.

Payload example:

```text
EVENT_RECEIVED
```

### ERROR

Reports a failure.

Payload example:

```text
CODE:NO_TARGET;MSG:Player not found
```

---

## Future Packet Fields

Later versions may add:

- Timestamp
- Sequence number
- Checksum
- Message priority
- Capability data
- Battery level
- Signal estimate
- Encryption/authentication

---

## Design Rule

Do not invent one-off packet formats for new features.

Add new packet types or payload keys to this protocol instead.
