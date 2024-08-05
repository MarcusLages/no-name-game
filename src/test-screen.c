/**
 * //////////////////////////////////////////////////////////////////////////
 * ? OBS: This file is only for testing collisions and will be replaced.
 * //////////////////////////////////////////////////////////////////////////
 */

#include "../include/screen.h"
#include "../include/collision.h"

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
Ray2D playerTest;
float deltaTime;
// Linked list of the collisions that ocurred in this update
CollisionNode* collisionList;

void TestScreenStartup() {
    // Setup the current screen.
    currentScreen = TEST_SCREEN;
    
    // ! IMPORTANT
    // Initializes the pointer to NULL.
    collisionList = NULL;

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

    examples[2] = (RectEntity) {
        .rec = (Rectangle) {
            .x = SCREEN_WIDTH / 2 - 150,
            .y = SCREEN_HEIGHT / 2 - 50,
            .width = 100,
            .height = 100
        },
        .col = WHITE
    };
    
    examples[3] = (RectEntity) {
        .rec = (Rectangle) {
            .x = SCREEN_WIDTH / 2 - 250,
            .y = SCREEN_HEIGHT / 2 - 50,
            .width = 100,
            .height = 100
        },
        .col = WHITE
    };
    
    examples[4] = (RectEntity) {
        .rec = (Rectangle) {
            .x = SCREEN_WIDTH / 2 - 350,
            .y = SCREEN_HEIGHT / 2 - 50,
            .width = 100,
            .height = 100
        },
        .col = WHITE
    };

    playerTest = (Ray2D) {
        .origin = (Vector2) { 200, 200 },
        .direction = Vector2Zero()
    };
}

void TestScreenUpdate() {
    // Delta time = time between frames, used to maintain consistent speed/velocity
    deltaTime = GetFrameTime();

    // * Get direction Vector2 relative to the origin point of the Rectangle (middle)
    // playerTest.direction = GetMousePosition();
    // playerTest.direction.x -= examples[0].rec.x + examples[0].rec.width / 2;
    // playerTest.direction.y -= examples[0].rec.y + examples[0].rec.height / 2;
    playerTest.direction = Vector2Zero();

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        playerTest.direction.x++;
    } else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        playerTest.direction.x--;
    } 
    
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        playerTest.direction.y++;
    } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        playerTest.direction.y--;
    }

    // if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // * Normalize direction Vector2
        playerTest.direction = Vector2Normalize(playerTest.direction);

        // * Normalized vector * scale
        // ! NOTE: Dont add deltatime on acceleration/velocity/direction.
        // !       add it on the speed or movement
        playerTest.direction.x *= 300.0f;
        playerTest.direction.y *= 300.0f;

        examples[0].dir = playerTest.direction;
    // }

    for(int i = 1; i < 5; i++) {
        RayCollision2D collision;
        collision = HitboxCollision(examples[0].rec, examples[0].dir, examples[i].rec);
        
        // ! IMPORTANT
        // * Check for a collision.timeHit >= 0.
        // > 0: considers collisions that happen only if they occur in that next direction vector and
        //      ignores collisions behind the hitboxIn rectangle
        // = 0: considers the collision in which both rectangles are touching each other
        if(collision.hit == true && collision.timeHit >= 0) {
            // If a collision appears, instead of instantly resolving the collision,
            // adds the collided hitbox index to a collision list.
            if(collisionList == NULL)
                collisionList = CreateCollisionList(i, 0, collision.timeHit);
            else
                AddCollisionNode(collisionList, i, 0, collision.timeHit);
        }
    }

    //* Sorts the collision in terms of closest to furthest collision hitbox (smaller to bigger hitTime)
    SortCollisionList(collisionList);

    //* Resolves the collisions from closest to furthest collision.
    CollisionNode* resolvingNode = collisionList;
    while(resolvingNode != NULL) {
        RayCollision2D collision;
        collision = HitboxCollision(examples[0].rec, examples[0].dir, examples[(int) resolvingNode->collidedHitbox.index.x].rec);
        if(collision.hit == true && collision.timeHit >= 0) {
            //* Formula to recalculate the direction vector according to the collision.
            // A force is added in the direction of contact normal vector of the collision proportional to:
            // the force of the direction vector in that direction and the "timeHit" of collision.
            examples[0].dir.x += collision.normalVector.x * ABS(examples[0].dir.x) * (1 - collision.timeHit);
            examples[0].dir.y += collision.normalVector.y * ABS(examples[0].dir.y) * (1 - collision.timeHit);
        }
        resolvingNode = resolvingNode->next;
    }

    // * Moves the rectangle according to the direction Vector 2 * deltaTime (modularity)
    examples[0].rec.x += examples[0].dir.x * deltaTime;
    examples[0].rec.y += examples[0].dir.y * deltaTime;
}

void TestScreenRender() {
    DrawText(TextFormat("Mouse vector direction: (%.2f, %.2f)", examples[0].dir.x, examples[0].dir.y), 100, 300, 15, RED);

    for(int i = 0; i < 5; i++)
        DrawRectangleRec(examples[i].rec, examples[i].col);

    //* Frees the collision list in the memory
    if(collisionList != NULL)
        FreeCollisionList(collisionList);
    collisionList = NULL;
}

void TestScreenUnload() {
    //* Frees the collision list in the memory
    if(collisionList != NULL)
        FreeCollisionList(collisionList);
    collisionList = NULL;
}