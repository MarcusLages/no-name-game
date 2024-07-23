#ifndef MAIN_H
#define MAIN_H

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
void DungeonStartup();
void DungeonUpdate();
void DungeonRender();
void DungeonUnload();

#endif // !MAIN_H