/***********************************************************************************************
*
**   entity.c is responsible for implementating functions needed to manage entities.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.1
*
*    @include entity.h
*
***********************************************************************************************/

#include "../include/entity.h"

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void EntityRender(Entity* entity, Animation* animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation) {
    if (entity == NULL || animation == NULL) return;
    DrawAnimation(
        animation, 
        (Rectangle) {(int) (entity->x) + xOffset, (int) (entity->y) + yOffset, 
                     entityWidth < 0 ? -entityWidth : entityWidth, 
                     entityHeight < 0 ? -entityHeight : entityHeight}, 
        entityWidth, 
        entityHeight, 
        rotation);
}