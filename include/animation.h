/**********************************************************************************************
 *
 **   animation.h is responsible for classifying, creating, and drawing sprite animations.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
 *
 *    @include texture.h, timer.h
 *
 **********************************************************************************************/

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "texture.h"
#include "timer.h"

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum for accessing specific animations in an Entity's
 * animation array.
 *
 * @attention Not all can be accessed if an entity doesn't have that
 *            animation.
 *
 * @param IDLE_ANIMATION      0
 * @param MOVE_ANIMATION      1
 * @param ATTACK_ANIMATION    2
 */
typedef enum AnimationType {
    IDLE_ANIMATION = 0,
    MOVE_ANIMATION,
    ATTACK_ANIMATION
} AnimationType;

//* ------------------------------------------
//* STRUCTURES

/**
 * Represents a Sprite Animation.
 *
 * ! @attention Timer checking is not handled internally and must be handled in order to control various states.
 *
 * @param fps           Frames per second (speed) of this animation
 * @param numOfFrames   Number of frames that capture each tile in texture.
 * @param frames        List of frames the capture each tile's x, y, width and height in texture.
 * @param curFrame      Current frame being rendered on the screen.
 * @param texture       Tile texture for this animation.
 * @param timer         Time for this animation.
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
    /**
     * Current frame being rendered on the screen.
     */
    int curFrame;
    /** The tile texture for this animation. */
    Texture2D texture;
    /**
     * The time for this animation.
     */
    Timer timer;
} Animation;

/**
 * Structure that represents an array of animations.
 *
 * ! @attention animationArr must be freed later.
 *
 * @param size          Size of the array
 * @param animationArr  Pointer to the animation array in the heap
 *
 * @note On Entity structs, specific animations in the array should be accessed
 *       through the AnimationType enum.
 */
typedef struct AnimationArray {
    /** The size of the animation array. */
    int size;
    /** Pointer to the animation array.
     * @note On entities, should be accessed through the AnimationType
     *       enum.
     */
    Animation* animationArr;
} AnimationArray;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Constructs an instance of an Animation in memory and returns a pointer.
 *
 * ! @attention returns an empty Animation if given an invalid textureType.
 *
 * @param fps               Rate at which the sprite frames are updated.
 * @param tileWidth         Width of a single tile.
 * @param tileHeight        Height of a single tile.
 * @param textureFileType   Type of texture as a TextureFile.
 *
 * ! @note This function is responsible for creating animation.reactangles in the heap.
 * ? @note The timer of this animation must be started/managed externally.
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
 * ? @note Assumes that the GameState and timer of this animation has been handled.
 * ? @note Uses DrawTexturePro from Raylib.
 * ? @note Uses TimerDone and GetElapsedTime from timer.c
 */
void DrawAnimation(Animation* animation, Rectangle dest, int entityWidth, int entityHeight, float rotation);

/**
 * Draws the provided animation at a given frame at the destination rectangle.
 *
 * ! @attention returns if given a NULL animation.
 *
 * @param animation     Animation to draw.
 * @param dest          Destination rectangle to draw on.
 * @param entityWidth   Width of an entity's sprite tile.
 * @param entityHeight  Height of an entity's sprite tile.
 * @param rotation      Rotation of the Rectangles to draw.
 * @param frame         Frame to draw.
 * 
 * ? @note Uses DrawTexturePro from Raylib.
 */
void DrawAnimationFrame(Animation* animation, Rectangle dest, int entityWidth, int entityHeight, float rotation, int frame);

/**
 * Unloads and frees the memory of all the Animations in the animation array.
 * Sets its size to zero and the pointer to NULL.
 *
 * @param animationArray Array of animations that need to be freed.
 */
void UnloadAnimationArray(AnimationArray* animationArray);

/**
 * Responsible for unloading an animation by unallocating the memory
 * used to store the frames and the timer in an animation.
 *
 * ! @attention returns if given a NULL animation.
 *
 * @param animation The animation to unallocate.
 *
 * ! @note This function frees the memory of animation.frames.
 */
void AnimationUnload(Animation* animation);

#endif // ANIMATION_H_