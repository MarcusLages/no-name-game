#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "raymath.h"

#define ENTITY_TILE_WIDTH 16
#define ENTITY_TILE_HEIGHT 32

#define ENTITY_IDLE_FPS 4

// Enum for the action state of entities for animation and properties.
typedef enum GameState {
    IDLE = 0,
    MOVING,
    ATTACKING
} GameState;

//* Player
// Structure to represent the player.
typedef struct Player {
    int x;
    int y;
    GameState state;
} Player;

//* Enemy
// Structure to represent the player.
typedef struct Enemy {
    int x;
    int y;
    GameState state;
    int health;
} Enemy;

// Structure to represent information needed for a Sprite Animation 
typedef struct Animation {
    int fps;
    int numOfRectangles;
    Rectangle *rectangles;
    Texture2D textures;
} Animation;

//* VARIABLES

extern Player player;

//------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Draws the provided animation at the destination rectangle. 
 * 
 * Assumes that the GameState has been checked for the entity and the animation desired is provided.
 * 
 * @param animation the animation profile as a structure of the sprite
 * @param dest the destination rectangle
 * @param origin the origin
 * @param rotation the rotation of the Rectangles to draw
 * @param color the tint profile
 */
void DrawAnimation(Animation animation, Rectangle dest, float rotation, Color color);

/**
 * 
 */
Animation CreateAnimation(int fps, int numOfRectangles, Rectangle *rectangles, Texture2D tiles);

/**
 * 
 */
void UnloadAnimation(Animation animation);

/**
 * 
 */
int FindNumOfTiles(int tileWidth, TextureFile textureFile);

/**
 * 
 */
Rectangle* GetSpriteRectangles(int numOfRectangles, int tileWidth, int tileHeight, TextureFile textureFile);

/**
 * Responsible for updating the Sprites
 */
void SpriteUpdate();

#endif // !ENTITIES_H