#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "raymath.h"
#include "animation.h"
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

//------------------------------------------
//* VARIABLES

extern Entity player;

//------------------------------------------
//* FUNCTION PROTOTYPES

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
void EntityRender(Entity* entity, Animation* animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation);

//* General entity logic
RayCollision2D EntityRectCollision(Entity entity, Rectangle hitboxTarget);
RayCollision2D EntitiesCollision(Entity entityIn, Entity entityTarget);

#endif // !ENTITIES_H