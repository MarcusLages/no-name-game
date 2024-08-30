/***********************************************************************************************
 *
 **   Provide functionality for setting enemy spawn points, managing movement, attack,
 **   and rendering of enemies and their animations.
 *
 *    TODO: Handle player to enemy collisions
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.3
 *
 *    @include <stdlib.h> enemy.h, utils.h
 *
 ***********************************************************************************************/

#include "../include/enemy.h"
#include "../include/utils.h"
#include <stdlib.h>

//* ------------------------------------------
//* DEFINITIONS

#define ENEMY_ATTACK_RANGE 30

//* ------------------------------------------
//* MACROS

/** Shortcut macro to access an enemy's animation array. */
#define enemyAnimArray enemy->animations.animationArr

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Sets the animations for the enemy based upon the given EnemyType.
 *
 * ! @attention returns if given an invlaid type or a NULL enemy.
 * ! @note Allocates memory for the array in enemy.animations.
 */
static void SetupEnemyAnimation(Entity* enemy, EnemyType type);

/**
 * Determines if the player is seen by a given enemy.
 *
 * @param enemy The enemy to check agaist.
 * @param type  Type of enemy.
 * @returns     True if the player is seen and false otherwise.
 */
static bool IsPlayerSeen(Entity* enemy, EnemyType type);

/**
 * Updates the given enemy's attack hitbox property.
 *
 * @param enemy The enemy to update.
 * @param type  Type of enemy.
 */
static void UpdateEnemyAttackHitbox(Entity* enemy, EnemyType type);

/**
 * Renders an enemy's attack animation based off of it's Direction.
 *
 * @param enemy The enemy to render an attack for.
 * @param type  Type of enemy.
 */
static void RenderEnemyAttack(Entity* enemy, EnemyType type);

/**
 * Handles the enemy movement towards a given position.
 *
 * @param enemy         The reference to the enemy to move.
 * @param position      The position to move the entity towards.
 * @param lastPlayerPos The last known position of the player relative to the given enemy.
 *
 * ? @note Calls MoveEntityTowardsPos()
 */
static void MoveEnemyToPos(Entity* enemy, Vector2 position, Vector2* lastPlayerPos);

/**
 * Sets the attack hitbox for the enemy: DEMON_WAFFLES.
 *
 * ! @attention enemy MUST be of entity DEMON_WAFFLES.
 *
 * @param enemy The waffles enemy.
 */
static void LoadWafflesAttackHitbox(Entity* enemy);

/**
 * Renders the attack animation for: DEMON_PABLO, DEMON_DIEGO.
 *
 * ! @attention enemy MUST be of entity DEMON_PABLO OR DEMON_DIEGO.
 *
 * @param enemy An enemy entity.
 */
static void RenderPabloDiegoAttack(Entity* enemy);

/**
 * Renders the attack animation for: DEMON_WAFFLES.
 *
 * ! @attention enemy MUST be of entity DEMON_WAFFLES.
 *
 * @param enemy The waffles enemy.
 */
static void RenderWafflesAttack(Entity* enemy);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

Entity EnemyStartup(Vector2 position, EnemyType type) {
    Entity enemy;
    enemy.pos           = position;
    enemy.direction     = Vector2Zero();
    enemy.faceValue     = 1;
    enemy.state         = IDLE;
    enemy.directionFace = RIGHT;
    enemy.speed         = GetSpeed(type);
    enemy.health        = GetHealth(type);

    int width  = GetWidth(type);
    int height = GetHeight(type);

    enemy.hitbox = (Rectangle){ .x      = enemy.pos.x,
                                .y      = enemy.pos.y + height / 2,
                                .width  = width,
                                .height = height / 2 };

    SetupEnemyAnimation(&enemy, type);
    TraceLog(LOG_INFO, "ENEMY.C (EnemyStartup): Enemy set successfully.");
    return enemy;
}

void EnemyMovement(Entity* enemy, Vector2* lastPlayerPos, EnemyType type) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (EnemyMovement, line: %d): NULL enemy was found.", __LINE__);
        return;
    }

    if(!IsPlayerSeen(enemy, type)) {
        if(IsVectorEqual(enemy->pos, *lastPlayerPos, 0.01f)) {
            enemy->pos   = *lastPlayerPos;
            enemy->state = IDLE;
        } else {
            MoveEnemyToPos(enemy, *lastPlayerPos, lastPlayerPos);
        }
        return;
    } else {
        *lastPlayerPos = player.pos;
    }

    MoveEnemyToPos(enemy, player.pos, lastPlayerPos);
}

void EnemyAttack(Entity* enemy, EnemyType type, bool* hasAttacked) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (EnemyAttack, line: %d): NULL enemy was found.", __LINE__);
        return;
    }

    float distance = Vector2Distance(enemy->pos, player.pos);
    Timer* timer   = &enemyAnimArray[ATTACK_ANIMATION].timer;

    if(distance <= ENEMY_ATTACK_RANGE && TimerDone(timer)) {
        *hasAttacked = false;
        enemy->state = MOVING;
        StartTimerWithDelay(timer, 0.5, 0.8);
    }

    if(CheckIfDelayed(timer)) return;

    enemy->state = ATTACKING;

    if(TimerDone(timer)) {
        enemy->state = IDLE;
    }

    if(enemyAnimArray[ATTACK_ANIMATION].curFrame == 1) {
        UpdateEnemyAttackHitbox(enemy, type);
        if(!(*hasAttacked)) {
            if(EntityAttack(enemy, &player, 1)) {
                *hasAttacked = true;
                TraceLog(LOG_INFO, "ENEMY.C (EnemyAttack): Player was hit by enemy.");
            }
        }
    }
}

void EnemyRender(Entity* enemy, EnemyType type) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (EnemyRender, line: %d): NULL enemy was found.", __LINE__);
        return;
    }

    int width  = GetWidth(type);
    int height = GetHeight(type);

    switch(enemy->state) {
        case IDLE:
            EntityRender(
                enemy, &enemyAnimArray[IDLE_ANIMATION],
                width * enemy->faceValue, height, 0, 0, 0.0f);
            break;
        case MOVING:
            EntityRender(
                enemy, &enemyAnimArray[MOVE_ANIMATION],
                width * enemy->faceValue, height, 0, 0, 0.0f);
            break;
        case ATTACKING: RenderEnemyAttack(enemy, type); break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (EnemyRender, line: %d): Invalid enemy state given.", __LINE__);
            break;
    }
}

void EnemyUnload(Entity* enemy) {
    if(enemy == NULL) {
        TraceLog(LOG_FATAL, "ENEMY.C (EnemyUnload, line: %d): NULL enemy was given.", __LINE__);
    }
    UnloadAnimationArray(&enemy->animations);

    TraceLog(LOG_INFO, "ENEMY.C (EnemyUnload): Enemy animations unloaded successfully.");
    TraceLog(LOG_INFO, "ENEMY.C (EnemyUnload): Enemy unloaded successfully.");
}

static void UpdateEnemyAttackHitbox(Entity* enemy, EnemyType type) {
    switch(type) {
        case DEMON_DIEGO:
        case DEMON_PABLO: LoadStandardEntityAttackHitbox(enemy); break;
        case DEMON_WAFFLES: LoadWafflesAttackHitbox(enemy); break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (UpdateEnemyAttackHitbox, line: %d): Invalid EnemyType was given.", __LINE__);
            break;
    }
}

static void RenderEnemyAttack(Entity* enemy, EnemyType type) {
    switch(type) {
        case DEMON_DIEGO:
        case DEMON_PABLO: RenderPabloDiegoAttack(enemy); break;
        case DEMON_WAFFLES: RenderWafflesAttack(enemy); break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (UpdateEnemyAttackHitbox, line: %d): Invalid EnemyType was given.", __LINE__);
            break;
    }
}

static bool IsPlayerSeen(Entity* enemy, EnemyType type) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (IsPlayerSeen, line: %d): NULL enemy was found.", __LINE__);
        return false;
    }

    // check if the enemy is in argo range
    float distance = Vector2Distance(player.pos, enemy->pos);
    if(distance > AGRO_RANGE) return false;

    // check if the vector from enemy to player is clear of any collisions
    float incrementAmount = 1 / distance;
    int width             = GetWidth(type);
    int height            = GetHeight(type);

    for(float i = 0; i < 1.0f; i += incrementAmount) {
        Vector2 playerCenter = { player.pos.x + ENTITY_TILE_WIDTH / 2,
                                 player.pos.y + ENTITY_TILE_HEIGHT / 2 };
        Vector2 enemyCenter = { enemy->pos.x + width / 2, enemy->pos.y + height / 2 };
        Vector2 resVec = Vector2Lerp(playerCenter, enemyCenter, i);

        CollisionNode* head = collidableTiles;
        while(head != NULL) {
            int x = (int) resVec.x / TILE_WIDTH;
            int y = (int) resVec.y / TILE_HEIGHT;

            if(x == head->collidedHitbox.index.x &&
               y == head->collidedHitbox.index.y) {
                return false;
            }
            head = head->next;
        }
    }
    return true;
}

static void SetupEnemyAnimation(Entity* enemy, EnemyType type) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (SetupEnemyAnimation, line: %d): NULL enemy was given.", __LINE__);
        return;
    }

    enemy->animations.size = MAX_ENEMY_ANIMATIONS;
    enemyAnimArray = (Animation*) malloc(sizeof(Animation) * enemy->animations.size);

    if(enemyAnimArray == NULL) {
        TraceLog(LOG_FATAL, "ENEMY.C (SetupEnemyAnimation, line: %d): Memory allocation failure.", __LINE__);
    }

    int width        = GetWidth(type);
    int height       = GetHeight(type);
    int attackWidth  = GetAttackWidth(type);
    int attackHeight = GetAttackHeight(type);
    int tiles[MAX_ENEMY_ANIMATIONS];
    GetTiles(tiles, MAX_ENEMY_ANIMATIONS, type);

    Animation idleEnemyAnimation =
        CreateAnimation(DEFAULT_IDLE_FPS, width, height, tiles[0]);

    Animation movingEnemyAnimation =
        CreateAnimation(DEFAULT_MOVING_FPS, width, height, tiles[1]);

    Animation attackEnemyAnimation =
        CreateAnimation(DEFAULT_ATTACK_FPS, attackWidth, attackHeight, tiles[2]);

    enemyAnimArray[IDLE_ANIMATION]   = idleEnemyAnimation;
    enemyAnimArray[MOVE_ANIMATION]   = movingEnemyAnimation;
    enemyAnimArray[ATTACK_ANIMATION] = attackEnemyAnimation;

    StartTimer(&enemyAnimArray[IDLE_ANIMATION].timer, -1.0);
    StartTimer(&enemyAnimArray[MOVE_ANIMATION].timer, -1.0);
}

static void MoveEnemyToPos(Entity* enemy, Vector2 position, Vector2* lastPlayerPos) {
    MoveEntityTowardsPos(enemy, position, lastPlayerPos);
}

static void LoadWafflesAttackHitbox(Entity* enemy) {
    int width  = ENEMY_WAFFLES_HEIGHT - 4;
    int height = ENEMY_WAFFLES_HEIGHT - 4;
    enemy->attack =
        (Rectangle){ .x = enemy->pos.x, .y = enemy->pos.y, .width = width, .height = height };
    enemy->attack.x += 0;
    enemy->attack.y += height / 4;

    switch(enemy->faceValue) {
        case 1: enemy->attack.x += -width / 4; break;
        default: break;
    }
    enemy->attack.x = floor(enemy->attack.x);
    enemy->attack.y = floor(enemy->attack.y);
}

static void RenderPabloDiegoAttack(Entity* enemy) {
    int width        = ENEMY_PABLO_WIDTH;
    int height       = ENEMY_PABLO_HEIGHT;
    int attackWidth  = ENEMY_PABLO_ATTACK_WIDTH;
    int attackHeight = ENEMY_PABLO_ATTACK_HEIGHT;

    EntityRender(enemy, &enemyAnimArray[IDLE_ANIMATION], width * enemy->faceValue, height, 0, 0, 0.0f);

    switch(enemy->directionFace) {
        case RIGHT:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], -attackWidth,
                attackHeight, width / 4, attackHeight / 2, 0.0f);
            break;
        case DOWN:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], -attackWidth,
                attackHeight * enemy->faceValue, width + width / 8, attackHeight, 90.0f);
            break;
        case LEFT:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], attackWidth,
                attackHeight, -width - width / 4, attackHeight / 2, 0.0f);
            break;
        case UP:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], -attackWidth,
                -attackHeight * enemy->faceValue, -width / 8,
                attackHeight + height / 8, -90.0f);
            break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (RenderPabloDiegoAttack, line: %d): Invalid enemy directionFace found.", __LINE__);
            break;
    }
}

static void RenderWafflesAttack(Entity* enemy) {
    int width        = ENEMY_WAFFLES_WIDTH;
    int height       = ENEMY_WAFFLES_HEIGHT;
    int attackWidth  = ENEMY_WAFFLES_ATTACK_WIDTH;
    int attackHeight = ENEMY_WAFFLES_ATTACK_HEIGHT;

    Animation idleAnimation = enemyAnimArray[IDLE_ANIMATION];
    idleAnimation.fps       = 10;

    switch(enemy->faceValue) {
        case 1:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], attackWidth,
                attackHeight, -attackWidth / 2 + width / 3, -height / 3, 0.0f);
            break;
        case -1:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], attackWidth,
                attackHeight, -width + width / 8, -height / 3, 0.0f);
            break;
        default: break;
    }
    // Render IDLE
    EntityRender(enemy, &idleAnimation, width * enemy->faceValue, height, 0, 0, 0.0f);
}