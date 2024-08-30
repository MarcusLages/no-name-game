/**********************************************************************************************
 *
 **   animation.c is responsible for implementating functions to create, draw, and unload animations.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
 *
 *    @include <stdlib.h>, animation.h, pause.h
 *
 **********************************************************************************************/

#include "../include/animation.h"
#include "../include/pause.h"
#include <stdlib.h>

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

Animation CreateAnimation(int fps, int tileWidth, int tileHeight, TextureFile textureType) {
    //! Returning a blank animation if an incorrect textureType is given.
    if(textureType < 0 || textureType > MAX_TEXTURES) return (Animation){};

    int numOfTiles    = FindNumOfTiles(tileWidth, textureType);
    Rectangle* frames = GetSpriteRectangles(numOfTiles, tileWidth, tileHeight);

    Animation animation = (Animation){ .fps         = fps,
                                       .numOfFrames = numOfTiles,
                                       .frames      = frames,
                                       .curFrame    = 0,
                                       .timer       = (Timer){ 0.0, 0.0 },
                                       .texture     = textures[textureType] };

    return animation;
}

void DrawAnimation(Animation* animation, Rectangle dest, int entityWidth, int entityHeight, float rotation) {
    if(animation == NULL) return;
    if(TimerDone(&animation->timer)) return;

    // if there is a delay return
    if(CheckIfDelayed(&animation->timer)) return;

    if(!isPaused) {
        animation->curFrame =
            (int) (GetElapsedTime(&animation->timer) * animation->fps) %
            animation->numOfFrames;
    }
    Rectangle source = animation->frames[animation->curFrame];

    source.width  = entityWidth;
    source.height = entityHeight;
    DrawTexturePro(animation->texture, source, dest, (Vector2){ 0, 0 }, rotation, WHITE);
}

void DrawAnimationFrame(
    Animation* animation, Rectangle dest, int entityWidth, int entityHeight,
    float rotation, int frame) {
    if(animation == NULL) return;
    if(frame >= animation->numOfFrames || frame < 0) {
        TraceLog(LOG_WARNING, "ANIMATION.C (DrawAnimationFrame, line: %d): Invalid frame index given.", __LINE__);
        return;
    }

    Rectangle source    = animation->frames[frame];
    animation->curFrame = frame;

    source.width  = entityWidth;
    source.height = entityHeight;
    DrawTexturePro(animation->texture, source, dest, (Vector2){ 0, 0 }, rotation, WHITE);
}

void UnloadAnimationArray(AnimationArray* animationArray) {
    for(int i = 0; i < animationArray->size; i++) {
        AnimationUnload(&(animationArray->animationArr[i]));
    }
    free(animationArray->animationArr);
    animationArray->animationArr = NULL;
    animationArray->size         = 0;
}

void AnimationUnload(Animation* animation) {
    if(animation == NULL) return;

    // Freeing frames rom the animation.
    free(animation->frames);
    animation->frames = NULL;
    animation         = NULL;
}

static Rectangle* GetSpriteRectangles(int numOfFrames, int tileWidth, int tileHeight) {
    Rectangle* frames = (Rectangle*) malloc(sizeof(Rectangle) * numOfFrames);

    //! If allocation fails program exits.
    if(frames == NULL) {
        TraceLog(LOG_FATAL, "ANIMATION.C (GetSpriteRectangles, line: %d): Memory allocation for animation failure.", __LINE__);
    }

    // Populating the array.
    for(int i = 0; i < numOfFrames; i++) {
        Rectangle r = { i * tileWidth, 0, tileWidth, tileHeight };
        frames[i]   = r;
    }

    return frames;
}

static int FindNumOfTiles(int tileWidth, TextureFile textureType) {
    if(textureType < 0 || textureType > MAX_TEXTURES) return -1;
    return textures[textureType].width / tileWidth;
}