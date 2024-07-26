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
    player.direction = (Vector2) {0, 0};
    player.face = 1;
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
    player.direction = (Vector2) {0, 0};

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.direction.x++;
        player.state = MOVING;
        player.face = 1;
    } else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.direction.x--;
        player.state = MOVING;
        player.face = -1;
    }
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.direction.y++;
        player.state = MOVING;
    } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.direction.y--;
        player.state = MOVING;
    }
    
    if(player.direction.x == 0 && player.direction.y == 0) {
        player.state = IDLE;
        return;
    }        

    player.direction = Vector2Normalize(player.direction);

    player.x += player.direction.x * player.speed;
    player.y += player.direction.y * player.speed;
}

void PlayerRender(GameState state) {  
    switch (state) {
        case IDLE:
            SpriteRender(player, idlePlayerAnimation, ENTITY_TILE_WIDTH * player.face, ENTITY_TILE_HEIGHT);
            break;
        case MOVING:
            SpriteRender(player, movingPlayerAnimation, ENTITY_TILE_WIDTH * player.face, ENTITY_TILE_HEIGHT);
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