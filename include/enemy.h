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

//* ------------------------------------------
//* DEFINITIONS

#define AGRO_RANGE           100
#define MAX_ENEMY_ANIMATIONS 3

/** Health points for each enemy. */
// TODO: Once attack systems are up and running place proper health points here.
#define ENEMY_PABLO_HEALTH   1
#define ENEMY_DIEGO_HEALTH   1
#define ENEMY_WAFFLES_HEALTH 1

/** Speed rates for each enemy. */
#define ENEMY_PABLO_SPEED   40
#define ENEMY_DIEGO_SPEED   35
#define ENEMY_WAFFLES_SPEED 25

/** Enemy dimensions. */
#define ENEMY_PABLO_WIDTH  16
#define ENEMY_PABLO_HEIGHT 32

#define ENEMY_DEIGO_WIDTH  16
#define ENEMY_DEIGO_HEIGHT 32

#define ENEMY_WAFFLES_WIDTH  32
#define ENEMY_WAFFLES_HEIGHT 48

/** Attack dimensions. */
//? NOTE: Dimensions are the same for now as sprite is the same.
//? Testing will be need once different dimensions and sprites are given.
#define ENEMY_PABLO_ATTACK_WIDTH  32
#define ENEMY_PABLO_ATTACK_HEIGHT 21

#define ENEMY_DEIGO_ATTACK_WIDTH  32
#define ENEMY_DEIGO_ATTACK_HEIGHT 21

#define ENEMY_WAFFLES_ATTACK_WIDTH  32
#define ENEMY_WAFFLES_ATTACK_HEIGHT 21

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum to treat different enemies based on their type.
 *
 * @param DEMON_PABLO   0
 * @param DEMON_DIEGO   1
 * @param DEMON_WAFFLES 2
 */
typedef enum EnemyType {
    DEMON_PABLO = 0,
    DEMON_DIEGO,
    DEMON_WAFFLES
} EnemyType;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Creates an instance of an enemy with the given position and type and returns
 * it. Starts any timers that need to run forever.
 *
 * ! @attention returns an EMPTY Entity when given an invalid type.
 *
 * @param position The initial position to set for the enemy.
 * @param type The type of enemy to start.
 * @returns An Entity.
 *
 * ? @note Timers started here are needed for the whole duration of the game.
 */
Entity EnemyStartup(Vector2 position, EnemyType type);

/**
 * Handles enemy movement of the given enemy and updates it's GameState and Direction.
 *
 * ! @attention returns when given a NULL enemy reference.
 *
 * @param enemy The enemy to handle movement.
 * @param lastPlayerPos The last known location of the player.
 *
 * ? @note Needs a reference to the lastPlayerPos of the given enemy.
 */
void EnemyMovement(Entity* enemy, Vector2* lastPlayerPos);

/**
 * Handles the given enemy's attack.
 *
 * ! @attention returns if the enemy is NULL, has an invalid state or if it is no longer attacking.
 *
 * @param enemy The reference to the enemy to handle the attack for.
 * @param attackWidth Attack hitbox width.
 * @param attackHeight Attack hitbox height.
 *
 * ? @note Manages the timer for the enemy attack animation.
 * ? @note Calls UpdateEnemyAttackHitbox to update the given enemy's attack hotbox.
 * ? @note Calls EntityAttack to handle enemy attack if the hitboxes intersect.
 */
void EnemyAttack(Entity* enemy, int attackWidth, int attackHeight);

/**
 * Renders the enemy animation based off of it's GameState.
 *
 * ! @attention returns if the enemy is NULL or has an invalid state.
 *
 * @param enemy The reference to the enemy to render.
 * @param width Enemy width.
 * @param height Enemy height.
 * @param attackWidth Attack hitbox width.
 * @param attackHeight Attack hitbox height.
 */
void EnemyRender(Entity* enemy, int width, int height, int attackWidth, int attackHeight);

/**
 * Unloads all the enemies and animations associated with an enemy entity.
 *
 * ! @attention exits the program if given a NULL enemy reference.
 *
 * @param enemy The reference to the enemy to render.
 *
 * ! @note Unallocates memory for the array in enemy.animations.
 */
void EnemyUnload(Entity* enemy);

/**
 * Returns the proper width of the given enemy type.
 *
 * @param type The enemy type.
 * @returns Returns the entity width.
 */
int GetWidth(EnemyType type);

/**
 * Returns the proper height of the given enemy type.
 *
 * @param type The enemy type.
 * @returns Returns the entity height.
 */
int GetHeight(EnemyType type);

/**
 * Returns an attack width associated with a given enemy type.
 *
 * @param type The enemy type.
 * @returns Returns the attack width.
 */
int GetAttackWidth(EnemyType type);

/**
 * Returns an attack height associated with a given enemy type.
 *
 * @param type The enemy type.
 * @returns Returns the attack height.
 */
int GetAttackHeight(EnemyType type);

#endif // ENEMY_H_