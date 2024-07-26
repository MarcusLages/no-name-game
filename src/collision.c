#include "raylib.h"
#include "raymath.h"
#include "collision.h"

//* DEFINITIONS

/**
 * Macro function to get the max between two values.
 * 
 * @param a Number
 * @param b Number
 */
#define max(a, b) ((a > b) ? a : b)

/**
 * Macro function to get the min between two values.
 * 
 * @param a Number
 * @param b Number
 */
#define min(a, b) ((a < b) ? a : b)

//* FUNCTION PROTOTYPES

/**
 * Swaps two float values.
 * 
 * @param a Pointer to a float number
 * @param b Pointer to a float number
 */
static void swap(float * a, float * b);

RayCollision2D RayRectCollision(Ray2D ray, Rectangle hitbox) {
    RayCollision2D collision;
    collision.hit = false;

    // Instead of using absolute direction from the origin point
    // we are changing it to use the direction based on the origin point
    ray.direction.x -= ray.origin.x;
    ray.direction.y -= ray.origin.y;
    
    // Get the near collision point and far collision point in terms
    // of time of collision normalized from 0 to 1.
    // 0 represents the origin and 1 represents the destination
    Vector2 nearCol = (Vector2) {
        .x = (hitbox.x - ray.origin.x) / ray.direction.x,
        .y = (hitbox.y - ray.origin.y) / ray.direction.y
    };
    // Far collision
    Vector2 farCol = (Vector2) {
        .x = (hitbox.x + hitbox.width - ray.origin.x) / ray.direction.x,
        .y = (hitbox.y + hitbox.height - ray.origin.y) / ray.direction.y
    };
    
    // Sort the collision point in case they are in different quadrants
    // instead of the origin quadrant (0,0)
    if (nearCol.x > farCol.x) swap(&nearCol.x, &farCol.x);
    if (nearCol.y > farCol.y) swap(&nearCol.y, &farCol.y);

    // Checks for collision rules, if they are not met,
    // returns RayCollision2D structure with no hit
    if (nearCol.x > farCol.y || nearCol.y > farCol.x)
        return collision;

    // Calculate time of collision
    collision.timeHit = max(nearCol.x, nearCol.y);
    float farTime = max(farCol.x, farCol.y);

    // Rejects collisions that happen before the origin point of the ray
    if (farTime < 0) return collision;

    // Rejects collisions that happen after the destination point of the ray
    // which means the ray is on the direction of the rectangle, but not there yet
    if (collision.timeHit >= 1) return collision;

    // Retrieving information on the contact point
    collision.contactPoint = (Vector2) {
        .x = ray.origin.x + collision.timeHit * ray.direction.x,
        .y = ray.origin.y + collision.timeHit * ray.direction.y,
    };

    // Retrieving information on the normal vector of the contact point,
    // or in an easier way to understand,
    // vector with the opposite in which direction the ray touches the rectangle
    if (nearCol.x > nearCol.y) {
        if (ray.direction.x < 0)
            collision.normalVector = (Vector2) { 1, 0 };
        else
            collision.normalVector = (Vector2) { -1, 0 };
    } else {
        if (ray.direction.y < 0)
            collision.normalVector = (Vector2) { 0, 1 };
        else
            collision.normalVector = (Vector2) { 0, -1 };
    }

    collision.hit = true;
    return collision;
    
}

RayCollision2D HitboxCollision(Rectangle hitboxIn, Rectangle hitboxTarget) {
    // RayCollision2D collision;
}

static void swap(float * a, float * b) {
    float temp;
    temp = *a;
    *a = *b;
    *b = temp;
}