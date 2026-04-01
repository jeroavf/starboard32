#pragma once
#include <TFT_eSPI.h>
#include "buttons.h"
#include "data_source.h"

// Initialize the LCARS frame renderer with the sprite to draw into.
void lcarsFrameInit(TFT_eSprite* spr);

// Draw the shared LCARS chrome (header, elbow, sidebar, ship ID, stardate, nav buttons).
// Returns the Y coordinate where screen-specific content should start drawing.
int lcarsFrameDraw(Screen activeScreen, const SystemData& data);

// Utility: draw a bar gauge at (x,y) with given width, fill ratio (0.0-1.0) and color.
void lcarsDrawGauge(int x, int y, int w, float ratio, uint16_t color);

// Utility: draw a status dot (pulsing) at (x,y) with given color.
void lcarsDrawStatusDot(int x, int y, uint16_t color);

// Get the sprite pointer (for screens that need direct drawing).
TFT_eSprite* lcarsGetSprite();

// Utility: draw a section label.
void lcarsDrawSectionLabel(int y, const char* text);

// Utility: draw a data row (label left, value right).
void lcarsDrawDataRow(int y, const char* label, const char* value,
                      uint16_t labelColor, uint16_t valueColor);
