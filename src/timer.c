/***********************************************************************************************
*
**   timer.c implements functions to start a timer, check if it has completed, see time passed, 
**   and time remaining.
*   
*    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
*    @version 0.1
*
*    @include timer.h
*
***********************************************************************************************/

#include "../include/timer.h"

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void StartTimer(Timer* timer, double lifetime) {
    if (timer == NULL) return;
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}

bool TimerDone(Timer* timer) {
    if (timer == NULL) return -1;
    if (timer->lifeTime == -1.0f) return false;
    return GetTime() - timer->startTime >= timer->lifeTime;
}

double GetElapsedTime(Timer* timer) { 
    if (timer == NULL) return -1.0f;
    return GetTime() - timer->startTime;
}

double TimeRemaining(Timer* timer) {
    if (timer == NULL) return -1.0f;
    if (timer->lifeTime == 0.0f) return INFINITY;
    return timer->startTime + timer->lifeTime - GetTime();
}