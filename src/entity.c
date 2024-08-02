/***********************************************************************************************
*
**   entity.c is responsible for implementating functions needed to manage entities.
*   
*    @authors Marcus Vinicius Lages Santos and Samarjit Bhogal
*    @version 1.0
*
***********************************************************************************************/

#include "../include/entity.h"

//* ------------------------------------------
//* FUNCTION PROTOTYPES

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void EntityRender(Entity* entity, Animation* animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation) {
    if (entity == NULL || animation == NULL) return;
    DrawAnimation(
        animation, 
        (Rectangle) {entity->x + xOffset, entity->y + yOffset, 
                     entityWidth < 0 ? -entityWidth : entityWidth, 
                     entityHeight < 0 ? -entityHeight : entityHeight}, 
        entityWidth, 
        entityHeight, 
        rotation);
}