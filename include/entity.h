/***********************************************************************************************
 *
 **   entity.h is responsible for defining properties needed for entities.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
 *
 *    @include animation.h, collision.h
 *
 ***********************************************************************************************/

#ifndef ENTITY_H
#define ENTITY_H

#include "animation.h"
#include "collision.h"

//* ------------------------------------------
//* DEFINITIONS

/** Default entity tile size. */
#define ENTITY_TILE_WIDTH  16
#define ENTITY_TILE_HEIGHT 32

// Entity attack rectangle size
#define ENTITY_ATTACK_WIDTH  32
#define ENTITY_ATTACK_HEIGHT 21

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
 * @param pos            The x and y position of the entity as a vector.
 * @param speed          Scalar speed
 * @param health         Health points
 * @param faceValue      Entity is turned to right or left
 * @param direction      Direction vector of the movement/velocity
 * @param state          Action state
 * @param directionFace  Direction entity is facing to (R, D, L, U)
 * @param animations     Animations array. Access it through AnimationType enum.
 *
 * @note Face must be either 1 or -1 where:
 *
 *  - -1 indicates the character is facing WEST (LEFT) and
 *
 *  - 1 indicates the character is facing EAST (RIGHT) (DEFAULT CASE)
 */
typedef struct Entity {
    /** The position of the entity. */
    Vector2 pos;
    /** The hitbox used for collision detection.
     *  @note Position needs to be updated everytime we use it
     */
    Rectangle hitbox;
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
    /** Array struct with all the animations this entity has.
     * @note Use AnimationType enum to access a specific animation in the array
     */
    AnimationArray animations;
    /** Struct that represents the attack hitbox of this entity. */
    Rectangle attack;
} Entity;

//* ------------------------------------------
//* GLOBAL VARIABLES

/** The player entity. */
extern Entity player;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Handles the entity (Enemy or Player) movement towards a given position.
 *
 * ! @attention Returns if given a NULL entity.
 *
 * @param entity        The reference to the entity to move.
 * @param position      The position to move the entity towards.
 * @param lastPlayerPos The last known position of the player relative to an enemy entity.
 *
 * ? @note pass NULL to lastPlayerPos if the entity is not an enemy.
 */
void MoveEntityTowardsPos(Entity* entity, Vector2 position, Vector2* lastPlayerPos);

/**
 * Responsible for rendering the entity with the specified animation.
 *
 * ! @attention returns if given either a NULL entity or animation.
 *
 * @param entity        Pointer to the entity to render.
 * @param animation     Animation to apply to the entity.
 * @param entityWidth   Width of the entity.
 * @param entityHeight  Height of the entity.
 * @param xOffset       X-direction pixel offset from the current x of the entity.
 * @param yOffset       Y-direction pixel offset from the current y of the entity.
 * @param rotation      Rotation amount as a float.
 *
 * ? @note rotation rotates the animation relative to the (entity.x + xOffset), (entity.y + yOffset)
 */
void EntityRender(
    Entity* entity, Animation* animation, int entityWidth, int entityHeight,
    int xOffset, int yOffset, float rotation);

//* Entity collision logic

/**
 * Function used to check if the attack of an entity hit another entity's hitbox.
 *  
 * @attention It's necessary to update the attacker's attack hitbox before calling the function.
 * 
 * @param attacker      Pointer to the attacker entity
 * @param victim        Pointer to the entity that is getting hit
 * @param attackPoints  How much life will the victim lose
 * @return              If the attack happened or not.
 *                      (OBS: Might be used or not or changed to a callback function)
 */
bool EntityAttack(Entity* attacker, Entity* victim, int attackPoints);

/**
 * Function that should be called to update the entity hitbox collision before a collision is called.
 *
 * @param entity Pointer to the entity that will update its hitbox
 */
void UpdateEntityHitbox(Entity* entity);

/**
 * Function used to check if there was a collision between a moving entity and a Rectangle
 * hitbox.
 *
 * ! @attention Use this only for general entities (16x32 with only the lower 16pxls collidable)
 *
 * @param entity        Moving entity to test the collision
 * @param hitboxTarget  Rectangle that the entity will check a collision with
 * @return              Information about the collision.
 *
 * ? @note - Important to check the timeHit (-n, -1, 0, +1 or +n) of the returned collision
 *              even if the collision.hit is true.
 * ? @note - Resolving the collision is still necessary. This function is only for detection.
 */
RayCollision2D EntityRectCollision(Entity entity, Rectangle hitboxTarget);

/**
 * Function used to check if there was a collision between a moving entity and another
 * entity's hitbox.
 *
 * ! @attention Use this only for general entities (16x32 with only the lower 16pxls collidable)
 *
 * @param entityIn      Moving entity to test the collision
 * @param entityTarget  Target entity that the entityIn will check a collision with
 * @return              Information about the collision.
 *
 * ? @note - Important to check the timeHit (-n, -1, 0, +1 or +n) of the returned collision
 *              even if the collision.hit is true.
 * ? @note - Resolving the collision is still necessary. This function is only for detection.
 */
RayCollision2D EntitiesCollision(Entity entityIn, Entity entityTarget);

/**
 * Handles entity collision with the world tilemap(Tile**) through the collidableTiles list.
 *
 * ! @attention Use this only for general entities (16x32 with only the lower 16pxls collidable)
 *
 * @param entity Pointer to the entity that will check collision with all the collidable tiles on the map
 */
void EntityWorldCollision(Entity* entity);

/**
 * Sets the attack hitbox for the entities of standard size.
 * 
 * An entity of standard size is one with:
 *  - Width: 16 pixels Height: 32 pixels.
 * 
 * ! @attention entity MUST have a standard size.
 */
void LoadStandardEntityAttackHitbox(Entity* entity);

#endif // !ENTITY_H