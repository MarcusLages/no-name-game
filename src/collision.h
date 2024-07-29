#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "raymath.h"

/**
 * Struct representing a 2D ray with origin and direction.
 * 
 * @param Vector2 origin
 * @param Vector2 direction
 */
typedef struct Ray2D {
    /** Vector2 representing the origin point of the 2D ray. */
    Vector2 origin;
    /** Vector2 representing the direction point of the 2D ray.
     * ! @attention Needs to be used in relative position to the origin for collisions.
     */
    Vector2 direction;
} Ray2D;

/**
 * Struct representing the result of a ray collision.
 * 
 * @param hit bool
 * @param timeHit float
 * @param contactPoint Vector2
 * @param normalVector Vector2
 */
typedef struct RayCollision2D {
    /** Bool variable that represents if the ray had a collision
     * ! @attention Collisions can still be true even if they happened after a ray's direction point
     *              or if the ray started inside the shape.
     */
    bool hit;
    /** Float variable that represents when the collision occures relative to the origin/direction distance ray.
     * ? @attention Zero (0) represents zero distance from origin to collision.
     * ? @attention One (1) represents collision at the ray's direction point.
    */
    float timeHit;
    /** Vector2 with the coordinate of the point of contact/collision of the ray with the shape. */
    Vector2 contactPoint;
    /** Unit Vector2 that corresponds to the normal vector force at the point of contact/collision of the ray with the shape. */
    Vector2 normalVector;
} RayCollision2D;

/**
 * Function used to check if there was a collision between a Ray2D and a Rectangle hitbox.
 * 
 * @param ray Ray2D
 * @param hitbox Rectangle
 * @return RayCollision2D
 */
RayCollision2D RayRectCollision(Ray2D ray, Rectangle hitbox);

/**
 * Function used to check if there was a collision between a moving Rectangle with a direction
 * Vector2 and target Rectangle.
 * 
 * @param hitboxIn Rectangle
 * @param direction Vector2
 * @param hitboxTarget Rectangle
 * @return RayCollision2D
 */
RayCollision2D HitboxCollision(Rectangle hitboxIn, Vector2 direction, Rectangle hitboxTarget);

#endif //!COLLISION_H