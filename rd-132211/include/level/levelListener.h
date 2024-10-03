#ifndef LEVEL_LISTENER_H
#define LEVEL_LISTENER_H

#include "types.h"

typedef struct _LevelListener {
    void* base;
    void (*tileChanged)();
    void (*lightColumnChanged)();
    void (*allChanged)();
} LevelListener;

#endif