#include "screen.h"

#define MAX_MENU_BUTTONS 3

//* FUNCTIONS PROTOTYPES

/** 
 * Used to return the x position that a component should have to be
 * centered on the screen based on its width.
 * 
 * @param componentWidth
 * @return Centered x position the component should have 
 * 
*/
static int CenterComponentX(int componentWidth);

//* ENUMERATIONS

/**
 * Enumeration representing the possible menu mainMenuButtonBox.
 * 
 * @param START_BUTTON 0
 * @param OPTIONS_BUTTON 1
 * @param EXIT_BUTTON 2
 */
typedef enum MainMenuOptions {
    START_BUTTON = 0,
    OPTIONS_BUTTON,
    EXIT_BUTTON
} MainMenuOptions;

//* VARIABLES

// TODO: Change variables to not be global.
// Array with all button text options for the main menu.
char mainMenuOptionsText[MAX_MENU_BUTTONS][8] = {"Start", "Options", "Exit"};

// Button box array
Rectangle mainMenuButtonBox[MAX_MENU_BUTTONS];

// Represents the index of the clicked button.
// ? (-1) represents no button clicked
int hoveredButton;

void MainMenuStartup() {
    // Sets up currentScreen
    currentScreen = MAIN_MENU;

    hoveredButton = -1;

    int initialButtonY = SCREEN_HEIGHT / 2;
    for(int i = 0; i < MAX_MENU_BUTTONS; i++) {
        mainMenuButtonBox[i] = (Rectangle) {
            .x = (float) CenterComponentX(240),
            .y = initialButtonY,
            .width = 240,
            .height = 60
        };
        initialButtonY += 60 + 20;
    }
}

void MainMenuUpdate() {
    Vector2 mouse = GetMousePosition();

    // Checks mouse click or hover on a button on the main menu
    for(int i = 0; i < MAX_MENU_BUTTONS; i++) {
        if(CheckCollisionPointRec(mouse, mainMenuButtonBox[i])) {
            // If mouse hovered, changes the value of the hoveredButton var to the
            // index of the mouse button and breaks the collision checking loop.
            if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                hoveredButton = i;
                break;
            }
            else {
                // If mouse clicked, changes the current screen to the appropriate screen
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
            // Default hoveredButton when there's no collision
            hoveredButton = -1;
        }
    }
}

void MainMenuRender() {
    // Draft clear background main menu
    ClearBackground(GRAY);

    // Game name
    DrawText("No Name Game Name", CenterComponentX(460), SCREEN_HEIGHT / 3, 50, BLACK);

    // Draws all mainMenuButtonBox
    for(int i = 0; i < MAX_MENU_BUTTONS; i++) {
        DrawRectangleRec(mainMenuButtonBox[i], ((i == hoveredButton) ? RED : BLACK));
        DrawText(mainMenuOptionsText[i], CenterComponentX(MeasureText(mainMenuOptionsText[i], 40)), mainMenuButtonBox[i].y + 10, 40, RAYWHITE);
    }

}

void MainMenuUnload() {}

static int CenterComponentX(int componentWidth) {
    return (SCREEN_WIDTH - componentWidth) / 2;
}
