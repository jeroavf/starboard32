#include "screen_tac.h"
#include "lcars_frame.h"
#include "config.h"
#include "colors.h"
#include <stdio.h>

static int8_t rssiHistory[WAVEFORM_SAMPLES];
static int    historyIdx = 0;
static bool   historyFull = false;

void screenTacInit() {
    memset(rssiHistory, -100, sizeof(rssiHistory));
    historyIdx = 0;
    historyFull = false;
}

void screenTacAddSample(int8_t rssi) {
    rssiHistory[historyIdx] = rssi;
    historyIdx = (historyIdx + 1) % WAVEFORM_SAMPLES;
    if (historyIdx == 0) historyFull = true;
}

// Map RSSI (-90..-30) to 0-100%
static int rssiToPercent(int8_t rssi) {
    int pct = (rssi + 90) * 100 / 60;
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    return pct;
}

static void drawWaveform(int x, int y, int w, int h) {
    TFT_eSprite* spr = lcarsGetSprite();
    int barW = 2;
    int gap  = 1;
    int count = w / (barW + gap);
    if (count > WAVEFORM_SAMPLES) count = WAVEFORM_SAMPLES;

    for (int i = 0; i < count; i++) {
        int idx = (historyIdx - count + i + WAVEFORM_SAMPLES) % WAVEFORM_SAMPLES;
        int pct = rssiToPercent(rssiHistory[idx]);
        int barH = h * pct / 100;
        if (barH < 1) barH = 1;
        int bx = x + i * (barW + gap);
        int by = y + h - barH;
        uint16_t color = LCARS_BLUE;
        if (pct < 30) color = LCARS_RED;
        else if (pct < 60) color = LCARS_YELLOW;
        spr->fillRect(bx, by, barW, barH, color);
    }
}

void screenTacDraw(int startY, const SystemData& data) {
    // Add new RSSI sample
    static uint32_t lastSampleMs = 0;
    if (millis() - lastSampleMs >= WAVEFORM_UPDATE_MS) {
        screenTacAddSample(data.rssi);
        lastSampleMs = millis();
    }

    int y = startY;
    char buf[24];

    // ── Shields (WiFi RSSI) ──
    lcarsDrawSectionLabel(y, "SHIELDS");
    y += 12;

    int shieldPct = rssiToPercent(data.rssi);
    uint16_t dotColor = LCARS_GREEN;
    if (shieldPct < 30) dotColor = LCARS_RED;
    else if (shieldPct < 60) dotColor = LCARS_YELLOW;

    lcarsDrawStatusDot(CONTENT_X + 4, y + 3, dotColor);
    snprintf(buf, sizeof(buf), "%d%%", shieldPct);
    lcarsDrawDataRow(y, "  INTEGRITY", buf, LCARS_LT_BLUE, LCARS_LT_BLUE);
    y += 11;

    lcarsDrawGauge(CONTENT_X + 2, y, CONTENT_W - 4, shieldPct / 100.0f, LCARS_BLUE);
    y += 7;

    snprintf(buf, sizeof(buf), "%d dBm", data.rssi);
    lcarsDrawDataRow(y, "SIGNAL", buf, LCARS_LT_BLUE, LCARS_ORANGE);
    y += 13;

    // ── Comm Status (WiFi) ──
    lcarsDrawSectionLabel(y, "COMM STATUS");
    y += 12;

    lcarsDrawStatusDot(CONTENT_X + 4, y + 3,
                       data.wifiConnected ? LCARS_GREEN : LCARS_RED);
    lcarsDrawDataRow(y, "  LINK", data.wifiConnected ? "ACTIVE" : "OFFLINE",
                     LCARS_LT_BLUE, data.wifiConnected ? LCARS_ORANGE : LCARS_RED);
    y += 11;

    lcarsDrawDataRow(y, "SSID", data.ssid, LCARS_LT_BLUE, LCARS_LT_ORANGE);
    y += 11;

    lcarsDrawDataRow(y, "IP", data.ip, LCARS_LT_BLUE, LCARS_TAN);
    y += 13;

    // ── Signal Analysis (Waveform) ──
    lcarsDrawSectionLabel(y, "SIGNAL ANALYSIS");
    y += 12;

    drawWaveform(CONTENT_X + 2, y, CONTENT_W - 4, 20);
}
