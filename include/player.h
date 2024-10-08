/***********************************************************************************************
*
**   player.h is responsible for classifying processes needed to handle the player on startup, 
**   update, render, and unload.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.3
*
*    @include entity.h
*
***********************************************************************************************/

#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"

//* ------------------------------------------
//* DEFINITIONS

/** How many different animations the player has. */
#define MAX_PLAYER_ANIMATIONS 3

/** Player's health points. */
#define PLAYER_HEALTH 1

/** Player's speed. */
#define PLAYER_SPEED 100

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Initializes the player entity, animations, and timers. 
 * 
 * ? @note Timers started here are needed for the whole duration of the game.
 */
void PlayerStartup();

/**
 * Renders the player animation based off of it's GameState.
 */
void PlayerRender();

/**
 * Unloads all the animations associated with the player entity.
 */
void PlayerUnload();

/**
 * Updates information required to move the player and handle their attacks.
 * 
 * ? @note Calls PlayerMovement and PlayerAttack.
 */
void PlayerUpdate();

#endif //PLAYER_H_