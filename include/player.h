#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

/** The animation for an idle player. */
extern Animation* idlePlayerAnimation;

/** The animation for the player moving. */
extern Animation* movingPlayerAnimation;

/** The animation for a player attack. */
extern Animation* attackPlayerAnimation;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Initializes the player entity, animations, and timers. 
 * 
 * ? Timers started here are needed for the whole duration of the game.
 */
void PlayerStartup();

/**
 * Handles player movement and updates it's GameState and Direction. 
 */
void PlayerMovement();

/**
 * Handles the player's attack.
 * 
 * ? Manages the timer for the player attack animation.
 * TODO: collisions, enemy health, etc..
 */
void PlayerAttack();

/**
 * Renders the player animation based off of it's GameState.
 */
void PlayerRender();

/**
 * Unloads all the animations associated with the player entity.
 */
void PlayerUnload();

#endif //PLAYER_H_