#ifndef TESSELATOR_H
#define TESSELATOR_H

#include "types.h"
#include "java/FloatBuffer.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

typedef struct _Tesselator {
    FloatBuffer vertexBuffer;
    FloatBuffer texCoordBuffer;
    FloatBuffer colorBuffer;
    s32 vertices;
    float u;
    float v;
    float r;
    float g;
    float b;
    bool hasColor;
    bool hasTexture;
} Tesselator;

Tesselator* newTesselator(void);
void flush(Tesselator* t);
void clear(Tesselator* t);
void tesselatorInit(Tesselator* t);
void tex(Tesselator* t, float u, float v);
void color(Tesselator* t, float r, float g, float b);
void vertex(Tesselator* t, float x, float y, float z);

#endif