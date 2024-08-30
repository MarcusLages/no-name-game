/**********************************************************************************************
 *
 **   Provides the functionality for displaying the UI (or HUD) in game.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
 *
 *    @include screen.h, animation.h, timer.h, player.h and utils.h
 *
 **********************************************************************************************/

#include "../include/screen.h"
#include "../include/animation.h"
#include "../include/timer.h"
#include "../include/player.h"
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

/** Timer object for the player personal record. */
static Timer timer = { 0.0, 0.0 };

/** The animation object for the health meter. */
static Animation heartMeterAnimation;

void UIScreenStartup() {
    heartMeterAnimation =
        CreateAnimation(0, HEART_METER_WIDTH, HEART_METER_HEIGHT, TILE_HEALTH_METER);
    heartMeterAnimation.curFrame = 0;
    timerAsStr = (char*) malloc((STANDARD_TIMER_LEN + 1) * sizeof(char));

    if(timerAsStr == NULL) {
        TraceLog(LOG_FATAL, "UI-SCREEN.C (UIScreenStartup, line: %d): Memory allocation failure.", __LINE__);
    }
    StartTimer(&timer, -1.0);
    TraceLog(LOG_INFO, "UI-SCREEN.C (UIScreenStartup): UI screen set successfully.");
}

void UIScreenUpdate() {
    if(player.health <= 0) {
        heartMeterAnimation.curFrame = 1;
    } else {
        double elapsedTime = GetElapsedTime(&timer);
        ConvertToTimeFormat(timerAsStr, elapsedTime);
        heartMeterAnimation.curFrame = 0;
    }
}

void UIScreenRender() {
    //? Removed UI shapes for now.
    // DrawRectangle(0, 0, 400, 100, ColorAlpha(GRAY, 0.8f));
    // DrawTriangle((Vector2){ 400, 0 }, (Vector2){ 400, 100 }, (Vector2){ 450, 0 }, ColorAlpha(GRAY, 0.8f));
    // DrawRectangle(SCREEN_WIDTH - 250, 0, 250, 40, ColorAlpha(GRAY, 0.8f));

    DrawText(TextFormat("Elapsed Time: %s", timerAsStr), 10, 10, 30, RED);

    DrawAnimationFrame(
        &heartMeterAnimation,
        (Rectangle){ 10, 100 / 2, HEART_METER_WIDTH * 4, HEART_METER_HEIGHT * 4 },
        HEART_METER_WIDTH, HEART_METER_HEIGHT, 0.0f, heartMeterAnimation.curFrame);

    DrawText("[SPACE] For Menu", SCREEN_WIDTH - 210, 10, 20, RED);
}

void UIScreenUnload() {
    free(timerAsStr);
    timerAsStr = NULL;
    AnimationUnload(&heartMeterAnimation);
    TraceLog(LOG_INFO, "UI-SCREEN.C (UIScreenUnload): UI screen unloaded successfully.");
}