# PROGMEM WebUI Build Plan v0.1

## Goal

Convert the HUNT Command Center web files into C/C++ header files that can be served directly from ESP32-S3 flash memory.

## Source Folder

```text
web/command-center/
```

## Generated Output

```text
firmware/shared/web/
├── index_html.h
├── main_css.h
├── app_js.h
└── manifest_json.h
```

## Build Tool

A future Python tool will read each web file, optionally minify it, and generate PROGMEM C string headers.

Planned script:

```text
tools/build_webui.py
```

## Firmware Serving

The Base Node will use an ESP32 web server to serve:

- `/` → index.html
- `/css/main.css` → main.css
- `/js/app.js` → app.js
- `/manifest.json` → manifest.json
- `/api/system` → JSON system status
- `/api/nodes` → JSON node list
- `/api/players` → JSON player list
- `/api/command` → action endpoint

## WebSocket

Future live updates should use WebSocket.

Examples:

- Player state changed
- Node discovered
- Relay triggered
- Scene loaded
- Emergency stop activated

## Rule

The WebUI must work offline.

No CDN.
No remote fonts.
No external JavaScript.
No external CSS.
