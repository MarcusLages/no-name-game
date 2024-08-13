/**********************************************************************************************
*
**   texture.h is responsible for dealing with general texture and tileset information.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.1
*
*    @include raylib.h
*    @cite raylib
*
**********************************************************************************************/

#ifndef TEXTURE_H
#define TEXTURE_H

#include "raylib.h"

//* ------------------------------------------
//* DEFINITIONS

/** Size of one world tile in pixels.  */
#define TILE_WIDTH      16
#define TILE_HEIGHT     16

/** How many textures are loaded in the game.  */
#define MAX_TEXTURES    7

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum for accessing the textures loaded in the game through
 * the textures.
 * 
 * @param TILE_MAP              0
 * @param TILE_PLAYER_IDLE      1
 * @param TILE_PLAYER_MOVE      2
 * @param TILE_PLAYER_ATTACK    3
 * @param TILE_ENEMY_IDLE       4
 * @param TILE_ENEMY_MOVE       5
 * @param TILE_ENEMY_ATTACK     6
 */
typedef enum TextureFile {
    TILE_MAP = 0,
    TILE_PLAYER_IDLE,
    TILE_PLAYER_MOVE,
    TILE_PLAYER_ATTACK,
    TILE_ENEMY_IDLE,
    TILE_ENEMY_MOVE,
    TILE_ENEMY_ATTACK
} TextureFile;

//* ------------------------------------------
//* GLOBAL VARIABLES

/** Pointer to an array of Texture2D's with all the textures loaded in the game on the moment. */
extern Texture2D* textures;

#endif //TEXTURE_H