/**********************************************************************************************
*
**   screen.h is responsible for dealing with screen, game stages and window related actions
**   and settings in the game, as well as connecting all game screens through the game life
**   cycle functions.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.1
*
*    @include raylib.h
*    @cite raylib
*
**********************************************************************************************/

#ifndef SCREEN_H
#define SCREEN_H

#include "raylib.h"

//* ------------------------------------------
//* DEFINITIONS

/** Window rendering variables (size and refresh rate). */
#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720
#define FRAME_RATE      60

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum for the possible screens/stages the game can display.
 * 
 * @param MAIN_MENU     0
 * @param DUNGEON       1
 * @param SETTINGS      2
 * @param PAUSE         3
 * @param TEST_SCREEN   99
 */
enum GameScreen {
    MAIN_MENU = 0,
    DUNGEON = 1,
    SETTINGS = 2,
    PAUSE = 3,
    TEST_SCREEN = 99
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

//* TestScreen functions

/** Starts and loads all test screen resources. */
void TestScreenStartup();
/** Updates test screen state. */
void TestScreenUpdate();
/** Renders test screen current state on the screen. */
void TestScreenRender();
/** Unloads (frees memory of) all test screen resources. */
void TestScreenUnload();

#endif // !SCREEN_H