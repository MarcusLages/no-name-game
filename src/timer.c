/***********************************************************************************************
 *
 **   timer.c implements functions to start a timer, check if it has completed, see time passed,
 **   and time remaining.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.3
 *
 *    @include timer.h
 *
 ***********************************************************************************************/

#include "../include/timer.h"
#include "../include/utils.h"

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void StartTimer(Timer* timer, double lifetime) {
    if(timer == NULL) {
        TraceLog(LOG_WARNING, "TIMER.C (StartTimer, line: %d): NULL timer given, Timer could not be started.", __LINE__);
        return;
    }
    if(IsDoubleEqual(lifetime, 0.0, 0.00001f)) {
        TraceLog(LOG_WARNING, "TIMER.C (StartTimer, line: %d): Could not start a timer with a lifetime of 0.0.", __LINE__);
        return;
    }
    timer->startTime = GetTime();
    timer->lifeTime  = lifetime;
}

void StartTimerWithDelay(Timer* timer, double lifetime, double delay) {
    if(timer == NULL) {
        TraceLog(LOG_WARNING, "TIMER.C (StartTimerWithDelay, line: %d): NULL timer given, Timer could not be started.", __LINE__);
        return;
    }
    if(IsDoubleEqual(lifetime, 0.0, 0.00001f)) {
        TraceLog(LOG_WARNING, "TIMER.C (StartTimerWithDelay, line: %d): Could not start a timer with a lifetime of 0.0.", __LINE__);
        return;
    }
    timer->startTime = GetTime() + delay;
    timer->lifeTime  = lifetime;
}

bool TimerDone(Timer* timer) {
    if(timer == NULL) {
        TraceLog(LOG_WARNING, "TIMER.C (TimerDone, line: %d): NULL timer given.", __LINE__);
        return false;
    }
    if(timer->lifeTime == -1.0f) return false;
    return GetTime() - (timer->lifeTime + timer->startTime) >= 0.0;
}

double GetElapsedTime(Timer* timer) {
    if(timer == NULL) {
        TraceLog(LOG_WARNING, "TIMER.C (GetElapsedTime, line: %d): NULL timer given.", __LINE__);
        return -1.0;
    }
    return GetTime() - timer->startTime;
}

double TimeRemaining(Timer* timer) {
    if(timer == NULL) {
        TraceLog(LOG_WARNING, "TIMER.C (TimeRemaining, line: %d): NULL timer given.", __LINE__);
        return -1.0;
    }
    double currTime = GetTime();
    if(currTime < timer->startTime) {
        return (timer->lifeTime + timer->startTime);
    } else {
        return (timer->lifeTime + timer->startTime) - currTime;
    }
}

bool CheckIfDelayed(Timer* timer) {
    double elapsedTime = GetElapsedTime(timer);
    return elapsedTime < 0.0f;
}

void ResetTimer(Timer* timer) {
    if(timer == NULL) {
        TraceLog(LOG_WARNING, "TIMER.C (ResetTimer, line: %d): NULL timer given, Timer could not reset.", __LINE__);
        return;
    }
    timer->lifeTime  = 0.0;
    timer->startTime = 0.0;
}