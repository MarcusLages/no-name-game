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
    enemy.x = 50;
    enemy.y = 50;
    enemy.speed = 100;
    enemy.health = 100;
    enemy.direction = Vector2Zero();
    enemy.faceValue = 1;
    enemy.state = IDLE;
    enemy.directionFace = RIGHT;

    // Initializing the idle animation
    idleEnemyAnimation = CreateAnimation(
        DEFAULT_IDLE_FPS,
        ENTITY_TILE_WIDTH,
        ENTITY_TILE_HEIGHT,
        TILE_ENEMY_IDLE,
        textures[TILE_ENEMY_IDLE]);

    // Initializing the moving animation
    movingEnemyAnimation = CreateAnimation(
        DEFAULT_MOVING_FPS,
        ENTITY_TILE_WIDTH,
        ENTITY_TILE_HEIGHT,
        TILE_ENEMY_MOVE,
        textures[TILE_ENEMY_MOVE]);

    // Initializing the attacking animation
    attackEnemyAnimation = CreateAnimation(
        DEFAULT_ATTACK_FPS,
        TEMP_ATTACK_WIDTH,
        TEMP_ATTACK_HEIGHT,
        TILE_ENEMY_ATTACK,
        textures[TILE_ENEMY_ATTACK]);

    // Starting timers for both idle and moving animations
    StartTimer(&idleEnemyAnimation.timer, -1.0f);
    StartTimer(&movingEnemyAnimation.timer, -1.0f);
}

void EnemyMovement() {
    // Ensures the enemy cannot move while attacking
    if (enemy.state == ATTACKING) return;

    // Sets the enemy to IDLE if not in agro range.
    if ((abs(player.x - enemy.x) > AGRO_X && abs(player.y - enemy.y) > AGRO_Y)) {
        enemy.state = IDLE;
        return;
    } 

    float deltaTime = GetFrameTime();
    enemy.direction = (Vector2) {player.x - enemy.x, player.y - enemy.y};

    // Set the enemy to MOVING if not ATTACKING.
    enemy.state = enemy.state == ATTACKING ? ATTACKING : MOVING;
    
    // Set the enemy to IDLE if not ATTACKING or moving on any direction
    if(enemy.direction.x == 0 && enemy.direction.y == 0 && enemy.state != ATTACKING) {
        enemy.state = IDLE;
        return;
    }  

    // DrawText(TextFormat("Enemy x: %d", enemy.x), 0, 0, 20, RED);
    // DrawText(TextFormat("Enemy y: %d", enemy.y), 0, 20, 20, RED);  
    // DrawText(TextFormat("Enemy speed: %d", enemy.speed), 0, 40, 20, RED);
    // DrawText(TextFormat("Enemy before dir x: %f", enemy.direction.x), 0, 60, 20, RED); 
    // DrawText(TextFormat("Enemy before dir y: %f", enemy.direction.y), 0, 80, 20, RED); 

    enemy.direction = Vector2Normalize(enemy.direction);

    // DrawText(TextFormat("Enemy after norm dir x: %f", enemy.direction.x), 0, 100, 20, RED); 
    // DrawText(TextFormat("Enemy after norm dir y: %f", enemy.direction.y), 0, 120, 20, RED); 

    enemy.direction.x *= enemy.speed;
    enemy.direction.y *= enemy.speed;

    // DrawText(TextFormat("Enemy after dir * speed x: %f", enemy.direction.x), 0, 140, 20, RED); 
    // DrawText(TextFormat("Enemy after dir * speed y: %f", enemy.direction.y), 0, 160, 20, RED);

    //TODO: enemy world collision

    // DrawText(TextFormat("Enemy after dir * deltatime x: %f", enemy.direction.x * deltaTime), 0, 180, 20, RED);
    // DrawText(TextFormat("Enemy after dir * deltatime y: %f", enemy.direction.y * deltaTime), 0, 200, 20, RED);

    enemy.x += enemy.direction.x * deltaTime;
    enemy.y += enemy.direction.y * deltaTime;

    // DrawText(TextFormat("Enemy x: %d", enemy.x), 0, 220, 20, RED);
    // DrawText(TextFormat("Enemy y: %d", enemy.y), 0, 240, 20, RED);
}

void EnemyAttack() {
    
}

void EnemyRender() {
    switch (enemy.state) {
        case IDLE:
            EntityRender(&enemy, &idleEnemyAnimation, ENTITY_TILE_WIDTH * enemy.faceValue, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case MOVING:
            EntityRender(&enemy, &movingEnemyAnimation, ENTITY_TILE_WIDTH * enemy.faceValue, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case ATTACKING:
            
            break;
        default:
            break;
    }
}

void EnemyUnload() {
    AnimationUnload(&idleEnemyAnimation);
    AnimationUnload(&movingEnemyAnimation);
    AnimationUnload(&attackEnemyAnimation);
}