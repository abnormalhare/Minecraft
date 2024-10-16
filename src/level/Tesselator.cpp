#include "level/Tesselator.hpp"

void Tesselator::flush(void) {
    glVertexPointer(3, GL_FLOAT, 0, this->vertexBuffer);
    if (this->hasTexture) {
        glTexCoordPointer(2, GL_FLOAT, 0, this->texCoordBuffer);
    }
    if (this->hasColor) {
        glColorPointer(3, GL_FLOAT, 0, this->colorBuffer);
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    if (this->hasTexture) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    if (this->hasColor) {
        glEnableClientState(GL_COLOR_ARRAY);
    }
    glDrawArrays(GL_QUADS, 0, this->vertices);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (this->hasTexture) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    if (this->hasColor) {
        glDisableClientState(GL_COLOR_ARRAY);
    }
    clear();
}

void Tesselator::clear(void) {
    this->vertices = 0;
    std::fill(this->vertexBuffer, this->vertexBuffer + MAX_VERTICES * 3, 0.0f);
    std::fill(this->texCoordBuffer, this->texCoordBuffer + MAX_VERTICES * 2, 0.0f);
    std::fill(this->colorBuffer, this->colorBuffer + MAX_VERTICES * 3, 0.0f);
}

void Tesselator::init(void) {
    clear();
    this->hasColor = false;
    this->hasTexture = false;
}

void Tesselator::tex(float u, float v) {
    this->hasTexture = true;
    this->u = u;
    this->v = v;
}

void Tesselator::color(float r, float g, float b) {
    this->hasColor = true;
    this->r = r;
    this->g = g;
    this->b = b;
}

void Tesselator::vertex(float x, float y, float z) {
    this->vertexBuffer[this->vertices * 3] = x;
    this->vertexBuffer[this->vertices * 3 + 1] = y;
    this->vertexBuffer[this->vertices * 3 + 2] = z;
    if (this->hasTexture) {
        this->texCoordBuffer[this->vertices * 2] = this->u;
        this->texCoordBuffer[this->vertices * 2 + 1] = this->v;
    }
    if (this->hasColor) {
        this->colorBuffer[this->vertices * 3] = this->r;
        this->colorBuffer[this->vertices * 3 + 1] = this->g;
        this->colorBuffer[this->vertices * 3 + 2] = this->b;
    }
    this->vertices++;
    if (this->vertices == MAX_VERTICES) {
        flush();
    }
}