/***********************************************************************************************
 *
 **   player.c is responsible for implementating functions to setup a player, manage movement,
 **   and attacks. Animations are created on setup and managed based on player movements and attack.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include player.h
 *
 ***********************************************************************************************/

#include "../include/player.h"

//* ------------------------------------------
//* MACROS

/** Shortcut macro to access the player's animation array. */
#define playerAnimArray player.animations.animationArr

//* ------------------------------------------
//* GLOBAL VARIABLES

Entity player;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Renders the player attack animation based off of it's Direction.
 */
static void RenderPlayerAttack();

/**
 * TODO: Implementation + checking how to pass the list or make it global
 * Handles player collision with the enemies list by movement.
 */
static void PlayerEnemyCollision();

/**
 * Handles the player movement towards a given position.
 *
 * @param position The position to move the player to.
 *
 * ? @note Calls MoveEntityTowardsPos()
 */
static void MovePlayerToPos(Vector2 position);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void PlayerStartup() {
    player.pos = (Vector2){ (float) 11 * TILE_WIDTH, (float) 4 * TILE_HEIGHT };
    player.hitbox        = (Rectangle){ .x     = player.pos.x,
                                        .y     = player.pos.y + ENTITY_TILE_HEIGHT / 2,
                                        .width = ENTITY_TILE_WIDTH,
                                        .height = ENTITY_TILE_HEIGHT / 2 };
    player.speed         = 100;
    player.health        = 1;
    player.direction     = Vector2Zero();
    player.faceValue     = 1;
    player.state         = IDLE;
    player.directionFace = RIGHT;

    // Create and initialize animations in the player's animation array
    player.animations.size = MAX_PLAYER_ANIMATIONS;
    player.animations.animationArr =
        (Animation*) malloc(sizeof(Animation) * player.animations.size);

    Animation idlePlayerAnimation =
        CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_IDLE);

    Animation movingPlayerAnimation =
        CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_MOVE);

    Animation attackPlayerAnimation =
        CreateAnimation(DEFAULT_ATTACK_FPS, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, TILE_PLAYER_ATTACK);

    player.animations.animationArr[IDLE_ANIMATION]   = idlePlayerAnimation;
    player.animations.animationArr[MOVE_ANIMATION]   = movingPlayerAnimation;
    player.animations.animationArr[ATTACK_ANIMATION] = attackPlayerAnimation;

    // Starting timers for both idle and moving animations
    StartTimer(&playerAnimArray[IDLE_ANIMATION].timer, -1.0f);
    StartTimer(&playerAnimArray[MOVE_ANIMATION].timer, -1.0f);
}

void PlayerMovement() {
    player.direction = Vector2Zero();

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.direction.x++;
        player.faceValue     = 1;
        player.directionFace = RIGHT;
    } else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.direction.x--;
        player.faceValue     = -1;
        player.directionFace = LEFT;
    }

    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.direction.y++;
        player.directionFace = DOWN;
    } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.direction.y--;
        player.directionFace = UP;
    }

    MovePlayerToPos(player.direction);

    //* Ensure if this needs to happen before position assignment of player. If so we
    //* should make a general entity-entity in entity.c collsion relationship as other enemies
    //* can collide with other enemies.
    PlayerEnemyCollision();
}

// TODO: Fix player attack so that the animation cant display in a different direction while triggered in one direction already
void PlayerAttack() {
    if(IsKeyPressed(KEY_E) && player.state != ATTACKING) {
        player.state = ATTACKING;
        StartTimer(&playerAnimArray[ATTACK_ANIMATION].timer, 0.5f);
    }

    if(player.state == ATTACKING && TimerDone(&playerAnimArray[ATTACK_ANIMATION].timer)) {
        player.state = IDLE;
    }
}

void PlayerRender() {
    switch(player.state) {
        case IDLE:
            EntityRender(
                &player, &playerAnimArray[IDLE_ANIMATION],
                ENTITY_TILE_WIDTH * player.faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case MOVING:
            EntityRender(
                &player, &playerAnimArray[MOVE_ANIMATION],
                ENTITY_TILE_WIDTH * player.faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case ATTACKING: RenderPlayerAttack(); break;
        default: break;
    }
}

void PlayerUnload() { UnloadAnimationArray(&player.animations); }

static void PlayerEnemyCollision() {}

static void RenderPlayerAttack() {
    // Rendering idle animation of player as the player should not move while attacking.
    EntityRender(
        &player, &playerAnimArray[IDLE_ANIMATION],
        ENTITY_TILE_WIDTH * player.faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);

    //? NOTE: commented out animations are kept for alternating animations
    switch(player.directionFace) {
        case RIGHT:
            EntityRender(
                &player, &playerAnimArray[ATTACK_ANIMATION], TEMP_ATTACK_WIDTH,
                -TEMP_ATTACK_HEIGHT, TEMP_ATTACK_WIDTH + 2, TEMP_ATTACK_HEIGHT + 6, 180.0f);
            // EntityRender(&player, &playerAnimArray[ATTACK_ANIMATION],
            // TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, 32, 0, 90.0f);
            break;
        case DOWN:
            EntityRender(
                &player, &playerAnimArray[ATTACK_ANIMATION], TEMP_ATTACK_WIDTH,
                -TEMP_ATTACK_HEIGHT * player.faceValue, TEMP_ATTACK_WIDTH - 40,
                TEMP_ATTACK_HEIGHT + 16, -90.0f);
            // EntityRender(&player, &playerAnimArray[ATTACK_ANIMATION], TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT,
            //     25, 48, 180.0f);
            break;
        case LEFT:
            EntityRender(
                &player, &playerAnimArray[ATTACK_ANIMATION], TEMP_ATTACK_WIDTH,
                TEMP_ATTACK_HEIGHT, TEMP_ATTACK_WIDTH - 48, TEMP_ATTACK_HEIGHT - 26, 0.0f);
            // EntityRender(&player, &playerAnimArray[ATTACK_ANIMATION],
            // TEMP_ATTACK_WIDTH, -TEMP_ATTACK_HEIGHT, 16, 0, 90.0f);
            break;
        case UP:
            EntityRender(
                &player, &playerAnimArray[ATTACK_ANIMATION], -TEMP_ATTACK_WIDTH,
                -TEMP_ATTACK_HEIGHT * player.faceValue, TEMP_ATTACK_WIDTH - 40,
                TEMP_ATTACK_HEIGHT - 6, -90.0f);
            //  EntityRender(&player, &playerAnimArray[ATTACK_ANIMATION], -TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT,
            //     -10, 0, 0.0f);
            break;
        default: break;
    }
}

static void MovePlayerToPos(Vector2 position) {
    MoveEntityTowardsPos(&player, position, NULL);
}