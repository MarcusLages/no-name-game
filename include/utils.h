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
 * Used to return the x position that a component should have to be
 * centered on the screen based on its width.
 *
 * @param componentWidth
 * @return Centered x position the component should have
 *
 */
int CenterComponentX(int componentWidth);

/**
 * Used to return the y position that a component should have to be
 * centered on the screen based on its height.
 *
 * @param componenteHeight
 * @return Centered y position the component should have
 *
 */
int CenterComponentY(int componenteHeight);

#endif // UTILS_H_