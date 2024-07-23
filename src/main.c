#include "raylib.h"
#include "raymath.h"

// * FUNCTION PROTOTYPES
// Game life cicle functions
static void GameStartup();  
static void GameUpdate();
static void GameRender();
static void GameClosing();

int main() {
    GameStartup();

    while(!WindowShouldClose()) {
        GameUpdate();
        GameRender();
    }

    GameClosing();

    return 0;
}

static void GameStartup() {}

static void GameUpdate() {}
static void GameRender() {}
static void GameClosing() {}