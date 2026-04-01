#include "lcars_frame.h"
#include "config.h"
#include "colors.h"

static TFT_eSprite* spr = nullptr;
static uint32_t frameCount = 0;

// Sidebar segment colors (top to bottom)
static const uint16_t sidebarColors[] = {
    LCARS_BLUE, LCARS_PEACH, LCARS_LAVENDER, LCARS_ORANGE,
    LCARS_MAUVE, LCARS_LT_ORANGE, LCARS_BLUE, LCARS_PEACH
};
static const int NUM_SEGMENTS = sizeof(sidebarColors) / sizeof(sidebarColors[0]);

// Nav button colors and labels
static const uint16_t navColors[] = { LCARS_MAUVE, LCARS_PEACH, LCARS_ORANGE, LCARS_LT_BLUE };
static const char* navLabels[] = { "ENG", "TAC", "OPS", "SCI" };

void lcarsFrameInit(TFT_eSprite* sprite) {
    spr = sprite;
    frameCount = 0;
}

static void drawHeader() {
    // Main orange bar
    spr->fillRect(0, 0, SCREEN_WIDTH, HEADER_H, LCARS_ORANGE);
    // Lavender left cap
    spr->fillRect(0, 0, SIDEBAR_W, HEADER_H, LCARS_LAVENDER);
    // Bottom-right rounded corner cutout
    spr->fillCircle(SCREEN_WIDTH - 8, HEADER_H, 8, LCARS_BG);
    spr->fillRect(0, HEADER_H, SCREEN_WIDTH - 8, 1, LCARS_BG);
    // Header text
    spr->setTextColor(LCARS_BG);
    spr->setTextFont(1);
    spr->setTextSize(1);
    spr->setTextDatum(MR_DATUM);
    spr->drawString("LCARS 47", SCREEN_WIDTH - 4, 5);
}

static void drawElbow() {
    // Solid lavender block for elbow
    spr->fillRect(0, HEADER_H, SIDEBAR_W, ELBOW_H, LCARS_LAVENDER);
    // Inner curve cutout: black circle at bottom-right of elbow
    spr->fillCircle(SIDEBAR_W, HEADER_H + ELBOW_H, 10, LCARS_BG);
    // Clean up: fill the area outside the sidebar with black
    spr->fillRect(SIDEBAR_W, HEADER_H, SCREEN_WIDTH - SIDEBAR_W, ELBOW_H, LCARS_BG);
    // Re-fill the lavender that the circle might have erased above
    spr->fillRect(0, HEADER_H, SIDEBAR_W, ELBOW_H - 6, LCARS_LAVENDER);
    // Thin blue bar below elbow
    int barY = HEADER_H + ELBOW_H + 1;
    spr->fillRect(0, barY, SIDEBAR_W, 3, LCARS_BLUE);
}

static void drawSidebar() {
    int startY = HEADER_H + ELBOW_H + 5;  // after elbow + blue bar + gap
    int endY   = SCREEN_HEIGHT - 1;
    int totalH = endY - startY;
    int segH   = totalH / NUM_SEGMENTS;
    int gap     = 1;

    for (int i = 0; i < NUM_SEGMENTS; i++) {
        int y = startY + i * segH;
        int h = segH - gap;
        if (i == NUM_SEGMENTS - 1) {
            h = endY - y;  // last segment takes remaining space
            // Rounded bottom-left corner on last segment
            spr->fillRoundRect(0, y, SIDEBAR_W, h, 6, sidebarColors[i]);
        } else {
            spr->fillRect(0, y, SIDEBAR_W, h, sidebarColors[i]);
        }
    }
}

static void drawNavButtons(Screen active) {
    int totalW = CONTENT_W;
    int btnW   = (totalW - 6) / 4;  // 4 buttons, 3 gaps of 2px
    int x      = CONTENT_X;

    for (int i = 0; i < 4; i++) {
        uint16_t color = navColors[i];
        // Dim non-active buttons
        if ((Screen)i != active) {
            // Darken: shift RGB components right
            uint16_t r = ((color >> 11) & 0x1F) >> 1;
            uint16_t g = ((color >> 5)  & 0x3F) >> 1;
            uint16_t b = (color & 0x1F) >> 1;
            color = (r << 11) | (g << 5) | b;
        }
        spr->fillRoundRect(x, NAV_Y, btnW, NAV_H, 5, color);
        // Label
        spr->setTextColor((Screen)i == active ? LCARS_BG : 0x4208);
        spr->setTextFont(1);
        spr->setTextSize(1);
        spr->setTextDatum(MC_DATUM);
        spr->drawString(navLabels[i], x + btnW / 2, NAV_Y + NAV_H / 2);
        x += btnW + 2;
    }
}

static void drawShipId(const SystemData& data) {
    spr->setTextColor(LCARS_LT_ORANGE);
    spr->setTextFont(1);
    spr->setTextSize(1);
    spr->setTextDatum(TR_DATUM);
    spr->drawString("USS OKUDAGRAM", SCREEN_WIDTH - 3, CONTENT_Y);
    spr->setTextColor(LCARS_PEACH);
    spr->drawString("NCC-ESP32", SCREEN_WIDTH - 3, CONTENT_Y + 9);

    // Stardate
    char sd[16];
    snprintf(sd, sizeof(sd), "SD %.1f", data.stardate);
    spr->setTextColor(LCARS_TAN);
    spr->setTextDatum(TR_DATUM);
    spr->drawString(sd, SCREEN_WIDTH - 3, CONTENT_Y + 18);
}

int lcarsFrameDraw(Screen activeScreen, const SystemData& data) {
    frameCount++;
    spr->fillSprite(LCARS_BG);

    drawHeader();
    drawElbow();
    drawSidebar();
    drawShipId(data);
    drawNavButtons(activeScreen);

    // Divider below ship ID
    int contentStartY = CONTENT_Y + 28;
    for (int x = CONTENT_X + 2; x < SCREEN_WIDTH - 2; x++) {
        uint8_t alpha = 255 * abs(x - (CONTENT_X + CONTENT_W / 2)) / (CONTENT_W / 2);
        if (alpha < 200) {
            spr->drawPixel(x, contentStartY, LCARS_BLUE);
        }
    }
    contentStartY += 2;

    return contentStartY;
}

TFT_eSprite* lcarsGetSprite() {
    return spr;
}

void lcarsDrawGauge(int x, int y, int w, float ratio, uint16_t color) {
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    int fillW = (int)(w * ratio);
    spr->fillRoundRect(x, y, w, 4, 2, LCARS_DARK_GRAY);
    if (fillW > 2) {
        spr->fillRoundRect(x, y, fillW, 4, 2, color);
    }
}

void lcarsDrawStatusDot(int x, int y, uint16_t color) {
    bool on = ((frameCount / 15) % 2 == 0);  // toggle every ~750ms at 20fps
    if (on) {
        spr->fillCircle(x, y, 2, color);
    } else {
        // Dim version
        uint16_t r = ((color >> 11) & 0x1F) >> 2;
        uint16_t g = ((color >> 5)  & 0x3F) >> 2;
        uint16_t b = (color & 0x1F) >> 2;
        spr->fillCircle(x, y, 2, (r << 11) | (g << 5) | b);
    }
}

void lcarsDrawSectionLabel(int y, const char* text) {
    spr->setTextColor(LCARS_MAUVE);
    spr->setTextFont(1);
    spr->setTextSize(1);
    spr->setTextDatum(TL_DATUM);
    spr->drawString(text, CONTENT_X + 2, y);
    spr->drawFastHLine(CONTENT_X + 2, y + 9, CONTENT_W - 4, LCARS_MAUVE);
}

void lcarsDrawDataRow(int y, const char* label, const char* value,
                      uint16_t labelColor, uint16_t valueColor) {
    spr->setTextFont(1);
    spr->setTextSize(1);
    spr->setTextColor(labelColor);
    spr->setTextDatum(TL_DATUM);
    spr->drawString(label, CONTENT_X + 8, y);
    spr->setTextColor(valueColor);
    spr->setTextDatum(TR_DATUM);
    spr->drawString(value, SCREEN_WIDTH - 3, y);
}
