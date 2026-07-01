# Milestone Alpha

**Goal:** Run a complete minimum HUNT game using one Base Node and one Player Node.

## Alpha Definition

Milestone Alpha is complete when HUNT OS can:

- Boot one Base Node
- Boot one Player Node
- Discover the Player from the Base
- Show the Player in Command Center
- Start a game
- End a game
- Change Base scene
- Send an event from Base to Player
- Update Player state
- Update Base lighting
- Log events

## Alpha Systems

- HUNT Kernel
- Service Manager
- Event Bus
- Node Registry
- Network Service
- Player Service
- Scene Service
- Lighting Service
- Audio Service
- Web Service
- Logger

## Alpha Non-Goals

These are not required for Alpha:

- Full scenario package loading
- OTA updates
- Full HUNT Studio
- Marketplace
- Complex mission editor
- Multiple Relay Nodes
- Multiple Base Nodes

## Success Test

1. Power on Base.
2. Power on Player.
3. Base discovers Player.
4. Open Command Center on phone.
5. Player appears online.
6. Press Start Game.
7. Trigger Safe Zone from Base.
8. Player receives event.
9. Player LED / buzzer / state changes.
10. Event log updates.
