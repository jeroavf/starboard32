#include "screen_ops.h"
#include "lcars_frame.h"
#include "config.h"
#include "colors.h"
#include <stdio.h>

static void formatUptime(uint32_t ms, char* buf, size_t len) {
    uint32_t sec = ms / 1000;
    uint32_t min = sec / 60;
    uint32_t hr  = min / 60;
    uint32_t day = hr / 24;
    snprintf(buf, len, "%lud %luh %lum",
             (unsigned long)day, (unsigned long)(hr % 24), (unsigned long)(min % 60));
}

void screenOpsDraw(int startY, const SystemData& data) {
    int y = startY;
    char buf[32];

    // ── Ship Status (Uptime) ──
    lcarsDrawSectionLabel(y, "SHIP STATUS");
    y += 12;

    lcarsDrawStatusDot(CONTENT_X + 4, y + 3, LCARS_GREEN);
    lcarsDrawDataRow(y, "  SYSTEMS", "NOMINAL", LCARS_LT_BLUE, LCARS_ORANGE);
    y += 13;

    formatUptime(data.uptimeMs, buf, sizeof(buf));
    lcarsDrawDataRow(y, "UPTIME", buf, LCARS_LT_BLUE, LCARS_LT_BLUE);
    y += 13;

    // ── Stardate (prominent) ──
    lcarsDrawSectionLabel(y, "STARDATE");
    y += 14;

    snprintf(buf, sizeof(buf), "%.1f", data.stardate);
    // Draw stardate larger using font 2
    lcarsGetSprite()->setTextColor(LCARS_GOLD);
    lcarsGetSprite()->setTextFont(2);
    lcarsGetSprite()->setTextSize(1);
    lcarsGetSprite()->setTextDatum(TC_DATUM);
    lcarsGetSprite()->drawString(buf, CONTENT_X + CONTENT_W / 2, y);
    y += 20;

    // Current time below
    if (data.ntpSynced) {
        strftime(buf, sizeof(buf), "%H:%M:%S", &data.time);
        lcarsGetSprite()->setTextColor(LCARS_TAN);
        lcarsGetSprite()->setTextFont(1);
        lcarsGetSprite()->setTextDatum(TC_DATUM);
        lcarsGetSprite()->drawString(buf, CONTENT_X + CONTENT_W / 2, y);
    }
    y += 13;

    // ── System Alerts ──
    lcarsDrawSectionLabel(y, "SYSTEM ALERTS");
    y += 12;

    // Check alerts
    bool lowMem  = (data.freeHeap < 30000);
    bool noWifi  = !data.wifiConnected;
    bool noNtp   = !data.ntpSynced;

    if (!lowMem && !noWifi && !noNtp) {
        lcarsDrawStatusDot(CONTENT_X + 4, y + 3, LCARS_GREEN);
        lcarsDrawDataRow(y, "  ALL", "CLEAR", LCARS_LT_BLUE, LCARS_ORANGE);
    } else {
        if (lowMem) {
            lcarsDrawStatusDot(CONTENT_X + 4, y + 3, LCARS_RED);
            lcarsDrawDataRow(y, "  MEMORY", "LOW", LCARS_LT_BLUE, LCARS_RED);
            y += 11;
        }
        if (noWifi) {
            lcarsDrawStatusDot(CONTENT_X + 4, y + 3, LCARS_RED);
            lcarsDrawDataRow(y, "  COMMS", "OFFLINE", LCARS_LT_BLUE, LCARS_RED);
            y += 11;
        }
        if (noNtp) {
            lcarsDrawStatusDot(CONTENT_X + 4, y + 3, LCARS_YELLOW);
            lcarsDrawDataRow(y, "  CHRONO", "NO SYNC", LCARS_LT_BLUE, LCARS_YELLOW);
        }
    }
}
