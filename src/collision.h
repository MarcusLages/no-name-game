#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "raymath.h"

typedef struct Ray2D {
    Vector2 origin;
    Vector2 direction;
} Ray2D;

typedef struct RayCollision2D {
    bool hit;
    float timeHit;
    Vector2 contactPoint;
    Vector2 normalVector;
} RayCollision2D;

RayCollision2D RayRectCollision(Ray2D ray, Rectangle hitbox);
RayCollision2D HitboxCollision(Rectangle hitboxIn, Vector2 direction, Rectangle hitboxTarget);

#endif //!COLLISION_H