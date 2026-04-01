#include <TFT_eSPI.h>
#include "config.h"
#include "colors.h"
#include "wifi_setup.h"
#include "ntp_sync.h"
#include "buttons.h"
#include "data_source.h"
#include "lcars_frame.h"
#include "screen_eng.h"
#include "screen_tac.h"
#include "screen_ops.h"
#include "screen_sci.h"

TFT_eSPI    tft    = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

static Screen      currentScreen = SCREEN_ENG;
static SystemData  sysData;
static uint32_t    lastFrameMs   = 0;
static uint32_t    lastDataMs    = 0;
static uint32_t    lastBrightMs  = 0;

// ── Splash screen ───────────────────────────────────────

static void splashMsg(const char* msg) {
    tft.fillScreen(LCARS_BG);
    // Draw a mini LCARS frame on splash
    tft.fillRect(0, 0, SCREEN_WIDTH, 16, LCARS_ORANGE);
    tft.fillRect(0, 0, 18, 16, LCARS_LAVENDER);
    tft.setTextColor(LCARS_BG);
    tft.setTextFont(1);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("LCARS 47", SCREEN_WIDTH - 4, 8);
    // Message
    tft.setTextColor(LCARS_LT_BLUE);
    tft.setTextFont(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(msg, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

// ── Brightness ──────────────────────────────────────────

static void updateBrightness() {
    struct tm t = ntpGetTime();
    bool night = (t.tm_hour >= NIGHT_START || t.tm_hour < NIGHT_END);
    ledcWrite(TFT_BL, night ? BRIGHT_NIGHT : BRIGHT_DAY);
}

// ── Setup ───────────────────────────────────────────────

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("\n[Starboard32] Iniciando...");

    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(LCARS_BG);

    ledcAttach(TFT_BL, 10000, 8);
    ledcWrite(TFT_BL, BRIGHT_DAY);

    splashMsg("CONNECTING...");
    wifiSetup();

    splashMsg("NTP SYNC...");
    ntpInit();

    // Create full-screen sprite (135x240 = 64,800 bytes)
    bool ok = sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    Serial.printf("[Sprite] Criado: %s | heap: %lu bytes\n",
                  ok ? "OK" : "FALHOU", (unsigned long)ESP.getFreeHeap());

    sprite.setColorDepth(16);

    buttonsInit();
    dataSourceInit();
    lcarsFrameInit(&sprite);
    screenTacInit();

    // Initial data read
    dataSourceUpdate(sysData);

    randomSeed(analogRead(0));
    Serial.println("[Starboard32] Setup completo!");
}

// ── Loop ────────────────────────────────────────────────

void loop() {
    // Button handling
    if (buttonsUpdate(currentScreen)) {
        Serial.printf("[Nav] Tela: %d\n", currentScreen);
    }
    if (buttonsWifiResetRequested()) {
        splashMsg("WIFI RESET...");
        wifiReset();
        return;
    }

    // NTP re-sync
    ntpSyncIfDue();

    // Brightness update every 60s
    if (millis() - lastBrightMs >= 60000UL) {
        updateBrightness();
        lastBrightMs = millis();
    }

    // Data update
    if (millis() - lastDataMs >= DATA_UPDATE_MS) {
        dataSourceUpdate(sysData);
        lastDataMs = millis();
    }

    // Frame rate limiter
    if (millis() - lastFrameMs < FRAME_INTERVAL_MS) return;
    lastFrameMs = millis();

    // Draw LCARS frame (shared chrome)
    int contentY = lcarsFrameDraw(currentScreen, sysData);

    // Draw active screen content
    switch (currentScreen) {
        case SCREEN_ENG: screenEngDraw(contentY, sysData); break;
        case SCREEN_TAC: screenTacDraw(contentY, sysData); break;
        case SCREEN_OPS: screenOpsDraw(contentY, sysData); break;
        case SCREEN_SCI: screenSciDraw(contentY, sysData); break;
        default: break;
    }

    // Push sprite to display
    sprite.pushSprite(0, 0);
}
