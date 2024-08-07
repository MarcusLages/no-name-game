/**********************************************************************************************
 *
 **   animation.h is responsible for classifying, creating, and drawing sprite animations.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.1
 *
 *    @include timer.h, texture.h
 *
 **********************************************************************************************/

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "texture.h"
#include "timer.h"

//* ------------------------------------------
//* STRUCTURES

/**
 * Represents a Sprite Animation.
 *
 * ! @attention Timer checking is not handled internally and must be handled in order to control various states.
 *
 * ? @note - Timer must be started with StartTimer from timer.c see Timer.
 * ? @note - Each animation must be unloaded with AnimationUnload.
 */
typedef struct Animation {
    /** The frames per second of this animation. */
    int fps;
    /** The number of frames that capture each tile in texture. */
    int numOfFrames;
    /**
     * ! @attention This pointer will point to a location in heap that must be freed.
     *
     * The list of frames the capture each tile's x, y, width and height in texture.
     */
    Rectangle* frames;
    /** The texture for this animation. */
    Texture2D texture;
    /**
     * The time for this animation.
     */
    Timer timer;
} Animation;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Constructs an instance of an Animation in memory and returns a pointer.
 *
 * ! @attention returns an empty Animation if given an invalid textureType.
 * ! @note This function is responsible for creating animation.reactangles in the heap.
 *
 * @param fps               Rate at which the sprite frames are updated.
 * @param tileWidth         Width of a single tile.
 * @param tileHeight        Height of a single tile.
 * @param textureFileType   Type of texture as a TextureFile.
 *
 * @returns An Animation.
 */
Animation CreateAnimation(int fps, int tileWidth, int tileHeight, TextureFile textureType);

/**
 * Draws the provided animation at the destination rectangle.
 *
 * ! @attention returns if given a NULL animation.
 *
 * @param animation     Animation to draw.
 * @param dest          Destination rectangle to draw on.
 * @param entityWidth   Width of an entity's sprite tile.
 * @param entityHeight  Height of an entity's sprite tile.
 * @param rotation      Rotation of the Rectangles to draw.
 *
 * ? @note Assumes that the GameState has been handles for the entity and the animation desired is provided.
 * ? @note Uses DrawTexturePro from Raylib.
 * ? @note Uses TimerDone and GetElapsedTime from timer.c
 */
void DrawAnimation(Animation* animation, Rectangle dest, int entityWidth, int entityHeight, float rotation);

/**
 * Responsible for unloading an animation by unallocating the memory
 * used to store the frames and the timer in an animation.
 *
 * ! @attention returns if given a NULL animation.
 * ! @note This function frees the memory of animation.frames.
 *
 * @param animation The animation to unallocate.
 */
void AnimationUnload(Animation* animation);

#endif // ANIMATION_H_