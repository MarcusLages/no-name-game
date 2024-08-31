/***********************************************************************************************
 *
 **   utisl.c provides the implementations for general math utils and macros that are used often.
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.3
 *
 *    @include stdio.h, stdlib.h, string.h, utils.h
 *
 ***********************************************************************************************/

#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CenterComponentOnScreenX(int componentWidth) {
    return (GetScreenWidth() - componentWidth) / 2;
}

int CenterComponentOnsScreenY(int componentHeight) {
    return (GetScreenHeight() - componentHeight) / 2;
}

int CenterInnerComponentX(int innerComponentWidth, int outerComponentX, int outerComponentWidth) {
    return outerComponentX + outerComponentWidth / 2 - innerComponentWidth / 2;
}

bool IsVectorEqual(Vector2 v1, Vector2 v2, float precision) {
    int result = ((fabsf(v1.x - v2.x)) <=
                  (precision * fmaxf(1.0f, fmaxf(fabsf(v1.x), fabsf(v2.x))))) &&
        ((fabsf(v1.y - v2.y)) <=
         (precision * fmaxf(1.0f, fmaxf(fabsf(v1.y), fabsf(v2.y)))));

    return result;
}

bool IsDoubleEqual(double d1, double d2, float precision) {
    return fabs(d1 - d2) < precision;
}

void ConvertToTimeFormat(char* str, int size, double s) {
    double totalSeconds = s;
    int hours           = totalSeconds / 3600;
    totalSeconds -= hours * 3600;
    int minutes = totalSeconds / 60;
    totalSeconds -= minutes * 60;
    int seconds      = totalSeconds;
    int floatSeconds = (totalSeconds - seconds) * 100;

    char hoursStr[3];
    char minutesStr[3];
    char secondsStr[3];
    char floatSecondsStr[3];

    int numbers[]      = { hours, minutes, seconds, floatSeconds };
    int numbersSize    = 4;
    char* numbersStr[] = { hoursStr, minutesStr, secondsStr, floatSecondsStr };

    for(int i = 0; i < numbersSize; i++) {
        sprintf(numbersStr[i], "%d", numbers[i]);
        if(numbers[i] < 10) {
            char zero[] = "0";
            strcat(zero, numbersStr[i]);
            strcpy(numbersStr[i], zero);
        }
    }

    int finalSize = size < 11 ? 11 : size;
    char time[finalSize];

    strcpy(time, hoursStr);
    strcat(time, ":");
    strcat(time, minutesStr);
    strcat(time, ":");
    strcat(time, secondsStr);
    strcat(time, ".");
    strcat(time, floatSecondsStr);
    strcpy(str, time);
}