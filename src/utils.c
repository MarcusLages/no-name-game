#include "../include/utils.h"

bool IsVectorEqual(Vector2 p, Vector2 q, float precision) {
    int result = ((fabsf(p.x - q.x)) <=
                  (precision * fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
        ((fabsf(p.y - q.y)) <= (precision * fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y)))));

    return result;
}