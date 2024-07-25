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

#define MAX_TEXTURES 3

//* STRUCTURES
typedef struct Tile {
    int x;
    int y;
} Tile;

//* ENUMERATIONS
typedef enum TextureFile {
    TILE_MAP = 0,
    TILE_PLAYER_IDLE,
    TILE_ENEMY
} TextureFile;

//* GLOBAL VARIABLES
extern Tile **world;
extern Texture2D *textures;

//------------------------------------------
//* FUNCTION PROTOTYPES
void DrawTile(int x_pos, int y_pos, int texture_tile_x, int texture_tile_y, TextureFile tileTexture);
void DrawEntityTile(int x_pos, int y_pos, int texture_tile_x, int texture_tile_y, TextureFile tileTexture);

#endif //TEXTURE_H