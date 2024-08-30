/***********************************************************************************************
 *
 **   Provide functionality for required utility functions specialized for enemy entities.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.3
 *
 *    @include enemy.h
 *
 ***********************************************************************************************/

#include "../include/enemy.h"

int GetWidth(EnemyType type) {
    int width = 0;

    switch(type) {
        case DEMON_PABLO: width = ENEMY_PABLO_WIDTH; break;
        case DEMON_DIEGO: width = ENEMY_DEIGO_WIDTH; break;
        case DEMON_WAFFLES: width = ENEMY_WAFFLES_WIDTH; break;

        default:
            TraceLog(LOG_ERROR, "ENEMY-UTILS.C (GetAttackWidth, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            width = ENEMY_PABLO_WIDTH;
            break;
    }
    return width;
}

int GetHeight(EnemyType type) {
    int height = 0;

    switch(type) {
        case DEMON_PABLO: height = ENEMY_PABLO_HEIGHT; break;
        case DEMON_DIEGO: height = ENEMY_DEIGO_HEIGHT; break;
        case DEMON_WAFFLES: height = ENEMY_WAFFLES_HEIGHT; break;

        default:
            TraceLog(LOG_WARNING, "ENEMY-UTILS.C (GetAttackHeight, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            height = ENEMY_PABLO_HEIGHT;
            break;
    }
    return height;
}

int GetAttackWidth(EnemyType type) {
    int width = 0;

    switch(type) {
        case DEMON_PABLO: width = ENEMY_PABLO_ATTACK_WIDTH; break;
        case DEMON_DIEGO: width = ENEMY_DEIGO_ATTACK_WIDTH; break;
        case DEMON_WAFFLES: width = ENEMY_WAFFLES_ATTACK_WIDTH; break;

        default:
            TraceLog(LOG_WARNING, "ENEMY-UTILS.C (GetAttackWidth, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            width = ENEMY_PABLO_ATTACK_WIDTH;
            break;
    }
    return width;
}

int GetAttackHeight(EnemyType type) {
    int height = 0;

    switch(type) {
        case DEMON_PABLO: height = ENEMY_PABLO_ATTACK_HEIGHT; break;
        case DEMON_DIEGO: height = ENEMY_DEIGO_ATTACK_HEIGHT; break;
        case DEMON_WAFFLES: height = ENEMY_WAFFLES_ATTACK_HEIGHT; break;

        default:
            TraceLog(LOG_WARNING, "ENEMY-UTILS.C (GetAttackHeight, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            height = ENEMY_PABLO_ATTACK_HEIGHT;
            break;
    }
    return height;
}

void GetTiles(int* tiles, int size, EnemyType type) {
    int tileNum = TILE_ENEMY_PABLO_IDLE;
    switch(type) {
        case DEMON_PABLO: tileNum = TILE_ENEMY_PABLO_IDLE; break;
        case DEMON_DIEGO: tileNum = TILE_ENEMY_DIEGO_IDLE; break;
        case DEMON_WAFFLES: tileNum = TILE_ENEMY_WAFFLES_IDLE; break;

        default:
            TraceLog(LOG_WARNING, "ENEMY-UTILS.C (GetTiles, line: %d): Invalid EnemyType given. Defaulting to PABLO.", __LINE__);
            break;
    }
    for(int i = 0; i < size; i++) {
        tiles[i] = tileNum++;
    };
}