#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "types.h"
#include "java/FloatBuffer.h"
#include "phys/AABB.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

typedef struct _Frustum {
    float m_Frustum[6][4];
    FloatBuffer _proj;
    FloatBuffer _modl;
    FloatBuffer _clip;
    float* proj;
    float* modl;
    float* clip;
} Frustum;

Frustum* getFrustum(void);
void normalizePlane(float frustum[6][4], s32 side);
Frustum* newFrustum(void);
void calculateFrustum(Frustum* this);
bool cubeInFrustum(Frustum* this, float x0, float y0, float z0, float x1, float y1, float z1);
bool cubeInFrustumA(Frustum* this, AABB* aabb);

#endif