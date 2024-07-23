#ifndef SCREEN_H
#define SCREEN_H

#include "raylib.h"
#include "raymath.h"

//* DEFINITIONS
// Window rendering variables
#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720
#define FRAME_RATE      60

//* ENUMS
enum GameScreen {
    MAIN_MENU = 0,
    DUNGEON = 1,
    SETTINGS = 2,
    PAUSE = 3
};
typedef enum GameScreen GameScreen;

//* GLOBAL VARIABLES
extern Camera2D camera;
extern GameScreen currentScreen;
extern GameScreen nextScreen;

//------------------------------------------
//* SCREEN FUNCTION PROTOTYPES

// Main menu functions
void MainMenuStartup();
void MainMenuUpdate();
void MainMenuRender();
void MainMenuUnload();

// Dungeon functions
// void DungeonStartup();
// void DungeonUpdate();
// void DungeonRender();
// void DungeonUnload();

#endif // !SCREEN_H