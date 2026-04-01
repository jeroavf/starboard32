#include "data_source.h"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_system.h>

void dataSourceInit() {
    // Nothing to init for now
}

static float calculateStardate(struct tm* t) {
    if (t->tm_year == 0) return 41000.0f;  // fallback
    int year = t->tm_year + 1900;
    float dayFrac = t->tm_yday +
                    (t->tm_hour * 3600 + t->tm_min * 60 + t->tm_sec) / 86400.0f;
    return (year - 2323.0f) * 1000.0f + dayFrac * 2.7397f;
}

void dataSourceUpdate(SystemData& d) {
    // Memory
    d.freeHeap  = ESP.getFreeHeap();
    d.totalHeap = ESP.getHeapSize();
    d.hasPsram  = (ESP.getPsramSize() > 0);
    if (d.hasPsram) {
        d.freePsram  = ESP.getFreePsram();
        d.totalPsram = ESP.getPsramSize();
    }

    // WiFi
    d.wifiConnected = (WiFi.status() == WL_CONNECTED);
    if (d.wifiConnected) {
        d.rssi = WiFi.RSSI();
        strncpy(d.ssid, WiFi.SSID().c_str(), sizeof(d.ssid) - 1);
        d.ssid[sizeof(d.ssid) - 1] = '\0';
        strncpy(d.ip, WiFi.localIP().toString().c_str(), sizeof(d.ip) - 1);
        d.ip[sizeof(d.ip) - 1] = '\0';
    } else {
        d.rssi = -100;
        strcpy(d.ssid, "---");
        strcpy(d.ip, "0.0.0.0");
    }

    // System
    d.uptimeMs   = millis();
    d.cpuFreqMhz = getCpuFrequencyMhz();
    d.tempC      = (temperatureRead() - 32.0f) * 5.0f / 9.0f;  // F -> C
    d.hallValue  = 0;  // hallRead() removed in ESP32 core 3.x

    // Time
    d.ntpSynced = getLocalTime(&d.time);
    d.stardate  = calculateStardate(&d.time);
}
