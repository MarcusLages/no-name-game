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
    // Initial generic button
    Rectangle button = {
        .x = CenterComponentX(240),
        .y = SCREEN_HEIGHT / 2,
        .width = 240,
        .height = 60
    };

    // Draft clear background main menu
    ClearBackground(GRAY);

    // Game name
    DrawText("No Name Game Name", CenterComponentX(460), SCREEN_HEIGHT / 3, 50, BLACK);

    // Start button
    DrawRectangle(button.x, button.y, button.width, button.height, BLACK);
    DrawText("Start", CenterComponentX(MeasureText("Start", 40)), button.y + 10, 40, RAYWHITE);

    // Options button
    button.y += button.height + 20;
    DrawRectangle(button.x, button.y, button.width, button.height, BLACK);

    // Quit button
    button.y += button.height + 20;
    DrawRectangle(button.x, button.y, button.width, button.height, BLACK);
}

void MainMenuUnload() {}
