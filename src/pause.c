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
#include "../include/audio.h"
#include "../include/utils.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

bool isPaused;

//* ------------------------------------------
//* MODULAR VARIABLES

/** Array with all button text options for the main menu. */
// static char mainMenuOptionsText[MAX_MENU_BUTTONS][8] = { "Start", "Options", "Exit" };

/** Button box array */
// static Rectangle mainMenuButtonBox[MAX_MENU_BUTTONS];

/**
 * Represents the index of the clicked button.
 * ? (-1) represents no button clicked
 */
// static int hoveredButton;

// * ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void PauseStartup() {}

void PauseUpdate() {
    Vector2 mouse = GetMousePosition();
    float masterIncrement, sfxIncrement, musicIncrement;
    masterIncrement = sfxIncrement = musicIncrement = 0.0f;

    // if(CheckCollisionPointRec(mouse, button)) {
    //     masterIncrement++;
    // }

    IncrementVolume(masterIncrement, sfxIncrement, musicIncrement);
}

void PauseRender() {
    char pauseMsg[]     = "PAUSED";
    Rectangle pauseMenu = (Rectangle){ .height = 450, .width = 600 };
    pauseMenu.x = CenterComponentX(pauseMenu.width);
    pauseMenu.y = CenterComponentY(pauseMenu.height);

    DrawRectangleRec(pauseMenu, ColorAlpha(GRAY, 0.8f));
    DrawText(pauseMsg, CenterComponentX(MeasureText(pauseMsg, 30)), pauseMenu.y + 30, 30, RED);
}