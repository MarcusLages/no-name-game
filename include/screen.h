/**********************************************************************************************
 *
 **   screen.h is responsible for dealing with screen, game stages and window related actions
 **   and settings in the game, as well as connecting all game screens through the game life
 **   cycle functions.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.2
 *
 *    @include raylib.h
 *    @cite raylib
 *
 **********************************************************************************************/

#ifndef SCREEN_H
#define SCREEN_H

#include "raylib.h"
#include "timer.h"

//* ------------------------------------------
//* DEFINITIONS

/** Window rendering variables (size and refresh rate). */
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define FRAME_RATE    60

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum for the possible screens/stages the game can display.
 *
 * @param MAIN_MENU     0
 * @param DUNGEON       1
 * @param SETTINGS      2
 * @param PAUSE         3
 * @param FINAL_SCREEN  4
 */
enum GameScreen {
    MAIN_MENU    = 0,
    DUNGEON      = 1,
    SETTINGS     = 2,
    PAUSE        = 3,
    FINAL_SCREEN = 4
};
typedef enum GameScreen GameScreen;

//* ------------------------------------------
//* GLOBAL VARIABLES

/** Camera object used in non-menu screens. */
extern Camera2D camera;

/** Current screen the game will be displaying. */
extern GameScreen currentScreen;

/** Next screen the game will be displaying (used for transitions and loading new screens). */
extern GameScreen nextScreen;

/** Closes the game if true. */
extern bool isRunning;

/** The timer for player personal record. */
extern Timer timer;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

//* Main menu functions

/** Starts and loads all main menu screen resources. */
void MainMenuStartup();
/** Updates main menu screen state. */
void MainMenuUpdate();
/** Renders main menu screen current state on the screen. */
void MainMenuRender();
/** Unloads (frees memory of) all main menu screen resources. */
void MainMenuUnload();

//* Dungeon functions

/** Starts and loads all dungeon screen resources. */
void DungeonStartup();
/** Updates dungeon screen state. */
void DungeonUpdate();
/** Renders dungeon screen current state on the screen. */
void DungeonRender();
/** Unloads (frees memory of) all dungeon screen resources. */
void DungeonUnload();

/** Starts and loads all final screen resources. */
void FinalScreenStartup();
/** Updates final screen state. */
void FinalScreenUpdate();
/** Renders final screen current state on the screen. */
void FinalScreenRender();
/** Unloads (frees memory of) all final screen resources. */
void FinalScreenUnload();

/** Starts the UI screen by setting shapes and loading images needed. */
void UIScreenStartup();
/** Updates the UI screen. */
void UIScreenUpdate();
/** Renders the UI screen. */
void UIScreenRender();
/** Unloads anything used in memory by the UI screen. */
void UIScreenUnload();

#endif // !SCREEN_H