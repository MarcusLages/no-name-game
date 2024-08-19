/***********************************************************************************************
 *
 **   Provides definitions for handling multiple enemies.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include enemy.h
 *
 ***********************************************************************************************/

#ifndef ENEMY_LIST_H_
#define ENEMY_LIST_H_

#include "../include/spawner.h"
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
 * @param lastPlayerPos Last known location of player to this enemy.
 * @param next  Next enemy node.
 */
typedef struct EnemyNode EnemyNode;
struct EnemyNode {
    /** The enemy entity. */
    Entity enemy;
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
EnemyNode* CreateEnemyList(Entity enemy);

/**
 * Creates an EnemyNode and adds it to the end of the enemies linked list.
 *
 * ! @attention Returns if given a NULL enemy or NULL head pointer.
 *
 * @param enemy The enemy to add as an EnemyNode.
 *
 * ! @note Allocates memory for an EnemyNode.
 */
void AddEnemyNode(Entity enemy);

/**
 * Unloads the list of enemies.
 *
 * ! @attention Exits the program if the list of enemies is not found.
 * ! @note Unallocates memory for each EntityNode.
 */
void UnloadEnemies();

/**
 * Populates the enemies linked list by creates entities around the level and
 * assigning them to an EnityNode.
 *
 * ! @note Calls CreateEnemyList and AddEnemyNode.
 * ? @note Calls EnemyStartup on each enemy (see enemy.c).
 */
void SetupEnemies();

/**
 * TODO: comment
 */
void AdjustEnemies();

/**
 * TODO: comment
 */
int GetNumOfEnemies(RoomSize roomSize);

/**
 * TODOL comment
 */
void AddEnemies(int numOfEnemies, Vector2* positions, int* randNums);

/**
 * Handles the movement of of each enemy in the list of enemies.
 *
 * ? @note Calls EnemyMovement on each enemy (see enemy.c).
 */
void MoveEnemies();

/**
 * Handles rendering each enemt in the list of enemies.
 *
 * ? @note Calls EnemyRender on each enemy (see enemy.c).
 */
void RenderEnemies();

#endif // ENEMY_LIST_H_