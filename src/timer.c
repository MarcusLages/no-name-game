#include "raylib.h"
#include "raymath.h"
#include "timer.h"

void StartTimer(Timer *timer, double lifetime) {
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}

bool TimerDone(Timer timer) {
    return GetTime() - timer.startTime >= timer.lifeTime;
}

double GetElapsed(Timer timer) {
    return GetTime() - timer.startTime;
}

double TimeRemaining(Timer timer) {
    return timer.startTime + timer.lifeTime - GetTime();
}