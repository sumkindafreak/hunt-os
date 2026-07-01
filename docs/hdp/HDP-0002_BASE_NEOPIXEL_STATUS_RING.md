# HDP-0002: Base Unit NeoPixel Status Ring

**Status:** Approved for prototype  
**Date:** 2026-06-30  
**Author:** Toby Brandon  
**Review:** AI Design Advisor

---

## 1. Summary

The HUNT Base unit will use an addressable NeoPixel ring or strip for role and status feedback instead of a basic RGB ring.

---

## 2. Problem

A simple RGB ring can only show one colour at a time and limits the amount of feedback the Base can provide.

The Base unit is intended to become many different game objects, so it needs richer visual feedback for roles, timers, activity, warnings and game events.

---

## 3. Goals

- Show Base role clearly from a distance.
- Support animations and progress effects.
- Allow countdowns, scans and activation feedback.
- Keep hardware simple with one data pin.
- Make the Base feel more like a finished product.

---

## 4. Proposed Decision

Use WS2812 / NeoPixel-compatible addressable LEDs for the Base status ring.

Prototype default:

- 12 pixels
- 1 data pin
- 5V supply preferred
- 330R resistor on data line recommended
- Large capacitor across 5V and GND recommended for longer strips or bright effects

---

## 5. Default Role Colours

- Safe Zone: Green
- Scanner / Infection: Purple
- Objective: Blue
- Extraction: Yellow
- Error: Red
- Idle / Setup: White dim
- Communication activity: Cyan pulse

---

## 6. Planned Effects

v0.1 effects:

- Solid role colour
- Slow breathing role colour
- Flash on activation
- Spinner while waiting
- Red error flash

Future effects:

- Countdown progress ring
- Capture progress
- Scanner sweep
- Player nearby pulse
- Objective complete celebration

---

## 7. Consequences

### Positive

- More expressive than basic RGB.
- One data pin controls many LEDs.
- Enables polished UI feedback.
- Supports future gameplay mechanics.

### Trade-offs

- Requires Adafruit NeoPixel or compatible library.
- Needs more current than a single RGB LED.
- Requires careful brightness limits for battery use.

---

## 8. Implementation Notes

The Base firmware should use a dedicated NeoPixel manager rather than scattering LED code through the main sketch.

Initial manager name:

```text
HuntNeoPixels
```

The Base config should define:

```cpp
#define BASE_NEOPIXEL_PIN 9
#define BASE_NEOPIXEL_COUNT 12
#define BASE_NEOPIXEL_BRIGHTNESS 40
```

Pins remain prototype defaults until final hardware is confirmed.

---

## 9. Final Decision

Approved.

The Base unit will move from an RGB ring concept to a NeoPixel status ring.
