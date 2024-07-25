#include "raylib.h"
#include "raymath.h"
#include "entities.h"

// The player entity
Entity player;

//* Player Animations

// The animation for an idle player
Animation idlePlayerAnimation;

// The animation for the player moving 
Animation movingPlayerAnimation;

void PlayerStartup() {
    player.x = 0;
    player.y = 0;
    player.speed = 4;
    player.health = 1;
    player.state = IDLE;

    // Initializing the idle animation
    idlePlayerAnimation = CreateAnimation(
        ENTITY_IDLE_FPS, 
        TILE_PLAYER_IDLE,
        textures[TILE_PLAYER_IDLE]);

    // Initializing the moving animation
    movingPlayerAnimation = CreateAnimation(
        ENTITY_MOVING_FPS, 
        TILE_PLAYER_MOVE,
        textures[TILE_PLAYER_MOVE]);
}

void PlayerMovement() {        
    Vector2 direction = {0, 0};

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        direction.x++;
    } else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        direction.x--;
    }
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        direction.y++;
    } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        direction.y--;
    }

    player.state = MOVING;
    
    if(direction.x == 0 && direction.y == 0) {
        player.state = IDLE;
        return;
    }        

    direction = Vector2Normalize(direction);

    player.x += direction.x * player.speed;
    player.y += direction.y * player.speed;
    
}

void PlayerRender(GameState state) {       
    switch (state) {
        case IDLE:
            SpriteRender(player, idlePlayerAnimation);
            break;
        case MOVING:
            SpriteRender(player, movingPlayerAnimation);
            break;
        case ATTACKING:
            break;
        default:
            break;
    }    
}

void PlayerUnload() {
    SpriteUnload(idlePlayerAnimation);
}