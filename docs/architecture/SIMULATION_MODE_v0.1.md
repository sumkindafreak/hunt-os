# HUNT Simulation Mode v0.1

## Purpose

Simulation Mode allows HUNT game logic to be tested without every physical device being powered on.

It is intended to speed up development, scenario design and debugging.

---

## Goals

Simulation Mode should allow the Base/Command Center to create fake devices such as:

- Player Nodes
- Relay Nodes
- Audio Nodes
- Sensor Nodes
- Lighting Nodes

These simulated nodes should appear in the Node Registry like real devices.

---

## Use Cases

- Test WebUI before hardware is finished
- Test game rules with 8 fake players
- Test infection flow
- Test safe zone logic
- Test extraction events
- Test relay/audio/lighting actions
- Demo HUNT without full hardware kit

---

## Example Simulated Game

```text
SIM: create PLAYER-001
SIM: create PLAYER-002
SIM: create PLAYER-003
SIM: start game
SIM: infect PLAYER-002
SIM: activate SAFE_ZONE
SIM: open EXTRACTION
```

---

## Command Center Controls

Simulation Mode should eventually include:

- Add fake player
- Remove fake player
- Infect fake player
- Heal fake player
- Eliminate fake player
- Simulate relay node
- Simulate audio node
- Simulate sensor trigger
- Clear simulation

---

## Firmware Design

Simulation should be implemented as a service:

```text
HuntSimulationService
```

It should publish normal HUNT events into the Event Bus so the rest of the system does not care whether the event came from real hardware or simulation.

---

## Design Rule

Simulated nodes must use the same event and registry path as real nodes.

No duplicate game logic should exist just for simulation.
