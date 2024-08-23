/***********************************************************************************************
 *
 **   pause.h is responsible for handling pausing and display the pause menu on screen.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 * 
 *    @include stdbool.h
 *
 ***********************************************************************************************/

#ifndef PAUSE_H_
#define PAUSE_H_

#include <stdbool.h>

//* ------------------------------------------
//* GLOBAL VARIABLES

/** Checks if the screen is paused or not.
 * 
 * ! @attention Necessary to initialize it to false at the beginning of the game.
*/
extern bool isPaused;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Handles event listening and processing on pause screen.
 */
void PauseUpdate();

/**
 * Handles rendering the pause screen.
 */
void PauseRender();

#endif // PAUSE_H_