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
        TraceLog(LOG_FATAL, "ENEMY-LIST.C (CreateEnemyList, line: %d): Memory allocation failure.", __LINE__);
    }

    enemyNode->enemy         = enemy;
    enemyNode->lastPlayerPos = enemy.pos;
    enemyNode->next          = NULL;
    return enemyNode;
}

void AddEnemyNode(Entity enemy) {
    if(enemies == NULL) return;

    EnemyNode* cursor    = enemies;
    EnemyNode* enemyNode = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemyNode == NULL) {
        TraceLog(LOG_FATAL, "ENEMY-LIST.C (AddEnemyNode, line: %d): Memory allocation failure.", __LINE__);
    }

    enemyNode->enemy         = enemy;
    enemyNode->lastPlayerPos = enemy.pos;
    enemyNode->next          = NULL;

    while(cursor->next != NULL)
        cursor = cursor->next;

    cursor->next = enemyNode;
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
            prev = cursor;
            cursor = cursor->next;
        }
    }
}

void UnloadEnemies() {
    if(enemies == NULL) {
        TraceLog(LOG_WARNING, "ENEMY-LIST.C (UnloadEnemies, line: %d): Enemies list is empty or could not be found.",__LINE__);
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

void SetupEnemies() {
    //! NOTE: LoadRandomSequence does negative values too! min and max are just magnitudes use abs if needed!
    // int* randNumsX = LoadRandomSequence(MAX_ENEMIES, 0, WORLD_WIDTH * TILE_WIDTH);
    // int* randNumsY = LoadRandomSequence(MAX_ENEMIES, 0, WORLD_HEIGHT * TILE_HEIGHT);
    for(int i = 0; i < MAX_ENEMIES; i++) {
        // TODO: Make a coordinate assigning system that places enemies at a correct x and y
        //? NOTE: LoadRandomSequence is a temp solution

        // TODO randomize enemy types.
        Entity enemy =
            EnemyStartup((Vector2){ (float) 21 * TILE_WIDTH, (float) 4 * TILE_HEIGHT }, DEMON_PABLO);

        if(enemies == NULL) {
            enemies = CreateEnemyList(enemy);
        } else {
            AddEnemyNode(enemy);
        }
    }
    // UnloadRandomSequence(randNumsY);
    // UnloadRandomSequence(randNumsX);

    TraceLog(LOG_INFO, "ENEMY-LIST.C (SetupEnemies): Enemies set successfully.");
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
