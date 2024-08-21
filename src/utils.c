/***********************************************************************************************
 *
 **   Provides the implementations for general math utils and macros that are used often.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include utils.h
 *
 ***********************************************************************************************/

#include "../include/utils.h"

int CenterComponentX(int componentWidth) {
    return (GetScreenWidth() - componentWidth) / 2;
}

bool IsVectorEqual(Vector2 v1, Vector2 v2, float precision) {
    int result = ((fabsf(v1.x - v2.x)) <=
                  (precision * fmaxf(1.0f, fmaxf(fabsf(v1.x), fabsf(v2.x))))) &&
        ((fabsf(v1.y - v2.y)) <=
         (precision * fmaxf(1.0f, fmaxf(fabsf(v1.y), fabsf(v2.y)))));

    return result;
}