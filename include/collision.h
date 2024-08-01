#ifndef COLLISION_H
#define COLLISION_H

#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

/**
 * Struct representing a 2D ray with origin and direction.
 * 
 * ! @attention Direction vector needs to be used in relative position to the origin for collisions.
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
     * ? @note Zero (0) represents zero distance from origin to collision.
     * ? @note One (1) represents collision at the ray's direction point.
     * ? @note Minus number (-n) represents collision at the opposite direction of the ray.
    */
    float timeHit;
    /** Vector2 with the coordinate of the point of contact/collision of the ray with the shape. */
    Vector2 contactPoint;
    /** Unit Vector2 that corresponds to the normal vector force at the point of contact/collision of the ray with the shape. */
    Vector2 normalVector;
} RayCollision2D;

/**
 * Structure that contains information about a specific collided hitbox so it
 * can be sorted for better collision resolving.
 * 
 * @param index int
 * @param timeHit float
 * 
 * @note Check RayCollision2D for more in-depth information about timeHit.
 */
typedef struct CollidedHitboxInfo {
    /** Index of the collided hitbox in the array of hitboxes. */
    int index;
    /** Contact time of the collision. Used to sort the closest collision.*/
    float timeHit;
} CollidedHitboxInfo;

/**
 * Collision node struct to keep track of the detected collisions in a collision linked list.
 * 
 * @param collidedHitbox CollidedHitboxInfo
 * @param next CollisionNode *
 */
typedef struct CollisionNode CollisionNode;
struct CollisionNode {
    /** Information about the collided hitbox. */
    CollidedHitboxInfo collidedHitbox;
    /** Pointer to the next node on the list. */
    CollisionNode* next;
};

//* FUNCTION PROTOTYPES

/**
 * Function used to check if there was a collision between a Ray2D and a Rectangle hitbox.
 * 
 * @param ray Ray2D
 * @param hitbox Rectangle
 * @return Information about the collision.
 * 
 * ? @note Important to check the timeHit (-n, -1, 0, +1 or +n) of the returned collision
 *              even if the collision.hit is true.
 * ? @note Resolving the collision is still necessary. This function is only for detection.
 */
RayCollision2D RayRectCollision(Ray2D ray, Rectangle hitbox);

/**
 * Function used to check if there was a collision between a moving Rectangle with a direction
 * Vector2 and target Rectangle.
 * 
 * @param hitboxIn Rectangle
 * @param direction Vector2
 * @param hitboxTarget Rectangle
 * @return Information about the collision.
 * 
 * ? @note Resolving the collision is still necessary. This function is only for detection.
 */
RayCollision2D HitboxCollision(Rectangle hitboxIn, Vector2 direction, Rectangle hitboxTarget);

/**
 * Creates a linked lists for detected collisions and returns a pointer to the list.
 * 
 * @param collision RayCollision2D
 * @return Pointer to the head of the CollisionList
 */
CollisionNode* CreateCollisionList(int index, float timeHit);

/**
 * Adds a RayCollision2D to the end of the CollisionNode linked list.
 * 
 * @param head CollisionNode*
 * @param RayCollision2D collision
 */
void AddCollisionNode(CollisionNode* head, int index, float timeHit);

/**
 * Sorts a CollisionNode linked list based on the collisionTime of the
 * RayCollision2D in the node.
 * 
 * @param head CollisionNode*
 */
void SortCollisionList(CollisionNode* head);

/**
 * Frees the memory of all the nodes in the CollisionNode linked list.
 * 
 * @param head CollisionNode*
 */
void FreeCollisionList(CollisionNode* head);

#endif //!COLLISION_H