/***********************************************************************************************
*
**   timer.h is responsible for defining a simple timer to be easily used to time game processes.
*   
*    @authors Marcus Vinicius Lages Santos and Samarjit Bhogal
*    @version 1.0
*
*?   @note raymath.h and raylib.h are used.
*?   @cite raylib
*
***********************************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

//* -----------------------------------------
//* STRUCTURES

/**
 * Represents a timer.
 * 
 * ! @attention declaring a Timer with a lifeTime of -1 creates a timer that lasts for the lifetime of the game (forever).
 * 
 * @param startTime double
 * @param lifeTime  double
 */
typedef struct Timer {
    /** Start time of this timer in seconds. */ 
    double startTime;
    /** The amount of seconds this timer should last for. */ 
    double lifeTime;
} Timer;

//* -----------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Starts the specified Timer with the specified time.
 * 
 * ! @attention returns if given a NULL pointer.
 * 
 * @param timer     Timer to start.
 * @param lifetime  Life of the timer in seconds.
 * 
 * ? @note declaring a Timer with a lifeTime of -1 creates a timer that lasts for the lifetime of the game.
 */
void StartTimer(Timer* timer, double lifetime);

/**
 * Determines if the specified timer is complete.
 * 
 * ! @attention returns -1 if given a NULL pointer.
 * 
 * @param timer Timer to check.
 * 
 * @returns True if the timer is done false otherwise.
 */
bool TimerDone(Timer* timer);

/**
 * Returns the time elaspsed so far with the specified timer.
 * 
 * ! @attention returns -1.0 if given a NULL pointer.
 * 
 * @param timer Timer to check.
 * 
 * @returns Time elaspsed.
 */
double GetElapsedTime(Timer* timer);

/**
 *Returns the time remaining with the specified timer.
 * 
 * ! @attention returns -1.0 if given a NULL pointer.
 * 
 * @param timer Timer to check.
 * 
 * @returns Time remaining. Returns INFINITY if the timer was started with -1.0f (infinite timer).
 */
double TimeRemaining(Timer* timer);

#endif //TIMER_H