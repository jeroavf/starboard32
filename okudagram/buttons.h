#pragma once
#include <stdint.h>

enum Screen : uint8_t {
    SCREEN_ENG = 0,
    SCREEN_TAC,
    SCREEN_OPS,
    SCREEN_SCI,
    SCREEN_COUNT
};

void buttonsInit();

// Call every loop iteration. Returns true if screen changed.
bool buttonsUpdate(Screen& currentScreen);

// Returns true if WiFi reset was requested (long press BTN_LEFT).
bool buttonsWifiResetRequested();
