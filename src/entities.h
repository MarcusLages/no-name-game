#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "raymath.h"
#include "texture.h"

#define ENTITY_TILE_WIDTH 16
#define ENTITY_TILE_HEIGHT 32

#define ENTITY_IDLE_FPS 2
#define ENTITY_MOVING_FPS 8
#define ENTITY_ATTACK_FPS 4

#define ENITIY_ATTACK_FRAMES 30

// Enum for the action state of entities for animation and properties.
typedef enum GameState {
    IDLE = 0,
    MOVING,
    ATTACKING
} GameState;

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
} Entity;

/**
 * Structure to represent information needed for a Sprite Animation
 */
typedef struct Animation {
    int fps;
    int numOfRectangles;
    int animationFrame;
    Rectangle *rectangles;
    Texture2D textures;
} Animation;

//* VARIABLES

extern Entity player;

//------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Constructs an instance of an Animation struct and returns it.
 * 
 * @param fps the rate at which the sprite rectangles are updated.
 * @param textureFileType the type of texture as a TextureFile.
 * @param tiles the sprite texture as a Texture2D.
 * @returns an animation.
 */
Animation CreateAnimation(int fps, TextureFile textureFileType, Texture2D tiles);

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
 * @param loop indicates if the animation should loop.
 */
void AnimationRender(Entity entity, Animation animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation, bool loop);

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
void PlayerMovement();
void PlayerAttack();
void PlayerUnload();

#endif // !ENTITIES_H