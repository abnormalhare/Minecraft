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

void flush(Tesselator* this) {
    floatBufferFlip(&this->vertexBuffer);
    floatBufferFlip(&this->texCoordBuffer);
    floatBufferFlip(&this->colorBuffer);

    glVertexPointer(3, GL_FLOAT, 0, this->vertexBuffer.data);
    if (this->hasTexture)
        glTexCoordPointer(2, GL_FLOAT, 0, this->texCoordBuffer.data);
    if (this->hasColor)
        glColorPointer(3, GL_FLOAT, 0, this->colorBuffer.data);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    if (this->hasTexture)
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if (this->hasColor)
        glEnableClientState(GL_COLOR_ARRAY);
    
    glDrawArrays(7, 0, this->vertices);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (this->hasTexture)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (this->hasColor)
        glDisableClientState(GL_COLOR_ARRAY);

    clear(this);
}

void clear(Tesselator* this) {
    this->vertices = 0;
    floatBufferClear(&this->vertexBuffer);
    floatBufferClear(&this->texCoordBuffer);
    floatBufferClear(&this->colorBuffer);
}

void tesselatorInit(Tesselator* this) {
    clear(this);
    this->hasColor = FALSE;
    this->hasTexture = FALSE;
}

void tex(Tesselator* this, float u, float v) {
    this->hasTexture = TRUE;
    this->u = u;
    this->v = v;
}

void color(Tesselator* this, float r, float g, float b) {
    this->hasColor = TRUE;
    this->r = r;
    this->g = g;
    this->b = b;
}

void vertex(Tesselator* this, float x, float y, float z) {
    floatBufferPut(&this->vertexBuffer, this->vertices * 3 + 0, x);
    floatBufferPut(&this->vertexBuffer, this->vertices * 3 + 1, y);
    floatBufferPut(&this->vertexBuffer, this->vertices * 3 + 2, z);

    if (this->hasTexture) {
        floatBufferPut(&this->texCoordBuffer, this->vertices * 2 + 0, this->u);
        floatBufferPut(&this->texCoordBuffer, this->vertices * 2 + 1, this->v);
    }

    if (this->hasColor) {
        floatBufferPut(&this->colorBuffer, this->vertices * 3 + 0, this->r);
        floatBufferPut(&this->colorBuffer, this->vertices * 3 + 1, this->g);
        floatBufferPut(&this->colorBuffer, this->vertices * 3 + 2, this->b);
    }

    this->vertices++;
    if (this->vertices == 100000)
        flush(this);
}