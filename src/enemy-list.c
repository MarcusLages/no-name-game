#include "../include/enemy-list.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

EnemyNode* enemies;

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

EnemyNode* CreateEnemyList(Entity* enemy) {
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

void AddEnemyNode(EnemyNode* enemiesHead, Entity* enemy) {
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

void UnloadEnemies() {
    while(enemies != NULL) {
        EnemyNode* temp = enemies;
        enemies         = enemies->next;

        // Freeing information in an enemy
        EnemyUnload(temp->enemy);

        // Freeing enemy from memory
        free(temp->enemy);
        temp->enemy = NULL;

        // Freeing EnemyNode
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
        //* NOTE: change from ptr to memory to value
        Entity* enemy = (Entity*) malloc(sizeof(Entity));

        if(enemy == NULL) {
            TraceLog(LOG_FATAL, "enemy.c: Memory allocation failure.");
            exit(EXIT_FAILURE);
        }
        
        *enemy = EnemyStartup((Vector2){ (float) 21 * TILE_WIDTH, (float) 4 * TILE_HEIGHT }, DEMON_PABLO);


        if(enemies == NULL) {
            enemies = CreateEnemyList(enemy);
        } else {
            AddEnemyNode(enemies, enemy);
        }
    }
    // UnloadRandomSequence(randNumsY);
    // UnloadRandomSequence(randNumsX);
}

void MoveEnemies() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        EnemyMovement(currEnemy->enemy, &(currEnemy->lastPlayerPos));
        currEnemy = currEnemy->next;
    }
}

void RenderEnemies() {
    EnemyNode* currEnemy = enemies;
    while(currEnemy != NULL) {
        EnemyRender(currEnemy->enemy);
        currEnemy = currEnemy->next;
    }
}
