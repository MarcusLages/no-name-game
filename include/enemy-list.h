/***********************************************************************************************
 *
 **   Provides definitions for handling multiple enemies.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.3
 *
 *    @include enemy.h
 *
 ***********************************************************************************************/

#ifndef ENEMY_LIST_H_
#define ENEMY_LIST_H_

#include "enemy.h"

//* ------------------------------------------
//* STRUCTURES

/**
 * EnemyNode struct represents an enemy who has a reference to another enemy.
 *
 * @param enemy         Enemy entity of this node.
 * @param type          Enemy type of this node.
 * @param lastPlayerPos Last known location of player to this enemy.
 * @param hasAttacked   Indicates if this enemy has attacked.
 * @param next          Next enemy node.
 */
typedef struct EnemyNode EnemyNode;
struct EnemyNode {
    /** The enemy entity. */
    Entity enemy;
    /** The enemy type. */
    EnemyType type;
    /** The last known location of player relative to this enemy. */
    Vector2 lastPlayerPos;
    /** Indicates if this enemy has attacked. */
    bool hasAttacked;
    /** The pointer to the next enemy entity. */
    EnemyNode* next;
};

//* ------------------------------------------
//* GLOBAL VARIABLES

/** The list of all enemies. */
extern EnemyNode* enemies;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

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
 */
void SetupEnemies();

/**
 * Updates information required to move enemies and handle their attacks.
 * 
 * ! @attention Does not update enemies if the player has expired it's health points.
 * 
 * ? @note Calls MoveEnemies and HandleEnemiesAttack.
 */
void UpdateEnemies();

/**
 * Handles rendering each enemt in the list of enemies.
 *
 * ? @note Calls EnemyRender on each enemy (see enemy.c).
 */
void RenderEnemies();

/**
 * Deletes all enemies on the list that have less than or zero (0) health points.
 * 
 * ? @note May change enemies list to NULL (empty)
 */
void CleanUpEnemies();

#endif // ENEMY_LIST_H_