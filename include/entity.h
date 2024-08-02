/***********************************************************************************************
*
**   entity.h is responsible for defining properties needed for entities.
*   
*    @authors Marcus Vinicius Lages Santos and Samarjit Bhogal
*    @version 1.0
*
*?   @note animation.h and collision.h are used.
*
***********************************************************************************************/

#ifndef ENTITY_H
#define ENTITY_H

#include "animation.h"
#include "collision.h"

//* ------------------------------------------
//* DEFINITIONS

#define ENTITY_TILE_WIDTH 16
#define ENTITY_TILE_HEIGHT 32

#define DEFAULT_IDLE_FPS 2
#define DEFAULT_MOVING_FPS 8
#define DEFAULT_ATTACK_FPS 8

// ? OBS: Might be nuked later
#define TEMP_ATTACK_WIDTH 32
#define TEMP_ATTACK_HEIGHT 32

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum for the action state of entities.
 * 
 * @param IDLE      0
 * @param MOVING    1
 * @param ATTACKING 2
 */
typedef enum GameState {
    /** The idle state of an entity. */
    IDLE = 0,
    /** The moving state of an entity. */
    MOVING,
    /** The attacking state of an entity. */
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
    /** The entity is facing right. */
    RIGHT = 0,
    /** The entity is facing down. */
    DOWN,
    /** The entity is facing left. */
    LEFT,
    /** The entity is facing up. */
    UP
} Direction;

//* ------------------------------------------
//* STRUCTURES

/**
 * Structure to represent an entity (player / enemy).
 * 
 * @param x              X coordinate on the screen
 * @param y              Y coordinate on the screen
 * @param speed          Scalar speed
 * @param health         Health points
 * @param faceValue      Entity is turned to right or left
 * @param direction      Direction vector of the movement/velocity
 * @param state          Action state
 * @param directionFace  Direction entity is facing to (R, D, L, U)
 * 
 * @note Face must be either 1 or -1 where:
 * 
 *  - -1 indicates the character is facing WEST (LEFT) and
 * 
 *  - 1 indicates the character is facing EAST (RIGHT) (DEFAULT CASE)
 */
typedef struct Entity {
    /** X-position of the entity. */
    int x;
    /** Y-position of the entity. */
    int y;
    /** Speed the entity moves at. */
    int speed;
    /** Health of the entity. */
    int health;
    /** The faceValue of the entity (right, left) */
    int faceValue;
    /** Directional movement vector of the entity. */
    Vector2 direction;
    /** GameState of the entity. */
    GameState state;
    /** Direction the entity is facing. (right, down, left, up) */
    Direction directionFace;
} Entity;

//* ------------------------------------------
//* GLOBAL VARIABLES

/** The player entity. */
extern Entity player;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Responsible for rendering the entity with the specified animation.
 * 
 * ! @attention returns if given either a NULL entity or animation.
 * 
 * @param entity        Entity to render.
 * @param animation     Animation to apply to the entity.
 * @param entityWidth   Width of the entity.
 * @param entityHeight  Height of the entity.
 * @param xOffset       X-direction pixel offset from the current x of the entity.
 * @param yOffset       Y-direction pixel offset from the current y of the entity.
 * @param rotation      Rotation amount as a float.
 */
void EntityRender(Entity* entity, Animation* animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation);

//* ------------------------------------------
//* General entity logic

RayCollision2D EntityRectCollision(Entity entity, Rectangle hitboxTarget);
RayCollision2D EntitiesCollision(Entity entityIn, Entity entityTarget);

#endif // !ENTITY_H