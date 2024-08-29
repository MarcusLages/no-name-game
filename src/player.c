/***********************************************************************************************
 *
 **   player.c is responsible for implementating functions to setup a player, manage movement,
 **   and attacks. Animations are created on setup and managed based on player movements and attack.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include player.h
 *
 ***********************************************************************************************/

#include "../include/player.h"
#include "../include/audio.h"
#include "../include/enemy-list.h"
#include "../include/utils.h"

//* ------------------------------------------
//* MACROS

/** Shortcut macro to access the player's animation array. */
#define playerAnimArray player.animations.animationArr

//* ------------------------------------------
//* GLOBAL VARIABLES

Entity player;

//* ------------------------------------------
//* MODULAR VARIABLES

/** Timer for the step sfx of the player. */
static Timer playerStepTimer;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Handles player movement and updates it's GameState and Direction.
 */
static void PlayerMovement();

/**
 * Handles the player's attack.
 *
 * ? @note Manages the timer for the player attack animation.
 * TODO: collisions, enemy health, etc..
 */
static void PlayerAttack();

/**
 * Renders the player attack animation based off of it's Direction.
 */
static void RenderPlayerAttack();

/**
 * Checks if the player has hit an enemy.
 *
 * @param attackHitbox Hitbox of the attack that will check the detection with
 *                     all the enemies
 */
static void PlayerAttackHit();

/**
 * Handles the player movement towards a given position.
 *
 * @param position The position to move the player to.
 *
 * ? @note Calls MoveEntityTowardsPos()
 */
static void MovePlayerToPos(Vector2 position);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void PlayerStartup() {
    player.pos = (Vector2){ (float) 11 * TILE_WIDTH, (float) 4 * TILE_HEIGHT };
    player.hitbox        = (Rectangle){ .x     = player.pos.x,
                                        .y     = player.pos.y + ENTITY_TILE_HEIGHT / 2,
                                        .width = ENTITY_TILE_WIDTH,
                                        .height = ENTITY_TILE_HEIGHT / 2 };
    player.speed         = 100;
    player.health        = 1;
    player.direction     = Vector2Zero();
    player.faceValue     = 1;
    player.state         = IDLE;
    player.directionFace = RIGHT;

    // Create and initialize animations in the player's animation array
    player.animations.size = MAX_PLAYER_ANIMATIONS;
    player.animations.animationArr =
        (Animation*) malloc(sizeof(Animation) * player.animations.size);

    Animation idlePlayerAnimation =
        CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_IDLE);

    Animation movingPlayerAnimation =
        CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_PLAYER_MOVE);

    Animation attackPlayerAnimation =
        CreateAnimation(DEFAULT_ATTACK_FPS, PLAYER_ATTACK_WIDTH, PLAYER_ATTACK_HEIGHT, TILE_PLAYER_ATTACK);

    player.animations.animationArr[IDLE_ANIMATION]   = idlePlayerAnimation;
    player.animations.animationArr[MOVE_ANIMATION]   = movingPlayerAnimation;
    player.animations.animationArr[ATTACK_ANIMATION] = attackPlayerAnimation;

    // Starting timers for both idle and moving animations
    StartTimer(&playerAnimArray[IDLE_ANIMATION].timer, -1.0);
    StartTimer(&playerAnimArray[MOVE_ANIMATION].timer, -1.0);

    playerStepTimer = (Timer) {0.0f, 0.0f};
    StartTimer(&playerStepTimer, 0.45f);

    TraceLog(LOG_INFO, "PLAYER.C (PlayerStartup): Player set successfully.");
}

void PlayerRender() {
    switch(player.state) {
        case IDLE:
            EntityRender(
                &player, &playerAnimArray[IDLE_ANIMATION],
                ENTITY_TILE_WIDTH * player.faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case MOVING:
            EntityRender(
                &player, &playerAnimArray[MOVE_ANIMATION],
                ENTITY_TILE_WIDTH * player.faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
            break;
        case ATTACKING: RenderPlayerAttack(); break;
        default: break;
    }
}

void PlayerUnload() {
    UnloadAnimationArray(&player.animations);
    TraceLog(LOG_INFO, "PLAYER.C (PlayerUnload): Player animations unloaded successfully.");
}

void PlayerUpdate() {
    PlayerMovement();
    PlayerAttack();
}

static void PlayerMovement() {
    player.direction = Vector2Zero();

    if(player.state == ATTACKING) return;

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.direction.x++;
        player.faceValue     = 1;
        player.directionFace = RIGHT;
    } else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.direction.x--;
        player.faceValue     = -1;
        player.directionFace = LEFT;
    }

    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.direction.y++;
        player.directionFace = DOWN;
    } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.direction.y--;
        player.directionFace = UP;
    }

    if(!Vector2Equals(player.direction, Vector2Zero()) && TimerDone(&playerStepTimer)) {
        PlaySound(soundFX[STEP_SFX]);
        StartTimer(&playerStepTimer, playerStepTimer.lifeTime);
    }

    MovePlayerToPos(player.direction);
}

static void PlayerAttack() {
    if(IsKeyPressed(KEY_E) && player.state != ATTACKING) {
        player.state = ATTACKING;
        StartTimer(&playerAnimArray[ATTACK_ANIMATION].timer, 0.5);
        PlaySound(soundFX[PLAYER_SLASH_SFX]);

        LoadStandardEntityAttackHitbox(&player);

        PlayerAttackHit();
    }

    if(player.state == ATTACKING && TimerDone(&playerAnimArray[ATTACK_ANIMATION].timer)) {
        player.state = IDLE;
    }
}

void PlayerEnemyCollision() {
    if(player.health > 0) {
        EnemyNode* currEnemy = enemies;

        while(currEnemy != NULL && player.health > 0) {
            Entity* enemy = &currEnemy->enemy;

            UpdateEntityHitbox(&player);
            UpdateEntityHitbox(enemy);
            bool attackHit = CheckCollisionRecs(enemy->hitbox, player.hitbox);
            if(attackHit) player.health--;

            currEnemy = currEnemy->next;
        }
    }
}

static void PlayerAttackHit() {
    EnemyNode* currEnemy = enemies;
    bool soundHit        = false;

    while(currEnemy != NULL) {
        Entity* enemy = &currEnemy->enemy;
        if(EntityAttack(&player, enemy, 1) && !soundHit) {
            // TODO: Test SLASH_HIT_SFX, slash + enemy dying and just enemy dying
            PlaySound(soundFX[ENEMY_DEAD_SFX]);
            soundHit = false;
        }
        currEnemy = currEnemy->next;
    }
}

static void RenderPlayerAttack() {
    // Rendering idle animation of player as the player should not move while attacking.
    EntityRender(
        &player, &playerAnimArray[IDLE_ANIMATION],
        ENTITY_TILE_WIDTH * player.faceValue, ENTITY_TILE_HEIGHT, 0, 0, 0.0f);

    switch(player.directionFace) {
        case RIGHT:
            EntityRender(
                &player, &playerAnimArray[ATTACK_ANIMATION],
                -PLAYER_ATTACK_WIDTH, PLAYER_ATTACK_HEIGHT,
                ENTITY_TILE_WIDTH / 4, PLAYER_ATTACK_HEIGHT / 2, 0.0f);
            break;
        case DOWN:
            EntityRender(
                &player, &playerAnimArray[ATTACK_ANIMATION],
                -PLAYER_ATTACK_WIDTH, PLAYER_ATTACK_HEIGHT * player.faceValue,
                ENTITY_TILE_WIDTH + ENTITY_TILE_WIDTH / 8, PLAYER_ATTACK_HEIGHT, 90.0f);
            break;
        case LEFT:
            EntityRender(
                &player, &playerAnimArray[ATTACK_ANIMATION], PLAYER_ATTACK_WIDTH,
                PLAYER_ATTACK_HEIGHT, -ENTITY_TILE_WIDTH - ENTITY_TILE_WIDTH / 4,
                PLAYER_ATTACK_HEIGHT / 2, 0.0f);
            break;
        case UP:
            EntityRender(
                &player, &playerAnimArray[ATTACK_ANIMATION], -PLAYER_ATTACK_WIDTH,
                -PLAYER_ATTACK_HEIGHT * player.faceValue, -ENTITY_TILE_WIDTH / 8,
                PLAYER_ATTACK_HEIGHT + ENTITY_TILE_HEIGHT / 8, -90.0f);
            break;
        default: break;
    }
}

static void MovePlayerToPos(Vector2 position) {
    MoveEntityTowardsPos(&player, position, NULL);
}