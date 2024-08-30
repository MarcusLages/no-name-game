/**********************************************************************************************
 *
 **   trace-log.c is responsible for implementating a custom trace log function.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
 *
 *    @include trace-log.h, stdio.h, stdlib.h
 *
 **********************************************************************************************/

#include "../include/trace-log.h"
#include <stdio.h>
#include <stdlib.h>

void CustomLog(int msgType, const char* text, va_list args) {
    switch(msgType) {
        case LOG_ALL:
        case LOG_TRACE:
            break;
        case LOG_DEBUG:
            printf("[DEBUG] : ");
            break;
        case LOG_INFO:
            printf("[INFO] : ");
            break;
        case LOG_WARNING:
            printf("[WARNING] : ");
            break;
        case LOG_ERROR:
            printf("[ERROR] : ");
            break;
        case LOG_FATAL:
            printf("[FATAL ERROR] : ");
            exit(EXIT_FAILURE);
            break;
        case LOG_NONE:
            break;
    }

    vprintf(text, args);
    printf("\n");
}