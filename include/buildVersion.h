#ifndef BUILD_VERSION_H
#define BUILD_VERSION_H

// Auto Generated from Infile. Do NOT Edit!!!
// Author Robert J. Gebis (oxoocoffee)

// the configured options and settings 
#define APP_BUILD_VERSION       "0.1.1.23d8ae5-dirty"
#define APP_BUILD_SYSTEM        "Linux-4.4.0-21-generic"

#define APP_VERSION_MAJOR       "0"
#define APP_VERSION_MINOR       "1"
#define APP_VERSION_PATCH       "1"
#define APP_VERSION_REVISION    "23d8ae5-dirty"

// Define Path seperator based on system build
#define PATH_SEPARATOR          '/'

#define DEBUG                   1
#define APP_TRACE               0

#define COMPILER_VERSION        __VERSION__

#define STACK_TRACING_ENABLED           0
#define PERFORMANCE_COUNTING_ENABLED    0

#if (STACK_TRACING_ENABLED or PERFORMANCE_COUNTING_ENABLED)
    #include "PoppyDebugTools.h"
#else
    #define STACK
    #define STACK_BLOCK(x)
    #define STACK_SECTION(x)
    #define STACK_VAL(var, value)
#endif

#endif // BUILD_VERSION_H
