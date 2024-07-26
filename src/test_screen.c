#include "raylib.h"
#include "raymath.h"
#include "screen.h"
#include "entities.h"

Rectangle examples[10];

void TestScreenStartup() {
    // Setup the current screen.
    currentScreen = TEST_SCREEN;

    camera.target = (Vector2) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    camera.offset = (Vector2) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;

    examples[0] = (Rectangle) {
        .x = SCREEN_WIDTH / 2 - 25,
        .y = SCREEN_HEIGHT / 2 - 25,
        .width = 50,
        .height = 50
    };
}

void TestScreenUpdate() {

}

void TestScreenRender() {
    DrawRectangleRec(examples[0], YELLOW);
}

void TestScreenUnload() {

}