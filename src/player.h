#ifndef PLAYER_H_
#define PLAYER_H_

#include "entities.h"

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
 * Renders the player attack animation based off of it's Direction.
 */
void RenderPlayerAttack();

/**
 * Unloads all the animations associated with the player entity.
 */
void PlayerUnload();

#endif //PLAYER_H_