/***********************************************************************************************
*
**   main.c is responsible for taking care of initializing basic core functionalities and all
**   low-level assemblying of the code, managing the game life cycle functions, choosing which
**   screens to load, update, render, unload and transition to.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.2
*
*    @include trace-log.h, screen.h, audio.h 
*
***********************************************************************************************/

#include "../include/screen.h"
#include "../include/trace-log.h"
#include "../include/audio.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

/** Camera object used in non-menu screens. */
Camera2D camera;

/** Current screen the game will be displaying. */
GameScreen currentScreen;

/** Next screen the game will be displaying (used for transitions and loading new screens). */
GameScreen nextScreen;

/** Closes the game if true. */
bool isRunning;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

//* Game life cicle functions

/** Starts and loads all initial game resources (window, framerate, camera, etc.). */
static void GameStartup();
/** Updates current game state. */
static void GameUpdate();
/** Renders current game state on the screen. */
static void GameRender();
/** Unloads (frees memory of) all currently used game resources and closes game window and task. */
static void GameClosing();

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

/** Entry point for the game. */
int main() {
    GameStartup();

    // Main game loop.
    // Runs while player doesn't close the window or presses ESC.
    while(!WindowShouldClose() && isRunning) {
        GameUpdate();
        GameRender();
    }

    GameClosing();

    return 0;
}

static void GameStartup() {
    // Set custom tracelog function to TraceLog()
    SetTraceLogCallback(CustomLog);

    // Game running
    isRunning = true;

    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "No name game name");
    SetTargetFPS(FRAME_RATE);

    // Setup audio devices
    InitAudioDevice();
    LoadAudio();

    // Sets up initial screen to Main Menu
    currentScreen = MAIN_MENU;
    nextScreen = MAIN_MENU;

    // Starts up Main Menu
    MainMenuStartup();

    TraceLog(LOG_INFO, "MAIN.C (GameStartup): Game initialized at screen : %d successfully.", currentScreen);

}

static void GameUpdate() {
    // Checks for a transitions to the next screen
    if(currentScreen != nextScreen) {
        // Unloads currentScreen
        switch(currentScreen) {
            case MAIN_MENU:
                MainMenuUnload();
                break;
            case DUNGEON:
                DungeonUnload();
                break;
            case FINAL_SCREEN:
                FinalScreenUnload();
                break;
            default: break;
        }

        // Starts up nextScreen
        switch(nextScreen) {
            case MAIN_MENU:
                MainMenuStartup();
                break;
            case DUNGEON:
                DungeonStartup();
                break;
            case FINAL_SCREEN:
                FinalScreenStartup();
                break;
            default: break;
        }
    }

    // Updates the current screen
    switch(currentScreen) {
            case MAIN_MENU:
                MainMenuUpdate();
                break;
            case DUNGEON:
                DungeonUpdate();
                break;
            case FINAL_SCREEN:
                FinalScreenUpdate();
                break;
            default: break;
    }
}

static void GameRender() {
    // Starts the camera and rendering process
    BeginDrawing();
        ClearBackground(BLACK);

        // Renders the currentScreen
        switch(currentScreen) {
            case MAIN_MENU:
                MainMenuRender();
                break;
            case DUNGEON:
                BeginMode2D(camera);
                DungeonRender();
                EndMode2D();
                break;
            case FINAL_SCREEN:
                FinalScreenRender();
                break;
            default: break;
        }

    EndDrawing();
}

static void GameClosing() {
    // Unloads everything from the currentScreen
    switch(currentScreen) {
            case MAIN_MENU:
                MainMenuUnload();
                break;
            case DUNGEON:
                DungeonUnload();
                break;
            case FINAL_SCREEN:
                FinalScreenUnload();
                break;
            default: break;
    }

    // Close audio and music
    UnloadAudio();
    CloseAudioDevice();

    TraceLog(LOG_INFO, "MAIN.C (GameClosing): Game unloaded and closed successfully.");

    CloseWindow();
}