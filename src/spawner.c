/***********************************************************************************************
 *
 **   Provides functionality for ...
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include spawner.h
 *
 ***********************************************************************************************/

#include "../include/spawner.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

RoomNode* rooms;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/** */
static PositionArray CreatePositionArray(int arraySize);

/** */
static void UnloadPositionArray(PositionArray* positionArray);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

static PositionArray CreatePositionArray(int arraySize) {
    PositionArray newPosArr;
    newPosArr.positions = (Vector2*) calloc(arraySize, sizeof(Vector2));

    if(newPosArr.positions == NULL) {
        TraceLog(LOG_FATAL, "spawner.c-CreatePositionArray: Memory allocation failure.");
        exit(EXIT_FAILURE);
    }

    newPosArr.currSize = 0;
    newPosArr.size     = arraySize;

    return newPosArr;
}

void AddPosition(PositionArray* positionArray, Vector2 position) {
    if(positionArray == NULL) {
        TraceLog(LOG_WARNING, "spawner.c-AddPosition: NULL positionArray was given.");
        return;
    }

    if(positionArray->currSize == positionArray->size) {
        TraceLog(LOG_WARNING, "spawner.c-AddPosition: Inserting into a full array.");
        return;
    }

    if(positionArray->positions == NULL) {
        TraceLog(LOG_WARNING, "spawner.c-AddPosition: NULL array of position was found.");
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
        TraceLog(LOG_WARNING, "spawner.c-UnloadPositionArray: NULL positionArray was given.");
        return;
    }

    if(positionArray->positions == NULL) {
        TraceLog(LOG_WARNING, "spawner.c-UnloadPositionArray: NULL array of position was found.");
        return;
    }

    free(positionArray->positions);
    positionArray->positions = NULL;
}

RoomNode* CreateRoomList(Vector2 initPos, int roomNumber, RoomSize roomSize, RoomType roomType) {
    RoomNode* room = (RoomNode*) malloc(sizeof(RoomNode));

    if(room == NULL) {
        TraceLog(LOG_FATAL, "spawner.c-CreateRoomList: Memory allocation failure.");
        exit(EXIT_FAILURE);
    }

    int arraySize = 0;

    switch(roomSize) {
        case SMALL: arraySize = SM_ROOM_POS; break;
        case MEDIUM: arraySize = MD_ROOM_POS; break;
        case LARGE: arraySize = LG_ROOM_POS; break;
        default:
            TraceLog(LOG_FATAL, "spawner.c-CreateRoomList: Invlaid roomSize given. Could not allocate memory in positionArray.");
            exit(EXIT_FAILURE);
            break;
    }

    room->positionArray = CreatePositionArray(arraySize);
    room->roomNumber    = roomNumber;
    room->roomSize      = roomSize;
    room->roomType      = roomType;
    room->next          = NULL;

    AddPosition(&room->positionArray, initPos);

    return room;
}

void AddRoomNode(Vector2 position, int roomNumber, RoomSize roomSize, RoomType roomType) {
    RoomNode* room = CreateRoomList(position, roomNumber, roomSize, roomType);

    RoomNode* cursor = rooms;
    while(cursor->next != NULL) {
        cursor = cursor->next;
    }
    cursor->next = room;
}

void UnloadRooms() {
    if(rooms == NULL) {
        TraceLog(LOG_WARNING, "spawner.c-UnloadRooms: Reference to rooms list is lost could not unload.");
        return;
    }

    while(rooms != NULL) {
        RoomNode* temp = rooms;
        rooms          = rooms->next;

        UnloadPositionArray(&temp->positionArray);

        free(temp);
        temp = NULL;
    }
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