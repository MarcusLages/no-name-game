/***********************************************************************************************
 *
 **   Provides definitions for math utils and macros that are used often.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include raylib.h, raymath.h
 *
 ***********************************************************************************************/

#ifndef UTILS_H_
#define UTILS_H_

#include "raylib.h"
#include "raymath.h"

/**
 * Determines if the given vectors are equal with a given precision.
 *
 * @param v1 First vector to compare.
 * @param v2 Second vector to compare.
 *
 * @returns True if the vectors are equal false otherwise.
 */
bool IsVectorEqual(Vector2 v1, Vector2 v2, float precision);

#endif // UTILS_H_