/***********************************************************************************************
 *
 **   pause.c is responsible for implementing pausing and display the pause menu on screen.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 * 
 *    @include raylib.h, pause.h
 *
 ***********************************************************************************************/

#include "raylib.h"
#include "../include/pause.h"
#include "../include/utils.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

bool isPaused;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

void PauseUpdate() {}

void PauseRender() {
    char pauseMsg[] = "PAUSED";
    DrawText(pauseMsg, CenterComponentX(MeasureText(pauseMsg, 30)), GetScreenHeight() / 2, 30, RED);
}