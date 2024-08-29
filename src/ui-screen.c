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
#include "../include/player.h"
#include "../include/screen.h"
#include "../include/utils.h"

//* ------------------------------------------
//* DEFINITIONS

#define STANDARD_TIMER_LEN 11
#define HEART_METER_WIDTH  13
#define HEART_METER_HEIGHT 12

//* ------------------------------------------
//* GLOBAL VARIABLES
// TODO: does not pause when pause menu is started.
char* timerAsStr;

//* ------------------------------------------
//* MODULAR VARIABLES

/**  */
static Timer timer = { 0.0, 0.0 };

/** */
static Animation heartMeterAnimation;

static int heartFrame;

void UIScreenStartup() {
    heartMeterAnimation =
        CreateAnimation(0, HEART_METER_WIDTH, HEART_METER_HEIGHT, TILE_HEALTH_METER);
    heartFrame = 0;
    timerAsStr = (char*) malloc((STANDARD_TIMER_LEN + 1) * sizeof(char));

    if(timerAsStr == NULL) {
        TraceLog(LOG_FATAL, "UI-SCREEN.C (UIScreenStartup, line: %d): Memory allocation failure.", __LINE__);
    }
    StartTimer(&timer, -1.0);
    TraceLog(LOG_INFO, "UI-SCREEN.C (UIScreenStartup): UI screen set successfully.");
}

void UIScreenUpdate() {
    if(player.health <= 0) {
        heartFrame = 1;
    } else {
        heartFrame = 0;
    }
}

void UIScreenRender() {
    double elapsedTime = GetElapsedTime(&timer);
    //? Removed UI shapes for now.
    // DrawRectangle(0, 0, 400, 100, ColorAlpha(GRAY, 0.8f));
    // DrawTriangle((Vector2){ 400, 0 }, (Vector2){ 400, 100 }, (Vector2){ 450, 0 }, ColorAlpha(GRAY, 0.8f));
    // DrawRectangle(SCREEN_WIDTH - 250, 0, 250, 40, ColorAlpha(GRAY, 0.8f));

    ConvertToTimeFormat(timerAsStr, elapsedTime);
    DrawText(TextFormat("Elapsed Time: %s", timerAsStr), 10, 10, 30, RED);

    DrawAnimationFrame(
        &heartMeterAnimation,
        (Rectangle){ 10, 100 / 2, HEART_METER_WIDTH * 4, HEART_METER_HEIGHT * 4 },
        HEART_METER_WIDTH, HEART_METER_HEIGHT, 0.0f, heartFrame);

    DrawText("[SPACE] For Menu", SCREEN_WIDTH - 210, 10, 20, RED);
}

void UIScreenUnload() {
    free(timerAsStr);
    timerAsStr = NULL;
    AnimationUnload(&heartMeterAnimation);
    TraceLog(LOG_INFO, "UI-SCREEN.C (UIScreenUnload): UI screen unloaded successfully.");
}