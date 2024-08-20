/**********************************************************************************************
 *
 **   trace-log.h is responsible for defining a custom trace log function.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.2
 *
 *    @include raylib.h
 *
 **********************************************************************************************/

#ifndef TRACE_LOG_H_
#define TRACE_LOG_H_

#include "raylib.h"

/**
 * Custom trace log function used as callbacks by TraceLog function from
 * raylib.
 * 
 * @param msgType   Log level/type according to raylib's TraceLogLevel
 * @param text      Message log
 * @param args      Number of var arguments
 */
void CustomLog(int msgType, const char *text, va_list args);

#endif // TRACE_LOG_H_