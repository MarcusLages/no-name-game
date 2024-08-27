/**********************************************************************************************
*
**   collision.c is responsible to implementing functions for general collision detection and
**   information for 2D rays and rectangles.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.2
*
*    @include collision.h, utils.h
*
**********************************************************************************************/

#include "../include/collision.h"
#include "../include/utils.h"

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Swap the value of two nodes in the CollisionNode linked list.
 * 
 * @param nodeA CollisionNode*
 * @param nodeB CollisionNode*
 */
static void SwapCollisionNode(CollisionNode* nodeA, CollisionNode* nodeB);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

RayCollision2D RayRectCollision(Ray2D ray, Rectangle hitbox) {
    // Initializes the collision structure with no collision hit.
    RayCollision2D collision;
    collision.hit = false;

    // ! WARNING:
    // ! Before doing all calculations, if the direction vector is absolute,
    // ! it's important to make it relative to the origin vector.
    // ray.direction.x -= ray.origin.x;
    // ray.direction.y -= ray.origin.y;
    
    // Get the near collision point and far collision point in terms
    // of time of collision normalized from 0 to 1.
    // 0 represents the origin and 1 represents the destination

    // Near collision
    Vector2 nearColTime = (Vector2) {
        .x = (hitbox.x - ray.origin.x) / ray.direction.x,
        .y = (hitbox.y - ray.origin.y) / ray.direction.y
    };

    // Far collision
    Vector2 farColTime = (Vector2) {
        .x = (hitbox.x + hitbox.width - ray.origin.x) / ray.direction.x,
        .y = (hitbox.y + hitbox.height - ray.origin.y) / ray.direction.y
    };

    // Check for cases in which you divide by zero because it's too close to the target.
    if(nearColTime.x != nearColTime.x || nearColTime.y != nearColTime.y) return collision;
    if(farColTime.x != farColTime.x || farColTime.y != farColTime.y) return collision;
    
    // Sort the collision point in case they are in different quadrants
    // instead of the origin quadrant (0,0)
    if (nearColTime.x > farColTime.x) SWAP(nearColTime.x, farColTime.x);
    if (nearColTime.y > farColTime.y) SWAP(nearColTime.y, farColTime.y);

    // Checks for collision rules, if they are not met,
    // returns RayCollision2D structure with no hit
    if (nearColTime.x > farColTime.y || nearColTime.y > farColTime.x)
        return collision;

    // Calculate time of collision
    collision.timeHit = __max(nearColTime.x, nearColTime.y);
    float farTime = __min(farColTime.x, farColTime.y);

    // Rejects collisions that happen entirely before the origin point of the ray (at negative time).
    // ? NOTE: Depending on your use of ray collision, you might need to fine tune this.
    if (farTime < 0) return collision;

    // Used to reject collisions that happen after the destination point of the ray
    // which means the ray is on the direction of the rectangle, but not there yet.
    // Commented because it should be used outside when treating the collision.

    // if (collision.timeHit > 1) collision.hit = false;
    // else collision.hit = true;

    // Retrieving information on the contact point
    collision.contactPoint = Vector2Add(ray.origin, Vector2Scale(ray.direction, collision.timeHit));

    // Retrieving information on the normal vector of the contact point,
    // or in an easier way to understand,
    // vector with the opposite in which direction the ray touches the rectangle
    if (nearColTime.x > nearColTime.y) {
        if (ray.direction.x < 0)
            collision.normalVector = (Vector2) { 1, 0 };
        else
            collision.normalVector = (Vector2) { -1, 0 };
    } else {
        if (ray.direction.y < 0)
            collision.normalVector = (Vector2) { 0, 1 };
        else
            collision.normalVector = (Vector2) { 0, -1 };
    }

    collision.hit = true;
    
    return collision;
    
}

RayCollision2D HitboxCollision(Rectangle hitboxIn, Vector2 direction, Rectangle hitboxTarget) {
    // Initializes the collision structure with no collition
    RayCollision2D collision;
    // Gets the delta time during the collision
    float deltaTime = GetFrameTime();

    // Considers that initially the Rectangles are not steady and inside each other and
    // returns if the direction is zero
    if(direction.x == 0 && direction.y == 0) {
        collision.hit = false;
        return collision;
    }

    // Creates an expanded target rectangle based on the size of the hitboxIn rectangle which
    // is where the direction ray will collide.
    Rectangle expandedTarget = (Rectangle) {
        .x = hitboxTarget.x - (hitboxIn.width / 2),
        .y = hitboxTarget.y - (hitboxIn.height / 2),
        .width = hitboxTarget.width + hitboxIn.width,
        .height = hitboxTarget.height + hitboxIn.height
    };

    // Creates a Ray2D from the origin of the hitboxIn rectangle to the direction vector,
    // but modulated by deltaTime.
    Ray2D hitboxRay = (Ray2D) {
        .origin = { hitboxIn.x + hitboxIn.width / 2, hitboxIn.y + hitboxIn.height / 2 },
        .direction = { direction.x * deltaTime, direction.y * deltaTime }
    };

    // Gets the collision of the hitboxIn moving using direction with the
    // expandedTarget Rectangle
    collision = RayRectCollision(hitboxRay, expandedTarget);

    // After the ray collision is done, checks if the time of the collision is less than one.
    // Which means if the ray gets to the hitboxTarget on the actual direction Vector2 or if it's
    // just in the direction of the target and will collide later.
    if(collision.hit == false || collision.timeHit > 1) collision.hit = false;

    return collision;

}

CollisionNode* CreateCollisionList(int indexX, int indexY, float timeHit) {
    CollisionNode* newList = (CollisionNode*) malloc(sizeof(CollisionNode));
    if (newList == NULL) {
        TraceLog(LOG_FATAL, "COLLISION.C (CreateCollisionList, line: %d): Memory allocation failure.", __LINE__);
    }
    newList->collidedHitbox = (CollidedHitboxInfo) { (Vector2) {indexX, indexY}, timeHit };
    newList->next = NULL;

    return newList;
}

void AddCollisionNode(CollisionNode* head, int indexX, int indexY, float timeHit) {
    CollisionNode * currentNode;
    CollisionNode * newNode;

    currentNode = head;

    newNode = (CollisionNode*) malloc(sizeof(CollisionNode));
    if (newNode == NULL) {
        TraceLog(LOG_FATAL, "COLLISION.C (AddCollisionNode, line: %d): Memory allocation failure.", __LINE__);
    }

    newNode->collidedHitbox = (CollidedHitboxInfo) { (Vector2) {indexX, indexY}, timeHit };
    newNode->next = NULL;

    while(currentNode->next != NULL)
        currentNode = currentNode->next;

    currentNode->next = newNode;
}

void SortCollisionList(CollisionNode* head) {
    if(head == NULL) return;

    CollisionNode * currentNode;
    CollisionNode * indexNode;

    currentNode = head;
    indexNode = head->next;

    while(currentNode->next != NULL) {
        while(indexNode != NULL) {
            if(currentNode->collidedHitbox.timeHit > indexNode->collidedHitbox.timeHit) {
                SwapCollisionNode(currentNode, indexNode);
            }

            indexNode = indexNode->next;
        }

        currentNode = currentNode->next;
    }
}

void FreeCollisionList(CollisionNode* head) {
    if(head == NULL) return;
    
    CollisionNode* currentNode;
    while(head != NULL) {
        currentNode = head;
        head = head->next;
        free(currentNode);
        currentNode = NULL;
    }

}

static void SwapCollisionNode(CollisionNode* nodeA, CollisionNode* nodeB) {
    CollidedHitboxInfo temp;
    temp = nodeA->collidedHitbox;
    nodeA->collidedHitbox = nodeB->collidedHitbox;
    nodeB->collidedHitbox = temp;
}