# HDP-0001: Communications Strategy for HUNT v0.1

**Status:** Approved for prototype  
**Date:** 2026-06-29  
**Author:** Toby Brandon  
**Review:** AI Design Advisor

---

## 1. Summary

HUNT v0.1 will prototype local device communication using ESP-NOW as the primary game-event transport, with serial debug output on every device.

Wi-Fi UDP remains a secondary test option for later admin tools and debugging, but the first playable firmware should focus on ESP-NOW.

---

## 2. Problem

HUNT devices must communicate without relying on internet access or a venue router.

The first prototype needs one Player Node and one Base Node to exchange game messages reliably.

---

## 3. Goals

- Work offline.
- Avoid router dependency.
- Keep latency low.
- Keep setup simple.
- Support small game-event messages.
- Allow Player Nodes and Base Nodes to talk directly.

---

## 4. Non-Goals

- Cloud sync.
- Mobile app control.
- Marketplace downloads.
- Long-range outdoor radio.
- Full encryption strategy.
- Final commercial protocol lock-in.

---

## 5. Proposed Decision

Use ESP-NOW for v0.1 direct game events.

All devices will print detailed serial debug logs during development.

Message format will remain based on the HUNT text packet:

```text
HUNT|0.1|TYPE|SOURCE|TARGET|PAYLOAD
```

Example:

```text
HUNT|0.1|EVENT|BASE_01|PLAYER_01|HEAL:25
```

---

## 6. Alternatives Considered

### Wi-Fi UDP

Pros:

- Easy to debug.
- Human-readable.
- Good for future admin panels.

Cons:

- Usually needs an access point or AP mode.
- Adds setup friction.
- Less ideal for instant local game interactions.

### BLE

Pros:

- Useful for phones later.
- Short-range pairing options.

Cons:

- More complexity.
- Not ideal as the main game-event network for many devices.

### ESP-NOW

Pros:

- Direct ESP32-to-ESP32 communication.
- No router required.
- Good for offline games.
- Fast enough for HUNT v0.1.

Cons:

- Peer management needs careful design.
- Payloads must stay small.
- Debugging is less convenient than UDP.

---

## 7. Reasoning

HUNT must work in parks, forests, halls, scare attractions and temporary venues.

A network that depends on internet, router access or complicated setup would weaken the first user experience.

ESP-NOW best supports the first prototype goal:

> Power on a Player Node and Base Node, then have them interact locally.

---

## 8. Consequences

### Positive

- Offline-first foundation.
- Low setup friction.
- Good fit for battery-powered devices.
- Strong prototype path.

### Trade-offs

- Need to test range and reliability early.
- Need a pairing or broadcast strategy.
- May need another transport later for admin tools.

---

## 9. Implementation Notes

v0.1 firmware should support:

- Broadcast HELLO packets.
- Simple device IDs.
- Event messages.
- ACK messages for important events.
- HEARTBEAT messages.
- Serial debug for every sent and received packet.

---

## 10. Testing Notes

Test with:

- One Player Node and one Base Node on desk.
- Same room range.
- Different room range.
- Outdoor line-of-sight range.
- Body obstruction.
- Repeated events.
- Missed ACK behaviour.

---

## 11. Final Decision

Approved for HUNT v0.1 prototype.

ESP-NOW will be used first.

Wi-Fi UDP will remain a future secondary channel, not the first firmware target.
