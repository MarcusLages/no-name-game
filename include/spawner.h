/***********************************************************************************************
 *
 **   Provides definitions for ...
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include <stdlib.h>, raylib.h and raymath.h
 *
 ***********************************************************************************************/

#ifndef SPAWNER_H_
#define SPAWNER_H_

#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

//* ------------------------------------------
//* DEFINITIONS

#define LG_ROOM_POS 30
#define MD_ROOM_POS 20
#define SM_ROOM_POS 10

#define LG_ROOM_MAX_ENEMIES 10
#define MD_ROOM_MAX_ENEMIES 6
#define SM_ROOM_MAX_ENEMIES 2

//* ------------------------------------------
//* ENUMERATIONS

/**
 *
 */
typedef enum RoomSize { SMALL = 0, MEDIUM, LARGE } RoomSize;

/**
 *
 */
typedef enum RoomType { SAFE = 0, HOSTILE } RoomType;

//* ------------------------------------------
//* STRUCTURES

/**
 *
 */
typedef struct PositionArray {
    /** Array of possible positions. */
    Vector2* positions;
    /** The current size of the array. */
    int currSize;
    /** The max size of the array. */
    int size;
} PositionArray;

/**
 *
 */
typedef struct RoomNode RoomNode;
struct RoomNode {
    /** Array of possible positions. */
    PositionArray positionArray;
    /** The room number. */
    int roomNumber;
    /** The size of the room. */
    RoomSize roomSize;
    /** The type of room. */
    RoomType roomType;
    /** The next room. */
    RoomNode* next;
};

//* ------------------------------------------
//* GLOBAL VARIABLES

extern RoomNode* rooms;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/** */
PositionArray CreatePositionArray(int arraySize);

/** */
void AddPosition(PositionArray* positionArray, Vector2 position);

void UnloadPositionArray(PositionArray* positionArray);

/**
 *
 */
RoomNode* CreateRoomList(Vector2 initPos, int roomNumber, RoomSize roomSize, RoomType roomType);

/** */
void AddRoomNode(Vector2 position, int roomNumber, RoomSize roomSize, RoomType roomType);

/**
 * 
 */
void UnloadRooms();

#endif // SPAWNER_H_