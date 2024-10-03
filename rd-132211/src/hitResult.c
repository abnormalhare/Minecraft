#include "hitResult.h"

HitResult* newHitResult(s32 x, s32 y, s32 z, s32 o, s32 f) {
    HitResult* hitResult = malloc(sizeof(HitResult));
    if (!hitResult) {
        fprintf(stderr, "Failed to allocate for hitResult");
        return NULL;
    }
    
    hitResult->x = x;
    hitResult->y = y;
    hitResult->z = z;
    hitResult->o = o;
    hitResult->f = f;

    return hitResult;
}