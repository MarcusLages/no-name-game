/***********************************************************************************************
 *
 **   entity.c is responsible for implementating functions needed to manage general entities
 **   like rendering and general logic.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.2
 *
 *    @include entity.h
 *
 ***********************************************************************************************/

#include "../include/entity.h"

//* ------------------------------------------
//* DEFINITIONS

/**
 * Macro function to get the absolute/module value of a number.
 *
 * @param x Number
 */
#define ABS(x) (x > 0 ? x : x * (-1))

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void EntityRender(
    Entity* entity, Animation* animation, int entityWidth, int entityHeight,
    int xOffset, int yOffset, float rotation) {
    if(entity == NULL || animation == NULL) return;
    DrawAnimation(
        animation,
        (Rectangle){ (int) (entity->pos.x) + xOffset, (int) (entity->pos.y) + yOffset,
                     entityWidth < 0 ? -entityWidth : entityWidth,
                     entityHeight < 0 ? -entityHeight : entityHeight },
        entityWidth, entityHeight, rotation);
}

void UpdateEntityHitbox(Entity* entity) {
    entity->hitbox.x = entity->pos.x;
    entity->hitbox.y = entity->pos.y + entity->hitbox.height;
}

RayCollision2D EntityRectCollision(Entity entity, Rectangle hitboxTarget) {
    RayCollision2D collision;
    collision.hit = false;

    if(Vector2Equals(entity.direction, Vector2Zero())) return collision;

    UpdateEntityHitbox(&entity);

    collision = HitboxCollision(entity.hitbox, entity.direction, hitboxTarget);
    return collision;
}

RayCollision2D EntitiesCollision(Entity entityIn, Entity entityTarget) {
    RayCollision2D collision;
    collision.hit = false;

    if(Vector2Equals(entityIn.direction, Vector2Zero())) return collision;

    UpdateEntityHitbox(&entityIn);
    UpdateEntityHitbox(&entityTarget);

    collision = HitboxCollision(entityIn.hitbox, entityIn.direction, entityTarget.hitbox);
    return collision;
}

void EntityWorldCollision(Entity* entity) {
    if(collidableTiles == NULL) return;

    CollisionNode* entityCollisionList;
    entityCollisionList = NULL;

    CollisionNode* collidingTile;
    collidingTile = collidableTiles;

    while(collidingTile != NULL) {
        RayCollision2D entityCollision;
        Rectangle tileHitbox =
            (Rectangle){ .x = collidingTile->collidedHitbox.index.x * TILE_WIDTH,
                         .y = collidingTile->collidedHitbox.index.y * TILE_HEIGHT,
                         .width  = TILE_WIDTH,
                         .height = TILE_HEIGHT };

        entityCollision = EntityRectCollision(*entity, tileHitbox);
        if(entityCollision.hit == true && entityCollision.timeHit >= 0) {
            if(entityCollisionList == NULL)
                entityCollisionList = CreateCollisionList(
                    collidingTile->collidedHitbox.index.x,
                    collidingTile->collidedHitbox.index.y, entityCollision.timeHit);
            else
                AddCollisionNode(
                    entityCollisionList, collidingTile->collidedHitbox.index.x,
                    collidingTile->collidedHitbox.index.y, entityCollision.timeHit);
        }
        collidingTile = collidingTile->next;
    }

    if(entityCollisionList != NULL) {
        SortCollisionList(entityCollisionList);

        CollisionNode* resolvingNode = entityCollisionList;
        while(resolvingNode != NULL) {
            RayCollision2D entityCollision;
            Rectangle tileHitbox = (Rectangle){
                .x = resolvingNode->collidedHitbox.index.x * TILE_WIDTH,
                .y = resolvingNode->collidedHitbox.index.y * TILE_HEIGHT,
                .width  = TILE_WIDTH,
                .height = TILE_HEIGHT
            };

            entityCollision = EntityRectCollision(*entity, tileHitbox);
            if(entityCollision.hit == true && entityCollision.timeHit >= 0) {
                entity->direction.x += entityCollision.normalVector.x *
                    ABS(entity->direction.x) * (1 - entityCollision.timeHit);
                entity->direction.y += entityCollision.normalVector.y *
                    ABS(entity->direction.y) * (1 - entityCollision.timeHit);
            }
            resolvingNode = resolvingNode->next;
        }

        FreeCollisionList(entityCollisionList);
        entityCollisionList = NULL;
    }
}