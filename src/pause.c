/***********************************************************************************************
 *
 **   pause.c is responsible for implementing pausing and display the pause menu on screen.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include raylib.h, pause.h
 *
 ***********************************************************************************************/

#include "../include/pause.h"
#include "../include/screen.h"
#include "../include/audio.h"
#include "../include/utils.h"
#include "raylib.h"

//* ------------------------------------------
//* DEFINITIONS

#define MAX_PAUSE_MENU_BUTTONS 7

//* ------------------------------------------
//* GLOBAL VARIABLES

bool isPaused;

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum for accessing all pause menu buttons on pauseOptionsText and pauseButtonBox.
 *
 * @param MASTER_MINUS  0
 * @param MASTER_PLUS   1
 * @param SFX_MINUS     2
 * @param SFX_PLUS      3
 * @param MUSIC_MINUS   4
 * @param MUSIC_PLUS    5
 * @param GO_MAIN_MENU  6
 */
typedef enum PauseMenuOptions {
    MASTER_MINUS = 0,
    MASTER_PLUS,
    SFX_MINUS,
    SFX_PLUS,
    MUSIC_MINUS,
    MUSIC_PLUS,
    GO_MAIN_MENU
} PauseMenuOptions;

//* ------------------------------------------
//* MODULAR VARIABLES

/** Array with all button text options for the pause menu. */
static char pauseMenuOptionsText[MAX_PAUSE_MENU_BUTTONS][10] = { "-", "+", "-",
                                                                "+", "-", "+",
                                                                "Main Menu" };

/** Button box array */
static Rectangle pauseMenuButtonBox[MAX_PAUSE_MENU_BUTTONS];

/**
 * Represents the index of the clicked button.
 * ? (-1) represents no button clicked
 */
static int hoveredButton;

// * ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void PauseStartup() {
    hoveredButton = -1;

    int initialButtonY = GetScreenHeight() / 2 - 120;
    for(int i = 0; i < MAX_PAUSE_MENU_BUTTONS; i++) {
        pauseMenuButtonBox[i] = (Rectangle){ .x = (float) CenterComponentOnScreenX(60) + 160,
                                             .y = initialButtonY,
                                             .width  = 60,
                                             .height = 60 };
        if(i == (MAX_PAUSE_MENU_BUTTONS - 1)) {
            pauseMenuButtonBox[i].width = 240;
            pauseMenuButtonBox[i].x = (float) CenterComponentOnScreenX(pauseMenuButtonBox[i].width);
            pauseMenuButtonBox[i].y += 20;
        } else if(i % 2 == 1) {
            initialButtonY += 60 + 20;
            pauseMenuButtonBox[i].x += 80;
        }
    }
}

void PauseUpdate() {
    Vector2 mouse = GetMousePosition();

    float master = GetMasterVolume();
    float sfx    = GetSfxVolume();
    float music  = GetMusicVolume();

    for(int button = 0; button < MAX_PAUSE_MENU_BUTTONS; button++) {
        if(CheckCollisionPointRec(mouse, pauseMenuButtonBox[button])) {
            // If mouse hovered, changes the value of the hoveredButton var to the
            // index of the mouse button and breaks the collision checking loop.
            if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                // Sound keeps repeating on hover
                // PlaySound(soundFX[HOVER_SFX]);
                hoveredButton = button;
                break;
            } else {
                PlaySound(soundFX[CLICK_SFX]);
                // If mouse clicked, changes the current screen to the appropriate screen
                switch(button) {
                    case MASTER_MINUS: master -= 0.1f; break;
                    case MASTER_PLUS: master += 0.1f; break;
                    case SFX_MINUS: sfx -= 0.1f; break;
                    case SFX_PLUS: sfx += 0.1f; break;
                    case MUSIC_MINUS: music -= 0.1f; break;
                    case MUSIC_PLUS: music += 0.1f; break;
                    case GO_MAIN_MENU: nextScreen = MAIN_MENU; break;
                    default: break;
                }
            }
        } else {
            // Default hoveredButton when there's no collision
            hoveredButton = -1;
        }
    }

    SetAudioVolume(master, sfx, music);
}

void PauseRender() {
    char pauseMsg[]     = "PAUSED";
    Rectangle pauseMenu = (Rectangle){ .height = 450, .width = 600 };
    pauseMenu.x         = CenterComponentOnScreenX(pauseMenu.width);
    pauseMenu.y         = CenterComponentOnsScreenY(pauseMenu.height);

    DrawRectangleRec(pauseMenu, ColorAlpha(GRAY, 0.8f));
    DrawText(pauseMsg, CenterComponentOnScreenX(MeasureText(pauseMsg, 30)), pauseMenu.y + 30, 30, RED);

    // Draws all pauseMenuButtonBox
    for(int i = 0; i < MAX_PAUSE_MENU_BUTTONS; i++) {
        switch(i) {
            case 0:
                DrawText("Master volume", pauseMenuButtonBox[i].x - 400, pauseMenuButtonBox[i].y, 40, WHITE);
                break;
            case 2:
                DrawText("Sfx volume", pauseMenuButtonBox[i].x - 400, pauseMenuButtonBox[i].y, 40, WHITE);
                break;
            case 4:
                DrawText("Music volume", pauseMenuButtonBox[i].x - 400, pauseMenuButtonBox[i].y, 40, WHITE);
                break;
        }
        DrawRectangleRec(pauseMenuButtonBox[i], ((i == hoveredButton) ? RED : BLACK));
        DrawText(
            pauseMenuOptionsText[i],
            CenterInnerComponentX(MeasureText(pauseMenuOptionsText[i], 40), pauseMenuButtonBox[i].x, pauseMenuButtonBox[i].width),
            pauseMenuButtonBox[i].y + 10, 40, RAYWHITE);
    }
}