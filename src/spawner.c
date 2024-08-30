/***********************************************************************************************
 *
 **   Provides functionality for forming rooms to handle enemy spawning.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.3
 *
 *    @include <stdlib.h>, spawner.h
 *
 ***********************************************************************************************/

#include "../include/spawner.h"
#include <stdlib.h>

//* ------------------------------------------
//* GLOBAL VARIABLES

RoomNode* rooms;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Creates a PositionArray object with a positions array size specified.
 *
 * ! @note Allocates memory for the positions array inside the PositionArray structure.
 */
static PositionArray CreatePositionArray(int arraySize);

/**
 * Unallocates the memory used be the positions array in the given PositionArray structure.
 *
 * ! @note Unallocates memory for the positions array inside the PositionArray structure.
 */
static void UnloadPositionArray(PositionArray* positionArray);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

static PositionArray CreatePositionArray(int arraySize) {
    PositionArray newPosArr;
    newPosArr.positions = (Vector2*) calloc(arraySize, sizeof(Vector2));

    if(newPosArr.positions == NULL) {
        TraceLog(LOG_FATAL, "SPAWNER.C (CreatePositionArray, line: %d): Memory allocation failure.", __LINE__);
    }

    newPosArr.currSize = 0;
    newPosArr.size     = arraySize;
    return newPosArr;
}

void AddPosition(PositionArray* positionArray, Vector2 position) {
    if(positionArray == NULL) {
        TraceLog(LOG_WARNING, "SPAWNER.C (AddPosition, line: %d): NULL positionArray was given.", __LINE__);
        return;
    }

    if(positionArray->currSize == positionArray->size) {
        TraceLog(LOG_WARNING, "SPAWNER.C (AddPosition, line: %d): Inserting into a full array.", __LINE__);
        return;
    }

    if(positionArray->positions == NULL) {
        TraceLog(LOG_WARNING, "SPAWNER.C (AddPosition, line: %d): NULL array of positions was found.", __LINE__);
        return;
    }

    positionArray->currSize++;
    int idx                       = positionArray->currSize - 1;
    positionArray->positions[idx] = position;
}

void AddPositionToRoom(int roomNumber, Vector2 position) {
    RoomNode* cursor = rooms;
    while(cursor != NULL) {
        if(cursor->roomNumber == roomNumber) {
            AddPosition(&cursor->positionArray, position);
            break;
        }
        cursor = cursor->next;
    }
}

static void UnloadPositionArray(PositionArray* positionArray) {
    if(positionArray == NULL) {
        TraceLog(LOG_WARNING, "SPAWNER.C (UnloadPositionArray, line: %d): NULL positionArray was given.", __LINE__);
        return;
    }

    if(positionArray->positions == NULL) {
        TraceLog(LOG_WARNING, "SPAWNER.C (UnloadPositionArray, line: %d): NULL array of positions was found.", __LINE__);
        return;
    }

    free(positionArray->positions);
    positionArray->positions = NULL;
}

RoomNode* CreateRoomList(Vector2 initPos, int roomNumber, RoomSize roomSize) {
    RoomNode* room = (RoomNode*) malloc(sizeof(RoomNode));

    if(room == NULL) {
        TraceLog(LOG_FATAL, "SPAWNER.C (CreateRoomList, line: %d): Memory allocation failure.", __LINE__);
    }

    int arraySize = SM_ROOM_POS;

    switch(roomSize) {
        case SMALL: break;
        case MEDIUM: arraySize = MD_ROOM_POS; break;
        case LARGE: arraySize = LG_ROOM_POS; break;
        default:
            TraceLog(LOG_INFO, "SPAWNER.C (CreateRoomList, line: %d): Invalid RoomSize given. Defaulting to SMALL RoomSize.", __LINE__);
            break;
    }

    room->positionArray = CreatePositionArray(arraySize);
    room->roomNumber    = roomNumber;
    room->roomSize      = roomSize;
    room->next          = NULL;

    AddPosition(&room->positionArray, initPos);
    return room;
}

void AddRoomNode(Vector2 position, int roomNumber, RoomSize roomSize) {
    RoomNode* room = CreateRoomList(position, roomNumber, roomSize);

    RoomNode* cursor = rooms;
    while(cursor->next != NULL) {
        cursor = cursor->next;
    }
    cursor->next = room;
}

void UnloadRooms() {
    if(rooms == NULL) {
        TraceLog(LOG_WARNING, "SPAWNER.C (UnloadRooms, line: %d): Reference to rooms list is lost. Could not unload.", __LINE__);
        return;
    }

    while(rooms != NULL) {
        RoomNode* temp = rooms;
        rooms          = rooms->next;

        UnloadPositionArray(&temp->positionArray);
        free(temp);
        temp = NULL;
    }
    TraceLog(LOG_INFO, "SPAWNER.C (UnloadRooms): All rooms have been unloaded.");
}

bool CheckRoomExists(int roomNumber) {
    RoomNode* cursor = rooms;
    while(cursor != NULL) {
        if(cursor->roomNumber == roomNumber) {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}