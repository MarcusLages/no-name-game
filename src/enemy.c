/***********************************************************************************************
 *
 **   ...
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.1.0
 *
 *    @include enemy.h
 *
 ***********************************************************************************************/

#include "../include/enemy.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

Entity enemy;

//* ------------------------------------------
//* MODULAR VARIABLES

/** The animation for an idle enemy. */
static Animation idleEnemyAnimation;

/** The animation for the enemy moving. */
static Animation movingEnemyAnimation;

/** The animation for a enemy attack. */
static Animation attackEnemyAnimation;

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void EnemyStartup() {
    enemy.x             = 50.0f;
    enemy.y             = 50.0f;
    enemy.speed         = 100;
    enemy.health        = 100;
    enemy.direction     = Vector2Zero();
    enemy.faceValue     = 1;
    enemy.state         = IDLE;
    enemy.directionFace = RIGHT;

    // Initializing the idle animation
    idleEnemyAnimation =
        CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_IDLE);

    // Initializing the moving animation
    movingEnemyAnimation =
        CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_MOVE);

    // Initializing the attacking animation
    attackEnemyAnimation =
        CreateAnimation(DEFAULT_ATTACK_FPS, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, TILE_ENEMY_ATTACK);

    // Starting timers for both idle and moving animations
    StartTimer(&idleEnemyAnimation.timer, -1.0f);
    StartTimer(&movingEnemyAnimation.timer, -1.0f);
}

void EnemyMovement() {
    // For debugging:
    DrawText(TextFormat("Enemy x: %f", enemy.x), 0, 40, 20, RED);
    DrawText(TextFormat("Enemy y: %f", enemy.y), 0, 60, 20, RED);

    // Ensures the enemy cannot move while attacking
    if(enemy.state == ATTACKING) return;

    // Sets the enemy to IDLE if not in agro range.
    if((abs(player.x - enemy.x) > AGRO_X && abs(player.y - enemy.y) > AGRO_Y)) {
        enemy.state = IDLE;
        return;
    }

    float deltaTime = GetFrameTime();
    enemy.direction =
        (Vector2){ (int) player.x - (int) enemy.x, (int) player.y - (int) enemy.y };

    // Set the enemy to MOVING if not ATTACKING.
    enemy.state = enemy.state == ATTACKING ? ATTACKING : MOVING;

    // Set the enemy to IDLE if not ATTACKING or moving on any direction
    if(enemy.direction.x == 0 && enemy.direction.y == 0 && enemy.state != ATTACKING) {
        enemy.state = IDLE;
        return;
    }

    enemy.direction = Vector2Normalize(enemy.direction);

    //! NOTE: Do not add deltaTime before checking collisions only after.
    enemy.direction.x *= enemy.speed;
    enemy.direction.y *= enemy.speed;

    // TODO: enemy world collision

    enemy.x += enemy.direction.x * deltaTime;
    enemy.y += enemy.direction.y * deltaTime;
}

void EnemyAttack() {}

void EnemyRender() {
    switch(enemy.state) {
    case IDLE:
        EntityRender(
            &enemy, &idleEnemyAnimation, ENTITY_TILE_WIDTH * enemy.faceValue,
            ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
        break;
    case MOVING:
        EntityRender(
            &enemy, &movingEnemyAnimation, ENTITY_TILE_WIDTH * enemy.faceValue,
            ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
        break;
    case ATTACKING: break;
    default: break;
    }
}

void EnemyUnload() {
    AnimationUnload(&idleEnemyAnimation);
    AnimationUnload(&movingEnemyAnimation);
    AnimationUnload(&attackEnemyAnimation);
}