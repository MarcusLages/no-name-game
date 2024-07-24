#include "raylib.h"
#include "raymath.h"
#include "screen.h"

#define MAX_MENU_BUTTONS 3

//* FUNCTIONS PROTOTYPES

/*
    Used to return the x position that a component should have to be
    centered on the screen based on its width.
    @param componentWidth
    @return Centered x position the component should have 
 */
static int CenterComponentX(int componentWidth);

//* VARIABLES

// Array with all button text options for the main menu.
char menuButtonsText[MAX_MENU_BUTTONS][8] = {"Start", "Options", "Exit"};

// Initial generic button
Rectangle button;

void MainMenuStartup() {
    // Sets up currentScreen
    currentScreen = MAIN_MENU;
}

void MainMenuUpdate() {
    Vector2 mouse = GetMousePosition();

    // TODO: make mouse collision with main menu buttons.
}

void MainMenuRender() {
    button = (Rectangle) {
        .x = (float) CenterComponentX(240),
        .y = SCREEN_HEIGHT / 2,
        .width = 240,
        .height = 60
    };

    // Draft clear background main menu
    ClearBackground(GRAY);

    // Game name
    DrawText("No Name Game Name", CenterComponentX(460), SCREEN_HEIGHT / 3, 50, BLACK);

    for(int i = 0; i < MAX_MENU_BUTTONS; i++) {
        DrawRectangle(button.x, button.y, button.width, button.height, BLACK);
        DrawText(menuButtonsText[i], CenterComponentX(MeasureText(menuButtonsText[i], 40)), button.y + 10, 40, RAYWHITE);
        button.y += button.height + 20;
    }

}

void MainMenuUnload() {}

static int CenterComponentX(int componentWidth) {
    return (SCREEN_WIDTH - componentWidth) / 2;
}
