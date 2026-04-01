#include "screen_sci.h"
#include "lcars_frame.h"
#include "config.h"
#include "colors.h"
#include <stdio.h>

static uint32_t scanLineFrame = 0;
static float    tempHistory[WAVEFORM_SAMPLES];
static int      tempIdx = 0;

static void drawScanSweep(int x, int y, int w, int h) {
    TFT_eSprite* spr = lcarsGetSprite();
    scanLineFrame++;
    int period = 60;  // ~3s at 20fps
    int lineY = y + (scanLineFrame % period) * h / period;

    for (int row = y; row < y + h; row++) {
        int dist = abs(row - lineY);
        if (dist < 4) {
            for (int col = x; col < x + w; col++) {
                if ((col + row) % 3 == 0) {
                    spr->drawPixel(col, row, LCARS_BLUE);
                }
            }
        }
    }
    spr->drawFastHLine(x, lineY, w, LCARS_LT_BLUE);
}

static void drawTempWaveform(int x, int y, int w, int h) {
    TFT_eSprite* spr = lcarsGetSprite();
    int barW = 2;
    int gap  = 1;
    int count = w / (barW + gap);
    if (count > WAVEFORM_SAMPLES) count = WAVEFORM_SAMPLES;

    for (int i = 0; i < count; i++) {
        int idx = (tempIdx - count + i + WAVEFORM_SAMPLES) % WAVEFORM_SAMPLES;
        float t = tempHistory[idx];
        int pct = (int)((t - 20.0f) / 60.0f * 100.0f);
        if (pct < 0) pct = 0;
        if (pct > 100) pct = 100;
        int barH = h * pct / 100;
        if (barH < 1) barH = 1;
        int bx = x + i * (barW + gap);
        int by = y + h - barH;
        spr->fillRect(bx, by, barW, barH, LCARS_LAVENDER);
    }
}

void screenSciDraw(int startY, const SystemData& data) {
    // Add temperature sample periodically
    static uint32_t lastTempMs = 0;
    if (millis() - lastTempMs >= WAVEFORM_UPDATE_MS) {
        tempHistory[tempIdx] = data.tempC;
        tempIdx = (tempIdx + 1) % WAVEFORM_SAMPLES;
        lastTempMs = millis();
    }

    int y = startY;
    char buf[24];

    // ── Temperature ──
    lcarsDrawSectionLabel(y, "TEMPERATURE");
    y += 12;

    snprintf(buf, sizeof(buf), "%.1f C", data.tempC);
    lcarsDrawDataRow(y, "CORE TEMP", buf, LCARS_LT_BLUE, LCARS_ORANGE);
    y += 11;

    float tempRatio = (data.tempC - 20.0f) / 60.0f;
    if (tempRatio < 0) tempRatio = 0;
    if (tempRatio > 1) tempRatio = 1;
    uint16_t tempColor = LCARS_LAVENDER;
    if (tempRatio > 0.7f) tempColor = LCARS_RED;
    else if (tempRatio > 0.5f) tempColor = LCARS_YELLOW;
    lcarsDrawGauge(CONTENT_X + 2, y, CONTENT_W - 4, tempRatio, tempColor);
    y += 13;

    // ── Magnetic Field (Hall sensor) ──
    lcarsDrawSectionLabel(y, "MAGNETIC FIELD");
    y += 12;

    snprintf(buf, sizeof(buf), "%d uT", data.hallValue);
    lcarsDrawDataRow(y, "HALL", buf, LCARS_LT_BLUE, LCARS_LT_BLUE);
    y += 11;

    // Map hall value to gauge (-100..100 -> 0..1)
    float hallRatio = (data.hallValue + 100.0f) / 200.0f;
    if (hallRatio < 0) hallRatio = 0;
    if (hallRatio > 1) hallRatio = 1;
    lcarsDrawGauge(CONTENT_X + 2, y, CONTENT_W - 4, hallRatio, LCARS_PEACH);
    y += 13;

    // ── Sensor Sweep ──
    lcarsDrawSectionLabel(y, "SENSOR SWEEP");
    y += 12;

    drawScanSweep(CONTENT_X + 2, y, CONTENT_W - 4, 14);
    y += 16;

    drawTempWaveform(CONTENT_X + 2, y, CONTENT_W - 4, 16);
}
