#include "raylib.h"
#include "raymath.h"
#include "screen.h"

void MainMenuStartup() {
    // Sets up currentScreen
    currentScreen = MAIN_MENU;
}

void MainMenuUpdate() {}

void MainMenuRender() {
    // Draft clear background main menu
    ClearBackground(BLACK);
    DrawText("No Name Game Name", SCREEN_WIDTH / 3 + 120, SCREEN_HEIGHT / 2, 50, RAYWHITE);
}

void MainMenuUnload() {}
