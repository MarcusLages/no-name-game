/**********************************************************************************************
*
**   texture.h is responsible for dealing with general texture and tileset texture information
**   and display.
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

/** Size of the world in tiles.  */
#define WORLD_WIDTH     50
#define WORLD_HEIGHT    50

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
//* STRUCTURES

/**
 * Struct representing a tile (a standard easily reproduced sprite).
 * 
 * @param x             X coordinate of the tile on the map.
 * @param y             Y coordinate of the tile on the map.
 * @param isCollidable  Checks if a tile is collidable or not.
 * 
 * ? @note Standard size of the tile can vary when using functions, but a standard
 *         size is defined as TILE_WIDTH/TILE_HEIGHT
 */
typedef struct Tile {
    int x;
    int y;
    bool isCollidable;
} Tile;

//* ------------------------------------------
//* GLOBAL VARIABLES

/** Pointer to a 2D array of Tile's representing the world displayed on the screen. */
extern Tile** world;

/** Pointer to an array of Texture2D's with all the textures loaded in the game on the moment. */
extern Texture2D* textures;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Draws tiles at (x, y) from the tilemap from (x, y).
 * 
 * @param xPos the x-coord to draw on
 * @param yPos the y-coord to draw on
 * @param textureTileX the x-coord the tile in present on
 * @param textureTileY the y-coord the tile in present on
 */
void DrawTile(int xPos, int yPos, int textureTileX, int textureTileY, TextureFile tileTexture);

#endif //TEXTURE_H