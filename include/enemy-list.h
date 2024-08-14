#ifndef ENEMY_LIST_H_
#define ENEMY_LIST_H_

#include "enemy.h"

//* ------------------------------------------
//* DEFINITIONS

#define MAX_ENEMIES 10

//* ------------------------------------------
//* STRUCTURES

/**
 * EnemyNode struct represents an enemy who has a reference to another enemy.
 *
 * @param enemy Enemy entity of this node.
 * @param next  Next enemy node.
 */
typedef struct EnemyNode EnemyNode;
struct EnemyNode {
    /** The pointer to an enemy entity. */
    Entity* enemy;
    /** The last known location of player relative to this enemy. */
    Vector2 lastPlayerPos;
    /** The pointer to the next enemy entity. */
    EnemyNode* next;
};

//* ------------------------------------------
//* GLOBAL VARIABLES

extern EnemyNode* enemies;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

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
EnemyNode* CreateEnemyList(Entity* enemy);

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
void AddEnemyNode(EnemyNode* enemiesHead, Entity* enemy);

/**
 * Unloads the list of enemies.
 *
 * ! @note Unallocates memory for each Entity and EntityNode.
 */
void UnloadEnemies();

/**
 * Populates the enemies linked list by creates entities around the level and
 * assigning them to an EnityNode.
 *
 * ! @note Allocates memory for each Entity.
 */
void SetupEnemies();

void MoveEnemies();

void RenderEnemies();

#endif // ENEMY_LIST_H_