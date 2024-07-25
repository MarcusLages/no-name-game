#include "texture.h"
#include "entities.h"

void SpriteUpdate() {
    Animation stationaryAnimation = {
        .fps = 2,
        .numOfRectangles = 4,
        .rectangles = (Rectangle[]) {
            {0, 0, 16, 32},
            {1 * ENTITY_TILE_WIDTH, 0, 16, 32},
            {2 * ENTITY_TILE_WIDTH, 0, 16, 32},
            {3 * ENTITY_TILE_WIDTH, 0, 16, 32}},
        .textures = textures[TILE_PLAYER_IDLE]    
    };

    Rectangle dest = { 
        (float) 0, 
        (float) 0, 
        (float) 16, 
        (float) 32
        };
    Vector2 origin = { 0, 0 };

    DrawAnimation(stationaryAnimation, dest, origin, 0.0f, WHITE);
}

void DrawAnimation(Animation animation, Rectangle dest, Vector2 origin, float rotation, Color color) {
    int idx = (int) (GetTime() * animation.fps) % animation.numOfRectangles;
    Rectangle source = animation.rectangles[idx];
    DrawTexturePro(animation.textures, source, dest, origin, rotation, color);
}
