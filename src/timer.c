#include "raylib.h"
#include "raymath.h"
#include "timer.h"

void StartTimer(Timer *timer, double lifetime) {
    if (timer == NULL) return;
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}

bool TimerDone(Timer *timer) {
    if (timer == NULL) return -1;
    if (timer->lifeTime == -1.0f) return false;
    return GetTime() - timer->startTime >= timer->lifeTime;
}

double GetElapsedTime(Timer *timer) { 
    if (timer == NULL) return -1.0f;
    return GetTime() - timer->startTime;
}

double TimeRemaining(Timer *timer) {
    if (timer == NULL) return -1.0f;
    if (timer->lifeTime == 0.0f) return INFINITY;
    return timer->startTime + timer->lifeTime - GetTime();
}