#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "raymath.h"
#include "texture.h"
#include "timer.h"
#include "collision.h"

#define ENTITY_TILE_WIDTH 16
#define ENTITY_TILE_HEIGHT 32

#define DEFAULT_IDLE_FPS 2
#define DEFAULT_MOVING_FPS 8
#define DEFAULT_ATTACK_FPS 8

// ? OBS: Might be nuked later
#define TEMP_ATTACK_WIDTH 32
#define TEMP_ATTACK_HEIGHT 32

//------------------------------------------
//* ENUMS

/**
 * Enum for the action state of entities for animation and properties.
 * 
 * @param IDLE      0
 * @param MOVING    1
 * @param ATTACKING 2
 */
typedef enum GameState {
    /** Highlights the idle state of an entity. */
    IDLE = 0,
    /** Highlights the moving state of an entity. */
    MOVING,
    /** Highlights the attacking state of an entity. */
    ATTACKING
} GameState;

/**
 * Enum for the direction the entity is facing.
 * 
 * @param RIGHT 0
 * @param DOWN  1
 * @param LEFT  2
 * @param UP    3
 */
typedef enum Direction {
    /** Indicates the entity is facing right. */
    RIGHT = 0,
    /** Indicates the entity is facing down. */
    DOWN,
    /** Indicates the entity is facing left. */
    LEFT,
    /** Indicates the entity is facing up. */
    UP
} Direction;

//------------------------------------------
//* Structures

/**
 * Structure to represent an entity (player / enemy).
 * 
 * @param x              int
 * @param y              int
 * @param speed          int
 * @param health         int
 * @param faceValue           int
 * @param direction      Vector2
 * @param state          GameState
 * @param directionFace  Direction
 * 
 * @note Face must be either 1 or -1 where:
 * 
 *  - -1 indicates the character is facing WEST (LEFT) and
 * 
 *  - 1 indicates the character is facing EAST (RIGHT) (DEFAULT CASE)
 */
typedef struct Entity {
    /** The x-position of the entity. */
    int x;
    /** The y-position of the entity. */
    int y;
    /** The speed the entity moves at. */
    int speed;
    /** The health of the entity. */
    int health;
    /** The faceValue of the entity (right, left) */
    int faceValue;
    /** The directional movement of the entity. */
    Vector2 direction;
    /** The GameState of the entity. */
    GameState state;
    /** The Direction the entity is facing. (right, down, left, up) */
    Direction directionFace;
} Entity;

/**
 * Represents information needed for a Sprite Animation.
 * 
 * ! @attention Timer checking is not handled internally and must be handled in order to control various states.
 * 
 * ? @note - Timer must be started with StartTimer from timer.c see Timer.
 * ? @note - Each animation must be unloaded with AnimationUnload.
 */
typedef struct Animation {
    /** The frames per second of this animation. */
    int fps;
    /** The number of frames that capture each tile in texture. */
    int numOfFrames;
    /** 
     * The list of frames the capture each tile's x, y, width and height in texture. 
     * 
     * ! @attention This pointer will point to a location in heap that must be freed.
     */ 
    Rectangle* frames;
    /** The texture for this animation. */
    Texture2D texture;
    /** 
     * The time for this animation.
     * 
     * ! @attention This pointer will point to a location in heap that must be freed.
     */
    Timer* timer;
} Animation;

//------------------------------------------
//* VARIABLES

extern Entity player;

//------------------------------------------
//* FUNCTION PROTOTYPES

//* General entity sprites and visuals
/**
 * Constructs an instance of an Animation struct and returns it.
 * 
 * ! @note This function is responsible for creating the animation, animation.reactangles, and animation.timer in the heap.
 * 
 * @param fps the rate at which the sprite frames are updated.
 * @param tileWidth the width of a single tile.
 * @param tileHeight the height of a single tile.
 * @param textureFileType the type of texture as a TextureFile.
 * @param texture the sprite texture as a Texture2D.
 * @returns a pointer to an animation in memory.
 * 
 * ! @attention returns NULL if given an invalid textureType.
 */
Animation* CreateAnimation(int fps, int tileWidth, int tileHeight, TextureFile textureType, Texture2D texture);

/**
 * Responsible for rendering the entity with the specified animation.
 * 
 * @param entity the entity to render.
 * @param animation the animation to apply to the entity.
 * @param entityWidth the width of the entity.
 * @param entityHeight the height of the entity.
 * @param xOffset x-direction pixel offset from the current x of the entity.
 * @param yOffset y-direction pixel offset from the current y of the entity.
 * @param rotation the rotation amount as a float.
 * 
 * ! @attention returns if given either a NULL entity or animation.
 */
void AnimationRender(Entity* entity, Animation* animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation);

/**
 * Responsible for unloading an animation by unallocating the memory 
 * used to store the frames and the timer in an animation.
 * 
 * ! @note This function frees the memory of animation.frames, animation.timer and the animation.
 * 
 * @param animation the animation to unallocate.
 * 
 * ! @attention returns if given a NULL animation.
 */
void AnimationUnload(Animation* animation);

//* General entity logic
RayCollision2D EntityRectCollision(Entity entity, Rectangle hitboxTarget);
RayCollision2D EntitiesCollision(Entity entityIn, Entity entityTarget);

//* Player functions

/**
 * Initializes the player entity, animations, and timers. 
 * 
 * ? Timers started here are needed for the whole duration of the game.
 */
void PlayerStartup();

/**
 * Handles player movement and updates it's GameState and Direction. 
 */
void PlayerMovement();

/**
 * Handles the player's attack.
 * 
 * ? Manages the timer for the player attack animation.
 * TODO: collisions, enemy health, etc..
 */
void PlayerAttack();

/**
 * Renders the player animation based off of it's GameState.
 */
void PlayerRender();

/**
 * Renders the player attack animation based off of it's Direction.
 */
void RenderPlayerAttack();

/**
 * Unloads all the animations associated with the player entity.
 */
void PlayerUnload();

#endif // !ENTITIES_H