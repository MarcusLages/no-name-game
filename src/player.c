#include "raylib.h"
#include "raymath.h"
#include "entities.h"

Player player;

void PlayerStartup() {
    player.x = 0;
    player.y = 0;
    player.speed = 4;
    player.state = IDLE;
}

void PlayerMovement() {
    Vector2 direction = {0, 0};

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        direction.x++;
    else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        direction.x--;
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        direction.y++;
    else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        direction.y--;

    if(direction.x == 0 && direction.y == 0)
        return;

    direction = Vector2Normalize(direction);

    player.x += direction.x * player.speed;
    player.y += direction.y * player.speed;
    
}

void PlayerRender() {

}

void PlayerUnload() {

}