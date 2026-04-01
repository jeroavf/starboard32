#pragma once
#include <stdint.h>
#include <time.h>

struct SystemData {
    // Memory
    uint32_t freeHeap;
    uint32_t totalHeap;
    uint32_t freePsram;
    uint32_t totalPsram;
    bool     hasPsram;

    // WiFi
    int8_t   rssi;
    bool     wifiConnected;
    char     ssid[33];
    char     ip[16];

    // System
    uint32_t uptimeMs;
    uint16_t cpuFreqMhz;
    float    tempC;
    int      hallValue;

    // Time
    struct tm time;
    bool      ntpSynced;
    float     stardate;
};

void dataSourceInit();
void dataSourceUpdate(SystemData& data);
