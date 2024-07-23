#include "raylib.h"
#include "raymath.h"
#include "main.h"

void MainMenuStartup() {
    currentScreen = MAIN_MENU;
}

void MainMenuUpdate() {}

void MainMenuRender() {
    ClearBackground(BLACK);
    DrawText("No Name Game Name", SCREEN_WIDTH / 3 + 120, SCREEN_HEIGHT / 2, 50, RAYWHITE);
}

void MainMenuUnload() {}
