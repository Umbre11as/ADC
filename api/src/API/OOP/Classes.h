#pragma once

#include <stdlib.h>

#define SALLOC(baseName) malloc(sizeof(baseName))
#define NEW(className, ...) New##className(__VA_ARGS__)
