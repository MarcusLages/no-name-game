#include "texture.h"
#include "entities.h"

//TODO: these function need to be present in player.c they are present here for now

void SpriteUpdate() {
    Animation stationaryAnimation = CreateAnimation(2, 4, (Rectangle[]) {
            {0, 0, 16, 32},
            {1 * ENTITY_TILE_WIDTH, 0, 16, 32},
            {2 * ENTITY_TILE_WIDTH, 0, 16, 32},
            {3 * ENTITY_TILE_WIDTH, 0, 16, 32}}, 
            textures[TILE_PLAYER_IDLE]);

    Rectangle dest = { 
        (float) 0, 
        (float) 0, 
        (float) 16, 
        (float) 32
        };
    Vector2 origin = { 0, 0 };

    DrawAnimation(stationaryAnimation, dest, origin, 0.0f, WHITE);
}

Animation CreateAnimation(int fps, int numOfRectangles, Rectangle rectangles[], Texture2D textures) {
    Rectangle *rect = (Rectangle*) malloc(sizeof(Rectangle) * numOfRectangles);

    for (int i = 0; i < numOfRectangles; i++) {
        rect[i] = rectangles[i];
    }

    Animation animation = {
        .fps = fps,
        .numOfRectangles = numOfRectangles,
        .rectangles = rect,
        .textures = textures
    };

    return animation;
}

void UnloadAnimation(Animation animation) {
    free(animation.rectangles);
    animation.rectangles = NULL;
}    

void DrawAnimation(Animation animation, Rectangle dest, Vector2 origin, float rotation, Color color) {
    int idx = (int) (GetTime() * animation.fps) % animation.numOfRectangles;
    Rectangle source = animation.rectangles[idx];
    DrawTexturePro(animation.textures, source, dest, origin, rotation, color);
}
