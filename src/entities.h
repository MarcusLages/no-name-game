#ifndef ENTITIES_H
#define ENTITIES_H

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

//* VARIABLES

extern Player player;

#endif // !ENTITIES_H