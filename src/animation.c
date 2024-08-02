/**********************************************************************************************
*
**   animation.c is responsible for implementating functions to create, draw, and unload animations.
*   
*    @authors Marcus Vinicius Lages Santos and Samarjit Bhogal
*    @version 1.0
*
**********************************************************************************************/

#include "../include/animation.h"

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Returns a pointer to an array of frames where each frame is a rectangle that marks the 
 * x, y, width, and height of a sprite tile.
 * 
 * ! @attention This pointer will point to a location in heap that must be freed.
 * 
 * @param numOfFrames   The number of frames/tiles in the sprite.
 * @param tileWidth     The width of a single sprite tile.
 * @param tileHeight    The height of a single sprite tile.
 * 
 * @returns A pointer to an array of Rectangles.
 */
static Rectangle* GetSpriteRectangles(int numOfFrames, int tileWidth, int tileHeight);

/**
 * Returns the number of tiles present in a specified sprite-map with the given tile width.
 * 
 * ! @attention returns -1 if given an invalid textureType.
 * 
 * @param tileWidth     The with of a single tile on the sprite.
 * @param textureFile   The type of TextureFile to calculate.
 * 
 * @returns The number of tiles as an int.
 */
static int FindNumOfTiles(int tileWidth, TextureFile textureType);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

Animation* CreateAnimation(int fps, int tileWidth, int tileHeight, TextureFile textureType, Texture2D texture) {
    if (textureType < TILE_MAP || textureType > TILE_ENEMY_ATTACK) return NULL;

    int numOfTiles = FindNumOfTiles(tileWidth, textureType);
    Rectangle *frames = GetSpriteRectangles(numOfTiles, tileWidth, tileHeight);
    Timer *timer = (Timer*) malloc(sizeof(Timer));

    /** Creating an animation. NOTE: timer is not started only an instance is created. */     
    Animation *animation = (Animation*) malloc(sizeof(Animation));

    // If allocation fails program exits.
    if (timer == NULL || animation == NULL) exit(EXIT_FAILURE);
    
    *animation = (Animation) {
        .fps = fps,
        .numOfFrames = numOfTiles,
        .frames = frames,
        .timer = timer,
        .texture = texture
    };

    return animation;
}


void DrawAnimation(Animation* animation, Rectangle dest, int entityWidth, int entityHeight, 
    float rotation) {
    if (TimerDone(animation->timer) || animation == NULL) return; 

    int idx = (int) (GetElapsedTime(animation->timer) * animation->fps) % animation->numOfFrames;
    Rectangle source = animation->frames[idx];

    source.width = entityWidth;
    source.height = entityHeight;
    DrawTexturePro(animation->texture, source, dest, (Vector2) { 0, 0 }, rotation, WHITE);
}

void AnimationUnload(Animation* animation) {
    if (animation == NULL) return;

    // Freeing frames and timer from the animation.
    free(animation->frames);
    free(animation->timer);
    animation->frames = NULL;
    animation->timer = NULL;

    // Freeing the animation.
    free(animation);
    animation = NULL;
}

static Rectangle* GetSpriteRectangles(int numOfFrames, int tileWidth, int tileHeight) {
    Rectangle *frames = (Rectangle*) malloc(sizeof(Rectangle) * numOfFrames);

    // If allocation fails program exits.
    if (frames == NULL) exit(EXIT_FAILURE);

    // Populating the array.
    for (int i = 0; i < numOfFrames; i++) {
        Rectangle r = {i * tileWidth, 0, tileWidth, tileHeight};
        frames[i] = r;
    }

    return frames;
}

static int FindNumOfTiles(int tileWidth, TextureFile textureType) {
    if (textureType < TILE_MAP || textureType > TILE_ENEMY_ATTACK) return -1;
    return textures[textureType].width / tileWidth;
}