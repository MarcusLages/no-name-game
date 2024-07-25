#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "raymath.h"
#include "texture.h"

#define ENTITY_TILE_WIDTH 16
#define ENTITY_TILE_HEIGHT 32

#define ENTITY_IDLE_FPS 4

// Enum for the action state of entities for animation and properties.
typedef enum GameState {
    IDLE = 0,
    MOVING,
    ATTACKING
} GameState;

//* Entity
// Structure to represent a player / enemy.
typedef struct Entity {
    int x;
    int y;
    int speed;
    int health;
    GameState state;
} Entity;

// Structure to represent information needed for a Sprite Animation 
typedef struct Animation {
    int fps;
    int numOfRectangles;
    Rectangle *rectangles;
    Texture2D textures;
} Animation;

//* VARIABLES

extern Entity player;
// The animation for an idle player
extern Animation idlePlayerAnimation;

//------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Draws the provided animation at the destination rectangle. 
 * 
 * Assumes that the GameState has been checked for the entity and the animation desired is provided.
 * 
 * @param animation the animation profile as a structure of the sprite.
 * @param dest the destination rectangle.
 * @param origin the origin.
 * @param rotation the rotation of the Rectangles to draw.
 * @param color the tint profile.
 * 
 * Uses DrawTexturePro from Raylib.
 */
void DrawAnimation(Animation animation, Rectangle dest, float rotation, Color color);

/**
 * Constructs an instance of an Animation struct and returns it.
 * 
 * @param fps the rate at which the sprite rectangles are updated.
 * @param numOfRectangles the number of rectangles/tiles in the sprite.
 * @param rectangles an array of rectangles where each Reactangle represents a tile in the sprite.
 * @param tiles the sprite texture as a Texture2D.
 * @returns an animation.
 */
Animation CreateAnimation(int fps, TextureFile textureFileType, Texture2D tiles);

/**
 * Unallocates the memory used to store the rectangles in an animation.
 */
void UnloadAnimation(Animation animation);

/**
 * Returns the number of tiles present in a specified sprite with the given tile width.
 * 
 * @param tileWidth the with of a single tile on the sprite.
 * @param textureFile the type of TextureFile to calculate.
 * @returns number of tiles as an int.
 */
int FindNumOfTiles(int tileWidth, TextureFile textureFile);

/**
 * Returns a pointer to an array of rectangles.
 * 
 * The array of rectangles depends on the type of TextureFile given and number of rectangles present.
 * 
 * @param numOfRectangles the number of rectangles/tiles in the sprite.
 * @param tileWidth the width of a single tile.
 * @param tileHeight the height of a single tile.
 * @param textureFile the type of TextureFile to look at.
 * @returns a pointer to an array of Rectangles.
 */
Rectangle* GetSpriteRectangles(int numOfRectangles, int tileWidth, int tileHeight, TextureFile textureFile);

/**
 * Responsible for updating the Sprites
 */
void SpriteRender(Entity entity, Animation animation);

/**
 * Responsible for unloading the Sprites
 */
void SpriteUnload();

// Player functions
void PlayerStartup();
void PlayerRender();
void PlayerMovement();
void PlayerUnload();

#endif // !ENTITIES_H