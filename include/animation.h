#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "texture.h"
#include "timer.h"

//* STRUCTURES

/**
 * Represents information needed for a Sprite Animation.
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
     * The list of frames the capture each tile's x, y, width and height in texture. 
     * 
     * ! @attention This pointer will point to a location in heap that must be freed.
     */ 
    Rectangle* frames;
    /** The texture for this animation. */
    Texture2D texture;
    /** 
     * The time for this animation.
     * 
     * ! @attention This pointer will point to a location in heap that must be freed.
     */
    Timer* timer;
} Animation;

//* FUNCTION PROTOTYPES

/**
 * Constructs an instance of an Animation struct and returns it.
 * 
 * ! @note This function is responsible for creating the animation, animation.reactangles, and animation.timer in the heap.
 * 
 * @param fps the rate at which the sprite frames are updated.
 * @param tileWidth the width of a single tile.
 * @param tileHeight the height of a single tile.
 * @param textureFileType the type of texture as a TextureFile.
 * @param texture the sprite texture as a Texture2D.
 * @returns a pointer to an animation in memory.
 * 
 * ! @attention returns NULL if given an invalid textureType.
 */
Animation* CreateAnimation(int fps, int tileWidth, int tileHeight, TextureFile textureType, Texture2D texture);

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
 * 
 * ! @attention returns if given a NULL animation.
 */
void DrawAnimation(Animation* animation, Rectangle dest, int entityWidth, int entityHeight, float rotation);

/**
 * Responsible for unloading an animation by unallocating the memory 
 * used to store the frames and the timer in an animation.
 * 
 * ! @note This function frees the memory of animation.frames, animation.timer and the animation.
 * 
 * @param animation the animation to unallocate.
 * 
 * ! @attention returns if given a NULL animation.
 */
void AnimationUnload(Animation* animation);

#endif //ANIMATION_H_