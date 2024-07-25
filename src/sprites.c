#include "texture.h"
#include "entities.h"

//TODO: these function need to be present in player.c they are present here for now

void SpriteUpdate() {
    Animation stationaryAnimation = CreateAnimation(2, FindNumOfTiles(ENTITY_TILE_WIDTH, TILE_PLAYER_IDLE), (Rectangle[]) {
            {0, 0, 16, 32},
            {1 * ENTITY_TILE_WIDTH, 0, 16, 32},
            {2 * ENTITY_TILE_WIDTH, 0, 16, 32},
            {3 * ENTITY_TILE_WIDTH, 0, 16, 32}},
            (Rectangle) {0, 0, 16, 32}, 
            textures[TILE_PLAYER_IDLE]);

    DrawAnimation(stationaryAnimation, stationaryAnimation.destination, 0.0f, WHITE);
}

Animation CreateAnimation(int fps, int numOfRectangles, Rectangle rectangles[], Rectangle dest, Texture2D tiles) {
    Rectangle *rect = (Rectangle*) malloc(sizeof(Rectangle) * numOfRectangles);

    for (int i = 0; i < numOfRectangles; i++) {
        rect[i] = rectangles[i];
    }

    Animation animation = {
        .fps = fps,
        .numOfRectangles = numOfRectangles,
        .rectangles = rect,
        .textures = tiles,
        .destination = dest
    };

    return animation;
}

void UnloadAnimation(Animation animation) {
    free(animation.rectangles);
    animation.rectangles = NULL;
}    

void DrawAnimation(Animation animation, Rectangle dest, float rotation, Color color) {
    int idx = (int) (GetTime() * animation.fps) % animation.numOfRectangles;
    Rectangle source = animation.rectangles[idx];
    Vector2 origin = { 0, 0 };
    DrawTexturePro(animation.textures, source, dest, origin, rotation, color);
}

int FindNumOfTiles(int tileWidth, TextureFile textureFile) {
    return textures[textureFile].width / tileWidth;
}
