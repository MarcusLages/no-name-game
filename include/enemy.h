/***********************************************************************************************
 *
 **   Provides definitions that are needed to manage an enemy.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.2
 *
 *    @include entity.h
 *
 ***********************************************************************************************/

#ifndef ENEMY_H_
#define ENEMY_H_

#include "entity.h"

#define MAX_ENEMY_ANIMATIONS 3

//* ------------------------------------------
//* DEFINITIONS

#define AGRO_RANGE 100

typedef enum EnemyType { DEMON_PABLO = 0, DEMON_DIEGO, WAFFLE_FRIES } EnemyType;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Initializes the enemy entity, animations, and timers.
 *
 * ? @note Timers started here are needed for the whole duration of the game.
 */
Entity EnemyStartup(Vector2 position, EnemyType type);

void SetupEnemyAnimation(Entity* enemy, EnemyType type);

/**
 * Handles enemy movement and updates it's GameState and Direction.
 */
void EnemyMovement(Entity* enemy, Vector2* lastPlayerPos);

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
void EnemyRender(Entity* enemy);

/**
 * Unloads all the enemies and animations associated with an enemy entity.
 */
void EnemyUnload(Entity* enemy);

#endif // ENEMY_H_