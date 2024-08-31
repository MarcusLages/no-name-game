/**********************************************************************************************
 *
 **   ui-screen.c provides the functionality for displaying the UI (or HUD) in game.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
 *
 *    @include <stdlib.h>, screen.h, animation.h, timer.h, player.h and utils.h
 *
 **********************************************************************************************/

#include "../include/animation.h"
#include "../include/player.h"
#include "../include/screen.h"
#include "../include/utils.h"
#include <stdlib.h>

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

/** Timer object for the player personal record. */
static Timer timer = { 0.0, 0.0 };

/** The animation object for the health meter. */
static Animation heartMeterAnimation;

/** Total time the game has been paused for. */
static double totalPausedTime = 0.0;

void UIScreenStartup() {
    timerAsStr = (char*) malloc((STANDARD_TIMER_LEN + 1) * sizeof(char));

    if(timerAsStr == NULL) {
        TraceLog(LOG_FATAL, "UI-SCREEN.C (UIScreenStartup, line: %d): Memory allocation failure.", __LINE__);
    }

    heartMeterAnimation =
        CreateAnimation(0, HEART_METER_WIDTH, HEART_METER_HEIGHT, TILE_HEALTH_METER);
    heartMeterAnimation.curFrame = 0;
    StartTimer(&timer, -1.0);

    TraceLog(LOG_INFO, "UI-SCREEN.C (UIScreenStartup): UI screen set successfully.");
}

void UIScreenUpdate(Timer* pauseTimer) {
    if(player.health <= 0) {
        // setting the heart to empty.
        heartMeterAnimation.curFrame = 1;
        ResetTimer(pauseTimer);
    } else {
        double elapsedTime = GetElapsedTime(&timer);

        // Adding paused time from the pauseTimer
        if(!IsDoubleEqual(pauseTimer->startTime, 0.0, 0.00001f)) {
            totalPausedTime += GetElapsedTime(pauseTimer);
            ResetTimer(pauseTimer);
        }

        // Calulating the difference and setting the string.
        elapsedTime -= totalPausedTime;
        ConvertToTimeFormat(timerAsStr, STANDARD_TIMER_LEN, elapsedTime);
    }
}

void UIScreenRender() {
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