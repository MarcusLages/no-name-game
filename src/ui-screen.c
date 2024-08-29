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

#include "../include/animation.h"
#include "../include/screen.h"
#include "../include/utils.h"

//* ------------------------------------------
//* DEFINITIONS

#define STANDARD_TIMER_LEN 11
#define HEART_METER_WIDTH  13
#define HEART_METER_HEIGHT 12

//* ------------------------------------------
//* GLOBAL VARIABLES

char* timerAsStr;

//* ------------------------------------------
//* MODULAR VARIABLES

/**  */
static Timer timer = { 0.0, 0.0 };

/** */
static Animation heartMeterAnimation;

void UIScreenStartup() {
    // setup positions and items for what need to be rendered
    heartMeterAnimation =
        CreateAnimation(0, HEART_METER_WIDTH, HEART_METER_HEIGHT, TILE_HEALTH_METER);
    timerAsStr = (char*) malloc((STANDARD_TIMER_LEN + 1) * sizeof(char));
    
    if(timerAsStr == NULL) {
        TraceLog(LOG_FATAL, "UI-SCREEN.C (UIScreenStartup, line: %d): Memory allocation failure.", __LINE__);
    }
    StartTimer(&timer, -1.0);
    TraceLog(LOG_INFO, "UI-SCREEN.C (UIScreenStartup): UI screen set successfully.");
}

void UIScreenUpdate() {
    // updated health time etc...
}

void UIScreenRender() {
    // Render HUD
    double elapsedTime = GetElapsedTime(&timer);
    DrawRectangle(0, 0, 400, 100, LIGHTGRAY);
    DrawTriangle((Vector2){ 400, 0 }, (Vector2){ 400, 100 }, (Vector2){ 450, 0 }, LIGHTGRAY);

    ConvertToTimeFormat(timerAsStr, elapsedTime);
    DrawText(TextFormat("Elapsed Time: %s", timerAsStr), 10, 10, 30, RED);

    DrawAnimationFrame(
        &heartMeterAnimation, (Rectangle){ 10, 105, HEART_METER_WIDTH, HEART_METER_HEIGHT },
        HEART_METER_WIDTH, HEART_METER_HEIGHT, 0.0f, 0);
}

void UIScreenUnload() {
    free(timerAsStr);
    timerAsStr = NULL;
    AnimationUnload(&heartMeterAnimation);
    TraceLog(LOG_INFO, "UI-SCREEN.C (UIScreenUnload): UI screen unloaded successfully.");
}