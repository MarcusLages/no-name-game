#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdbool.h>

// State of entities for animation and properties
typedef enum GameState {
    IDLE = 0,
    MOVING,
    ATTACKING
} GameState;

//* Player
typedef struct Player {
    int x;
    int y;
    GameState state;
} Player;

typedef struct Enemy {
    int x;
    int y;
    GameState state;
    int health;
} Enemy;

#endif // !ENTITIES_H