#include "texture.h"
#include "entities.h"

void SpriteUpdate() {
    int numOfTiles = FindNumOfTiles(ENTITY_TILE_WIDTH, TILE_PLAYER_IDLE);
    Animation stationaryAnimation = CreateAnimation(ENTITY_IDLE_FPS, 
        numOfTiles,
        GetSpriteRectangles(numOfTiles, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_IDLE),
        (Rectangle) {0, 0, 16, 32}, 
        textures[TILE_PLAYER_IDLE]);

    DrawAnimation(stationaryAnimation, stationaryAnimation.destination, 0.0f, WHITE);
}

Animation CreateAnimation(int fps, int numOfRectangles, Rectangle *rectangles, Rectangle dest, Texture2D tiles) {
    Animation animation = {
        .fps = fps,
        .numOfRectangles = numOfRectangles,
        .rectangles = rectangles,
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

Rectangle* GetSpriteRectangles(int numOfRectangles, int tileWidth, int tileHeight, TextureFile textureFile) {
    Rectangle *rectangles = (Rectangle*) malloc(sizeof(Rectangle) * numOfRectangles);
    for (int i = 0; i < numOfRectangles; i++) {
        Rectangle r = {i * tileWidth, 0, tileWidth, tileHeight};
        rectangles[i] = r;
    }

    return rectangles;
}

int FindNumOfTiles(int tileWidth, TextureFile textureFile) {
    return textures[textureFile].width / tileWidth;
}
