#include "buttons.h"
#include "config.h"
#include <Arduino.h>

static bool     btnRightDown    = false;
static uint32_t btnRightPressMs = 0;
static bool     btnLeftDown     = false;
static uint32_t btnLeftPressMs  = 0;
static bool     wifiResetFlag   = false;

void buttonsInit() {
    pinMode(BTN_RIGHT, INPUT);         // GPIO 35: input-only, external pullup
    pinMode(BTN_LEFT,  INPUT_PULLUP);  // GPIO 0: BOOT button, internal pullup
}

bool buttonsUpdate(Screen& currentScreen) {
    bool changed = false;
    wifiResetFlag = false;

    // Right button (GPIO 35): next screen
    bool r = (digitalRead(BTN_RIGHT) == LOW);
    if (r && !btnRightDown) {
        btnRightDown    = true;
        btnRightPressMs = millis();
    } else if (!r && btnRightDown) {
        uint32_t held = millis() - btnRightPressMs;
        if (held >= BTN_DEBOUNCE_MS && held < BTN_LONG_MS) {
            currentScreen = (Screen)((currentScreen + 1) % SCREEN_COUNT);
            changed = true;
        }
        btnRightDown = false;
    }

    // Left button (GPIO 0): prev screen / long = WiFi reset
    bool l = (digitalRead(BTN_LEFT) == LOW);
    if (l && !btnLeftDown) {
        btnLeftDown    = true;
        btnLeftPressMs = millis();
    } else if (!l && btnLeftDown) {
        uint32_t held = millis() - btnLeftPressMs;
        if (held >= BTN_WIFI_RESET_MS) {
            wifiResetFlag = true;
        } else if (held >= BTN_DEBOUNCE_MS) {
            currentScreen = (Screen)((currentScreen + SCREEN_COUNT - 1) % SCREEN_COUNT);
            changed = true;
        }
        btnLeftDown = false;
    }

    return changed;
}

bool buttonsWifiResetRequested() {
    return wifiResetFlag;
}
