/***********************************************************************************************
 *
 **   ...
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.1.0
 *
 *    @include entity.h
 *
 ***********************************************************************************************/

#ifndef ENEMY_H_
#define ENEMY_H_

#include "entity.h"

//* ------------------------------------------
//* DEFINITIONS

#define AGRO_RANGE 100
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
    /** The pointer to the next enemy entity. */
    EnemyNode* next;
};

//* ------------------------------------------
//* GLOBAL VARIABLES

/**
 * All enemy entities.
 *
 * ! @attention A global variable for now.
 */
extern EnemyNode* enemies;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Initializes the enemy entity, animations, and timers.
 *
 * ? @note Timers started here are needed for the whole duration of the game.
 */
void EnemyStartup();

/**
 * Handles enemy movement and updates it's GameState and Direction.
 */
void EnemyMovement();

/**
 * Handles the enemy's attack.
 *
 * ? @note Manages the timer for the enemy attack animation.
 * TODO: collisions, enemy health, etc..
 */
void EnemyAttack();

/**
 * Renders the enemy animation based off of it's GameState.
 */
void EnemyRender();

/**
 * Unloads all the enemies and animations associated with an enemy entity.
 */
void EnemyUnload();

#endif // ENEMY_H_