#include "entities.h"

// The player entity
Entity player;

//* Player Animations

// The animation for an idle player
Animation idlePlayerAnimation;

// The animation for the player moving 
Animation movingPlayerAnimation;

// The animation for a player attach
Animation attackPlayerAnimation;

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

    // Initializing the attacking animation
    attackPlayerAnimation = CreateAnimation(
        ENTITY_ATTACK_FPS,
        TILE_PLAYER_ATTACK,
        textures[TILE_PLAYER_ATTACK]);
}

void PlayerMovement() {        
    player.direction = (Vector2) {0, 0};

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.direction.x++;
        player.face = 1;
    } else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.direction.x--;
        player.face = -1;
    } 
    
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.direction.y++;
    } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.direction.y--;
    }

    player.state = player.state == ATTACKING ? ATTACKING : MOVING;
    
    if(player.direction.x == 0 && player.direction.y == 0 && player.state != ATTACKING) {
        player.state = IDLE;
        return;
    }     

    player.direction = Vector2Normalize(player.direction);

    player.x += player.direction.x * player.speed;
    player.y += player.direction.y * player.speed;
}

void PlayerAttack() {
    if (IsKeyPressed(KEY_E)) player.state = ATTACKING;

    if (attackPlayerAnimation.animationFrame == ENITIY_ATTACK_FRAMES) {
        attackPlayerAnimation.animationFrame = 0;
        player.state = IDLE;
    }
}

void PlayerRender() {
    switch (player.state) {
        case IDLE:
            AnimationRender(player, idlePlayerAnimation, ENTITY_TILE_WIDTH * player.face, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f, true);
            break;
        case MOVING:
            AnimationRender(player, movingPlayerAnimation, ENTITY_TILE_WIDTH * player.face, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f, true);
            break;
        case ATTACKING:
            // render attack animation on top of player animations (may require a different entity)
            AnimationRender(player, attackPlayerAnimation, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, 
                ENTITY_TILE_WIDTH * player.face, 0, 0.0f, false);
            attackPlayerAnimation.animationFrame++;
            break;
        default:
            break;
    }    
}

void PlayerUnload() {
    AnimationUnload(idlePlayerAnimation);
    AnimationUnload(movingPlayerAnimation);
    AnimationUnload(attackPlayerAnimation);
}