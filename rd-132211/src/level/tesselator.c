#include "level/tesselator.h"

s32 MAX_VERTICES = 100000;

Tesselator* newTesselator(void) {
    Tesselator* t = malloc(sizeof(Tesselator));

    if (!t) {
        fprintf(stderr, "Failed to allocate to tesselator");
        return NULL;
    }

    t->vertices = 0;
    t->hasColor = FALSE;
    t->hasTexture = FALSE;

    floatBufferInit(&t->vertexBuffer, 300000);
    floatBufferInit(&t->texCoordBuffer, 200000);
    floatBufferInit(&t->colorBuffer, 300000);

    return t;
}

void flush(Tesselator* t) {
    floatBufferFlip(&t->vertexBuffer);
    floatBufferFlip(&t->texCoordBuffer);
    floatBufferFlip(&t->colorBuffer);

    glVertexPointer(3, GL_FLOAT, 0, t->vertexBuffer.data);
    if (t->hasTexture)
        glTexCoordPointer(2, GL_FLOAT, 0, t->texCoordBuffer.data);
    if (t->hasColor)
        glColorPointer(3, GL_FLOAT, 0, t->colorBuffer.data);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    if (t->hasTexture)
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if (t->hasColor)
        glEnableClientState(GL_COLOR_ARRAY);
    
    glDrawArrays(7, 0, t->vertices);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (t->hasTexture)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (t->hasColor)
        glDisableClientState(GL_COLOR_ARRAY);

    clear(t);
}

void clear(Tesselator* t) {
    t->vertices = 0;
    floatBufferClear(&t->vertexBuffer);
    floatBufferClear(&t->texCoordBuffer);
    floatBufferClear(&t->colorBuffer);
}

void tesselatorInit(Tesselator* t) {
    clear(t);
    t->hasColor = FALSE;
    t->hasTexture = FALSE;
}

void tex(Tesselator* t, float u, float v) {
    t->hasTexture = TRUE;
    t->u = u;
    t->v = v;
}

void color(Tesselator* t, float r, float g, float b) {
    t->hasColor = TRUE;
    t->r = r;
    t->g = g;
    t->b = b;
}

void vertex(Tesselator* t, float x, float y, float z) {
    floatBufferPut(&t->vertexBuffer, t->vertices * 3 + 0, x);
    floatBufferPut(&t->vertexBuffer, t->vertices * 3 + 1, y);
    floatBufferPut(&t->vertexBuffer, t->vertices * 3 + 2, z);

    if (t->hasTexture) {
        floatBufferPut(&t->texCoordBuffer, t->vertices * 2 + 0, t->u);
        floatBufferPut(&t->texCoordBuffer, t->vertices * 2 + 1, t->v);
    }

    if (t->hasColor) {
        floatBufferPut(&t->colorBuffer, t->vertices * 3 + 0, t->r);
        floatBufferPut(&t->colorBuffer, t->vertices * 3 + 1, t->g);
        floatBufferPut(&t->colorBuffer, t->vertices * 3 + 2, t->b);
    }

    t->vertices++;
    if (t->vertices == 100000)
        flush(t);
}