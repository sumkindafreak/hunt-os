# HUNT Kernel v0.1

## Purpose

The HUNT Kernel is the central update loop for HUNT OS.

Instead of each sketch manually updating every subsystem, each subsystem becomes a service registered with the kernel.

## Target Loop

```cpp
void loop() {
  huntKernel.update();
}
```

## Core Responsibilities

- Start registered services
- Update services every loop
- Provide a shared event bus
- Provide a shared node registry
- Keep firmware structure consistent across node types

## Alpha Services

- Network Service
- Player Service
- Scene Service
- Lighting Service
- Audio Service
- Web Service
- Logger Service

## Design Rule

Features should not be hard-coded directly into the main loop.

New systems should become services or use the event bus.
