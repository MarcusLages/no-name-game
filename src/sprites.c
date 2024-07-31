#include "entities.h"

//* FUNCTION PROTOTYPES

/**
 * Draws the provided animation at the destination rectangle. 
 * 
 * ? @note Assumes that the GameState has been handles for the entity and the animation desired is provided.
 * 
 * @param animation the animation to draw.
 * @param dest the destination rectangle to draw on.
 * @param entityWidth the width of an entity's sprite tile.
 * @param entityHeight the height of an entity's sprite tile.
 * @param rotation the rotation of the Rectangles to draw.
 * 
 * ? Uses DrawTexturePro from Raylib.
 */
void DrawAnimation(Animation *animation, Rectangle dest, int entityWidth, int entityHeight, float rotation);

/**
 * Returns the number of tiles present in a specified sprite with the given tile width.
 * 
 * @param tileWidth the with of a single tile on the sprite.
 * @param textureFile the type of TextureFile to calculate.
 * @returns number of tiles as an int.
 */
int FindNumOfTiles(int tileWidth, TextureFile textureFile);

/**
 * Returns a pointer to an array of rectangles where each rectangle marks the 
 * x, y, width, and height of a sprite tile.
 * 
 * @param numOfRectangles the number of rectangles/tiles in the sprite.
 * @param tileWidth the width of a single sprite tile.
 * @param tileHeight the height of a single sprite tile.
 * @returns a pointer to an array of Rectangles.
 */
Rectangle* GetSpriteRectangles(int numOfRectangles, int tileWidth, int tileHeight);

void AnimationRender(Entity entity, Animation *animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation) {
    DrawAnimation(
        animation, 
        (Rectangle) {entity.x + xOffset, entity.y + yOffset, 
                     entityWidth < 0 ? -entityWidth : entityWidth, 
                     entityHeight < 0 ? -entityHeight : entityHeight}, 
        entityWidth, 
        entityHeight, 
        rotation);
}

Animation* CreateAnimation(int fps, int tileWidth, int tileHeight, TextureFile textureFileType, Texture2D tiles) {
    int numOfTiles = FindNumOfTiles(tileWidth, textureFileType);
    Rectangle *rectangles = GetSpriteRectangles(numOfTiles, tileWidth, tileHeight);
    Timer *timer = (Timer*) malloc(sizeof(Timer));

    /** Creating an animation. NOTE: timer is not started only an instance is created */     
    Animation *animation = (Animation*) malloc(sizeof(Animation));
    *animation = (Animation) {
        .fps = fps,
        .numOfRectangles = numOfTiles,
        .rectangles = rectangles,
        .timer = timer,
        .textures = tiles
    };

    return animation;
}

void AnimationUnload(Animation *animation) {
    // freeing animation's attributes
    free((*animation).rectangles);
    free((*animation).timer);
    (*animation).rectangles = NULL;
    (*animation).timer = NULL;
    // freeing animation
    free(animation);
    animation = NULL;
}

void DrawAnimation(Animation *animation, Rectangle dest, int entityWidth, int entityHeight, 
    float rotation) {
    if (TimerDone((*animation).timer)) return; 

    int idx = (int) (GetElapsedTime((*animation).timer) * (*animation).fps) % (*animation).numOfRectangles;
    Rectangle source = (*animation).rectangles[idx];

    source.width = entityWidth;
    source.height = entityHeight;
    DrawTexturePro((*animation).textures, source, dest, (Vector2) { 0, 0 }, rotation, WHITE);
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
