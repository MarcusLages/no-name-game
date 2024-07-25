#include "raylib.h"
#include "raymath.h"
#include "entities.h"

// The player entity
Entity player;

// The animation for an idle player
Animation idlePlayerAnimation;

void PlayerStartup() {
    player.x = 0;
    player.y = 0;
    player.speed = 4;
    player.health = 1;
    player.state = IDLE;
}

void PlayerMovement() {
    Vector2 direction = {0, 0};

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        direction.x++;
    else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        direction.x--;
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        direction.y++;
    else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        direction.y--;

    if(direction.x == 0 && direction.y == 0)
        return;

    direction = Vector2Normalize(direction);

    player.x += direction.x * player.speed;
    player.y += direction.y * player.speed;
    
}

void PlayerRender() {
    int numOfTiles = FindNumOfTiles(ENTITY_TILE_WIDTH, TILE_PLAYER_IDLE);

    // Initializing the indle animation
    idlePlayerAnimation = CreateAnimation(
        ENTITY_IDLE_FPS, 
        numOfTiles,
        GetSpriteRectangles(numOfTiles, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_IDLE), 
        textures[TILE_PLAYER_IDLE]);
        
    SpriteRender(player, idlePlayerAnimation);
}

void PlayerUnload() {
    SpriteUnload(idlePlayerAnimation);
}