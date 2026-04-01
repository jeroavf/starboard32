# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

LCARS/Starboard32 interface for the TTGO T-Display (ESP32 + ST7789 135x240px TFT, portrait mode). Displays real ESP32 telemetry (heap, WiFi RSSI, temperature, uptime, CPU freq) themed as Star Trek TNG ship systems across 4 navigable screens.

## Build & Upload

```bash
# Compile
arduino-cli compile --fqbn esp32:esp32:esp32 okudagram/

# Upload (check /dev/ttyACM0 or actual port)
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 okudagram/

# Serial monitor
arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
```

**Toolchain**: Arduino CLI with ESP32 core 3.3.6, TFT_eSPI 2.5.43, WiFiManager 2.0.17.

## Architecture

```
okudagram.ino          Main loop: buttons → data update → draw frame → draw screen → pushSprite
  ├── lcars_frame.cpp  Shared LCARS chrome (header, elbow, sidebar, nav buttons, utilities)
  ├── screen_eng.cpp   ENG: heap memory → "Warp Core", CPU freq → "Power Output", PSRAM → "Antimatter"
  ├── screen_tac.cpp   TAC: WiFi RSSI → "Shields", WiFi status → "Comm", RSSI waveform → "Signal Analysis"
  ├── screen_ops.cpp   OPS: uptime → "Ship Status", NTP → "Stardate", system alerts
  ├── screen_sci.cpp   SCI: temperature → "Core Temp", scan sweep animation, temp waveform
  ├── data_source.cpp  Collects all ESP32 metrics into SystemData struct (every 2s)
  ├── buttons.cpp      Debounced GPIO input: RIGHT=next, LEFT=prev, LEFT long=WiFi reset
  ├── wifi_setup.cpp   WiFiManager captive portal ("Starboard32-Config")
  └── ntp_sync.cpp     NTP time sync (pool.ntp.org, UTC-3, 12h interval)
```

**Rendering**: Full-screen `TFT_eSprite` (135x240, RGB565) double-buffer. All drawing goes to the sprite; `pushSprite(0,0)` transfers the complete frame at 20 FPS. This prevents flicker.

**Navigation**: 4 screens (ENG/TAC/OPS/SCI) cycled via 2 physical buttons (GPIO 35 right, GPIO 0 left). Long-press GPIO 0 (3s) resets WiFi credentials.

## Key Design Decisions

- **All config in `config.h`** as `#define` constants (layout zones, timings, pins, NTP settings).
- **Color palette in `colors.h`** as RGB565 values matching the HTML mockup's CSS palette.
- **`lcars_frame.cpp` provides drawing utilities** (`lcarsDrawGauge`, `lcarsDrawStatusDot`, `lcarsDrawDataRow`, `lcarsDrawSectionLabel`) — screens use these, not raw sprite calls, except for custom elements (waveforms, scan animations) which access the sprite via `lcarsGetSprite()`.
- **Screen functions receive `(int startY, const SystemData& data)`** — `startY` is returned by `lcarsFrameDraw()` after rendering the shared chrome.
- **Waveform buffers** (RSSI in screen_tac, temperature in screen_sci) use circular arrays of 40 samples with modulo indexing.
- **`hallRead()` is disabled** — removed in ESP32 core 3.x; `hallValue` is always 0.

## Hardware

- **Board**: TTGO T-Display v1 (ESP32-D0WDQ6)
- **Display**: ST7789 135x240, portrait (`setRotation(0)`)
- **Buttons**: GPIO 35 (input-only, external pullup), GPIO 0 (BOOT, internal pullup)
- **Backlight**: PWM on TFT_BL pin (200 day / 60 night, auto-switch 22:00-07:00)

## Visual Reference

`demo/index.html` is a browser mockup showing the LCARS layout at 1:1 and 3x scale with CSS animations. Use it as the visual spec when modifying the interface.
