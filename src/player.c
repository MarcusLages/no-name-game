#include "entities.h"

// The player entity
Entity player;

//* Player Animations

// The animation for an idle player
Animation* idlePlayerAnimation;

// The animation for the player moving 
Animation* movingPlayerAnimation;

// The animation for a player attach
Animation* attackPlayerAnimation;

void PlayerStartup() {
    player.x = 0;
    player.y = 0;
    player.speed = 4;
    player.health = 1;
    player.direction = (Vector2) {0, 0};
    player.faceValue = 1;
    player.state = IDLE;
    player.directionFace = RIGHT;

    // Initializing the idle animation
    idlePlayerAnimation = CreateAnimation(
        DEFAULT_IDLE_FPS,
        ENTITY_TILE_WIDTH,
        ENTITY_TILE_HEIGHT,
        TILE_PLAYER_IDLE,
        textures[TILE_PLAYER_IDLE]);

    // Initializing the moving animation
    movingPlayerAnimation = CreateAnimation(
        DEFAULT_MOVING_FPS,
        ENTITY_TILE_WIDTH,
        ENTITY_TILE_HEIGHT,
        TILE_PLAYER_MOVE,
        textures[TILE_PLAYER_MOVE]);

    // Initializing the attacking animation
    attackPlayerAnimation = CreateAnimation(
        DEFAULT_ATTACK_FPS,
        TEMP_ATTACK_WIDTH,
        TEMP_ATTACK_HEIGHT,
        TILE_PLAYER_ATTACK,
        textures[TILE_PLAYER_ATTACK]);

    // Starting timers for both idle and moving animations
    StartTimer(idlePlayerAnimation->timer, -1.0f);
    StartTimer(movingPlayerAnimation->timer, -1.0f);
}

void PlayerMovement() {
    // Ensures the player cannot move while attacking   
    if (player.state == ATTACKING) return; 
    player.direction = (Vector2) {0, 0};

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.direction.x++;
        player.faceValue = 1;
        player.directionFace = RIGHT;
    } else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.direction.x--;
        player.faceValue = -1;
        player.directionFace = LEFT;
    } 
    
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.direction.y++;
        player.directionFace = DOWN;
    } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.direction.y--;
        player.directionFace = UP;
    }

    // Set the player to MOVING if not ATTACKING.
    player.state = player.state == ATTACKING ? ATTACKING : MOVING;
    
    // Set the player to IDLE if not ATTACKING or moving on any direction
    if(player.direction.x == 0 && player.direction.y == 0 && player.state != ATTACKING) {
        player.state = IDLE;
        return;
    }     

    player.direction = Vector2Normalize(player.direction);

    // TODO: add delta time
    player.x += player.direction.x * player.speed;
    player.y += player.direction.y * player.speed;
}

void PlayerAttack() {
    if (IsKeyPressed(KEY_E)) {
        player.state = ATTACKING;
        StartTimer(attackPlayerAnimation->timer, 0.5f);
    }

    if (player.state == ATTACKING && TimerDone(attackPlayerAnimation->timer)) {
        player.state = IDLE;
    }
}

void PlayerRender() {
    switch (player.state) {
        case IDLE:
            AnimationRender(&player, idlePlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case MOVING:
            AnimationRender(&player, movingPlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case ATTACKING:
            RenderPlayerAttack();
            break;
        default:
            break;
    }
}

void RenderPlayerAttack() {
    switch (player.directionFace) {
        case RIGHT:
            AnimationRender(&player, attackPlayerAnimation, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, 
                32, 0, 90.0f);
            break;
        case DOWN:
            AnimationRender(&player, attackPlayerAnimation, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, 
                25, 48, 180.0f);
            break;
        case LEFT:
            AnimationRender(&player, attackPlayerAnimation, TEMP_ATTACK_WIDTH, -TEMP_ATTACK_HEIGHT, 
                16, 0, 90.0f);
            break;
        case UP:
            AnimationRender(&player, attackPlayerAnimation, -TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, 
                -10, 0, 0.0f);
            break;
        default:
            break;
    }
    // Rendering idle animation of player as the player should not move while attacking.
    AnimationRender(&player, idlePlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue, 
        ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
}

void PlayerUnload() {
    AnimationUnload(idlePlayerAnimation);
    AnimationUnload(movingPlayerAnimation);
    AnimationUnload(attackPlayerAnimation);
}