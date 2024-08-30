/**********************************************************************************************
 *
 **   texture.h is responsible for dealing with general texture and tileset information.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
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
#define TILE_WIDTH  16
#define TILE_HEIGHT 16

/** How many textures are loaded in the game.  */
#define MAX_TEXTURES 13

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum for accessing the textures loaded in the game through
 * the textures.
 *
 * @param TILE_PLAYER_IDLE              0
 * @param TILE_PLAYER_MOVE              1
 * @param TILE_PLAYER_ATTACK            2
 * @param TILE_ENEMY_PABLO_IDLE         3
 * @param TILE_ENEMY_PABLO_MOVE         4
 * @param TILE_ENEMY_PABLO_ATTACK       5
 * @param TILE_ENEMY_DIEGO_IDLE         6
 * @param TILE_ENEMY_DIEGO_MOVE         7
 * @param TILE_ENEMY_DIEGO_ATTACK       8
 * @param TILE_ENEMY_WAFFLES_IDLE       9
 * @param TILE_ENEMY_WAFFLES_MOVE      10
 * @param TILE_ENEMY_WAFFLES_ATTACK    11
 * @param TILE_HEALTH_METER            12
 */
typedef enum TextureFile {
    TILE_PLAYER_IDLE = 0,
    TILE_PLAYER_MOVE,
    TILE_PLAYER_ATTACK,
    TILE_ENEMY_PABLO_IDLE,
    TILE_ENEMY_PABLO_MOVE,
    TILE_ENEMY_PABLO_ATTACK,
    TILE_ENEMY_DIEGO_IDLE,
    TILE_ENEMY_DIEGO_MOVE,
    TILE_ENEMY_DIEGO_ATTACK,
    TILE_ENEMY_WAFFLES_IDLE,
    TILE_ENEMY_WAFFLES_MOVE,
    TILE_ENEMY_WAFFLES_ATTACK,
    TILE_HEALTH_METER,
} TextureFile;

//* ------------------------------------------
//* GLOBAL VARIABLES

/** Pointer to an array of Texture2D's with all the textures loaded in the game on the moment. */
extern Texture2D* textures;

#endif // TEXTURE_H