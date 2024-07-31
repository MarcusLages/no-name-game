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

// Buttons array
Rectangle buttons[MAX_MENU_BUTTONS];

int clickedButton;

void MainMenuStartup() {
    // Sets up currentScreen
    currentScreen = MAIN_MENU;

    clickedButton = -1;

    int buttonY = SCREEN_HEIGHT / 2;
    for(int i = 0; i < MAX_MENU_BUTTONS; i++) {
        buttons[i] = (Rectangle) {
            .x = (float) CenterComponentX(240),
            .y = buttonY,
            .width = 240,
            .height = 60
        };
        buttonY += 60 + 20;
    }
}

void MainMenuUpdate() {
    Vector2 mouse = GetMousePosition();

    for(int i = 0; i < MAX_MENU_BUTTONS; i++) {
        if(CheckCollisionPointRec(mouse, buttons[i])) {
            if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                clickedButton = i;
                break;
            }
            else {
                switch (i) {
                    case START_BUTTON:
                        nextScreen = DUNGEON;
                        break;
                    case OPTIONS_BUTTON:
                        break;
                    case EXIT_BUTTON:
                        break;
                    default: break;
                }
            }
        } else {
            clickedButton = -1;
        }
    }
}

void MainMenuRender() {
    // Draft clear background main menu
    ClearBackground(GRAY);

    // Game name
    DrawText("No Name Game Name", CenterComponentX(460), SCREEN_HEIGHT / 3, 50, BLACK);

    for(int i = 0; i < MAX_MENU_BUTTONS; i++) {
        DrawRectangleRec(buttons[i], ((i == clickedButton) ? RED : BLACK));
        DrawText(menuButtonsText[i], CenterComponentX(MeasureText(menuButtonsText[i], 40)), buttons[i].y + 10, 40, RAYWHITE);
    }

}

void MainMenuUnload() {}

static int CenterComponentX(int componentWidth) {
    return (SCREEN_WIDTH - componentWidth) / 2;
}
