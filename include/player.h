/***********************************************************************************************
*
**   player.h is responsible for classifying processes needed to handle the player on startup, 
**   update, render, and unload.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.2
*
*    @include entity.h
*
***********************************************************************************************/

#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"

#define MAX_PLAYER_ANIMATIONS 3

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