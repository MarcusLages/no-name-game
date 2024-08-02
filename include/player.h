/***********************************************************************************************
*
**   player.h is responsible for classifying processes needed to handle the player on startup, 
**   update, render, and unload.
*   
*    @authors Marcus Vinicius Lages Santos and Samarjit Bhogal
*    @version 1.0
*
*?   @note entity.h is used.
*
***********************************************************************************************/

#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Initializes the player entity, animations, and timers. 
 * 
 * ? @note Timers started here are needed for the whole duration of the game.
 */
void PlayerStartup();

/**
 * Handles player movement and updates it's GameState and Direction. 
 */
void PlayerMovement();

/**
 * Handles the player's attack.
 * 
 * ? @note Manages the timer for the player attack animation.
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