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
 * 
 * ! @attention returns if given a NULL animation.
 */
static void DrawAnimation(Animation *animation, Rectangle dest, int entityWidth, int entityHeight, float rotation);

/**
 * Returns a pointer to an array of frames where each rectangle marks the 
 * x, y, width, and height of a sprite tile.
 * 
 * ! @attention This pointer will point to a location in heap that must be freed.
 * 
 * @param numOfFrames the number of frames/tiles in the sprite.
 * @param tileWidth the width of a single sprite tile.
 * @param tileHeight the height of a single sprite tile.
 * @returns a pointer to an array of Rectangles.
 */
static Rectangle* GetSpriteRectangles(int numOfFrames, int tileWidth, int tileHeight);

/**
 * Returns the number of tiles present in a specified sprite with the given tile width.
 * 
 * ! @attention returns -1 if given an invalid textureType.
 * 
 * @param tileWidth the with of a single tile on the sprite.
 * @param textureFile the type of TextureFile to calculate.
 * @returns number of tiles as an int.
 */
static int FindNumOfTiles(int tileWidth, TextureFile textureType);

void AnimationRender(Entity *entity, Animation *animation, int entityWidth, 
    int entityHeight, int xOffset, int yOffset, float rotation) {
    if (entity == NULL || animation == NULL) return;
    DrawAnimation(
        animation, 
        (Rectangle) {entity->x + xOffset, entity->y + yOffset, 
                     entityWidth < 0 ? -entityWidth : entityWidth, 
                     entityHeight < 0 ? -entityHeight : entityHeight}, 
        entityWidth, 
        entityHeight, 
        rotation);
}

Animation* CreateAnimation(int fps, int tileWidth, int tileHeight, TextureFile textureType, Texture2D texture) {
    if (textureType < TILE_MAP || textureType > TILE_ENEMY_ATTACK) return NULL;

    int numOfTiles = FindNumOfTiles(tileWidth, textureType);
    Rectangle *frames = GetSpriteRectangles(numOfTiles, tileWidth, tileHeight);
    Timer *timer = (Timer*) malloc(sizeof(Timer));

    /** Creating an animation. NOTE: timer is not started only an instance is created */     
    Animation *animation = (Animation*) malloc(sizeof(Animation));

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

void AnimationUnload(Animation *animation) {
    if (animation == NULL) return;

    // freeing animation's attributes
    free(animation->frames);
    free(animation->timer);
    animation->frames = NULL;
    animation->timer = NULL;

    // freeing animation
    free(animation);
    animation = NULL;
}

static void DrawAnimation(Animation *animation, Rectangle dest, int entityWidth, int entityHeight, 
    float rotation) {
    if (TimerDone(animation->timer) || animation == NULL) return; 

    int idx = (int) (GetElapsedTime(animation->timer) * animation->fps) % animation->numOfFrames;
    Rectangle source = animation->frames[idx];

    source.width = entityWidth;
    source.height = entityHeight;
    DrawTexturePro(animation->texture, source, dest, (Vector2) { 0, 0 }, rotation, WHITE);
}

static Rectangle* GetSpriteRectangles(int numOfFrames, int tileWidth, int tileHeight) {
    // Allocating memory for the appropiate amount of frames
    Rectangle *frames = (Rectangle*) malloc(sizeof(Rectangle) * numOfFrames);
    if (frames == NULL) exit(EXIT_FAILURE);

    // Populating the array
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
