#include "raylib.h"
#include "raymath.h"
#include "timer.h"

void StartTimer(Timer *timer, double lifetime) {
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}

bool TimerDone(Timer *timer) {
    if (timer->lifeTime == -1.0f) return false;
    return GetTime() - timer->startTime >= timer->lifeTime;
}

double GetElapsedTime(Timer *timer) {    
    return GetTime() - timer->startTime;
}

double TimeRemaining(Timer *timer) {
    if (timer->lifeTime == 0.0f) return -1.0f;
    return timer->startTime + timer->lifeTime - GetTime();
}