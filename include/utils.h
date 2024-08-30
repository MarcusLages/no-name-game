/***********************************************************************************************
 *
 **   Provides definitions for math utils and macros that are used often.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.3
 *
 *    @include raylib.h, raymath.h
 *    @cite raylib
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
 * @param componentWidth Width of the component
 * @return X position the component should have to be centered on screen
 *
 */
int CenterComponentOnScreenX(int componentWidth);

/**
 * Used to return the y position that a component should have to be
 * centered on the screen based on its height.
 *
 * @param componenteHeight
 * @return Centered y position the component should have
 *
 */
int CenterComponentOnsScreenY(int componenteHeight);

/**
 * Used to return the x position that a component should have to be
 * centered inside another outer component.
 *
 * @param innerComponentWidth   Width of the inner component
 * @param outerComponenX        X position of the outer component
 * @param outerComponentWidth   Width of the outer component
 * @return X position the component should have to be centered on screen
 *
 */
int CenterInnerComponentX(int innerComponentWidth, int outerComponentX, int outerComponentWidth);

/**
 * Determines if the given doubles are equal with a given precision.
 *
 * @param d1    First double to compare.
 * @param d2    Second double to compare.
 * @returns     True if the doubles are equal false otherwise.
 */
bool IsDoubleEqual(double d1, double d2, float precision);

/**
 * Converts the given seconds to a time format of HH:mm:ss:ns and returns it as a string.
 * 
 * ! @attention Assumes that the given string has been allocated a size of at least 11. 
 *
 * @param str       The string to format into.
 * @param size      The size of the str desired. (must match size of str given as well.)
 * @param s         Seconds to format.
 */
void ConvertToTimeFormat(char* str, int size, double s);

#endif // UTILS_H_