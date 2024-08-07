/***********************************************************************************************
 *
 **   ...
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.1.0
 *
 *    @include enemy.h
 *
 ***********************************************************************************************/

#include "../include/enemy.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

Entity enemy;

//* ------------------------------------------
//* MODULAR VARIABLES

/** The animation for an idle enemy. */
static Animation idleEnemyAnimation;

/** The animation for the enemy moving. */
static Animation movingEnemyAnimation;

/** The animation for a enemy attack. */
static Animation attackEnemyAnimation;

static EnemyNode* enemies;

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void EnemyStartup() {
    // Initializing the idle animation
    idleEnemyAnimation =
        CreateAnimation(DEFAULT_IDLE_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_IDLE);

    // Initializing the moving animation
    movingEnemyAnimation =
        CreateAnimation(DEFAULT_MOVING_FPS, ENTITY_TILE_WIDTH, ENTITY_TILE_HEIGHT, TILE_ENEMY_MOVE);

    // Initializing the attacking animation
    attackEnemyAnimation =
        CreateAnimation(DEFAULT_ATTACK_FPS, TEMP_ATTACK_WIDTH, TEMP_ATTACK_HEIGHT, TILE_ENEMY_ATTACK);

    // Starting timers for both idle and moving animations
    StartTimer(&idleEnemyAnimation.timer, -1.0f);
    StartTimer(&movingEnemyAnimation.timer, -1.0f);

    // Start populating enemies list

    Entity en1 = (Entity){
        .pos           = (Vector2){ 50.0f, 50.0f },
        .speed         = 100,
        .health        = 100,
        .direction     = Vector2Zero(),
        .faceValue     = 1,
        .state         = IDLE,
        .directionFace = RIGHT,
    };

    Entity en2 = (Entity){
        .pos           = (Vector2){ 70.0f, 50.0f },
        .speed         = 100,
        .health        = 100,
        .direction     = Vector2Zero(),
        .faceValue     = 1,
        .state         = IDLE,
        .directionFace = RIGHT,
    };

    Entity en3 = (Entity){
        .pos           = (Vector2){ 90.0f, 50.0f },
        .speed         = 100,
        .health        = 100,
        .direction     = Vector2Zero(),
        .faceValue     = 1,
        .state         = IDLE,
        .directionFace = RIGHT,
    };

    EnemyNode* nEn1 = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(nEn1 == NULL) exit(EXIT_FAILURE);
    enemies = nEn1;

    EnemyNode* nEn2 = (EnemyNode*) malloc(sizeof(EnemyNode));
    EnemyNode* nEn3 = (EnemyNode*) malloc(sizeof(EnemyNode));

    nEn1->enemy = en1;
    nEn1->next  = nEn2;

    nEn2->enemy = en2;
    nEn2->next  = nEn3;

    nEn3->enemy = en3;
    nEn3->next  = NULL;
}

void EnemyMovement() {
    // For debugging:
    DrawText(TextFormat("Enemy x: %f", enemy.pos.x), 0, 40, 20, RED);
    DrawText(TextFormat("Enemy y: %f", enemy.pos.y), 0, 60, 20, RED);

    // Ensures the enemy cannot move while attacking
    if(enemy.state == ATTACKING) return;

    // Sets the enemy to IDLE if not in agro range.
    if(Vector2Length(player.pos) - Vector2Length(enemy.pos) > AGRO_RANGE) {
        enemy.state = IDLE;
        return;
    }

    // Delta time helps not let player speed depend on framerate.
    // It helps to take account for time between frames too.
    float deltaTime = GetFrameTime();

    enemy.direction = (Vector2){ (int) player.pos.x - (int) enemy.pos.x,
                                 (int) player.pos.y - (int) enemy.pos.y };

    // Set the enemy to MOVING if not ATTACKING.
    enemy.state = enemy.state == ATTACKING ? ATTACKING : MOVING;

    // Set the enemy to IDLE if not ATTACKING or moving on any direction
    if(enemy.direction.x == 0 && enemy.direction.y == 0 && enemy.state != ATTACKING) {
        enemy.state = IDLE;
        return;
    }

    enemy.faceValue = player.faceValue;

    enemy.direction = Vector2Normalize(enemy.direction);

    //! NOTE: Do not add deltaTime before checking collisions only after.
    // Velocity:
    enemy.direction = Vector2Scale(enemy.direction, enemy.speed);

    // TODO: enemy world collision

    enemy.pos = Vector2Add(enemy.pos, Vector2Scale(enemy.direction, deltaTime));
}

//TODO need a enemyupdate funtion to only render, update movement and attack to enemies in view

void EnemyAttack() {}

void EnemyRender() {
    switch(enemy.state) {
    case IDLE:
        EntityRender(
            &enemy, &idleEnemyAnimation, ENTITY_TILE_WIDTH * enemy.faceValue,
            ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
        break;
    case MOVING:
        EntityRender(
            &enemy, &movingEnemyAnimation, ENTITY_TILE_WIDTH * enemy.faceValue,
            ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
        break;
    case ATTACKING: break;
    default: break;
    }
}

void EnemyUnload() {
    UnloadEnemyList();
    AnimationUnload(&idleEnemyAnimation);
    AnimationUnload(&movingEnemyAnimation);
    AnimationUnload(&attackEnemyAnimation);
}

void UnloadEnemyList() {
    while(enemies != NULL) {
        EnemyNode* temp = enemies;
        enemies = enemies->next;
        temp->next = NULL;
        free(temp);
        temp = NULL;
    }
}