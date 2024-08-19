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
//* FUNCTION IMPLEMENTATIONS

EnemyNode* CreateEnemyList(Entity enemy) {
    EnemyNode* enemyNode = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemyNode == NULL) {
        TraceLog(LOG_FATAL, "enemy-list.c-CreateEnemyList: Memory allocation failure.");
        exit(EXIT_FAILURE);
    }

    enemyNode->enemy         = enemy;
    enemyNode->lastPlayerPos = enemy.pos;
    enemyNode->next          = NULL;
    return enemyNode;
}

void AddEnemyNode(Entity enemy) {
    if(enemies == NULL) return;

    EnemyNode* enemyNode = CreateEnemyList(enemy);

    EnemyNode* cursor = enemies;
    while(cursor->next != NULL) {
        cursor = cursor->next;
    }

    cursor->next = enemyNode;
}

void UnloadEnemies() {
    if(enemies == NULL) {
        TraceLog(LOG_INFO, "enemy-list.c-UnloadEnemies: There are no enemies.");
        return;
    }

    while(enemies != NULL) {
        EnemyNode* temp = enemies;
        enemies         = enemies->next;

        EnemyUnload(&temp->enemy);

        free(temp);
        temp = NULL;
    }
}

//! NOTE: LoadRandomSequence does negative values too! min and max are just magnitudes use abs if needed!
void SetupEnemies() {
    RoomNode* cursor = rooms;
    while(cursor != NULL) {
        if(cursor->roomNumber != 0) {
            int numOfEnemies = GetNumOfEnemies(cursor->roomSize);

            // Got random idxs based upon room size
            int* randNums =
                LoadRandomSequence(numOfEnemies, 0, cursor->positionArray.size - 1);

            AddEnemies(numOfEnemies, cursor->positionArray.positions, randNums);
            UnloadRandomSequence(randNums);
        }
        cursor = cursor->next;
    }
    AdjustEnemies();
}

void AdjustEnemies() {
    EnemyNode* cursor = enemies;
    while(cursor != NULL) {
        Vector2 diff = cursor->enemy.pos;
        int hitbox_X = cursor->enemy.hitbox.x;
        int hitbox_Y = cursor->enemy.hitbox.y;

        diff.x = diff.x - hitbox_X;
        diff.y = diff.y - hitbox_Y;

        cursor->enemy.pos = Vector2Add(cursor->enemy.pos, diff);

        cursor = cursor->next;
    }
}

int GetNumOfEnemies(RoomSize roomSize) {
    int numOfEnemies = 0;
    switch(roomSize) {
        case SMALL: numOfEnemies = SM_ROOM_MAX_ENEMIES; break;
        case MEDIUM: numOfEnemies = MD_ROOM_MAX_ENEMIES; break;
        case LARGE: numOfEnemies = LG_ROOM_MAX_ENEMIES; break;

        default:
            TraceLog(LOG_ERROR, "enemy-list.c-GetMaxEnemies: Invalid roomSize given. Defaulting to small size.");
            numOfEnemies = SM_ROOM_MAX_ENEMIES;
            break;
    }
    return numOfEnemies;
}

void AddEnemies(int numOfEnemies, Vector2* positions, int* randNums) {
    for(int i = 0; i < numOfEnemies; i++) {
        int randomNum    = abs(randNums[i]);
        Vector2 position = positions[randomNum];

        // TODO randomize enemy types.
        Entity enemy = EnemyStartup(
            (Vector2){ (float) position.x * TILE_WIDTH, (float) position.y * TILE_HEIGHT },
            DEMON_PABLO);

        if(enemies == NULL) {
            enemies = CreateEnemyList(enemy);
        } else {
            AddEnemyNode(enemy);
        }
    }
}

void MoveEnemies() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        EnemyMovement(&currEnemy->enemy, &(currEnemy->lastPlayerPos));
        currEnemy = currEnemy->next;
    }
}

void RenderEnemies() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        EnemyRender(&currEnemy->enemy);
        currEnemy = currEnemy->next;
    }
}
