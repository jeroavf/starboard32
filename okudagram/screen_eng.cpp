#include "screen_eng.h"
#include "lcars_frame.h"
#include "config.h"
#include "colors.h"
#include <stdio.h>

void screenEngDraw(int startY, const SystemData& data) {
    int y = startY;
    char buf[24];

    // ── Warp Core (Heap Memory) ──
    lcarsDrawSectionLabel(y, "WARP CORE");
    y += 12;

    uint32_t usedHeap = data.totalHeap - data.freeHeap;
    float heapRatio = (float)usedHeap / (float)data.totalHeap;
    bool heapOk = (data.freeHeap > 30000);

    lcarsDrawStatusDot(CONTENT_X + 4, y + 3, heapOk ? LCARS_GREEN : LCARS_RED);
    lcarsDrawDataRow(y, "  STATUS", heapOk ? "ONLINE" : "WARN",
                     LCARS_LT_BLUE, heapOk ? LCARS_ORANGE : LCARS_RED);
    y += 11;

    lcarsDrawGauge(CONTENT_X + 2, y, CONTENT_W - 4, heapRatio, LCARS_ORANGE);
    y += 7;

    snprintf(buf, sizeof(buf), "%lu KB", (unsigned long)(data.freeHeap / 1024));
    lcarsDrawDataRow(y, "FREE", buf, LCARS_LT_BLUE, LCARS_ORANGE);
    y += 13;

    // ── Power Output (CPU Freq) ──
    lcarsDrawSectionLabel(y, "POWER OUTPUT");
    y += 12;

    snprintf(buf, sizeof(buf), "%u MHZ", data.cpuFreqMhz);
    lcarsDrawDataRow(y, "CPU FREQ", buf, LCARS_LT_BLUE, LCARS_LT_BLUE);
    y += 11;

    float cpuRatio = (float)data.cpuFreqMhz / 240.0f;
    lcarsDrawGauge(CONTENT_X + 2, y, CONTENT_W - 4, cpuRatio, LCARS_BLUE);
    y += 13;

    // ── Antimatter (PSRAM) ──
    if (data.hasPsram) {
        lcarsDrawSectionLabel(y, "ANTIMATTER");
        y += 12;

        uint32_t usedPsram = data.totalPsram - data.freePsram;
        float psramRatio = (float)usedPsram / (float)data.totalPsram;

        snprintf(buf, sizeof(buf), "%lu KB", (unsigned long)(data.freePsram / 1024));
        lcarsDrawDataRow(y, "PSRAM", buf, LCARS_LT_BLUE, LCARS_PEACH);
        y += 11;

        lcarsDrawGauge(CONTENT_X + 2, y, CONTENT_W - 4, psramRatio, LCARS_PEACH);
    } else {
        lcarsDrawSectionLabel(y, "ANTIMATTER");
        y += 12;
        lcarsDrawDataRow(y, "PSRAM", "N/A", LCARS_LT_BLUE, LCARS_TAN);
    }
}
