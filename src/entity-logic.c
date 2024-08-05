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
    Rectangle entityHitbox = (Rectangle) {
        .x = entity.x,
        .y = entity.y + ENTITY_TILE_HEIGHT / 2,
        .width = ENTITY_TILE_WIDTH,
        .height = ENTITY_TILE_HEIGHT / 2
    };
    return HitboxCollision(entityHitbox, entity.direction, hitboxTarget);
}

RayCollision2D EntitiesCollision(Entity entityIn, Entity entityTarget) {
    Rectangle entityInHitbox = (Rectangle) {
        .x = entityIn.x,
        .y = entityIn.y,
        .width = ENTITY_TILE_WIDTH,
        .height = ENTITY_TILE_HEIGHT
    };
    Rectangle entityTargetHitbox = (Rectangle) {
        .x = entityTarget.x,
        .y = entityTarget.y,
        .width = ENTITY_TILE_WIDTH,
        .height = ENTITY_TILE_HEIGHT
    };
    
    return HitboxCollision(entityInHitbox, entityIn.direction, entityTargetHitbox);
}

