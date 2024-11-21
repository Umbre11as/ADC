#pragma once

#include <stdarg.h>
#include <stdio.h>

#define ERR(x, ...) LogPrintError(x, __VA_ARGS__)
#define INFO(x, ...) LogPrintInfo(x, __VA_ARGS__)

inline void LogPrintError(const char* Format, ...) {
    va_list args;
    va_start(args, Format);

    char buffer[256];
    vsprintf_s(buffer, sizeof(buffer), Format, args);
    fprintf(stderr, "[error]: %s\n", buffer);

    va_end(args);
}

inline void LogPrintInfo(const char* Format, ...) {
    va_list args;
    va_start(args, Format);

    char buffer[256];
    vsprintf_s(buffer, sizeof(buffer), Format, args);
    printf("[info]: %s\n", buffer);

    va_end(args);
}
