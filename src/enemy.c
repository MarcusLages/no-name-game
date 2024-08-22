/***********************************************************************************************
 *
 **   Provide functionality for setting enemy spawn points, managing movement, attack,
 **   and rendering of enemies and their animations.
 *
 *?   @note Spawn points are not yet implemented with new tmx level.
 *?   @note Slight bug with the enemy moving to last known player position.
 *
 *    TODO: Fix bug
 *    TODO: Implements spawn point generation
 *    TODO: Handle enemy attacks
 *    TODO: Handle player to enemy collisions
 *    TODO: make entity movement function and generalize player movement too
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include enemy.h and utils.h
 *
 ***********************************************************************************************/

#include "../include/enemy.h"
#include "../include/utils.h"

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
 *
 * @returns True if the player is seen and false otherwise.
 */
static bool IsPlayerSeen(Entity* enemy);

/**
 * Updates the given enemy's attack hitbox property.
 * 
 * @param enemy The enemy to update.
 */
static void UpdateEnemyAttackHitbox(Entity* enemy);

/**
 * Renders an enemy's attack animation based off of it's Direction.
 *
 * @param enemy The enemy to render an attack for.
 */
static void RenderEnemyAttack(Entity* enemy);

/**
 * Handles the enemy movement towards a given position.
 *
 * @param enemy The reference to the enemy to move.
 * @param position The position to move the entity towards.
 * @param lastPlayerPos The last known position of the player relative to the given enemy.
 *
 * ? @note Calls MoveEntityTowardsPos()
 */
static void MoveEnemyToPos(Entity* enemy, Vector2 position, Vector2* lastPlayerPos);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

Entity EnemyStartup(Vector2 position, EnemyType type) {
    Entity enemy;
    enemy.pos           = position;
    enemy.direction     = Vector2Zero();
    enemy.faceValue     = 1;
    enemy.state         = IDLE;
    enemy.directionFace = RIGHT;

    switch(type) {
        case DEMON_PABLO:
            enemy.speed  = ENEMY_PABLO_SPEED;
            enemy.health = ENEMY_PABLO_HEALTH;
            enemy.hitbox = (Rectangle){ .x = enemy.pos.x,
                                        .y = enemy.pos.y + ENTITY_TILE_HEIGHT / 2,
                                        .width  = ENTITY_TILE_WIDTH,
                                        .height = ENTITY_TILE_HEIGHT / 2 };
            break;
        case DEMON_DIEGO:
            enemy.speed     = ENEMY_DIEGO_SPEED;
            enemy.health    = ENEMY_DIEGO_HEALTH;
            enemy.direction = Vector2Zero();
            enemy.hitbox    = (Rectangle){ .x = enemy.pos.x,
                                           .y = enemy.pos.y + ENTITY_TILE_HEIGHT / 2,
                                           .width  = ENTITY_TILE_WIDTH,
                                           .height = ENTITY_TILE_HEIGHT / 2 };
            break;
        case DEMON_WAFFLES:
            // TODO: IMPLEMENT FOR WHEN DEMON WAFFLE IS READY TO PLAY
            break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (EnemyStartup, line: %d): Invalid EnemyType was given.", __LINE__);
            return (Entity){};
    }

    SetupEnemyAnimation(&enemy, type);
    TraceLog(LOG_INFO, "ENEMY.C (EnemyStartup): Enemy set successfully.");
    return enemy;
}

void EnemyMovement(Entity* enemy, Vector2* lastPlayerPos) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (EnemyMovement, line: %d): NULL enemy was found.", __LINE__);
        return;
    }

    if(!IsPlayerSeen(enemy)) {
        // Does not set to idle if precision is off.
        // TODO Needs to be fixed
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

// TODO: EnemyType is not considered yet for different attack widths.
void EnemyAttack(Entity* enemy) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (EnemyAttack, line: %d): NULL enemy was found.", __LINE__);
        return;
    }

    if(enemy->state == ATTACKING && TimerDone(&enemyAnimArray[ATTACK_ANIMATION].timer)) {
        enemy->state = IDLE;
        return;
    }

    UpdateEnemyAttackHitbox(enemy);

    if(EntityAttack(enemy, &player, player.health) && enemy->state != ATTACKING) {
        enemy->state = ATTACKING;
        StartTimer(&enemyAnimArray[ATTACK_ANIMATION].timer, 0.5f);

        TraceLog(LOG_INFO, "ENEMY.C (EnemyAttack, line: %d): Player was hit by enemy.");
    }
}

// TODO: EnemyType is not considered yet for different attack widths.
static void UpdateEnemyAttackHitbox(Entity* enemy) {
    enemy->attack = (Rectangle){ .x      = enemy->pos.x,
                                 .y      = enemy->pos.y,
                                 .width  = ENEMY_ATTACK_WIDTH - 4,
                                 .height = ENEMY_ATTACK_HEIGHT - 8 };

    // handles setting the attack hitbox based upon the direction enemy is facing
    switch(enemy->directionFace) {
        case RIGHT:
            // Attack hitbox offset
            enemy->attack.x += 3;
            enemy->attack.y += 13;
            break;
        case DOWN:
            // Attack hitbox offset
            SWAP(enemy->attack.width, enemy->attack.height);
            enemy->attack.x += 1;
            enemy->attack.y += 19;
            break;
        case LEFT:
            // Attack hitbox offset
            enemy->attack.x -= 15;
            enemy->attack.y += 13;
            break;
        case UP:
            // Attack hitbox offset
            SWAP(enemy->attack.width, enemy->attack.height);
            enemy->attack.x += 1;
            enemy->attack.y -= 0;
            break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (EnemyAttack, line: %d): Invalid directionFace was found.", __LINE__);
            break;
    }

    enemy->attack.x = floor(enemy->attack.x);
    enemy->attack.y = floor(enemy->attack.y);
}

// TODO: EnemyType is not considered yet for different widths.
void EnemyRender(Entity* enemy) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (EnemyRender, line: %d): NULL enemy was found.", __LINE__);
        return;
    }

    switch(enemy->state) {
        case IDLE:
            EntityRender(
                enemy, &enemyAnimArray[IDLE_ANIMATION],
                ENTITY_TILE_WIDTH * enemy->faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case MOVING:
            EntityRender(
                enemy, &enemyAnimArray[MOVE_ANIMATION],
                ENTITY_TILE_WIDTH * enemy->faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case ATTACKING: RenderEnemyAttack(enemy); break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (EnemyRender, line: %d): Invalid enemy state given.", __LINE__);
            break;
    }
}

// TODO: EnemyType is not considered yet for different attack widths.
static void RenderEnemyAttack(Entity* enemy) {
    // Rendering idle animation of enemy as the enemy should not move while attacking.
    EntityRender(
        enemy, &enemyAnimArray[IDLE_ANIMATION],
        ENTITY_TILE_WIDTH * enemy->faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);

    //? NOTE: commented out animations are kept for alternating animations
    switch(enemy->directionFace) {
        case RIGHT:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], ENEMY_ATTACK_WIDTH, -ENEMY_ATTACK_HEIGHT,
                ENEMY_ATTACK_WIDTH + 3, ENEMY_ATTACK_HEIGHT + 10, 180.0f);
            break;
        case DOWN:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], ENEMY_ATTACK_WIDTH,
                -ENEMY_ATTACK_HEIGHT * enemy->faceValue,
                ENEMY_ATTACK_WIDTH - 35, ENEMY_ATTACK_HEIGHT + 30, -90.0f);
            break;
        case LEFT:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], ENEMY_ATTACK_WIDTH, ENEMY_ATTACK_HEIGHT,
                ENEMY_ATTACK_WIDTH - 51, ENEMY_ATTACK_HEIGHT - 11, 0.0f);
            break;
        case UP:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], -ENEMY_ATTACK_WIDTH,
                -ENEMY_ATTACK_HEIGHT * enemy->faceValue,
                ENEMY_ATTACK_WIDTH - 35, ENEMY_ATTACK_HEIGHT + 7, -90.0f);
            break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (RenderEnemyAttack, line: %d): Invalid enemy directionFace found.", __LINE__);
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

static bool IsPlayerSeen(Entity* enemy) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (IsPlayerSeen, line: %d): NULL enemy was found.", __LINE__);
        return false;
    }

    // check if the enemy is in argo range
    float distance = Vector2Distance(player.pos, enemy->pos);
    if(distance > AGRO_RANGE) return false;

    // check if the vector from enemy to player is clear of any collisions
    float incrementAmount = 1 / distance;

    for(float i = incrementAmount; i < 1.0f; i += incrementAmount) {
        Vector2 playerCenter = { player.pos.x + ENTITY_TILE_WIDTH / 2,
                                 player.pos.y + ENTITY_TILE_HEIGHT / 2 };
        Vector2 enemyCenter  = { enemy->pos.x + ENTITY_TILE_WIDTH / 2,
                                 enemy->pos.y + ENTITY_TILE_HEIGHT / 2 };
        Vector2 resVec       = Vector2Lerp(playerCenter, enemyCenter, i);
        CollisionNode* head  = collidableTiles;

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

    Animation idleEnemyAnimation;
    Animation movingEnemyAnimation;
    Animation attackEnemyAnimation;

    enemy->animations.size = MAX_ENEMY_ANIMATIONS;
    enemyAnimArray = (Animation*) malloc(sizeof(Animation) * enemy->animations.size);

    if(enemyAnimArray == NULL) {
        TraceLog(LOG_FATAL, "ENEMY.C (SetupEnemyAnimation, line: %d): Memory allocation failure.", __LINE__);
    }

    switch(type) {
        case DEMON_PABLO:
            idleEnemyAnimation =
                CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_PABLO_IDLE);

            movingEnemyAnimation =
                CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_PABLO_MOVE);

            attackEnemyAnimation =
                CreateAnimation(DEFAULT_ATTACK_FPS, PLAYER_ATTACK_WIDTH, PLAYER_ATTACK_HEIGHT, TILE_ENEMY_PABLO_ATTACK);
            break;
        case DEMON_DIEGO:
            idleEnemyAnimation =
                CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_DIEGO_IDLE);

            movingEnemyAnimation =
                CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_DIEGO_MOVE);

            attackEnemyAnimation =
                CreateAnimation(DEFAULT_ATTACK_FPS, PLAYER_ATTACK_WIDTH, PLAYER_ATTACK_HEIGHT, TILE_ENEMY_DIEGO_ATTACK);
            break;
        case DEMON_WAFFLES:
            // TODO: IMPLEMENT FOR WHEN DEMON WAFFLE FRIES IS READY TO PLAY
            break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (SetupEnemyAnimation, line: %d): Invalid EnemyType was given.", __LINE__);
            break;
    }

    enemyAnimArray[IDLE_ANIMATION]   = idleEnemyAnimation;
    enemyAnimArray[MOVE_ANIMATION]   = movingEnemyAnimation;
    enemyAnimArray[ATTACK_ANIMATION] = attackEnemyAnimation;

    // Starting timers for both idle and moving animations
    StartTimer(&enemyAnimArray[IDLE_ANIMATION].timer, -1.0f);
    StartTimer(&enemyAnimArray[MOVE_ANIMATION].timer, -1.0f);
}

static void MoveEnemyToPos(Entity* enemy, Vector2 position, Vector2* lastPlayerPos) {
    MoveEntityTowardsPos(enemy, position, lastPlayerPos);
}