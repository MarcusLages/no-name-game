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
    player.attacking = false;
    player.state = IDLE;

    // Initializing the idle animation
    idlePlayerAnimation = CreateAnimation(
        ENTITY_IDLE_FPS,
        ENTITY_TILE_WIDTH,
        ENTITY_TILE_HEIGHT,
        TILE_PLAYER_IDLE,
        textures[TILE_PLAYER_IDLE]);

    // Initializing the moving animation
    movingPlayerAnimation = CreateAnimation(
        ENTITY_MOVING_FPS,
        ENTITY_TILE_WIDTH,
        ENTITY_TILE_HEIGHT,
        TILE_PLAYER_MOVE,
        textures[TILE_PLAYER_MOVE]);

    // Initializing the attacking animation
    attackPlayerAnimation = CreateAnimation(
        ENTITY_ATTACK_FPS,
        32,
        32,
        TILE_PLAYER_ATTACK,
        textures[TILE_PLAYER_ATTACK]);

    // Starting timers for both idle and moving animations
    StartTimer(idlePlayerAnimation.timer, -1.0f);
    StartTimer(movingPlayerAnimation.timer, -1.0f);
}

void PlayerMovement() {       
    if (player.state == ATTACKING) return; 
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
    if (IsKeyPressed(KEY_E)) {
        //player.attacking = true;
        player.state = ATTACKING;
        StartTimer(attackPlayerAnimation.timer, 1.0f);
    }

    if (player.state == ATTACKING && TimerDone(attackPlayerAnimation.timer)) {
        //player.attacking = false;
        player.state = IDLE;
    }
}

void PlayerRender() {
    DrawText(TextFormat("STATE: %d", player.state), 0, 0, 15, RED);
    switch (player.state) {
        case IDLE:
            AnimationRender(player, idlePlayerAnimation, ENTITY_TILE_WIDTH * player.face, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case MOVING:
            AnimationRender(player, movingPlayerAnimation, ENTITY_TILE_WIDTH * player.face, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case ATTACKING:
            AnimationRender(player, idlePlayerAnimation, ENTITY_TILE_WIDTH * player.face, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            AnimationRender(player, attackPlayerAnimation, 32, 32 * player.face, 
                32, 0, 90.0f);
            break;
        default:
            break;
    }
    // if (player.attacking) {
    //     AnimationRender(player, attackPlayerAnimation, 32, 32 * player.face, 
    //         32, 0, 90.0f);
    // }
}

void PlayerUnload() {
    AnimationUnload(idlePlayerAnimation);
    AnimationUnload(movingPlayerAnimation);
    AnimationUnload(attackPlayerAnimation);
}