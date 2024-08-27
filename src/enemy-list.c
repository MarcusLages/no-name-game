/***********************************************************************************************
 *
 **   Provides functionality for handling multiple enemies.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include enemy-list.h
 *
 ***********************************************************************************************/

#include "../include/enemy-list.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

EnemyNode* enemies;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Adds a specified number of enemies to the enemies list with a given positionArray at random positions.
 *
 * @param numOfEnemies Number of enemies to add.
 * @param positionArray The array object holding the positions and the number of positions.
 *
 * ! @note Calls CreateEnemyList and AddEnemyNode.
 * ? @note Calls EnemyStartup on each enemy (see enemy.c).
 * ? @note Uses LoadRandomSequence from raylib.
 */
static void AddEnemies(int numOfEnemies, PositionArray positionArray);

/**
 * Creates the enemies linked list and returns a reference to the first EnemyNode in the list.
 *
 * @param enemy The enemy to add as the first EnemyNode.
 * @param type Type of enemy to add.
 *
 * @returns The reference to the first EnemyNode.
 *
 * ! @note Allocates memory for the first EnemyNode.
 */
static EnemyNode* CreateEnemyList(Entity enemy, EnemyType type);

/**
 * Creates an EnemyNode and adds it to the end of the enemies linked list.
 *
 * ! @attention Returns if there is a NULL head pointer.
 *
 * @param enemy The enemy to add as an EnemyNode.
 * @param type Type of enemy to add.
 *
 * ! @note Allocates memory for an EnemyNode.
 */
static void AddEnemyNode(Entity enemy, EnemyType type);

/**
 * Returns the number of enemies for a given roomSize.
 *
 * @param roomSize The roomSize to assess.
 *
 * @returns The number of enemies as an int.
 */
static int GetNumOfEnemies(RoomSize roomSize);

/**
 * Calculates a random value and returns an enemy type through a basic chance system.
 *
 * @returns An EnemyType.
 *
 * ? @note Uses GetRandomValue from raylib.
 */
static EnemyType GetRandomEnemyType();

/**
 * Adjusts the positions of all enemies to be clear from any obstacles.
 */
static void AdjustEnemies();

/**
 * Handles the movement of of each enemy in the list of enemies.
 *
 * ? @note Calls EnemyMovement on each enemy (see enemy.c).
 */
static void MoveEnemies();

/**
 * Handles enemy attack of each enemy in the list of enemies.
 *
 * ? @note Calls EnemyAttack on each enemy (see enemy.c).
 */
static void HandleEnemiesAttack();

/**
 * Returns the proper width of the given enemy type.
 *
 * @param type The enemy type.
 * @returns Returns the entity width.
 */
static int GetWidth(EnemyType type);

/**
 * Returns the proper height of the given enemy type.
 *
 * @param type The enemy type.
 * @returns Returns the entity height.
 */
static int GetHeight(EnemyType type);

/**
 * Returns an attack width associated with a given enemy type.
 *
 * @param type The enemy type.
 * @returns Returns the attack width.
 */
static int GetAttackWidth(EnemyType type);

/**
 * Returns an attack height associated with a given enemy type.
 *
 * @param type The enemy type.
 * @returns Returns the attack height.
 */
static int GetAttackHeight(EnemyType type);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void SetupEnemies() {
    RoomNode* cursor = rooms;
    while(cursor != NULL) {
        if(cursor->roomNumber != 0) {
            int numOfEnemies = GetNumOfEnemies(cursor->roomSize);
            AddEnemies(numOfEnemies, cursor->positionArray);
        }
        cursor = cursor->next;
    }
    AdjustEnemies();
    TraceLog(LOG_INFO, "ENEMY-LIST.C (SetupEnemies): Enemies set successfully.");
}

void UpdateEnemies() {
    // TODO: Temp. Checking player death is handled in dungeon.c the way Marcus did it. remove this.
    if(player.health <= 0) return;
    CleanUpEnemies();
    MoveEnemies();
    HandleEnemiesAttack();
}

void RenderEnemies() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        int attackWidth  = GetAttackWidth(currEnemy->type);
        int attackHeight = GetAttackHeight(currEnemy->type);
        int width        = GetWidth(currEnemy->type);
        int height       = GetHeight(currEnemy->type);
        EnemyRender(&currEnemy->enemy, width, height, attackWidth, attackHeight);
        currEnemy = currEnemy->next;
    }
}

void CleanUpEnemies() {
    EnemyNode* cursor = enemies;
    EnemyNode* prev   = NULL;
    while(cursor != NULL) {
        if(cursor->enemy.health <= 0) {
            if(prev == NULL) {
                enemies = cursor->next;
                EnemyUnload(&cursor->enemy);
                free(cursor);
                cursor = enemies;
            } else {
                EnemyUnload(&cursor->enemy);
                prev->next = cursor->next;
                free(cursor);
                cursor = prev->next;
            }
        } else {
            prev   = cursor;
            cursor = cursor->next;
        }
    }
}

void UnloadEnemies() {
    if(enemies == NULL) {
        TraceLog(LOG_WARNING, "ENEMY-LIST.C (UnloadEnemies, line: %d): Enemies list is empty or could not be found.", __LINE__);
    }

    while(enemies != NULL) {
        EnemyNode* temp = enemies;
        enemies         = enemies->next;

        EnemyUnload(&temp->enemy);

        free(temp);
        temp = NULL;
    }

    TraceLog(LOG_INFO, "ENEMY-LIST.C (UnloadEnemies): Enemies list unloaded successfully.");
}

static void AddEnemies(int numOfEnemies, PositionArray positionArray) {
    int* randNums = LoadRandomSequence(numOfEnemies, 0, positionArray.size - 1);
    Vector2* positions = positionArray.positions;

    for(int i = 0; i < numOfEnemies; i++) {
        int randomNum    = abs(randNums[i]);
        Vector2 position = positions[randomNum];

        EnemyType type = GetRandomEnemyType();
        Entity enemy   = EnemyStartup(
            (Vector2){ (float) position.x * TILE_WIDTH, (float) position.y * TILE_HEIGHT }, type);

        if(enemies == NULL) {
            enemies = CreateEnemyList(enemy, type);
        } else {
            AddEnemyNode(enemy, type);
        }
    }
    UnloadRandomSequence(randNums);
}

static EnemyNode* CreateEnemyList(Entity enemy, EnemyType type) {
    EnemyNode* enemyNode = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemyNode == NULL) {
        TraceLog(LOG_FATAL, "ENEMY-LIST.C (CreateEnemyList, line: %d): Memory allocation failure.", __LINE__);
    }

    enemyNode->enemy         = enemy;
    enemyNode->type          = type;
    enemyNode->lastPlayerPos = enemy.pos;
    enemyNode->next          = NULL;
    return enemyNode;
}

static void AddEnemyNode(Entity enemy, EnemyType type) {
    if(enemies == NULL) return;

    EnemyNode* enemyNode = CreateEnemyList(enemy, type);
    if(enemyNode == NULL) {
        TraceLog(LOG_FATAL, "ENEMY-LIST.C (AddEnemyNode, line: %d): Memory allocation failure.", __LINE__);
    }

    EnemyNode* cursor = enemies;
    while(cursor->next != NULL) {
        cursor = cursor->next;
    }

    cursor->next = enemyNode;
}

static int GetNumOfEnemies(RoomSize roomSize) {
    int numOfEnemies = 0;
    switch(roomSize) {
        case SMALL: numOfEnemies = SM_ROOM_MAX_ENEMIES; break;
        case MEDIUM: numOfEnemies = MD_ROOM_MAX_ENEMIES; break;
        case LARGE: numOfEnemies = LG_ROOM_MAX_ENEMIES; break;

        default:
            TraceLog(LOG_ERROR, "ENEMY-LIST.C (GetNumOfEnemies, line: %d): Invalid roomSize given. Defaulting to small size.", __LINE__);
            numOfEnemies = SM_ROOM_MAX_ENEMIES;
            break;
    }
    return numOfEnemies;
}

static EnemyType GetRandomEnemyType() {
    int value      = abs(GetRandomValue(0, 100));
    EnemyType type = DEMON_PABLO;

    if(value > 50 && value <= 80) {
        type = DEMON_DIEGO;
    } else if(value > 80 && value <= 100) {
        // TODO: Change to WAFFLES once ready.
        type = DEMON_DIEGO;
    }

    return type;
}

static void AdjustEnemies() {
    EnemyNode* cursor = enemies;
    while(cursor != NULL) {
        Vector2 diff = cursor->enemy.pos;
        int hitbox_X = cursor->enemy.hitbox.x;
        int hitbox_Y = cursor->enemy.hitbox.y;

        diff.x = diff.x - hitbox_X;
        diff.y = diff.y - hitbox_Y;

        cursor->enemy.pos = Vector2Add(cursor->enemy.pos, diff);
        cursor            = cursor->next;
    }
}

static void MoveEnemies() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        EnemyMovement(&currEnemy->enemy, &(currEnemy->lastPlayerPos));
        currEnemy = currEnemy->next;
    }
}

static void HandleEnemiesAttack() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        int attackWidth  = GetAttackWidth(currEnemy->type);
        int attackHeight = GetAttackHeight(currEnemy->type);
        EnemyAttack(&currEnemy->enemy, attackWidth, attackHeight);
        currEnemy = currEnemy->next;
    }
}

static int GetWidth(EnemyType type) {
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

static int GetHeight(EnemyType type) {
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

static int GetAttackWidth(EnemyType type) {
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

static int GetAttackHeight(EnemyType type) {
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
