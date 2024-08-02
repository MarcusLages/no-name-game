#ifndef COLLISION_H
#define COLLISION_H

#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

//* ------------------------------------------
//* STRUCTURES

/**
 * Struct representing a 2D ray with origin and direction.
 * 
 * ! @attention Direction vector needs to be used in relative position to the origin for collisions.
 * 
 * @param origin    Coordinates of the origin point of the ray
 * @param direction Direction/velocity vector of the ray relative to the origin point
 */
typedef struct Ray2D {
    /** Vector2 representing the coordinates of the origin point of the 2D ray. */
    Vector2 origin;
    /** Vector2 representing the direction/velocity vector of the 2D ray.
     * ! @attention Needs to be used in relative position to the origin for collisions.
     */
    Vector2 direction;
} Ray2D;

/**
 * Struct representing the result of a ray collision.
 * 
 * ! @attention Collisions can still be true even if it happened after a ray's direction point.
 * 
 * @param hit           If the collision happened or not
 * @param timeHit       Time in which the collision occurred
 * @param contactPoint  Coordinates of the point of contact of the collision
 * @param normalVector  Normal vector produced in the collision
 */
typedef struct RayCollision2D {
    /** Bool variable that represents if the ray had a collision
     * ! @attention Collisions can still be true even if it happened after a ray's direction point
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
 * @param index     Integer that represents the index of a collided hitbox in a specific array/list.
 * @param timeHit   Time of the collision
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
 * @param collidedHitbox    Information about a specific detected collided hitbox
 * @param next              Pointer to the next node on the linked list
 */
typedef struct CollisionNode CollisionNode;
struct CollisionNode {
    /** Information about the collided hitbox. */
    CollidedHitboxInfo collidedHitbox;
    /** Pointer to the next node on the list. */
    CollisionNode* next;
};

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Function used to check if there was a collision between a Ray2D and a Rectangle hitbox.
 * 
 * @param ray       Ray2D used to test the collision
 * @param hitbox    Rectangle that the ray will collide with
 * @return          Information about the collision.
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
 * ! @attention Direction parameter needs to be used in relative position to the middle of the
 *              hitboxIn Rectangle.
 * 
 * @param hitboxIn      Input Rectangle that will check collision
 * @param direction     Vector that corresponds to the input rectangle velocity/direction vector
 * @param hitboxTarget  Target Rectangle that the collision will be checked with
 * @return              Information about the collision.
 * 
 * ? @note Resolving the collision is still necessary. This function is only for detection.
 */
RayCollision2D HitboxCollision(Rectangle hitboxIn, Vector2 direction, Rectangle hitboxTarget);

/**
 * Creates a linked lists for detected collisions and returns a pointer to the first node.
 * 
 * @param index     Integer that represents the index of the collided hitbox in a specific array/list.
 * @param timeHit   Time of the collision (Check struct RayCollision2D)
 * @return          Pointer to the head node of the CollisionList
 */
CollisionNode* CreateCollisionList(int index, float timeHit);

/**
 * Adds a RayCollision2D to the end of the CollisionNode linked list.
 * 
 * @param head      Pointer to the head node of a CollisionNode list
 * @param index     Integer that represents the index of the collided hitbox in a specific array/list.
 * @param timeHit   Time of the collision (Check struct RayCollision2D)
 */
void AddCollisionNode(CollisionNode* head, int index, float timeHit);

/**
 * Sorts a CollisionNode linked list based on the timeHit of the
 * collisions detected in the list. (Smaller to bigger)
 * 
 * @param head      Pointer to the head node of a CollisionNode list
 */
void SortCollisionList(CollisionNode* head);

/**
 * Frees the memory of all the nodes in the CollisionNode linked list.
 * 
 * @param head      Pointer to the head node of a CollisionNode list
 */
void FreeCollisionList(CollisionNode* head);

#endif //!COLLISION_H