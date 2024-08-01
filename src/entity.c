#include "entity.h"

//* FUNCTION PROTOTYPES

void EntityRender(Entity *entity, Animation *animation, int entityWidth, 
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
