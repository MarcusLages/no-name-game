#ifndef TEXTURE_H
#define TEXTURE_H
#include "raylib.h"
#include "raymath.h"

//* DEFINITIONS
#define WORLD_WIDTH 50
#define WORLD_HEIGHT 50
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

//* STRUCTURES
typedef struct Tile {
    int x;
    int y;
} Tile;

//* GLOBAL VARIABLES
extern Tile **world;
extern Texture2D *textures;

#endif //TEXTURE_H