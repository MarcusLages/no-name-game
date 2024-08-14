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
 *    TODO: Rearrange functions into seprate files where needed. Move enemies variable to entity.h.
 *    TODO: Handle enemy attacks
 *    TODO: Handle player to enemy collisions
 *    TODO: make entitymovement function and generalize player movement too
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
//* GLOBAL VARIABLES

EnemyNode* enemies;

//* ------------------------------------------
//* MODULAR VARIABLES

/** The animation for an idle enemy. */
static Animation idleEnemyAnimation;

/** The animation for the enemy moving. */
static Animation movingEnemyAnimation;

/** The animation for a enemy attack. */
static Animation attackEnemyAnimation;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Populates the enemies linked list by creates entities around the level and
 * assigning them to an EnityNode.
 *
 * ! @note Allocates memory for each Entity.
 */
static void SetupEnemies();

/**
 * Creates the enemies linked list and returns a reference to the first EnemyNode in the list.
 *
 * ! @attention Returns NULL if given a NULL enemy.
 *
 * @param enemy The enemy to add as the first EnemyNode.
 *
 * @returns The reference to the first EnemyNode.
 *
 * ! @note Allocates memory for the first EnemyNode.
 */
static EnemyNode* CreateEnemyList(Entity* enemy);

/**
 * Creates an EnemyNode and adds it to the end of the enemies linked list.
 *
 * ! @attention Returns if given a NULL enemy or NULL head pointer.
 *
 * @param enemiesHead The head pointer to the linked list of enemies.
 * @param enemy The enemy to add as an EnemyNode.
 *
 * ! @note Allocates memory for the EnemyNode.
 */
static void AddEnemyNode(EnemyNode* enemiesHead, Entity* enemy);

/**
 * Determines if the player is seen by a given enemy.
 *
 * ! @attention Returns -1 if given a NULL enemy.
 *
 * @param enemy The enemy to check agaist.
 *
 * @returns True if the player is seen and false otherwise.
 */
static bool IsPlayerSeen(Entity* enemy);

/**
 * Handles the enemy movement towards a given position.
 *
 * ! @attention Returns if given a NULL enemyNode or zero Vector.
 *
 * @param enemyNode The current enemy to move as a EnemyNode.
 * @param position The position to move the enemy towards.
 */
static void MoveEnemyTowardsPos(EnemyNode* enemyNode, Vector2 position);

/**
 * Renders an enemy's attack animation based off of it's Direction.
 * 
 * TODO: Implement
 */
static void RenderEnemyAttack();

/**
 * Unloads the list of enemies.
 *
 * ! @note Unallocates memory for each Entity and EntityNode.
 */
static void UnloadEnemyList();

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void EnemyStartup() {
    // Initializing the idle animation
    idleEnemyAnimation =
        CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_IDLE);

    // Initializing the moving animation
    movingEnemyAnimation =
        CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_MOVE);

    // Initializing the attacking animation
    attackEnemyAnimation =
        CreateAnimation(DEFAULT_ATTACK_FPS, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, TILE_ENEMY_ATTACK);

    enemies = NULL;

    // Starting timers for both idle and moving animations
    StartTimer(&idleEnemyAnimation.timer, -1.0f);
    StartTimer(&movingEnemyAnimation.timer, -1.0f);

    SetupEnemies();
}

void EnemyMovement() {
    EnemyNode* currEnemy = enemies;
    Entity* enemy        = NULL;
    while(currEnemy != NULL) {
        enemy = currEnemy->enemy;

        // Ensures the enemy cannot move while attacking
        if(enemy->state == ATTACKING) {
            currEnemy = currEnemy->next;
            continue;
        }

        if(!IsPlayerSeen(enemy)) {
            // DrawText("Player not seen", player.pos.x + 16, player.pos.y + 32, 10, RED);

            // Does not set to idle if precision is off.
            // TODO Needs to be fixed
            if(IsVectorEqual(enemy->pos, currEnemy->lastPlayerPos, 0.01f)) {
                enemy->pos   = currEnemy->lastPlayerPos;
                enemy->state = IDLE;
            } else {
                MoveEnemyTowardsPos(currEnemy, currEnemy->lastPlayerPos);
            }
            // DrawText(
            //     TextFormat("State: %d", enemy->state), player.pos.x - 55,
            //     player.pos.y + 52, 10, RED);
            currEnemy = currEnemy->next;
            continue;
        } else {
            currEnemy->lastPlayerPos = player.pos;
        }

        MoveEnemyTowardsPos(currEnemy, player.pos);
        currEnemy = currEnemy->next;
    }
}

//TODO: Implement
void EnemyAttack() {}

void EnemyRender() {
    EnemyNode* currEnemy = enemies;
    Entity* enemy        = NULL;
    while(currEnemy != NULL) {
        enemy = currEnemy->enemy;
        switch(enemy->state) {
            case IDLE:
                EntityRender(
                    enemy, &idleEnemyAnimation, ENTITY_TILE_WIDTH * enemy->faceValue,
                    ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
                break;
            case MOVING:
                EntityRender(
                    enemy, &movingEnemyAnimation, ENTITY_TILE_WIDTH * enemy->faceValue,
                    ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
                break;
            case ATTACKING: break;
            default: break;
        }
        currEnemy = currEnemy->next;
    }
}

// TODO: Implement
void RenderEnemyAttack() {}

void EnemyUnload() {
    UnloadEnemyList();
    AnimationUnload(&idleEnemyAnimation);
    AnimationUnload(&movingEnemyAnimation);
    AnimationUnload(&attackEnemyAnimation);
}

static bool IsPlayerSeen(Entity* enemy) {
    if(enemy == NULL) return -1;

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

static void MoveEnemyTowardsPos(EnemyNode* enemyNode, Vector2 position) {
    if(enemyNode == NULL || Vector2Equals(position, Vector2Zero())) return;

    Entity* enemy = enemyNode->enemy;
    if(position.x > enemy->pos.x) {
        enemy->faceValue     = 1;
        enemy->directionFace = RIGHT;
    } else if(position.x < enemy->pos.x) {
        enemy->faceValue     = -1;
        enemy->directionFace = LEFT;
    }

    if(position.y > enemy->pos.y) {
        enemy->directionFace = DOWN;
    } else if(position.y < enemy->pos.y) {
        enemy->directionFace = UP;
    }

    enemy->direction = (Vector2){
        (int) position.x - (int) enemy->pos.x,
        (int) position.y - (int) enemy->pos.y,
    };

    if(Vector2Equals(enemy->direction, Vector2Zero()) && enemy->state != ATTACKING) {
        enemy->state             = IDLE;
        enemyNode->lastPlayerPos = enemy->pos;
        return;
    }

    // Delta time helps not let player speed depend on framerate.
    // It helps to take account for time between frames too.
    //! NOTE: Do not add deltaTime before checking collisions only after.
    float deltaTime = GetFrameTime();

    // Set the enemy to MOVING if not ATTACKING.
    enemy->state     = enemy->state == ATTACKING ? ATTACKING : MOVING;
    enemy->direction = Vector2Normalize(enemy->direction);

    // Velocity:
    enemy->direction = Vector2Scale(enemy->direction, enemy->speed);

    EntityWorldCollision(enemy);
    if(Vector2Equals(enemy->direction, Vector2Zero())) {
        enemy->state             = IDLE;
        enemyNode->lastPlayerPos = enemy->pos;
        return;
    }

    enemy->pos = Vector2Add(enemy->pos, Vector2Scale(enemy->direction, deltaTime));
}

static void SetupEnemies() {
    //! NOTE: LoadRandomSequence does negative values too! min and max are just magnitudes use abs if needed!
    // int* randNumsX = LoadRandomSequence(MAX_ENEMIES, 0, WORLD_WIDTH * TILE_WIDTH);
    // int* randNumsY = LoadRandomSequence(MAX_ENEMIES, 0, WORLD_HEIGHT * TILE_HEIGHT);
    for(int i = 0; i < 1; i++) {
        // TODO: Make a coordinate assigning system that places enemies at a correct x and y
        //? NOTE: LoadRandomSequence is a temp solution
        Entity* enemy = (Entity*) malloc(sizeof(Entity));

        if(enemy == NULL) {
            TraceLog(LOG_FATAL, "enemy.c: Memory allocation failure.");
            exit(EXIT_FAILURE);
        }

        enemy->pos = (Vector2){ (float) 21 * TILE_WIDTH, (float) 4 * TILE_HEIGHT };
        enemy->hitbox        = (Rectangle){ .x = enemy->pos.x,
                                            .y = enemy->pos.y + ENTITY_TILE_HEIGHT / 2,
                                            .width  = ENTITY_TILE_WIDTH,
                                            .height = ENTITY_TILE_HEIGHT / 2 };
        enemy->speed         = 35;
        enemy->health        = 100;
        enemy->direction     = Vector2Zero();
        enemy->faceValue     = 1;
        enemy->state         = IDLE;
        enemy->directionFace = RIGHT;

        if(enemies == NULL) {
            enemies = CreateEnemyList(enemy);
        } else {
            AddEnemyNode(enemies, enemy);
        }
    }
    // UnloadRandomSequence(randNumsY);
    // UnloadRandomSequence(randNumsX);
}

static EnemyNode* CreateEnemyList(Entity* enemy) {
    if(enemy == NULL) return NULL;

    EnemyNode* enemyNode = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemyNode == NULL) {
        TraceLog(LOG_FATAL, "enemy.c: Memory allocation failure.");
        exit(EXIT_FAILURE);
    }

    enemyNode->enemy         = enemy;
    enemyNode->lastPlayerPos = enemy->pos;
    enemyNode->next          = NULL;
    return enemyNode;
}

static void AddEnemyNode(EnemyNode* enemiesHead, Entity* enemy) {
    if(enemy == NULL || enemiesHead == NULL) return;

    EnemyNode* cursor    = enemiesHead;
    EnemyNode* enemyNode = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemyNode == NULL) {
        TraceLog(LOG_FATAL, "enemy.c: Memory allocation failure.");
        exit(EXIT_FAILURE);
    }

    enemyNode->enemy         = enemy;
    enemyNode->lastPlayerPos = enemy->pos;
    enemyNode->next          = NULL;

    while(cursor->next != NULL)
        cursor = cursor->next;

    cursor->next = enemyNode;
}

static void UnloadEnemyList() {
    while(enemies != NULL) {
        EnemyNode* temp = enemies;
        enemies         = enemies->next;
        free(temp->enemy);
        temp->enemy = NULL;
        free(temp);
        temp = NULL;
    }
}