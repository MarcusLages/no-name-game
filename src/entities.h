#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "raymath.h"
#include "texture.h"
#include "timer.h"

#define ENTITY_TILE_WIDTH 16
#define ENTITY_TILE_HEIGHT 32

#define ENTITY_IDLE_FPS 2
#define ENTITY_MOVING_FPS 8
#define ENTITY_ATTACK_FPS 8

// Enum for the action state of entities for animation and properties.
typedef enum GameState {
    IDLE = 0,
    MOVING,
    ATTACKING,
    // MOVING_LEFT,
    // MOVING_RIGHT,
    // MOVING_UP,
    // MOVING_DOWN
} GameState;

typedef enum Direction {
    RIGHT = 0,
    DOWN,
    LEFT,
    UP
} Direction;

//* Structures

/**
 * Structure to represent a player / enemy.
 * NOTE: face must be either 1 or -1 where:
 *           -1 indicates the character is facing WEST (LEFT) and
 *            1 indicates the character is facing EAST (RIGHT) (DEFAULT CASE)
 */
typedef struct Entity {
    int x;
    int y;
    int speed;
    int health;
    int face;
    Vector2 direction;
    GameState state;
    Direction directionFace;
} Entity;

/**
 * @brief Represents information needed for a Sprite Animation.
 * 
 * ! @attention Timer checking is not handled internally and must be handled in order to control various states.
 * 
 * ? @note - Timer must be started with StartTimer from timer.c see Timer.
 * ? @note - Each animation must be unloaded with AnimationUnload.
 */
typedef struct Animation {
    /** The frames per second of this animation. */
    int fps;
    /** The number of rectangles that capture each tile in textures. */
    int numOfRectangles;
    /** The list of reactangles the capture each tile's x, y, width and height in textures. */ 
    Rectangle *rectangles;
    //
    Texture2D textures;
    Timer *timer;
} Animation;

//* VARIABLES

extern Entity player;

//------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Constructs an instance of an Animation struct and returns it.
 * 
 * @param fps the rate at which the sprite rectangles are updated.
 * @param tileWidth the width of a single tile.
 * @param tileHeight the height of a single tile.
 * @param textureFileType the type of texture as a TextureFile.
 * @param tiles the sprite texture as a Texture2D.
 * @returns an animation.
 */
Animation CreateAnimation(int fps, int tileWidth, int tileHeight, TextureFile textureFileType, Texture2D tiles);

/**
 * Responsible for rendering the entity with the specified animation.
 * 
 * @param entity the entity to render.
 * @param animation the animation to apply to the entity.
 * @param entityWidth the width of the entity rectangle.
 * @param entityHeight the height of the entity rectangle.
 * @param xOffset the pixel offset in the x direction from the current x of the entity.
 * @param yOffset the pixel offset in the y direction from the current y of the entity.
 * @param rotation the rotation amount as a float.
 */
void AnimationRender(Entity entity, Animation animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation);

/**
 * Responsible for unloading the Animations by unallocating the memory 
 * used to store the rectangles in an animation.
 * 
 * @param animation the animation to unallocate.
 */
void AnimationUnload(Animation animation);

// Player functions
void PlayerStartup();
void PlayerRender();
void RenderPlayerAttack();
void PlayerMovement();
void PlayerAttack();
void PlayerUnload();

#endif // !ENTITIES_H