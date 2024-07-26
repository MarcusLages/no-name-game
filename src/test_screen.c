#include "raylib.h"
#include "raymath.h"
#include "screen.h"
#include "collision.h"

Rectangle examples[10];
Ray2D mouse;

void TestScreenStartup() {
    // Setup the current screen.
    currentScreen = TEST_SCREEN;

    examples[0] = (Rectangle) {
        .x = SCREEN_WIDTH / 2 - 50,
        .y = SCREEN_HEIGHT / 2 - 50,
        .width = 100,
        .height = 100
    };

    mouse = (Ray2D) {
        .origin = (Vector2) { 200, 200 },
        .direction = GetMousePosition()
    };
}

void TestScreenUpdate() {
    mouse.direction = GetMousePosition();
}

void TestScreenRender() {
    RayCollision2D collision = RayRectCollision(mouse, examples[0]);
    if (collision.hit == true) {
        DrawRectangleRec(examples[0], RED);
        DrawCircle((int) collision.contactPoint.x, (int) collision.contactPoint.y, 3, BLUE);
        DrawLine(collision.contactPoint.x, collision.contactPoint.y,
                collision.contactPoint.x + collision.normalVector.x * 20,
                collision.contactPoint.y + collision.normalVector.y * 20,
                GREEN);
    } else
        DrawRectangleRec(examples[0], YELLOW);

    DrawLine(mouse.origin.x, mouse.origin.y, mouse.direction.x, mouse.direction.y, RAYWHITE);
    
    DrawText(TextFormat("Collision time: %.2f", collision.timeHit), 100, 300, 15, RED);
}

void TestScreenUnload() {

}