/**********************************************************************************************
 *
 **   entity-logic.c is responsible for detecting collisions related to general entities.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.1
 *
 *    @include entity.h
 *
 **********************************************************************************************/

#include "../include/entity.h"

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

RayCollision2D EntityRectCollision(Entity entity, Rectangle hitboxTarget) {
    RayCollision2D collision;
    collision.hit = false;

    if(Vector2Equals(entity.direction, Vector2Zero())) return collision;

    Rectangle entityHitbox = (Rectangle){ .x = entity.pos.x,
                                          .y = entity.pos.y + ENTITY_TILE_HEIGHT / 2,
                                          .width  = ENTITY_TILE_WIDTH,
                                          .height = ENTITY_TILE_HEIGHT / 2 };

    collision = HitboxCollision(entityHitbox, entity.direction, hitboxTarget);
    return collision;
}

RayCollision2D EntitiesCollision(Entity entityIn, Entity entityTarget) {
    RayCollision2D collision;
    collision.hit = false;

    if(Vector2Equals(entityIn.direction, Vector2Zero())) return collision;

    Rectangle entityInHitbox     = (Rectangle){ .x      = entityIn.pos.x,
                                                .y      = entityIn.pos.y,
                                                .width  = ENTITY_TILE_WIDTH,
                                                .height = ENTITY_TILE_HEIGHT };
    Rectangle entityTargetHitbox = (Rectangle){ .x      = entityTarget.pos.x,
                                                .y      = entityTarget.pos.y,
                                                .width  = ENTITY_TILE_WIDTH,
                                                .height = ENTITY_TILE_HEIGHT };

    collision = HitboxCollision(entityInHitbox, entityIn.direction, entityTargetHitbox);
    return collision;
}
