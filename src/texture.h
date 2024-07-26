#ifndef TEXTURE_H
#define TEXTURE_H

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

//* DEFINITIONS
#define WORLD_WIDTH 50
#define WORLD_HEIGHT 50

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define MAX_TEXTURES 5

//* STRUCTURES
typedef struct Tile {
    int x;
    int y;
} Tile;

//* ENUMERATIONS
typedef enum TextureFile {
    TILE_MAP = 0,
    TILE_PLAYER_IDLE,
    TILE_PLAYER_MOVE,
    TILE_PLAYER_ATTACK,
    TILE_ENEMY
} TextureFile;

//* GLOBAL VARIABLES
extern Tile **world;
extern Texture2D *textures;

//------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Draws tiles at (x, y) from the tilemap from (x, y).
 * 
 * @param x_pos the x-coord to draw on
 * @param y_pos the y-coord to draw on
 * @param texture_tile_x the x-coord the tile in present on
 * @param texture_tile_y the y-coord the tile in present on
 */
void DrawTile(int x_pos, int y_pos, int texture_tile_x, int texture_tile_y, TextureFile tileTexture);

#endif //TEXTURE_H