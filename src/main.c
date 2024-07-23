#include "raylib.h"
#include "raymath.h"

// * DEFINITIONS
// Window rendering variables
#define SCREEN_WIDTH    1600
#define SCREEN_HEIGHT   900
#define FRAME_RATE      60

// * FUNCTION PROTOTYPES
// Game life cicle functions
static void GameStartup();  
static void GameUpdate();
static void GameRender();
static void GameClosing();

// * VARIABLES
Camera2D camera;

int main() {
    GameStartup();

    while(!WindowShouldClose()) {
        GameUpdate();
        GameRender();
    }

    GameClosing();

    return 0;
}

static void GameStartup() {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "No name game name");
    SetTargetFPS(FRAME_RATE);

    // Initialize camera
    camera.target = (Vector2) {0, 0};
    camera.offset = (Vector2) {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
}

static void GameUpdate() {}

static void GameRender() {
    BeginMode2D(camera);
    BeginDrawing();
        ClearBackground(BLACK);
    EndDrawing();
}

static void GameClosing() {
    CloseWindow();
}