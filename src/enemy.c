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
 * Renders an enemy's attack animation based off of it's Direction.
 */
// static void RenderEnemyAttack();

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

// TODO: enemy navigate around collision
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

        // Sets the enemy to IDLE if not in agro range.
        if(Vector2Length(player.pos) - Vector2Length(enemy->pos) > AGRO_RANGE) {
            enemy->state = IDLE;
            currEnemy    = currEnemy->next;
            continue;
        }

        // Delta time helps not let player speed depend on framerate.
        // It helps to take account for time between frames too.
        float deltaTime = GetFrameTime();

        if(player.pos.x > enemy->pos.x) {
            enemy->faceValue     = 1;
            enemy->directionFace = RIGHT;
        } else if(player.pos.x < enemy->pos.x) {
            enemy->faceValue     = -1;
            enemy->directionFace = LEFT;
        }

        if(player.pos.y > enemy->pos.y) {
            enemy->directionFace = DOWN;
        } else if(player.pos.y < enemy->pos.y) {
            enemy->directionFace = UP;
        }

        enemy->direction = (Vector2){
            (int) player.pos.x - (int) enemy->pos.x,
            (int) player.pos.y - (int) enemy->pos.y,
        };

        if(enemy->direction.x == 0 && enemy->direction.y == 0 && enemy->state != ATTACKING) {
            enemy->state = IDLE;
            currEnemy    = currEnemy->next;
            continue;
        }

        // Set the enemy to MOVING if not ATTACKING.
        enemy->state     = enemy->state == ATTACKING ? ATTACKING : MOVING;
        enemy->direction = Vector2Normalize(enemy->direction);

        //! NOTE: Do not add deltaTime before checking collisions only after.
        // Velocity:
        enemy->direction = Vector2Scale(enemy->direction, enemy->speed);

        EntityWorldCollision(enemy);

        enemy->pos = Vector2Add(enemy->pos, Vector2Scale(enemy->direction, deltaTime));
        currEnemy = currEnemy->next;
    }
}

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

//? Commented out for now.
// void RenderEnemyAttack() {}

void EnemyUnload() {
    UnloadEnemyList();
    AnimationUnload(&idleEnemyAnimation);
    AnimationUnload(&movingEnemyAnimation);
    AnimationUnload(&attackEnemyAnimation);
}

static void SetupEnemies() {
    int* randNumsX = LoadRandomSequence(10, 0, WORLD_WIDTH * TILE_WIDTH);
    int* randNumsY = LoadRandomSequence(10, 0, WORLD_HEIGHT * TILE_HEIGHT);
    for(int i = 0; i < MAX_ENEMIES; i++) {
        // TODO: Make a coordinate assigning system that places enemies at a correct x and y
        //? NOTE: LoadRandomSequence is a temp solution
        Entity* enemy = (Entity*) malloc(sizeof(Entity));

        if(enemy == NULL) exit(EXIT_FAILURE);

        enemy->pos           = (Vector2){ randNumsX[i], randNumsY[i] };
        enemy->speed         = 100;
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
    UnloadRandomSequence(randNumsY);
    UnloadRandomSequence(randNumsX);
}

static EnemyNode* CreateEnemyList(Entity* enemy) {
    if(enemy == NULL) return NULL;

    EnemyNode* enemyNode = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemyNode == NULL) exit(EXIT_FAILURE);

    enemyNode->enemy = enemy;
    enemyNode->next  = NULL;
    return enemyNode;
}

static void AddEnemyNode(EnemyNode* enemiesHead, Entity* enemy) {
    if(enemy == NULL || enemiesHead == NULL) return;

    EnemyNode* cursor    = enemiesHead;
    EnemyNode* enemyNode = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemyNode == NULL) exit(EXIT_FAILURE);

    enemyNode->enemy = enemy;
    enemyNode->next  = NULL;

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