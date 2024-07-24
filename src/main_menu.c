#include "raylib.h"
#include "raymath.h"
#include "screen.h"

//* ENUMERATIONS

// Button options for the main menu
typedef enum MenuButtons {
    START_BUTTON = 0,
    OPTIONS_BUTTON,
    EXIT_BUTTON
} MenuButtons;

static int CenterComponentX(int componentWidth) {
    return (SCREEN_WIDTH - componentWidth) / 2;
}

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
