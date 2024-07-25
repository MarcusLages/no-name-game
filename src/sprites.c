#include "texture.h"
#include "entities.h"

//* VARIABLES

// The animation for an idle player
Animation idlePlayerAnimation;

void SpriteUpdate() {
    int numOfTiles = FindNumOfTiles(ENTITY_TILE_WIDTH, TILE_PLAYER_IDLE);

    // Initializing the indle animation
    idlePlayerAnimation = CreateAnimation(ENTITY_IDLE_FPS, 
        numOfTiles,
        GetSpriteRectangles(numOfTiles, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_IDLE), 
        textures[TILE_PLAYER_IDLE]);

    //
    DrawAnimation(idlePlayerAnimation, (Rectangle) {0, 0, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT}, 0.0f, WHITE);
}

void SpriteUnload() {
    UnloadAnimation(idlePlayerAnimation);
}

Animation CreateAnimation(int fps, int numOfRectangles, Rectangle *rectangles, Texture2D tiles) {
    Animation animation = {
        .fps = fps,
        .numOfRectangles = numOfRectangles,
        .rectangles = rectangles,
        .textures = tiles
    };

    return animation;
}

void UnloadAnimation(Animation animation) {
    free(animation.rectangles);
    animation.rectangles = NULL;
}    

void DrawAnimation(Animation animation, Rectangle dest, float rotation, Color color) {
    // grabing a new index every new frame since the game was started
    int idx = (int) (GetTime() * animation.fps) % animation.numOfRectangles;
    Rectangle source = animation.rectangles[idx];
    Vector2 origin = { 0, 0 };

    DrawTexturePro(animation.textures, source, dest, origin, rotation, color);
}

Rectangle* GetSpriteRectangles(int numOfRectangles, int tileWidth, int tileHeight, TextureFile textureFile) {
    // Allocating memory for the appropiate amount of rectangles
    Rectangle *rectangles = (Rectangle*) malloc(sizeof(Rectangle) * numOfRectangles);

    // Populating the array
    for (int i = 0; i < numOfRectangles; i++) {
        Rectangle r = {i * tileWidth, 0, tileWidth, tileHeight};
        rectangles[i] = r;
    }

    return rectangles;
}

int FindNumOfTiles(int tileWidth, TextureFile textureFile) {
    return textures[textureFile].width / tileWidth;
}
