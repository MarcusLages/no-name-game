/***********************************************************************************************
 *
 **   Provide functionality for setting enemy spawn points, managing movement, attack,
 **   and rendering of enemies and their animations.
 *
 *    TODO: Fix bug where by chance the enemy thinks its moving in the same position (float point errors)
 *    TODO: Handle player to enemy collisions
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
 * @param attackWidth Attack hitbox width.
 * @param attackHeight Attack hitbox height.
 */
static void UpdateEnemyAttackHitbox(Entity* enemy, int attackWidth, int attackHeight);

/**
 * Renders an enemy's attack animation based off of it's Direction.
 *
 * @param enemy The enemy to render an attack for.
 * @param attackWidth Attack hitbox width.
 * @param attackHeight Attack hitbox height.
 */
static void RenderEnemyAttack(Entity* enemy, int attackWidth, int attackHeight);

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

/**
 * TODO: Comment
 */
static void GetTiles(int tiles[], EnemyType type);

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
                                        .y = enemy.pos.y + ENEMY_PABLO_HEIGHT / 2,
                                        .width  = ENEMY_PABLO_WIDTH,
                                        .height = ENEMY_PABLO_HEIGHT / 2 };
            break;
        case DEMON_DIEGO:
            enemy.speed  = ENEMY_DIEGO_SPEED;
            enemy.health = ENEMY_DIEGO_HEALTH;
            enemy.hitbox = (Rectangle){ .x = enemy.pos.x,
                                        .y = enemy.pos.y + ENEMY_DEIGO_HEIGHT / 2,
                                        .width  = ENEMY_DEIGO_WIDTH,
                                        .height = ENEMY_DEIGO_HEIGHT / 2 };
            break;
        case DEMON_WAFFLES:
            enemy.speed  = ENEMY_WAFFLES_SPEED;
            enemy.health = ENEMY_WAFFLES_HEALTH;
            enemy.hitbox = (Rectangle){ .x = enemy.pos.x,
                                        .y = enemy.pos.y + ENEMY_WAFFLES_HEIGHT / 2,
                                        .width  = ENEMY_WAFFLES_WIDTH,
                                        .height = ENEMY_WAFFLES_HEIGHT / 2 };
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

void EnemyAttack(Entity* enemy, int attackWidth, int attackHeight) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (EnemyAttack, line: %d): NULL enemy was found.", __LINE__);
        return;
    }

    if(enemy->state == ATTACKING && TimerDone(&enemyAnimArray[ATTACK_ANIMATION].timer)) {
        enemy->state = IDLE;
        return;
    }

    UpdateEnemyAttackHitbox(enemy, attackWidth, attackWidth);

    if(EntityAttack(enemy, &player, 0) && enemy->state != ATTACKING) {
        Timer* timer = &enemyAnimArray[ATTACK_ANIMATION].timer;

        if(TimerDone(timer)) {
            // Start a new timer.
            StartTimerWithDelay(timer, 0.5, 0.5);
        }
        // Wait on delay
        if(CheckIfDelayed(timer)) return;
        enemy->state = ATTACKING;
        // player.health = 0;
        TraceLog(LOG_INFO, "ENEMY.C (EnemyAttack): Player was hit by enemy.");
    }
}

// TODO: watch out for waffles
static void UpdateEnemyAttackHitbox(Entity* enemy, int attackWidth, int attackHeight) {
    enemy->attack = (Rectangle){ .x      = enemy->pos.x,
                                 .y      = enemy->pos.y,
                                 .width  = attackWidth - 4,
                                 .height = attackHeight - 8 };

    // handles setting the attack hitbox based upon the direction enemy is facing
    switch(enemy->directionFace) {
            // Attack hitbox offset
        case RIGHT:
            enemy->attack.x += 3;
            enemy->attack.y += 13;
            break;
        case DOWN:
            SWAP(enemy->attack.width, enemy->attack.height);
            enemy->attack.x += 1;
            enemy->attack.y += 19;
            break;
        case LEFT:
            enemy->attack.x -= 15;
            enemy->attack.y += 13;
            break;
        case UP:
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

void EnemyRender(Entity* enemy, int width, int height, int attackWidth, int attackHeight) {
    if(enemy == NULL) {
        TraceLog(LOG_WARNING, "ENEMY.C (EnemyRender, line: %d): NULL enemy was found.", __LINE__);
        return;
    }

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
        case ATTACKING:
            RenderEnemyAttack(enemy, attackWidth, attackHeight);
            break;
        default:
            TraceLog(LOG_WARNING, "ENEMY.C (EnemyRender, line: %d): Invalid enemy state given.", __LINE__);
            break;
    }
}

// TODO: watch out for waffles
static void RenderEnemyAttack(Entity* enemy, int attackWidth, int attackHeight) {
    // Rendering idle animation of enemy as the enemy should not move while attacking.
    EntityRender(
        enemy, &enemyAnimArray[IDLE_ANIMATION],
        ENTITY_TILE_WIDTH * enemy->faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);

    //? NOTE: commented out animations are kept for alternating animations
    switch(enemy->directionFace) {
        case RIGHT:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], attackWidth,
                -attackHeight, attackWidth + 3, attackHeight + 10, 180.0f);
            break;
        case DOWN:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], attackWidth,
                -attackHeight * enemy->faceValue, attackWidth - 35,
                attackHeight + 30, -90.0f);
            break;
        case LEFT:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], attackWidth,
                attackHeight, attackWidth - 51, attackHeight - 11, 0.0f);
            break;
        case UP:
            EntityRender(
                enemy, &enemyAnimArray[ATTACK_ANIMATION], -attackWidth,
                -attackHeight * enemy->faceValue, attackWidth - 35,
                attackHeight + 7, -90.0f);
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

// TODO: watch out for waffles
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

    for(float i = 0; i < 1.0f; i += incrementAmount) {
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

    enemy->animations.size = MAX_ENEMY_ANIMATIONS;
    enemyAnimArray = (Animation*) malloc(sizeof(Animation) * enemy->animations.size);

    if(enemyAnimArray == NULL) {
        TraceLog(LOG_FATAL, "ENEMY.C (SetupEnemyAnimation, line: %d): Memory allocation failure.", __LINE__);
    }

    int width        = GetWidth(type);
    int height       = GetHeight(type);
    int attackWidth  = GetAttackWidth(type);
    int attackHeight = GetAttackHeight(type);
    int tiles[3];
    GetTiles(tiles, type);

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
    StartTimer(&enemyAnimArray[ATTACK_ANIMATION].timer, 1.0);
}

static void MoveEnemyToPos(Entity* enemy, Vector2 position, Vector2* lastPlayerPos) {
    MoveEntityTowardsPos(enemy, position, lastPlayerPos);
}

int GetWidth(EnemyType type) {
    int width = 0;

    switch(type) {
        case DEMON_PABLO: width = ENEMY_PABLO_WIDTH; break;
        case DEMON_DIEGO: width = ENEMY_DEIGO_WIDTH; break;
        case DEMON_WAFFLES: width = ENEMY_WAFFLES_WIDTH; break;

        default:
            TraceLog(LOG_ERROR, "ENEMY-LIST.C (GetAttackWidth, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            width = ENEMY_PABLO_WIDTH;
            break;
    }
    return width;
}

int GetHeight(EnemyType type) {
    int height = 0;

    switch(type) {
        case DEMON_PABLO: height = ENEMY_PABLO_HEIGHT; break;
        case DEMON_DIEGO: height = ENEMY_DEIGO_HEIGHT; break;
        case DEMON_WAFFLES: height = ENEMY_WAFFLES_HEIGHT; break;

        default:
            TraceLog(LOG_ERROR, "ENEMY-LIST.C (GetAttackHeight, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            height = ENEMY_PABLO_HEIGHT;
            break;
    }
    return height;
}

int GetAttackWidth(EnemyType type) {
    int width = 0;

    switch(type) {
        case DEMON_PABLO: width = ENEMY_PABLO_ATTACK_WIDTH; break;
        case DEMON_DIEGO: width = ENEMY_DEIGO_ATTACK_WIDTH; break;
        case DEMON_WAFFLES: width = ENEMY_WAFFLES_ATTACK_WIDTH; break;

        default:
            TraceLog(LOG_ERROR, "ENEMY-LIST.C (GetAttackWidth, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            width = ENEMY_PABLO_ATTACK_WIDTH;
            break;
    }
    return width;
}

int GetAttackHeight(EnemyType type) {
    int height = 0;

    switch(type) {
        case DEMON_PABLO: height = ENEMY_PABLO_ATTACK_HEIGHT; break;
        case DEMON_DIEGO: height = ENEMY_DEIGO_ATTACK_HEIGHT; break;
        case DEMON_WAFFLES: height = ENEMY_WAFFLES_ATTACK_HEIGHT; break;

        default:
            TraceLog(LOG_ERROR, "ENEMY-LIST.C (GetAttackHeight, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            height = ENEMY_PABLO_ATTACK_HEIGHT;
            break;
    }
    return height;
}

static void GetTiles(int* tiles, EnemyType type) {
    int tileNum = TILE_ENEMY_PABLO_IDLE;
    switch(type) {
        case DEMON_PABLO: tileNum = TILE_ENEMY_PABLO_IDLE; break;
        case DEMON_DIEGO: tileNum = TILE_ENEMY_DIEGO_IDLE; break;
        case DEMON_WAFFLES: tileNum = TILE_ENEMY_WAFFLES_IDLE; break;

        default:
            TraceLog(LOG_ERROR, "ENEMY-LIST.C (GetTiles, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            break;
    }
    for(int i = 0; i < MAX_ENEMY_ANIMATIONS; i++) {
        tiles[i] = tileNum++;
    };
}