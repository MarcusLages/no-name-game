#include "raylib.h"
#include "raymath.h"
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
GameScreen currentScreen;
GameScreen nextScreen;


int main() {
    GameStartup();

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

    // currentScreen = MAIN_MENU;
    // nextScreen = MAIN_MENU;

    //MainMenuStartup();

    currentScreen = DUNGEON;
    nextScreen = DUNGEON;

    DungeonStartup();

    // Initialize camera
    camera.target = (Vector2) {25, 25};
    camera.offset = (Vector2) {0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
}

static void GameUpdate() {
    if(currentScreen != nextScreen) {
        switch(currentScreen) {
            case MAIN_MENU:
                MainMenuUnload();
                break;
            case DUNGEON:
                DungeonUnload();
                break;
            default: break;
        }

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
    BeginMode2D(camera);
    BeginDrawing();
        switch(currentScreen) {
            case MAIN_MENU:
                MainMenuRender();
                break;
            case DUNGEON:
                    DungeonRender();
                    break;
                default: break;
        }
    EndDrawing();
}

static void GameClosing() {
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