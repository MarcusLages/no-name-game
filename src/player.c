/***********************************************************************************************
 *
 **   player.c is responsible for implementating functions to setup a player, manage movement,
 **   and attacks. Animations are created on setup and managed based on player movements and attack.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.1
 *
 *    @include player.h
 *
 ***********************************************************************************************/

#include "../include/player.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

Entity player;

//* ------------------------------------------
//* MODULAR VARIABLES

/** The animation for an idle player. */
static Animation idlePlayerAnimation;

/** The animation for the player moving. */
static Animation movingPlayerAnimation;

/** The animation for a player attack. */
static Animation attackPlayerAnimation;

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

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void PlayerStartup() {
    player.pos           = (Vector2){ (float) 11 * TILE_WIDTH, (float) 4 * TILE_HEIGHT };
    player.hitbox        = (Rectangle){ .x     = player.pos.x,
                                        .y     = player.pos.y + ENTITY_TILE_HEIGHT / 2,
                                        .width = ENTITY_TILE_WIDTH,
                                        .height = ENTITY_TILE_HEIGHT / 2 };
    player.speed         = 200;
    player.health        = 1;
    player.direction     = Vector2Zero();
    player.faceValue     = 1;
    player.state         = IDLE;
    player.directionFace = RIGHT;

    // Initializing the idle animation
    idlePlayerAnimation =
        CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_IDLE);

    // Initializing the moving animation
    movingPlayerAnimation =
        CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_MOVE);

    // Initializing the attacking animation
    attackPlayerAnimation =
        CreateAnimation(DEFAULT_ATTACK_FPS, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, TILE_PLAYER_ATTACK);

    // Starting timers for both idle and moving animations
    StartTimer(&idlePlayerAnimation.timer, -1.0f);
    StartTimer(&movingPlayerAnimation.timer, -1.0f);
}

void PlayerMovement() {
    // For debugging:
    DrawText(TextFormat("Player x: %f", player.pos.x), 0, 0, 20, RED);
    DrawText(TextFormat("Player y: %f", player.pos.y), 0, 20, 20, RED);

    // Ensures the player cannot move while attacking
    if(player.state == ATTACKING) return;

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

    // Set the player to MOVING if not ATTACKING.
    player.state = player.state == ATTACKING ? ATTACKING : MOVING;

    // Set the player to IDLE if not ATTACKING or moving on any direction
    if(player.direction.x == 0 && player.direction.y == 0 && player.state != ATTACKING) {
        player.state = IDLE;
        return;
    }

    // Delta time helps not let player speed depend on framerate.
    // It helps to take account for time between frames too.
    //! NOTE: Do not add deltaTime before checking collisions only after.
    float deltaTime = GetFrameTime();

    player.direction = Vector2Normalize(player.direction);

    // Velocity:
    player.direction = Vector2Scale(player.direction, player.speed);

    EntityWorldCollision(&player);
    PlayerEnemyCollision();

    player.pos = Vector2Add(player.pos, Vector2Scale(player.direction, deltaTime));
}

static void PlayerEnemyCollision() {}

void PlayerAttack() {
    if(IsKeyPressed(KEY_E)) {
        player.state = ATTACKING;
        StartTimer(&attackPlayerAnimation.timer, 0.5f);
    }

    if(player.state == ATTACKING && TimerDone(&attackPlayerAnimation.timer)) {
        player.state = IDLE;
    }
}

void PlayerRender() {
    switch(player.state) {
        case IDLE:
            EntityRender(
                &player, &idlePlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue,
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case MOVING:
            EntityRender(
                &player, &movingPlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue,
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case ATTACKING: RenderPlayerAttack(); break;
        default: break;
    }
}

void PlayerUnload() {
    AnimationUnload(&idlePlayerAnimation);
    AnimationUnload(&movingPlayerAnimation);
    AnimationUnload(&attackPlayerAnimation);
}

static void RenderPlayerAttack() {
    // Rendering idle animation of player as the player should not move while attacking.
    EntityRender(
        &player, &idlePlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue,
        ENTITY_TILE_HEIGHT, 0, 0, 0.0f);

    //? NOTE: commented out animations are kept for alternating animations
    switch(player.directionFace) {
        case RIGHT:
            EntityRender(
                &player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, -TEMP_ATTACK_HEIGHT,
                TEMP_ATTACK_WIDTH + 2, TEMP_ATTACK_HEIGHT + 6, 180.0f);
            // EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH,
            // TEMP_ATTACK_HEIGHT, 32, 0, 90.0f);
            break;
        case DOWN:
            EntityRender(
                &player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH,
                -TEMP_ATTACK_HEIGHT * player.faceValue, TEMP_ATTACK_WIDTH - 40,
                TEMP_ATTACK_HEIGHT + 16, -90.0f);
            // EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT,
            //     25, 48, 180.0f);
            break;
        case LEFT:
            EntityRender(
                &player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT,
                TEMP_ATTACK_WIDTH - 48, TEMP_ATTACK_HEIGHT - 26, 0.0f);
            // EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH,
            // -TEMP_ATTACK_HEIGHT, 16, 0, 90.0f);
            break;
        case UP:
            EntityRender(
                &player, &attackPlayerAnimation, -TEMP_ATTACK_WIDTH,
                -TEMP_ATTACK_HEIGHT * player.faceValue, TEMP_ATTACK_WIDTH - 40,
                TEMP_ATTACK_HEIGHT - 6, -90.0f);
            //  EntityRender(&player, &attackPlayerAnimation, -TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT,
            //     -10, 0, 0.0f);
            break;
        default: break;
    }
}