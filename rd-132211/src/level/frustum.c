#include "level/frustum.h"

enum direction {
    RIGHT,
    LEFT,
    BOTTOM,
    TOP,
    BACK,
    FRONT
};

s32 A = 0;
s32 B = 1;
s32 C = 2;
s32 D = 3;

Frustum* frustum = NULL;

Frustum* getFrustum(void) {
    if (!frustum) {
        frustum = newFrustum();
    }
    calculateFrustum(frustum);
    return frustum;
}

void normalizePlane(float mFrustum[6][4], s32 side) {
    float magnitude = sqrtf(mFrustum[side][0] * mFrustum[side][0] + mFrustum[side][1] * mFrustum[side][1] + mFrustum[side][2] * mFrustum[side][2]);

    if (magnitude != 0) {
        mFrustum[side][0] /= magnitude;
        mFrustum[side][1] /= magnitude;
        mFrustum[side][2] /= magnitude;
        mFrustum[side][3] /= magnitude;
    } else {
        for (s32 i = 0; i < 4; i++) {
            if (mFrustum[side][i] >= 0.0f) {
                mFrustum[side][i] = INFINITE;
            } else {
                mFrustum[side][i] = -INFINITE;
            }
        }
    }
}

Frustum* newFrustum(void) {
    Frustum* frustum = malloc(sizeof(Frustum));
    if (!frustum) {
        fprintf(stderr, "Failed to allocate to frustum");
        return NULL;
    }

    floatBufferInit(&frustum->_proj, 16);
    floatBufferInit(&frustum->_modl, 16);
    floatBufferInit(&frustum->_clip, 16);

    frustum->proj = malloc(16 * sizeof(float));
    frustum->modl = malloc(16 * sizeof(float));
    frustum->clip = malloc(16 * sizeof(float));
    if (!frustum->proj || !frustum->modl || !frustum->clip) {
        fprintf(stderr, "Failed to allocate to frustum float list");
        return NULL;
    }


    return frustum;
}

void calculateFrustum(Frustum* frustum) {
    floatBufferClear(&frustum->_proj);
    floatBufferClear(&frustum->_modl);
    floatBufferClear(&frustum->_clip);

    glGetFloatv(GL_PROJECTION_MATRIX, frustum->_proj.data);
    glGetFloatv(GL_MODELVIEW_MATRIX, frustum->_modl.data);
    
    floatBufferFlip(&frustum->_proj);
    floatBufferLimit(&frustum->_proj, 16);
    floatBufferGetv(&frustum->_proj, frustum->proj);

    floatBufferFlip(&frustum->_modl);
    floatBufferLimit(&frustum->_modl, 16);
    floatBufferGetv(&frustum->_modl, frustum->modl);

    frustum->clip[0]  = frustum->modl[0]  * frustum->proj[0] + frustum->modl[1]  * frustum->proj[4] + frustum->modl[2]  * frustum->proj[8]  + frustum->modl[3]  * frustum->proj[12];
    frustum->clip[1]  = frustum->modl[0]  * frustum->proj[1] + frustum->modl[1]  * frustum->proj[5] + frustum->modl[2]  * frustum->proj[9]  + frustum->modl[3]  * frustum->proj[13];
    frustum->clip[2]  = frustum->modl[0]  * frustum->proj[2] + frustum->modl[1]  * frustum->proj[6] + frustum->modl[2]  * frustum->proj[10] + frustum->modl[3]  * frustum->proj[14];
    frustum->clip[3]  = frustum->modl[0]  * frustum->proj[3] + frustum->modl[1]  * frustum->proj[7] + frustum->modl[2]  * frustum->proj[11] + frustum->modl[3]  * frustum->proj[15];
    frustum->clip[4]  = frustum->modl[4]  * frustum->proj[0] + frustum->modl[5]  * frustum->proj[4] + frustum->modl[6]  * frustum->proj[8]  + frustum->modl[7]  * frustum->proj[12];
    frustum->clip[5]  = frustum->modl[4]  * frustum->proj[1] + frustum->modl[5]  * frustum->proj[5] + frustum->modl[6]  * frustum->proj[9]  + frustum->modl[7]  * frustum->proj[13];
    frustum->clip[6]  = frustum->modl[4]  * frustum->proj[2] + frustum->modl[5]  * frustum->proj[6] + frustum->modl[6]  * frustum->proj[10] + frustum->modl[7]  * frustum->proj[14];
    frustum->clip[7]  = frustum->modl[4]  * frustum->proj[3] + frustum->modl[5]  * frustum->proj[7] + frustum->modl[6]  * frustum->proj[11] + frustum->modl[7]  * frustum->proj[15];
    frustum->clip[8]  = frustum->modl[8]  * frustum->proj[0] + frustum->modl[9]  * frustum->proj[4] + frustum->modl[10] * frustum->proj[8]  + frustum->modl[11] * frustum->proj[12];
    frustum->clip[9]  = frustum->modl[8]  * frustum->proj[1] + frustum->modl[9]  * frustum->proj[5] + frustum->modl[10] * frustum->proj[9]  + frustum->modl[11] * frustum->proj[13];
    frustum->clip[10] = frustum->modl[8]  * frustum->proj[2] + frustum->modl[9]  * frustum->proj[6] + frustum->modl[10] * frustum->proj[10] + frustum->modl[11] * frustum->proj[14];
    frustum->clip[11] = frustum->modl[8]  * frustum->proj[3] + frustum->modl[9]  * frustum->proj[7] + frustum->modl[10] * frustum->proj[11] + frustum->modl[11] * frustum->proj[15];
    frustum->clip[12] = frustum->modl[12] * frustum->proj[0] + frustum->modl[13] * frustum->proj[4] + frustum->modl[14] * frustum->proj[8]  + frustum->modl[15] * frustum->proj[12];
    frustum->clip[13] = frustum->modl[12] * frustum->proj[1] + frustum->modl[13] * frustum->proj[5] + frustum->modl[14] * frustum->proj[9]  + frustum->modl[15] * frustum->proj[13];
    frustum->clip[14] = frustum->modl[12] * frustum->proj[2] + frustum->modl[13] * frustum->proj[6] + frustum->modl[14] * frustum->proj[10] + frustum->modl[15] * frustum->proj[14];
    frustum->clip[15] = frustum->modl[12] * frustum->proj[3] + frustum->modl[13] * frustum->proj[7] + frustum->modl[14] * frustum->proj[11] + frustum->modl[15] * frustum->proj[15];

    frustum->m_Frustum[0][0] = frustum->clip[3] - frustum->clip[0];
    frustum->m_Frustum[0][1] = frustum->clip[7] - frustum->clip[4];
    frustum->m_Frustum[0][2] = frustum->clip[11] - frustum->clip[8];
    frustum->m_Frustum[0][3] = frustum->clip[15] - frustum->clip[12];
    normalizePlane(frustum->m_Frustum, 0);

    frustum->m_Frustum[1][0] = frustum->clip[3] + frustum->clip[0];
    frustum->m_Frustum[1][1] = frustum->clip[7] + frustum->clip[4];
    frustum->m_Frustum[1][2] = frustum->clip[11] + frustum->clip[8];
    frustum->m_Frustum[1][3] = frustum->clip[15] + frustum->clip[12];
    normalizePlane(frustum->m_Frustum, 1);
    
    frustum->m_Frustum[2][0] = frustum->clip[3] + frustum->clip[1];
    frustum->m_Frustum[2][1] = frustum->clip[7] + frustum->clip[5];
    frustum->m_Frustum[2][2] = frustum->clip[11] + frustum->clip[9];
    frustum->m_Frustum[2][3] = frustum->clip[15] + frustum->clip[13];
    normalizePlane(frustum->m_Frustum, 2);
    
    frustum->m_Frustum[3][0] = frustum->clip[3] - frustum->clip[1];
    frustum->m_Frustum[3][1] = frustum->clip[7] - frustum->clip[5];
    frustum->m_Frustum[3][2] = frustum->clip[11] - frustum->clip[9];
    frustum->m_Frustum[3][3] = frustum->clip[15] - frustum->clip[13];
    normalizePlane(frustum->m_Frustum, 3);
    
    frustum->m_Frustum[4][0] = frustum->clip[3] - frustum->clip[2];
    frustum->m_Frustum[4][1] = frustum->clip[7] - frustum->clip[6];
    frustum->m_Frustum[4][2] = frustum->clip[11] - frustum->clip[10];
    frustum->m_Frustum[4][3] = frustum->clip[15] - frustum->clip[14];
    normalizePlane(frustum->m_Frustum, 4);
    
    frustum->m_Frustum[5][0] = frustum->clip[3] + frustum->clip[2];
    frustum->m_Frustum[5][1] = frustum->clip[7] + frustum->clip[6];
    frustum->m_Frustum[5][2] = frustum->clip[11] + frustum->clip[10];
    frustum->m_Frustum[5][3] = frustum->clip[15] + frustum->clip[14];
    normalizePlane(frustum->m_Frustum, 5);
}

#define getOneF(f, x, y, z) f[0] * x + f[1] * y + f[2] * z + f[3]

bool pointInFrustum(Frustum* frustum, float x, float y, float z) {
    for (s32 i = 0; i < 6; i++) {
        if (getOneF(frustum->m_Frustum[i], x, y, z) <= 0.0f)
            return FALSE;
    }
    return TRUE;
}

bool sphereInFrustum(Frustum* frustum, float x, float y, float z, float radius) {
    for (s32 i = 0; i < 6; i++) {
        if (getOneF(frustum->m_Frustum[i], x, y, z) <= -radius)
            return FALSE;
    }
    return TRUE;
}

bool cubeFullyInFrustum(Frustum* frustum, float x0, float y0, float z0, float x1, float y1, float z1) {
    for (s32 i = 0; i < 6; i++) {
        if (getOneF(frustum->m_Frustum[i], x0, y0, z0) <= 0.0f)
            return FALSE;
        if (getOneF(frustum->m_Frustum[i], x1, y0, z0) <= 0.0f)
            return FALSE;
        if (getOneF(frustum->m_Frustum[i], x0, y1, z0) <= 0.0f)
            return FALSE;
        if (getOneF(frustum->m_Frustum[i], x1, y1, z0) <= 0.0f)
            return FALSE;
        if (getOneF(frustum->m_Frustum[i], x0, y0, z1) <= 0.0f)
            return FALSE;
        if (getOneF(frustum->m_Frustum[i], x1, y0, z1) <= 0.0f)
            return FALSE;
        if (getOneF(frustum->m_Frustum[i], x0, y1, z1) <= 0.0f)
            return FALSE;
        if (getOneF(frustum->m_Frustum[i], x1, y1, z1) <= 0.0f)
            return FALSE;
    }
    return TRUE;
}

bool cubeInFrustum(Frustum* frustum, float x1, float y1, float z1, float x2, float y2, float z2) {
    for (int i = 0; i < 6;) {
        if (getOneF(frustum->m_Frustum[i], x1, y1, z1) > 0.0f || 
            getOneF(frustum->m_Frustum[i], x2, y1, z1) > 0.0f || 
            getOneF(frustum->m_Frustum[i], x1, y2, z1) > 0.0f || 
            getOneF(frustum->m_Frustum[i], x2, y2, z1) > 0.0f || 
            getOneF(frustum->m_Frustum[i], x1, y1, z2) > 0.0f || 
            getOneF(frustum->m_Frustum[i], x2, y1, z2) > 0.0f || 
            getOneF(frustum->m_Frustum[i], x1, y2, z2) > 0.0f || 
            getOneF(frustum->m_Frustum[i], x2, y2, z2) > 0.0f) {
            i++;
            continue;
        } 
        return FALSE;
    }
    return TRUE;
}

bool cubeInFrustumA(Frustum* frustum, AABB* aabb) {
    return cubeInFrustum(frustum, aabb->x0, aabb->y0, aabb->z0, aabb->x1, aabb->y1, aabb->z1);
}