/**********************************************************************************************
 *    TODO:
 **   ...
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.2
 *
 *    @include screen.h
 *
 **********************************************************************************************/

#include "../include/screen.h"
#include "../include/utils.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

Timer timer;

void UIScreenStartup() {
    // setup positions and items for what need to be rendered

    StartTimer(&timer, -1.0);
}

void UIScreenUpdate() {
    // updated health time etc...
}

void UIScreenRender() {
    // Render HUD
    double elapsedTime = GetElapsedTime(&timer);
    DrawRectangle(0, 0, 250, 100, BLUE);
    DrawTriangle((Vector2){ 250, 0 }, (Vector2){ 250, 100 }, (Vector2){ 300, 0 }, BLUE);

    char timeAsStr[11];
    ConvertToTimeFormat(timeAsStr, elapsedTime);

    DrawText(TextFormat("Elapsed Time: %s", timeAsStr), 10, 10, 40, RED);
}

void UIScreenUnload() {
    // unload anything memory related
}