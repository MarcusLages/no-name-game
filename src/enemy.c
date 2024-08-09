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

EnemyNode* enemies;

//* ------------------------------------------
//* MODULAR VARIABLES

/** The animation for an idle enemy. */
static Animation idleEnemyAnimation;

/** The animation for the enemy moving. */
static Animation movingEnemyAnimation;

/** The animation for a enemy attack. */
static Animation attackEnemyAnimation;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Unloads the list of enemies.
 */
static void UnloadEnemyList();

/**
 * Renders an enemy's attack animation based off of it's Direction.
 */
static void RenderEnemyAttack();

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

    /**
     * Prediction: entities are on the stack. enemies is on the heap trying to
     * access stack. we should only ever access from stack to heap not the other
     * way around.
     *
     * Prediction 2: en1, en2, en3 are cleared from stack therefore
     * any references are no longer pointing to them.
     */
    Entity* en1        = (Entity*) malloc(sizeof(Entity));
    en1->pos           = (Vector2){ 50.0f, 50.0f };
    en1->speed         = 100;
    en1->health        = 100;
    en1->direction     = Vector2Zero();
    en1->faceValue     = 1;
    en1->state         = IDLE;
    en1->directionFace = RIGHT;

    Entity* en2        = (Entity*) malloc(sizeof(Entity));
    en2->pos           = (Vector2){ 70.0f, 50.0f };
    en2->speed         = 100;
    en2->health        = 100;
    en2->direction     = Vector2Zero();
    en2->faceValue     = 1;
    en2->state         = IDLE;
    en2->directionFace = RIGHT;

    Entity* en3        = (Entity*) malloc(sizeof(Entity));
    en3->pos           = (Vector2){ 90.0f, 50.0f };
    en3->speed         = 100;
    en3->health        = 100;
    en3->direction     = Vector2Zero();
    en3->faceValue     = 1;
    en3->state         = IDLE;
    en3->directionFace = RIGHT;

    enemies         = (EnemyNode*) malloc(sizeof(EnemyNode));
    EnemyNode* nEn1 = (EnemyNode*) malloc(sizeof(EnemyNode));

    EnemyNode* nEn2 = (EnemyNode*) malloc(sizeof(EnemyNode));
    EnemyNode* nEn3 = (EnemyNode*) malloc(sizeof(EnemyNode));
    if(enemies == NULL || nEn1 == NULL || nEn2 == NULL || nEn3 == NULL)
        exit(EXIT_FAILURE);

    enemies = nEn1;

    nEn1->enemy = en1;
    nEn1->name  = 1;
    nEn1->next  = nEn2;

    nEn2->enemy = en2;
    nEn2->name  = 2;
    nEn2->next  = nEn3;

    nEn3->enemy = en3;
    nEn3->name  = 3;
    nEn3->next  = NULL;
}

void EnemyMovement() {
    EnemyNode* currEnemy = enemies;
    Entity* enemy        = NULL;
    while(currEnemy != NULL) {
        enemy = currEnemy->enemy;
        // Ensures the enemy cannot move while attacking
        if(enemy->state == ATTACKING) {
            currEnemy = currEnemy->next;
            continue;
        }

        // Sets the enemy to IDLE if not in agro range.
        if(Vector2Length(player.pos) - Vector2Length(enemy->pos) > AGRO_RANGE) {
            enemy->state = IDLE;
            currEnemy    = currEnemy->next;
            continue;
        }

        // Delta time helps not let player speed depend on framerate.
        // It helps to take account for time between frames too.
        float deltaTime = GetFrameTime();

        enemy->direction = (Vector2){ (int) player.pos.x - (int) enemy->pos.x,
                                      (int) player.pos.y - (int) enemy->pos.y };

        // Set the enemy to MOVING if not ATTACKING.
        enemy->state = enemy->state == ATTACKING ? ATTACKING : MOVING;

        // Set the enemy to IDLE if not ATTACKING or moving on any direction
        // ! N/A to enemies. Leaving it here still to confirm:
        // if(enemy->direction.x == 0 && enemy->direction.y == 0 &&
        //    enemy->state != ATTACKING) {
        //     enemy->state = IDLE;
        //     continue;
        // }

        enemy->faceValue = player.faceValue;

        enemy->direction = Vector2Normalize(enemy->direction);

        //! NOTE: Do not add deltaTime before checking collisions only after.
        // Velocity:
        enemy->direction = Vector2Scale(enemy->direction, enemy->speed);

        // TODO: enemy world collision

        enemy->pos = Vector2Add(enemy->pos, Vector2Scale(enemy->direction, deltaTime));
        currEnemy = currEnemy->next;
    }
}

void EnemyAttack() {}

void EnemyRender() {
    EnemyNode* currEnemy = enemies;

    while(currEnemy != NULL) {
        switch(currEnemy->enemy->state) {
            case IDLE:
                EntityRender(
                    currEnemy->enemy, &idleEnemyAnimation,
                    ENTITY_TILE_WIDTH * currEnemy->enemy->faceValue,
                    ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
                break;
            case MOVING:
                EntityRender(
                    currEnemy->enemy, &movingEnemyAnimation,
                    ENTITY_TILE_WIDTH * currEnemy->enemy->faceValue,
                    ENTITY_TILE_HEIGHT, 0, 0, 0.0f);
                break;
            case ATTACKING: break;
            default: break;
        }
        currEnemy = currEnemy->next;
    }
}

void RenderEnemyAttack() {}

void EnemyUnload() {
    UnloadEnemyList();
    AnimationUnload(&idleEnemyAnimation);
    AnimationUnload(&movingEnemyAnimation);
    AnimationUnload(&attackEnemyAnimation);
}

static void UnloadEnemyList() {
    while(enemies != NULL) {
        EnemyNode* temp = enemies;
        enemies         = enemies->next;
        temp->next      = NULL;
        free(temp->enemy);
        temp->enemy = NULL;
        free(temp);
        temp = NULL;
    }
}