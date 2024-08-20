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
#include "../include/utils.h"

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Sets the given entity's state to either IDLE or MOVING based upon its current direction.
 *
 * ! @attention Returns if given a NULL entity.
 *
 * @param entiy The reference to the entity.
 * @param lastPlayerPos The reference to the last known player position relative to an enemy entity.
 *
 * ? @note pass NULL to lastPlayerPos if the entity is not an enemy.
 */
static void SetEntityStatebyDir(Entity* entity, Vector2* lastPlayerPos);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void MoveEntityTowardsPos(Entity* entity, Vector2 position, Vector2* lastPlayerPos) {
    if(entity == NULL) {
        TraceLog(LOG_WARNING, "ENTITY-C (MoveEnemyTowardsPos, line: %d): NULL entity was given.", __LINE__);
        return;
    }

    // Ensures the entity cannot move while attacking
    if(entity->state == ATTACKING) return;

    if(lastPlayerPos != NULL) {
        // handle enemy props
        if(position.x > entity->pos.x) {
            entity->faceValue     = 1;
            entity->directionFace = RIGHT;
        } else if(position.x < entity->pos.x) {
            entity->faceValue     = -1;
            entity->directionFace = LEFT;
        }

        if(position.y > entity->pos.y) {
            entity->directionFace = DOWN;
        } else if(position.y < entity->pos.y) {
            entity->directionFace = UP;
        }
        entity->direction = (Vector2){
            (int) position.x - (int) entity->pos.x,
            (int) position.y - (int) entity->pos.y,
        };
    }

    SetEntityStatebyDir(entity, lastPlayerPos);

    //? Delta time helps not let entity speed depend on framerate.
    //? Iat helps to take account for time between frames too.
    //! NOTE: Do not add deltaTime before checking collisions only after.
    float deltaTime = GetFrameTime();

    entity->direction = Vector2Normalize(entity->direction);

    // Velocity:
    entity->direction = Vector2Scale(entity->direction, entity->speed);

    EntityWorldCollision(entity);
    SetEntityStatebyDir(entity, lastPlayerPos);

    entity->pos = Vector2Add(entity->pos, Vector2Scale(entity->direction, deltaTime));
}

// there are extra computations with this method
static void SetEntityStatebyDir(Entity* entity, Vector2* lastPlayerPos) {
    if(entity == NULL) {
        TraceLog(LOG_WARNING, "ENTITY-C (SetEntityStatebyDir, line: %d): NULL entity was given.", __LINE__);
        return;
    }

    if((Vector2Equals(entity->direction, Vector2Zero()) && entity->state != ATTACKING) ||
       Vector2Equals(entity->direction, Vector2Zero())) {
        entity->state = IDLE;
        if(lastPlayerPos != NULL) *lastPlayerPos = entity->pos;
    } else {
        entity->state = entity->state == ATTACKING ? ATTACKING : MOVING;
    }
}

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

bool EntityAttack(Entity* attacker, Entity* victim, int attackPoints) {
    UpdateEntityHitbox(victim);
    bool attackHit = CheckCollisionRecs(attacker->attack, victim->hitbox);

    if(attackHit) {
        victim->health -= attackPoints;
    }

    return attackHit;
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

    collision =
        HitboxCollision(entityIn.hitbox, entityIn.direction, entityTarget.hitbox);
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
            Rectangle tileHitbox =
                (Rectangle){ .x = resolvingNode->collidedHitbox.index.x * TILE_WIDTH,
                             .y = resolvingNode->collidedHitbox.index.y * TILE_HEIGHT,
                             .width  = TILE_WIDTH,
                             .height = TILE_HEIGHT };

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