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
 * Adds a specified enemy to the enemies list with a given pos.
 *
 * @param pos The position that the enemy should spawn.
 * @param type Type of enemy.
 *
 * ! @note Calls AddEnemyNode.
 * ? @note the given position must be a valid position on the map (position is not checked internally).
 */
static void AddParticularEnemy(Vector2 pos, EnemyType type);

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
    // 74, 10
    AddParticularEnemy((Vector2){ 14, 15 }, DEMON_WAFFLES);
    AdjustEnemies();
    UnloadRooms();
    TraceLog(LOG_INFO, "ENEMY-LIST.C (SetupEnemies): Enemies set successfully.");
}

void UpdateEnemies() {
    CleanUpEnemies();
    MoveEnemies();
    HandleEnemiesAttack();
}

void RenderEnemies() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        EnemyRender(&currEnemy->enemy, currEnemy->type);
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

static void AddParticularEnemy(Vector2 pos, EnemyType type) {
    Entity enemy =
        EnemyStartup((Vector2){ (float) pos.x * TILE_WIDTH, (float) pos.y * TILE_HEIGHT }, type);
    AddEnemyNode(enemy, type);
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

    if(value > 50 && value <= 100) {
        type = DEMON_DIEGO;
    }
    return type;
}

static void AdjustEnemies() {
    EnemyNode* cursor = enemies;
    while(cursor != NULL) {
        Vector2 diff   = cursor->enemy.pos;
        float hitbox_X = cursor->enemy.hitbox.x;
        float hitbox_Y = cursor->enemy.hitbox.y;

        diff.x = floorf(diff.x - hitbox_X);
        diff.y = floorf(diff.y - hitbox_Y);

        cursor->enemy.pos = Vector2Add(cursor->enemy.pos, diff);
        cursor            = cursor->next;
    }
}

static void MoveEnemies() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        EnemyMovement(&currEnemy->enemy, &(currEnemy->lastPlayerPos), currEnemy->type);
        currEnemy = currEnemy->next;
    }
}

static void HandleEnemiesAttack() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        EnemyAttack(&currEnemy->enemy, currEnemy->type);
        currEnemy = currEnemy->next;
    }
}
