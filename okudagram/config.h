#pragma once

// ── Display ─────────────────────────────────────────────
#define SCREEN_WIDTH   135
#define SCREEN_HEIGHT  240
#define TFT_ROTATION   0    // portrait

// ── Layout zones ────────────────────────────────────────
#define HEADER_H       16
#define SIDEBAR_W      18
#define CONTENT_X      20
#define CONTENT_Y      18
#define CONTENT_W      113
#define NAV_Y          227
#define NAV_H          10
#define ELBOW_H        22

// ── Brilho do backlight (PWM 0-255) ─────────────────────
#define BRIGHT_DAY     200
#define BRIGHT_NIGHT   60
#define NIGHT_START    22
#define NIGHT_END      7

// ── NTP ─────────────────────────────────────────────────
#define NTP_SERVER            "pool.ntp.org"
#define TZ_OFFSET             (-3)
#define NTP_SYNC_INTERVAL_MS  (12UL * 3600000UL)

// ── WiFi ────────────────────────────────────────────────
#define WIFI_AP_NAME    "Okudagram-Config"
#define WIFI_TIMEOUT_S  180

// ── Botoes ──────────────────────────────────────────────
#define BTN_RIGHT          35
#define BTN_LEFT           0
#define BTN_DEBOUNCE_MS    50UL
#define BTN_LONG_MS        2000UL
#define BTN_WIFI_RESET_MS  3000UL

// ── Timings ─────────────────────────────────────────────
#define FRAME_INTERVAL_MS    50UL    // 20 FPS
#define DATA_UPDATE_MS       2000UL  // dados a cada 2s
#define WAVEFORM_UPDATE_MS   200UL   // waveform a cada 200ms
#define PULSE_TOGGLE_MS      750UL   // pulse dots

// ── Waveform ────────────────────────────────────────────
#define WAVEFORM_SAMPLES     40

// ── Serial ──────────────────────────────────────────────
#define SERIAL_BAUD  115200
