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

#endif // !MAIN_H