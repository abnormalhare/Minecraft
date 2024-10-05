#ifndef AABB_H
#define AABB_H

#include "types.h"
#include <stdio.h>

typedef struct _AABB {
    float epsilon;
    float x0;
    float y0;
    float z0;
    float x1;
    float y1;
    float z1;
} AABB;

void addAABB(ArrayList* list, AABB* value);
void getAABB(ArrayList* list, size_t index, AABB* retVal);
void removeAABB(ArrayList* list, AABB* value);
AABB* newAABB(float x0, float y0, float z0, float x1, float y1, float z1);
AABB* expand(AABB* aabb, float xa, float ya, float za);
AABB* grow(AABB* aabb, float xa, float ya, float za);
float clipXCollide(AABB* aabb, AABB* c, float xa);
float clipYCollide(AABB* aabb, AABB* c, float ya);
float clipZCollide(AABB* aabb, AABB* c, float za);
bool intersects(AABB* aabb, AABB* c);
void AABBMove(AABB* aabb, float xa, float ya, float za);

#endif