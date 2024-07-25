#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "raymath.h"

// Enum for the action state of entities for animation and properties.
typedef enum GameState {
    IDLE = 0,
    MOVING,
    ATTACKING
} GameState;

//* Player
// Structure to represent the player.
typedef struct Player {
    int x;
    int y;
    GameState state;
} Player;

//* Enemy
// Structure to represent the player.
typedef struct Enemy {
    int x;
    int y;
    GameState state;
    int health;
} Enemy;

// Structure to represent information needed for a Sprite Animation 
typedef struct Animation {
    int fps;
    Rectangle *rectangles;
    Texture2D *textures;
} Animation;

//* VARIABLES

extern Player player;

//* FUNCTION PROTOTYPES

// Player functions
void PlayerStartup();
void PlayerMovement();

#endif // !ENTITIES_H