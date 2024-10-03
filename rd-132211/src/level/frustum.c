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

void normalizePlane(float frustum[6][4], s32 side) {
    float magnitude = sqrtf(frustum[side][0] * frustum[side][0] + frustum[side][1] * frustum[side][1] + frustum[side][2] * frustum[side][2]);

    if (magnitude != 0) {
        frustum[side][0] /= magnitude;
        frustum[side][1] /= magnitude;
        frustum[side][2] /= magnitude;
        frustum[side][3] /= magnitude;
    } else {
        for (s32 i = 0; i < 4; i++) {
            if (frustum[side][i] >= 0.0f) {
                frustum[side][i] = INFINITE;
            } else {
                frustum[side][i] = -INFINITE;
            }
        }
    }
}

Frustum* newFrustum(void) {
    Frustum* frustum = malloc(sizeof(Frustum));

    floatBufferInit(&frustum->_proj, 16);
    floatBufferInit(&frustum->_modl, 16);
    floatBufferInit(&frustum->_clip, 16);

    frustum->proj = malloc(16 * sizeof(float));
    frustum->modl = malloc(16 * sizeof(float));
    frustum->clip = malloc(16 * sizeof(float));

    return frustum;
}

void calculateFrustum(Frustum* this) {
    floatBufferClear(&this->_proj);
    floatBufferClear(&this->_modl);
    floatBufferClear(&this->_clip);

    glGetFloatv(GL_PROJECTION_MATRIX, this->_proj.data);
    glGetFloatv(GL_MODELVIEW_MATRIX, this->_modl.data);
    
    floatBufferFlip(&this->_proj);
    floatBufferLimit(&this->_proj, 16);
    floatBufferGetv(&this->_proj, this->proj);

    floatBufferFlip(&this->_modl);
    floatBufferLimit(&this->_modl, 16);
    floatBufferGetv(&this->_modl, this->modl);

    this->clip[0]  = this->modl[0]  * this->proj[0] + this->modl[1]  * this->proj[4] + this->modl[2]  * this->proj[8]  + this->modl[3]  * this->proj[12];
    this->clip[1]  = this->modl[0]  * this->proj[1] + this->modl[1]  * this->proj[5] + this->modl[2]  * this->proj[9]  + this->modl[3]  * this->proj[13];
    this->clip[2]  = this->modl[0]  * this->proj[2] + this->modl[1]  * this->proj[6] + this->modl[2]  * this->proj[10] + this->modl[3]  * this->proj[14];
    this->clip[3]  = this->modl[0]  * this->proj[3] + this->modl[1]  * this->proj[7] + this->modl[2]  * this->proj[11] + this->modl[3]  * this->proj[15];
    this->clip[4]  = this->modl[4]  * this->proj[0] + this->modl[5]  * this->proj[4] + this->modl[6]  * this->proj[8]  + this->modl[7]  * this->proj[12];
    this->clip[5]  = this->modl[4]  * this->proj[1] + this->modl[5]  * this->proj[5] + this->modl[6]  * this->proj[9]  + this->modl[7]  * this->proj[13];
    this->clip[6]  = this->modl[4]  * this->proj[2] + this->modl[5]  * this->proj[6] + this->modl[6]  * this->proj[10] + this->modl[7]  * this->proj[14];
    this->clip[7]  = this->modl[4]  * this->proj[3] + this->modl[5]  * this->proj[7] + this->modl[6]  * this->proj[11] + this->modl[7]  * this->proj[15];
    this->clip[8]  = this->modl[8]  * this->proj[0] + this->modl[9]  * this->proj[4] + this->modl[10] * this->proj[8]  + this->modl[11] * this->proj[12];
    this->clip[9]  = this->modl[8]  * this->proj[1] + this->modl[9]  * this->proj[5] + this->modl[10] * this->proj[9]  + this->modl[11] * this->proj[13];
    this->clip[10] = this->modl[8]  * this->proj[2] + this->modl[9]  * this->proj[6] + this->modl[10] * this->proj[10] + this->modl[11] * this->proj[14];
    this->clip[11] = this->modl[8]  * this->proj[3] + this->modl[9]  * this->proj[7] + this->modl[10] * this->proj[11] + this->modl[11] * this->proj[15];
    this->clip[12] = this->modl[12] * this->proj[0] + this->modl[13] * this->proj[4] + this->modl[14] * this->proj[8]  + this->modl[15] * this->proj[12];
    this->clip[13] = this->modl[12] * this->proj[1] + this->modl[13] * this->proj[5] + this->modl[14] * this->proj[9]  + this->modl[15] * this->proj[13];
    this->clip[14] = this->modl[12] * this->proj[2] + this->modl[13] * this->proj[6] + this->modl[14] * this->proj[10] + this->modl[15] * this->proj[14];
    this->clip[15] = this->modl[12] * this->proj[3] + this->modl[13] * this->proj[7] + this->modl[14] * this->proj[11] + this->modl[15] * this->proj[15];

    this->m_Frustum[0][0] = this->clip[3] - this->clip[0];
    this->m_Frustum[0][1] = this->clip[7] - this->clip[4];
    this->m_Frustum[0][2] = this->clip[11] - this->clip[8];
    this->m_Frustum[0][3] = this->clip[15] - this->clip[12];
    normalizePlane(this->m_Frustum, 0);

    this->m_Frustum[1][0] = this->clip[3] + this->clip[0];
    this->m_Frustum[1][1] = this->clip[7] + this->clip[4];
    this->m_Frustum[1][2] = this->clip[11] + this->clip[8];
    this->m_Frustum[1][3] = this->clip[15] + this->clip[12];
    normalizePlane(this->m_Frustum, 1);
    
    this->m_Frustum[2][0] = this->clip[3] + this->clip[1];
    this->m_Frustum[2][1] = this->clip[7] + this->clip[5];
    this->m_Frustum[2][2] = this->clip[11] + this->clip[9];
    this->m_Frustum[2][3] = this->clip[15] + this->clip[13];
    normalizePlane(this->m_Frustum, 2);
    
    this->m_Frustum[3][0] = this->clip[3] - this->clip[1];
    this->m_Frustum[3][1] = this->clip[7] - this->clip[5];
    this->m_Frustum[3][2] = this->clip[11] - this->clip[9];
    this->m_Frustum[3][3] = this->clip[15] - this->clip[13];
    normalizePlane(this->m_Frustum, 3);
    
    this->m_Frustum[4][0] = this->clip[3] - this->clip[2];
    this->m_Frustum[4][1] = this->clip[7] - this->clip[6];
    this->m_Frustum[4][2] = this->clip[11] - this->clip[10];
    this->m_Frustum[4][3] = this->clip[15] - this->clip[14];
    normalizePlane(this->m_Frustum, 4);
    
    this->m_Frustum[5][0] = this->clip[3] + this->clip[2];
    this->m_Frustum[5][1] = this->clip[7] + this->clip[6];
    this->m_Frustum[5][2] = this->clip[11] + this->clip[10];
    this->m_Frustum[5][3] = this->clip[15] + this->clip[14];
    normalizePlane(this->m_Frustum, 5);
}

#define getOneF(f, x, y, z) f[0] * x + f[1] * y + f[2] * z + f[3]

bool pointInFrustum(Frustum* this, float x, float y, float z) {
    for (s32 i = 0; i < 6; i++) {
        if (getOneF(this->m_Frustum[i], x, y, z) <= 0.0f)
            return FALSE;
    }
    return TRUE;
}

bool sphereInFrustum(Frustum* this, float x, float y, float z, float radius) {
    for (s32 i = 0; i < 6; i++) {
        if (getOneF(this->m_Frustum[i], x, y, z) <= -radius)
            return FALSE;
    }
    return TRUE;
}

bool cubeFullyInFrustum(Frustum* this, float x0, float y0, float z0, float x1, float y1, float z1) {
    for (s32 i = 0; i < 6; i++) {
        if (getOneF(this->m_Frustum[i], x0, y0, z0) <= 0.0f)
            return FALSE;
        if (getOneF(this->m_Frustum[i], x1, y0, z0) <= 0.0f)
            return FALSE;
        if (getOneF(this->m_Frustum[i], x0, y1, z0) <= 0.0f)
            return FALSE;
        if (getOneF(this->m_Frustum[i], x1, y1, z0) <= 0.0f)
            return FALSE;
        if (getOneF(this->m_Frustum[i], x0, y0, z1) <= 0.0f)
            return FALSE;
        if (getOneF(this->m_Frustum[i], x1, y0, z1) <= 0.0f)
            return FALSE;
        if (getOneF(this->m_Frustum[i], x0, y1, z1) <= 0.0f)
            return FALSE;
        if (getOneF(this->m_Frustum[i], x1, y1, z1) <= 0.0f)
            return FALSE;
    }
    return TRUE;
}

bool cubeInFrustum(Frustum* this, float x1, float y1, float z1, float x2, float y2, float z2) {
    for (int i = 0; i < 6;) {
        if (getOneF(this->m_Frustum[i], x1, y1, z1) > 0.0f || 
            getOneF(this->m_Frustum[i], x2, y1, z1) > 0.0f || 
            getOneF(this->m_Frustum[i], x1, y2, z1) > 0.0f || 
            getOneF(this->m_Frustum[i], x2, y2, z1) > 0.0f || 
            getOneF(this->m_Frustum[i], x1, y1, z2) > 0.0f || 
            getOneF(this->m_Frustum[i], x2, y1, z2) > 0.0f || 
            getOneF(this->m_Frustum[i], x1, y2, z2) > 0.0f || 
            getOneF(this->m_Frustum[i], x2, y2, z2) > 0.0f) {
            i++;
            continue;
        } 
        return FALSE;
    }
    return TRUE;
}

bool cubeInFrustumA(Frustum* this, AABB* aabb) {
    return cubeInFrustum(this, aabb->x0, aabb->y0, aabb->z0, aabb->x1, aabb->y1, aabb->z1);
}