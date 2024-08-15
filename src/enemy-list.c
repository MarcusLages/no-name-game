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

    EnemyNode* cursor    = enemies;
    EnemyNode* enemyNode = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemyNode == NULL) {
        TraceLog(LOG_FATAL, "enemy-list.c-AddEnemyNode: Memory allocation failure.");
        exit(EXIT_FAILURE);
    }

    enemyNode->enemy         = enemy;
    enemyNode->lastPlayerPos = enemy.pos;
    enemyNode->next          = NULL;

    while(cursor->next != NULL)
        cursor = cursor->next;

    cursor->next = enemyNode;
}

void UnloadEnemies() {
    if(enemies == NULL) {
        TraceLog(LOG_FATAL, "enemy-list.c-UnloadEnemies: Enemies list could not be found.");
        exit(EXIT_FAILURE);
    }

    while(enemies != NULL) {
        EnemyNode* temp = enemies;
        enemies         = enemies->next;

        EnemyUnload(&temp->enemy);

        free(temp);
        temp = NULL;
    }
}

void SetupEnemies() {
    //! NOTE: LoadRandomSequence does negative values too! min and max are just magnitudes use abs if needed!
    // int* randNumsX = LoadRandomSequence(MAX_ENEMIES, 0, WORLD_WIDTH * TILE_WIDTH);
    // int* randNumsY = LoadRandomSequence(MAX_ENEMIES, 0, WORLD_HEIGHT * TILE_HEIGHT);
    for(int i = 0; i < 1; i++) {
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
