# HDP-0003: Base Unit Decor / Expansion NeoPixel Output

**Status:** Approved for prototype  
**Date:** 2026-06-30  
**Author:** Toby Brandon  
**Review:** AI Design Advisor

---

## 1. Summary

The HUNT Base unit will include a second NeoPixel output dedicated to decor, prop lighting and expansion effects.

This output is separate from the main Base NeoPixel status ring.

---

## 2. Reason

The Base status ring should remain dedicated to user interface feedback such as role colour, activation state, errors and communication status.

Decor lighting may need to behave differently depending on the game scenario, prop or environment.

Separating the two outputs prevents decorative effects from hiding important Base status information.

---

## 3. Outputs

### Status NeoPixels

Used for Base UI.

Examples:

- Role colour
- Setup state
- Activation flash
- Error state
- Communication feedback

### Decor NeoPixels

Used for atmosphere and prop effects.

Examples:

- Safe zone glow
- Infection pulse
- Generator flicker
- Extraction beacon
- Treasure chest shimmer
- Puzzle progress lighting
- Scenario-specific effects

---

## 4. Prototype Defaults

```cpp
#define BASE_NEOPIXEL_PIN 9
#define BASE_NEOPIXEL_COUNT 12
#define BASE_NEOPIXEL_BRIGHTNESS 40

#define DECOR_NEOPIXEL_PIN 10
#define DECOR_NEOPIXEL_COUNT 30
#define DECOR_NEOPIXEL_BRIGHTNESS 60
```

Pins and counts remain prototype defaults until final hardware is confirmed.

---

## 5. Design Rule

The status output communicates what the Base is.

The decor output communicates what the world feels like.

Both should be controlled separately.

---

## 6. Final Decision

Approved.

The Base unit will support two independent NeoPixel outputs:

1. Status ring
2. Decor / expansion strip
