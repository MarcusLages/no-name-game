#include "raylib.h"
#include "raymath.h"
#include "screen.h"
#include "collision.h"

/**
 * Macro function to get the absolute/module value of a number.
 * 
 * @param x Number
 */
#define ABS(x) (x > 0 ? x : x * (-1))

/**
 * @brief Struct representing a moving displayable Rectangle with direction.
 * 
 * ! @attention Dir vector needs to be used in relative position to the origin for collisions.
 * 
 * @param rec Rectangle
 * @param dir Vector2
 * @param col Color
 */
typedef struct RectEntity {
    Rectangle rec;
    Vector2 dir;
    Color col;
} RectEntity;

RectEntity examples[10];
Ray2D mouse;
float deltaTime;

void TestScreenStartup() {
    // Setup the current screen.
    currentScreen = TEST_SCREEN;

    examples[0] = (RectEntity) {
        .rec = (Rectangle) {
            .x = 0,
            .y = 0,
            .width = 100,
            .height = 100
        },
        .col = YELLOW
    };
    
    examples[1] = (RectEntity) {
        .rec = (Rectangle) {
            .x = SCREEN_WIDTH / 2 - 50,
            .y = SCREEN_HEIGHT / 2 - 50,
            .width = 100,
            .height = 100
        },
        .col = WHITE
    };

    mouse = (Ray2D) {
        .origin = (Vector2) { 200, 200 },
        .direction = GetMousePosition()
    };
}

void TestScreenUpdate() {
    deltaTime = GetFrameTime();

    // * Get direction Vector2 relative to the origin point of the Rectangle (middle)
    mouse.direction = GetMousePosition();
    mouse.direction.x -= examples[0].rec.x + examples[0].rec.width / 2;
    mouse.direction.y -= examples[0].rec.y + examples[0].rec.height / 2;

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // * Normalize direction Vector2
        mouse.direction = Vector2Normalize(mouse.direction);

        // * Normalized vector * scale * deltaTime (modular)
        examples[0].dir.x += mouse.direction.x * 100.0f * deltaTime;
        examples[0].dir.y += mouse.direction.y * 100.0f * deltaTime;
    }

    for(int i = 1; i < 2; i++) {
        RayCollision2D collision;
        collision = HitboxCollision(examples[0].rec, examples[0].dir, examples[i].rec);
        
        // ! IMPORTANT
        // * Check for a collision.timeHit >= 0.
        // > 0: considers collisions that happen only if they occur in that next direction vector and
        //      ignores collisions behind the hitboxIn rectangle
        // = 0: considers the collision in which both rectangles are touching each other
        if(collision.hit == true && collision.timeHit >= 0) {
            examples[0].dir.x += collision.normalVector.x * ABS(examples[0].dir.x) * (1 - collision.timeHit);
            examples[0].dir.y += collision.normalVector.y * ABS(examples[0].dir.y) * (1 - collision.timeHit);
        }
    }

    // * Moves the rectangle according to the direction Vector 2 * deltaTime (modularity)
    examples[0].rec.x += examples[0].dir.x * deltaTime;
    examples[0].rec.y += examples[0].dir.y * deltaTime;
}

void TestScreenRender() {    
    DrawText(TextFormat("Mouse vector direction: (%.2f, %.2f)", examples[0].dir.x, examples[0].dir.y), 100, 300, 15, RED);

    for(int i = 0; i < 2; i++)
        DrawRectangleRec(examples[i].rec, examples[i].col);
    
}

void TestScreenUnload() {

}