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
 * Responsible for updating the Sprites
 */
void SpriteRender(Entity entity, Animation animation);

/**
 * Responsible for unloading the Sprites by unallocating the memory 
 * used to store the rectangles in an animation.
 */
void SpriteUnload(Animation animation);

// Player functions
void PlayerStartup();
void PlayerRender();
void PlayerMovement();
void PlayerUnload();

#endif // !ENTITIES_H