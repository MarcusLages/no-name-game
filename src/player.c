/***********************************************************************************************
*
**   player.c is responsible for implementating functions to setup a player, manage movement, 
**   and attacks. Animations are created on setup and managed based on player movements and attack.
*   
*    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
*    @version 0.1
*
*    @include player.h
*
***********************************************************************************************/

#include "../include/player.h"

//* ------------------------------------------
//* DEFINITIONS

/**
 * Macro function to get the absolute/module value of a number.
 * 
 * @param x Number
 */
#define ABS(x) (x > 0 ? x : x * (-1))

//* ------------------------------------------
//* GLOBAL VARIABLES

Entity player;

//* ------------------------------------------
//* MODULAR VARIABLES

/** The animation for an idle player. */
static Animation idlePlayerAnimation;

/** The animation for the player moving. */
static Animation movingPlayerAnimation;

/** The animation for a player attack. */
static Animation attackPlayerAnimation;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Renders the player attack animation based off of it's Direction.
 */
static void RenderPlayerAttack();

/**
 * Handles player collision with the world tilemap(Tile**)
 */
static void PlayerWorldCollision();

/**
 * TODO: Implementation + checking how to pass the list or make it global
 * Handles player collision with the enemies list by movement.
 */
static void PlayerEnemyCollision();

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void PlayerStartup() {
    player.x = 0;
    player.y = 0;
    player.speed = 300;
    player.health = 1;
    player.direction = Vector2Zero();
    player.faceValue = 1;
    player.state = IDLE;
    player.directionFace = RIGHT;

    // Initializing the idle animation
    idlePlayerAnimation = CreateAnimation(
        DEFAULT_IDLE_FPS,
        ENTITY_TILE_WIDTH,
        ENTITY_TILE_HEIGHT,
        TILE_PLAYER_IDLE,
        textures[TILE_PLAYER_IDLE]);

    // Initializing the moving animation
    movingPlayerAnimation = CreateAnimation(
        DEFAULT_MOVING_FPS,
        ENTITY_TILE_WIDTH,
        ENTITY_TILE_HEIGHT,
        TILE_PLAYER_MOVE,
        textures[TILE_PLAYER_MOVE]);

    // Initializing the attacking animation
    attackPlayerAnimation = CreateAnimation(
        DEFAULT_ATTACK_FPS,
        TEMP_ATTACK_WIDTH,
        TEMP_ATTACK_HEIGHT,
        TILE_PLAYER_ATTACK,
        textures[TILE_PLAYER_ATTACK]);

    // Starting timers for both idle and moving animations
    StartTimer(&idlePlayerAnimation.timer, -1.0f);
    StartTimer(&movingPlayerAnimation.timer, -1.0f);
}

//TODO: Movement is buggy, Vector normalization is not working properly. Player cannot move diagonally ata lower speeds.
void PlayerMovement() {
    // Ensures the player cannot move while attacking   
    if (player.state == ATTACKING) return; 

    // Delta time helps to not let player speed depend on framerate.
    float deltaTime = GetFrameTime();
    player.direction = Vector2Zero();

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.direction.x++;
        player.faceValue = 1;
        player.directionFace = RIGHT;
    } else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.direction.x--;
        player.faceValue = -1;
        player.directionFace = LEFT;
    } 
    
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.direction.y++;
        player.directionFace = DOWN;
    } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.direction.y--;
        player.directionFace = UP;
    }

    // Set the player to MOVING if not ATTACKING.
    player.state = player.state == ATTACKING ? ATTACKING : MOVING;
    
    // Set the player to IDLE if not ATTACKING or moving on any direction
    if(player.direction.x == 0 && player.direction.y == 0 && player.state != ATTACKING) {
        player.state = IDLE;
        return;
    }

    // DrawText(TextFormat("Player x: %d", player.x), 0, 0, 20, RED);
    // DrawText(TextFormat("Player y: %d", player.y), 0, 20, 20, RED);  
    // DrawText(TextFormat("Player speed: %d", player.speed), 0, 40, 20, RED);
    // DrawText(TextFormat("Player before dir x: %f", player.direction.x), 0, 60, 20, RED); 
    // DrawText(TextFormat("Player before dir y: %f", player.direction.y), 0, 80, 20, RED); 

    player.direction = Vector2Normalize(player.direction);

    // DrawText(TextFormat("Player after norm dir x: %f", player.direction.x), 0, 100, 20, RED); 
    // DrawText(TextFormat("Player after norm dir y: %f", player.direction.y), 0, 120, 20, RED); 

    // NOTE: Dont add deltatime on acceleration/velocity/direction, add it on the speed or movement.
    player.direction.x *= player.speed;
    player.direction.y *= player.speed;

    // DrawText(TextFormat("Player after dir * speed x: %f", player.direction.x), 0, 140, 20, RED); 
    // DrawText(TextFormat("Player after dir * speed y: %f", player.direction.y), 0, 160, 20, RED);
  
    PlayerWorldCollision();
    PlayerEnemyCollision();

    // DrawText(TextFormat("Enemy after dir * deltatime x: %f", player.direction.x * deltaTime), 0, 180, 20, RED);
    // DrawText(TextFormat("Enemy after dir * deltatime y: %f", player.direction.y * deltaTime), 0, 200, 20, RED);

    player.x += player.direction.x * deltaTime < 0 ? ceil(player.direction.x * deltaTime) : floor(player.direction.x * deltaTime);
    player.y += player.direction.y * deltaTime < 0 ? ceil(player.direction.y * deltaTime) : floor(player.direction.y * deltaTime);
}

static void PlayerWorldCollision() {
    if(world == NULL) return;

    CollisionNode* playerCollisionList;
    playerCollisionList = NULL;

    CollisionNode* collidingTile;
    collidingTile = collidableTiles;

    while(collidingTile != NULL) {
        RayCollision2D playerCollision;
        Rectangle tileHitbox = (Rectangle) {
            .x = collidingTile->collidedHitbox.index.x * TILE_WIDTH,
            .y = collidingTile->collidedHitbox.index.y * TILE_HEIGHT,
            .width = TILE_WIDTH,
            .height = TILE_HEIGHT
        };

        playerCollision = EntityRectCollision(player, tileHitbox);
        if(playerCollision.hit == true && playerCollision.timeHit >= 0) {
            if(playerCollisionList == NULL)
                playerCollisionList = CreateCollisionList(collidingTile->collidedHitbox.index.x, collidingTile->collidedHitbox.index.y, playerCollision.timeHit);
            else
                AddCollisionNode(playerCollisionList, collidingTile->collidedHitbox.index.x, collidingTile->collidedHitbox.index.y, playerCollision.timeHit);
        }
        collidingTile = collidingTile->next;
    }
    
    if(playerCollisionList != NULL) {
        SortCollisionList(playerCollisionList);

        CollisionNode* resolvingNode = playerCollisionList;
        while(resolvingNode != NULL) {
            RayCollision2D playerCollision;
            Rectangle tileHitbox = (Rectangle) {
                .x = world[(int) resolvingNode->collidedHitbox.index.y][(int) resolvingNode->collidedHitbox.index.x].x * TILE_WIDTH,
                .y = world[(int) resolvingNode->collidedHitbox.index.y][(int) resolvingNode->collidedHitbox.index.x].y * TILE_HEIGHT,
                .width = TILE_WIDTH,
                .height = TILE_HEIGHT
            };

            playerCollision = EntityRectCollision(player, tileHitbox);
            if(playerCollision.hit == true && playerCollision.timeHit >= 0) {
                player.direction.x += playerCollision.normalVector.x * ABS(player.direction.x) * (1 - playerCollision.timeHit);
                player.direction.y += playerCollision.normalVector.y * ABS(player.direction.y) * (1 - playerCollision.timeHit);
            }
            resolvingNode = resolvingNode->next;
        }

        FreeCollisionList(playerCollisionList);
        playerCollisionList = NULL;
    }

}

static void PlayerEnemyCollision() {}

void PlayerAttack() {
    if (IsKeyPressed(KEY_E)) {
        player.state = ATTACKING;
        StartTimer(&attackPlayerAnimation.timer, 0.5f);
    }

    if (player.state == ATTACKING && TimerDone(&attackPlayerAnimation.timer)) {
        player.state = IDLE;
    }
}

void PlayerRender() {
    DrawText(TextFormat("Player x: %d", player.x), 0, 0, 20, RED);
    DrawText(TextFormat("Player y: %d", player.y), 0, 20, 20, RED);
    switch (player.state) {
        case IDLE:
            EntityRender(&player, &idlePlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case MOVING:
            EntityRender(&player, &movingPlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue, 
                ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case ATTACKING:
            RenderPlayerAttack();
            break;
        default:
            break;
    }
}

static void RenderPlayerAttack() {
    // Rendering idle animation of player as the player should not move while attacking.
    EntityRender(&player, &idlePlayerAnimation, ENTITY_TILE_WIDTH * player.faceValue, 
        ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
    switch (player.directionFace) {
        case RIGHT:
            EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, -TEMP_ATTACK_HEIGHT, 
                TEMP_ATTACK_WIDTH + 2, TEMP_ATTACK_HEIGHT + 6, 180.0f);
            // EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, 
            // 32, 0, 90.0f);
            break;
        case DOWN:
            EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, 
                -TEMP_ATTACK_HEIGHT * player.faceValue, TEMP_ATTACK_WIDTH - 40, TEMP_ATTACK_HEIGHT + 16, -90.0f);
            // EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, 
            //     25, 48, 180.0f);
            break;
        case LEFT:
            EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, 
                TEMP_ATTACK_WIDTH - 48, TEMP_ATTACK_HEIGHT - 26, 0.0f);
            // EntityRender(&player, &attackPlayerAnimation, TEMP_ATTACK_WIDTH, -TEMP_ATTACK_HEIGHT, 
            // 16, 0, 90.0f);
            break;
        case UP:
            EntityRender(&player, &attackPlayerAnimation, -TEMP_ATTACK_WIDTH, 
                -TEMP_ATTACK_HEIGHT * player.faceValue, TEMP_ATTACK_WIDTH - 40, TEMP_ATTACK_HEIGHT - 6, -90.0f);
            //  EntityRender(&player, &attackPlayerAnimation, -TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, 
            //     -10, 0, 0.0f);
            break;
        default:
            break;
    }
}

void PlayerUnload() {
    AnimationUnload(&idlePlayerAnimation);
    AnimationUnload(&movingPlayerAnimation);
    AnimationUnload(&attackPlayerAnimation);
}