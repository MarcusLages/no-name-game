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
 * Renders an enemy's attack animation based off of it's Direction.
 *
 * TODO: Implement
 */
static void RenderEnemyAttack();

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
    enemy.speed         = 35;
    enemy.health        = 100;
    enemy.direction     = Vector2Zero();
    enemy.faceValue     = 1;
    enemy.state         = IDLE;
    enemy.directionFace = RIGHT;

    switch(type) {
        case DEMON_PABLO:
        case DEMON_DIEGO:
            enemy.hitbox = (Rectangle){ .x = enemy.pos.x,
                                        .y = enemy.pos.y + ENTITY_TILE_HEIGHT / 2,
                                        .width  = ENTITY_TILE_WIDTH,
                                        .height = ENTITY_TILE_HEIGHT / 2 };
            break;

        case WAFFLE_FRIES:
            // TODO: IMPLEMENT FOR WHEN DEMON WAFFLE FRIES IS READY TO PLAY
            break;
        default:
            TraceLog(LOG_WARNING, "enemy.c-EnemyStartup: Invalid EnemyType was given.");
            return (Entity){};
    }

    SetupEnemyAnimation(&enemy, type);
    return enemy;
}

void EnemyMovement(Entity* enemy, Vector2* lastPlayerPos) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "enemy.c-EnemyMovement: NULL enemy was found.");
        return;
    }

    if(!IsPlayerSeen(enemy)) {
        // DrawText("Player not seen", player.pos.x + 16, player.pos.y + 32, 10, RED);

        // Does not set to idle if precision is off.
        // TODO Needs to be fixed
        if(IsVectorEqual(enemy->pos, *lastPlayerPos, 0.01f)) {
            enemy->pos   = *lastPlayerPos;
            enemy->state = IDLE;
        } else {
            MoveEnemyToPos(enemy, *lastPlayerPos, lastPlayerPos);
        }
        // DrawText(
        //     TextFormat("State: %d", enemy->state), player.pos.x - 55,
        //     player.pos.y + 52, 10, RED);
        return;
    } else {
        *lastPlayerPos = player.pos;
    }

    MoveEnemyToPos(enemy, player.pos, lastPlayerPos);
}

// TODO: Implement
void EnemyAttack() {}

void EnemyRender(Entity* enemy) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "enemy.c-EnemyRender: NULL enemy was found.");
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
        case ATTACKING: break;
        default:
            TraceLog(LOG_WARNING, "enemy.c-EnemyRender: Invalid enemy state given.");
            break;
    }
}

// TODO: Implement
void RenderEnemyAttack() {}

void EnemyUnload(Entity* enemy) {
    if(enemy == NULL) {
        TraceLog(LOG_FATAL, "enemy-list.c-EnemyUnload: NULL enemy was given.");
        exit(EXIT_FAILURE);
    }
    UnloadAnimationArray(&enemy->animations);
}

static bool IsPlayerSeen(Entity* enemy) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "enemy.c-IsPlayerSeen: NULL enemy was found.");
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

        // DrawLineV(playerCenter, enemyCenter, RED);

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
        TraceLog(LOG_WARNING, "enemy.c-SetupEnemyAnimation: NULL enemy was given.");
        return;
    }

    Animation idleEnemyAnimation;
    Animation movingEnemyAnimation;
    Animation attackEnemyAnimation;

    enemy->animations.size = MAX_ENEMY_ANIMATIONS;
    enemy->animations.animationArr =
        (Animation*) malloc(sizeof(Animation) * enemy->animations.size);

    if(enemy->animations.animationArr == NULL) {
        TraceLog(LOG_FATAL, "enemy.c-SetupEnemyAnimation: Memory allocation failure.");
        exit(EXIT_FAILURE);
    }

    switch(type) {
        case DEMON_PABLO:
            idleEnemyAnimation =
                CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_IDLE);

            movingEnemyAnimation =
                CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_MOVE);

            attackEnemyAnimation =
                CreateAnimation(DEFAULT_ATTACK_FPS, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, TILE_ENEMY_ATTACK);
            break;
        case DEMON_DIEGO:
            // TODO: IMPLEMENT FOR WHEN DEMON DIEGO IS READY TO PLAY
            break;
        case WAFFLE_FRIES:
            // TODO: IMPLEMENT FOR WHEN DEMON WAFFLE FRIES IS READY TO PLAY
            break;
        default:
            TraceLog(LOG_WARNING, "enemy.c-SetupEnemyAnimation: Invalid EnemyType was given.");
            break;
    }

    enemy->animations.animationArr[IDLE_ANIMATION]   = idleEnemyAnimation;
    enemy->animations.animationArr[MOVE_ANIMATION]   = movingEnemyAnimation;
    enemy->animations.animationArr[ATTACK_ANIMATION] = attackEnemyAnimation;

    // Starting timers for both idle and moving animations
    StartTimer(&enemyAnimArray[IDLE_ANIMATION].timer, -1.0f);
    StartTimer(&enemyAnimArray[MOVE_ANIMATION].timer, -1.0f);
}

static void MoveEnemyToPos(Entity* enemy, Vector2 position, Vector2* lastPlayerPos) {
    MoveEntityTowardsPos(enemy, position, lastPlayerPos);
}