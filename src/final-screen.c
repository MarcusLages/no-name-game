/**********************************************************************************************
 *
 **   final-screen.c is responsible for managing and displaying the main menu screen.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.2
 *
 *    @include screen.h, utils.h, player.h
 *
 **********************************************************************************************/

#include "screen.h"
#include "utils.h"
#include "player.h"

//* ------------------------------------------
//* DEFINITIONS

#define MAX_FINAL_SCREEN_BUTTONS 3

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enumeration representing the possible menu button options.
 *
 * @param RESTART_BUTTON    0
 * @param MAIN_MENU_BUTTON  1
 * @param EXIT_BUTTON       2
 */
typedef enum FinalScreenButtons {
    RESTART_BUTTON = 0,
    MAIN_MENU_BUTTON,
    EXIT_BUTTON
} FinalScreenButtons;

//* ------------------------------------------
//* MODULAR VARIABLES

/** Array with all button text options for the main menu. */
static char finalScreenOptionsText[MAX_FINAL_SCREEN_BUTTONS][10] = { "Restart", "Main Menu",
                                                                     "Exit" };

/** Button box array */
static Rectangle finalScreenButtonBox[MAX_FINAL_SCREEN_BUTTONS];

/**
 * Represents the index of the clicked button.
 * ? (-1) represents no button clicked
 */
static int hoveredButton;

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void FinalScreenStartup() {
    // Setup the current screen.
    currentScreen = FINAL_SCREEN;

    hoveredButton = -1;

    int initialButtonY = SCREEN_HEIGHT / 2;
    for(int i = 0; i < MAX_FINAL_SCREEN_BUTTONS; i++) {
        finalScreenButtonBox[i] = (Rectangle){ .x = (float) CenterComponentX(240),
                                               .y      = initialButtonY,
                                               .width  = 240,
                                               .height = 60 };
        initialButtonY += 60 + 20;
    }

    TraceLog(LOG_INFO, "FINAL-SCREEN.C (FinalScreenStartup): Final screen set successfully.");
}

void FinalScreenUpdate() {
    Vector2 mouse = GetMousePosition();

    // Checks mouse click or hover on a button on the menu
    for(int button = 0; button < MAX_FINAL_SCREEN_BUTTONS; button++) {
        if(CheckCollisionPointRec(mouse, finalScreenButtonBox[button])) {
            // If mouse hovered, changes the value of the hoveredButton var to the
            // index of the mouse button and breaks the collision checking loop.
            if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                hoveredButton = button;
                break;
            } else {
                // If mouse clicked, changes the current screen to the appropriate screen
                switch(button) {
                    case RESTART_BUTTON: nextScreen = DUNGEON; break;
                    case MAIN_MENU_BUTTON: nextScreen = MAIN_MENU; break;
                    case EXIT_BUTTON: isRunning = false; break;
                    default: break;
                }
            }
        } else {
            // Default hoveredButton when there's no collision
            hoveredButton = -1;
        }
    }
}

void FinalScreenRender() {
    // Draft clear background main menu
    ClearBackground(GRAY);

    if(player.health >= 0)
        DrawText("You won?!", CenterComponentX(MeasureText("You won?!", 50)), SCREEN_HEIGHT / 3, 50, BLACK);        
    else
        DrawText("YOU LOST!", CenterComponentX(MeasureText("YOU LOST!", 50)), SCREEN_HEIGHT / 3, 50, BLACK);        

    // Draws all buttons
    for(int i = 0; i < MAX_FINAL_SCREEN_BUTTONS; i++) {
        DrawRectangleRec(finalScreenButtonBox[i], ((i == hoveredButton) ? RED : BLACK));
        DrawText(
            finalScreenOptionsText[i],
            CenterComponentX(MeasureText(finalScreenOptionsText[i], 40)),
            finalScreenButtonBox[i].y + 10, 40, RAYWHITE);
    }
}

void FinalScreenUnload() {}