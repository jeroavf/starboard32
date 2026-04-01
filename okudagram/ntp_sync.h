#pragma once
#include <time.h>

void ntpInit();
void ntpSync();
bool ntpSyncIfDue();
struct tm ntpGetTime();
