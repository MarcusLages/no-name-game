#include "screen.h"

//* FUNCTION PROTOTYPES
// Game life cicle functions

static void GameStartup();  
static void GameUpdate();
static void GameRender();
static void GameClosing();

//* VARIABLES
// Global variables
Camera2D camera;

// Variable for checking current screen that needs to be updated/rendered.
GameScreen currentScreen;

// Variable for checking next screen that needs to be transitioned into.
GameScreen nextScreen;

// Entry point for the game.
int main() {
    GameStartup();

    // Main game loop.
    // Runs while player doesn't close the window or presses ESC.
    while(!WindowShouldClose()) {
        GameUpdate();
        GameRender();
    }

    GameClosing();

    return 0;
}

static void GameStartup() {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "No name game name");
    SetTargetFPS(FRAME_RATE);

    // Sets up initial screen to Main Menu
    // currentScreen = MAIN_MENU;
    // nextScreen = MAIN_MENU;
    currentScreen = DUNGEON;
    nextScreen = DUNGEON;

    // Starts up Main Menu
    // MainMenuStartup();
    DungeonStartup();
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
            default: break;
        }

        // Starts up nextScreen
        switch(nextScreen) {
            case MAIN_MENU:
                MainMenuStartup();
                break;
            case DUNGEON:
                MainMenuStartup();
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
            default: break;
    }

    CloseWindow();
}