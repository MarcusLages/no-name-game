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

#define AGRO_RANGE 60

//* ------------------------------------------
//* STRUCTURES

/**
 * EnemyNode struct represents an enemy who has a reference to another enemy.
 *
 * @param enemy The enemy entity of this node.
 * @param next  The next enemy node.
 */
typedef struct EnemyNode EnemyNode;
struct EnemyNode {
    /** The enemy entity. */
    Entity enemy;
    /** The next enemy entity. */
    EnemyNode* next;
};

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Initializes the enemy entity, animations, and timers.
 *
 * ? @note Timers started here are needed for the whole duration of the game.
 */
void EnemyStartup();

/**
 * Manages enemies that are allowed to update their movement, update their
 * attack, and render on screen.
 */
void EnemyUpdate();

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