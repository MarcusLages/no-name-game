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

//* ------------------------------------------
//* DEFINITIONS
/**
 * Macro function to get the absolute/module value of a number.
 *
 * @param x Number
 */
#define ABS(x) (x > 0 ? x : x * (-1))


/**
 * Macro to swap two number variables.
 * 
 * @param a Number
 * @param b Number
 */
#define SWAP(a, b) ((a != b) ? (a += b, b = a - b, a -= b) : 0)

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Determines if the given vectors are equal with a given precision.
 *
 * @param v1 First vector to compare.
 * @param v2 Second vector to compare.
 *
 * @returns True if the vectors are equal false otherwise.
 */
bool IsVectorEqual(Vector2 v1, Vector2 v2, float precision);

/**
 * Determines if the given doubles are equal with a given precision.
 *
 * @param d1 First double to compare.
 * @param d2 Second double to compare.
 *
 * @returns True if the doubles are equal false otherwise.
 */
bool IsDoubleEqual(double d1, double d2, float precision);

#endif // UTILS_H_