#ifndef TIMER_H
#define TIMER_H

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

//-----------------------------------------
//* STUCTURES

/**
 * Represents a timer.
 * 
 * ! @attention declaring a Timer with a lifeTime of -1 creates a timer that lasts for the lifetime of the game (forever).
 * 
 * @param startTime double
 * @param lifeTime double
 */
typedef struct Timer {
    /** Start time of this timer in seconds. */ 
    double startTime;
    /** The amount of seconds this timer should last for. */ 
    double lifeTime;
} Timer;

//-----------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Starts the specified Timer with the specified time.
 * 
 * ! @attention returns if given a NULL pointer.
 * 
 * @param timer the timer to start.
 * @param lifetime the life of the timer in seconds.
 * 
 * ? @note declaring a Timer with a lifeTime of -1 creates a timer that lasts for the lifetime of the game.
 */
void StartTimer(Timer* timer, double lifetime);

/**
 * Determines if the specified timer is complete.
 * 
 * ! @attention returns -1 if given a NULL pointer.
 * 
 * @param timer the timer to check.
 * @returns true if the timer is done false otherwise.
 */
bool TimerDone(Timer* timer);

/**
 * Returns the time elaspsed so far with the specified timer.
 * 
 * @param timer the timer to check.
 * @returns the time elaspsed.
 * 
 * ! @attention returns -1.0 if given a NULL pointer.
 */
double GetElapsedTime(Timer* timer);

/**
 *Returns the time remaining with the specified timer.
 * 
 * @param timer the timer to check.
 * @returns the time remaining. Returns INFINITY if the timer was started with -1.0f (infinite timer).
 * 
 * ! @attention returns -1.0 if given a NULL pointer.
 */
double TimeRemaining(Timer* timer);

#endif //TIMER_H