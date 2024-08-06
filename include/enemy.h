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

#define AGRO_X 60
#define AGRO_Y 60

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
 * Unloads all the animations associated with the enemy entity.
 */
void EnemyUnload();

#endif // ENEMY_H_