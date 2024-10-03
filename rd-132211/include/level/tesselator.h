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
void flush(Tesselator* this);
void clear(Tesselator* this);
void tesselatorInit(Tesselator* this);
void tex(Tesselator* this, float u, float v);
void color(Tesselator* this, float r, float g, float b);
void vertex(Tesselator* this, float x, float y, float z);

#endif