#ifndef TIMER_H
#define TIMER_H

//* STUCTURES

/**
 * @struct Represents a timer.
 * 
 * @param startTime the start time of this timer in seconds.
 * @param lifeTime the amount of second this timer should last for.
 */
typedef struct Timer {
    // Start time in seconds
    double startTime;
    // Lifetime of this timer in seconds
    double lifeTime;
} Timer;

//* FUNCTION PROTOTYPES

/**
 * Starts the specified Timer with the specified time.
 * 
 * @param timer the timer to start.
 * @param lifetime the life of the timer.
 */
void StartTimer(Timer *timer, double lifetime);

/**
 * Determines if the specified timer is complete.
 * 
 * @param timer the timer to check.
 * @returns true if the timer is done false otherwise.
 */
bool TimerDone(Timer timer);

/**
 * Returns the time elaspsed so far with the specified timer.
 * 
 * @param timer the timer to check.
 * @returns the time elaspsed.
 */
double GetElapsed(Timer timer);

/**
 * Returns the time remaining with the specified timer.
 * 
 * @param timer the timer to check.
 * @returns the time remaining.
 */
double TimeRemaining(Timer timer);

#endif //TIMER_H