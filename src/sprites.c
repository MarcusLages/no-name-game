#include "entities.h"

/**
 * Draws the provided animation at the destination rectangle. 
 * 
 * Assumes that the GameState has been checked for the entity and the animation desired is provided.
 * 
 * @param animation the animation profile as a structure of the sprite.
 * @param dest the destination rectangle.
 * @param entityWidth the width of the entity rectangle.
 * @param entityHeight the height of the entity rectangle.
 * @param rotation the rotation of the Rectangles to draw.
 * @param color the tint profile.
 * 
 * Uses DrawTexturePro from Raylib.
 */
void DrawAnimation(Animation animation, Rectangle dest, int entityWidth, int entityHeight, float rotation, Color color, bool loop);

/**
 * Returns the number of tiles present in a specified sprite with the given tile width.
 * 
 * @param tileWidth the with of a single tile on the sprite.
 * @param textureFile the type of TextureFile to calculate.
 * @returns number of tiles as an int.
 */
int FindNumOfTiles(int tileWidth, TextureFile textureFile);

/**
 * Returns a pointer to an array of rectangles.
 * 
 * The array of rectangles depends on the type of TextureFile given and number of rectangles present.
 * 
 * @param numOfRectangles the number of rectangles/tiles in the sprite.
 * @param tileWidth the width of a single tile.
 * @param tileHeight the height of a single tile.
 * @returns a pointer to an array of Rectangles.
 */
Rectangle* GetSpriteRectangles(int numOfRectangles, int tileWidth, int tileHeight);

void SpriteRender(Entity entity, Animation animation, int entityWidth, int entityHeight, bool loop) {
    DrawAnimation(
        animation, 
        (Rectangle) {entity.x, entity.y, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT}, 
        entityWidth, 
        entityHeight, 
        0.0f, 
        WHITE,
        true);
}

Animation CreateAnimation(int fps, int xOffset, int yOffset, TextureFile textureFileType, Texture2D tiles) {
    int numOfTiles = FindNumOfTiles(ENTITY_TILE_WIDTH, textureFileType);
    Rectangle *rectangles = GetSpriteRectangles(numOfTiles, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT);

    Animation animation = {
        .fps = fps,
        .numOfRectangles = numOfTiles,
        .xOffset = xOffset,
        .yOffset = yOffset,
        .rectangles = rectangles,
        .textures = tiles
    };

    return animation;
}

void SpriteUnload(Animation animation) {
    free(animation.rectangles);
    animation.rectangles = NULL;
}

void DrawAnimation(Animation animation, Rectangle dest, int entityWidth, int entityHeight, float rotation, Color color, bool loop) {
    int idx;
    Rectangle source;
    if (!loop) {
        for (int i = 0; i < animation.numOfRectangles; i++) {
            idx = (int) (GetTime() * animation.fps) % animation.numOfRectangles;
            source = animation.rectangles[idx];
        }
    } else {
            idx = (int) (GetTime() * animation.fps) % animation.numOfRectangles;
            source = animation.rectangles[idx];
    }

    source.width = entityWidth;
    source.height = entityHeight;
    dest.x += animation.xOffset;
    dest.y += animation.yOffset;
    DrawTexturePro(animation.textures, source, dest, (Vector2) { 0, 0 }, rotation, color);
}

Rectangle* GetSpriteRectangles(int numOfRectangles, int tileWidth, int tileHeight) {
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
