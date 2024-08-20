/***********************************************************************************************
 *
 **   Provides definitions for forming rooms to handle enemy spawning.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include <stdlib.h>, raylib.h and raymath.h
 *
 ***********************************************************************************************/

#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

//* ------------------------------------------
//* DEFINITIONS

/** The number of possible positions for each room size. */
#define LG_ROOM_POS 30
#define MD_ROOM_POS 20
#define SM_ROOM_POS 10

/** The max number of enemies for each room size. */
#define LG_ROOM_MAX_ENEMIES 8
#define MD_ROOM_MAX_ENEMIES 4
#define SM_ROOM_MAX_ENEMIES 2

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum to define different room sizes.
 *
 * @param SMALL     0
 * @param MEDIUM    1
 * @param LARGE     2
 */
typedef enum RoomSize { SMALL = 0, MEDIUM, LARGE } RoomSize;

/**
 * Enum to define different room types.
 *
 * @param SAFE      0
 * @param HOSTILE   1
 */
typedef enum RoomType { SAFE = 0, HOSTILE } RoomType;

//* ------------------------------------------
//* STRUCTURES

/**
 * Represents information to describe an array of Vector2 positions.
 *
 * @param position Array of Vector2 positions.
 * @param currSize Current size of the array.
 * @param size The max possible size of the array.
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
 * Represents information needed to describe a room as a Node in a linked list.
 *
 * @param positionArray The array object for positions.
 * @param roomNumber The unique room number of this room.
 * @param roomSize The size of this room.
 * @param roomType The type of this room.
 * @param next Next RoomNode in the list.
 */
typedef struct RoomNode RoomNode;
struct RoomNode {
    /** Array of possible positions. */
    PositionArray positionArray;
    /**
     * The room number.
     *
     * ! @attention Room number must be unique.
     */
    int roomNumber;
    /** The size of the room. */
    RoomSize roomSize;
    /**
     * The type of room.
     *
     * ! @note This property may or may not be needed. Left here for now.
     */
    RoomType roomType;
    /** The next room. */
    RoomNode* next;
};

//* ------------------------------------------
//* GLOBAL VARIABLES

/** The list of all rooms. */
extern RoomNode* rooms;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Adds a specified position to given positionArray.
 */
void AddPosition(PositionArray* positionArray, Vector2 position);

/**
 * Adds a specified position to a positionArray that is in a given roomNumber.
 */
void AddPositionToRoom(int roomNumber, Vector2 position);

/**
 * Creates a RoomNode in memory and returns a reference.
 *
 * @param initPos The initial position to add in the room.
 * @param roomNumber The room number to set.
 * @param roomSize The room size to set.
 * @param roomType The room type to set.
 *
 * @returns A reference to the RoomNode created.
 *
 * ! @note Allocates memory for the RoomNode.
 */
RoomNode* CreateRoomList(Vector2 initPos, int roomNumber, RoomSize roomSize, RoomType roomType);

/**
 * Adds a RoomNode to the current list of rooms.
 *
 * @param initPos The initial position to add in the room.
 * @param roomNumber The room number to set.
 * @param roomSize The room size to set.
 * @param roomType The room type to set.
 *
 * ? @note Calls CreateRoomList to create the node.
 */
void AddRoomNode(Vector2 position, int roomNumber, RoomSize roomSize, RoomType roomType);

/**
 * Unallocates the entire list of rooms.
 *
 * ! @note Unallocates memory for the RoomNode.
 * ? @note Calls UnloadPositionArray to unallocate memory for each positionArray.
 */
void UnloadRooms();

/**
 * Checks if a room exists in memory with the given roomNumber.
 *
 * @returns true if it exists, false otherwise.
 */
bool CheckRoomExists(int roomNumber);

#endif // SPAWNER_H_