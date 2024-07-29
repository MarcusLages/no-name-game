#include "raylib.h"
#include "raymath.h"
#include "collision.h"
#include "entities.h"

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

    // ! WARNING:
    // ! Before doing all calculations, if the direction vector is absolute,
    // ! it's important to make it relative to the origin vector.
    // ray.direction.x -= ray.origin.x;
    // ray.direction.y -= ray.origin.y;
    
    // Get the near collision point and far collision point in terms
    // of time of collision normalized from 0 to 1.
    // 0 represents the origin and 1 represents the destination

    // Near collision
    Vector2 nearColTime = (Vector2) {
        .x = (hitbox.x - ray.origin.x) / ray.direction.x,
        .y = (hitbox.y - ray.origin.y) / ray.direction.y
    };

    // Far collision
    Vector2 farColTime = (Vector2) {
        .x = (hitbox.x + hitbox.width - ray.origin.x) / ray.direction.x,
        .y = (hitbox.y + hitbox.height - ray.origin.y) / ray.direction.y
    };
    
    // Sort the collision point in case they are in different quadrants
    // instead of the origin quadrant (0,0)
    if (nearColTime.x > farColTime.x) swap(&nearColTime.x, &farColTime.x);
    if (nearColTime.y > farColTime.y) swap(&nearColTime.y, &farColTime.y);

    // Checks for collision rules, if they are not met,
    // returns RayCollision2D structure with no hit
    if (nearColTime.x > farColTime.y || nearColTime.y > farColTime.x)
        return collision;

    // Calculate time of collision
    collision.timeHit = max(nearColTime.x, nearColTime.y);
    float farTime = max(farColTime.x, farColTime.y);

    // Rejects collisions that happen entirely before the origin point of the ray (at negative time).
    // ? NOTE: Depending on your use of ray collision, you might need to fine tune this.
    if (farTime < 0) return collision;

    // Used to reject collisions that happen after the destination point of the ray
    // which means the ray is on the direction of the rectangle, but not there yet.
    // Commented because it should be used outside when treating the collision.

    // if (collision.timeHit > 1) collision.hit = false;
    // else collision.hit = true;

    // Retrieving information on the contact point
    collision.contactPoint = Vector2Add(ray.origin, Vector2Scale(ray.direction, collision.timeHit));

    // Retrieving information on the normal vector of the contact point,
    // or in an easier way to understand,
    // vector with the opposite in which direction the ray touches the rectangle
    if (nearColTime.x > nearColTime.y) {
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

RayCollision2D HitboxCollision(Rectangle hitboxIn, Vector2 direction, Rectangle hitboxTarget) {
    RayCollision2D collision;

}

static void swap(float * a, float * b) {
    float temp;
    temp = *a;
    *a = *b;
    *b = temp;
}