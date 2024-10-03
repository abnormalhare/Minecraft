#ifndef HIT_RESULT_H
#define HIT_RESULT_H

#include "types.h"

typedef struct _HitResult {
    s32 x;
    s32 y;
    s32 z;
    s32 o;
    s32 f;
} HitResult;

HitResult* newHitResult(s32 x, s32 y, s32 z, s32 o, s32 f);

#endif