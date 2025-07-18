#include "render/Tesselator.hpp"

std::shared_ptr<Tesselator> Tesselator::instance = std::make_shared<Tesselator>();

Tesselator::Tesselator() {
    this->len = 3;
    this->p = 0;
}

void Tesselator::flush(void) {
    if (this->hasTexture && this->hasColor) {
        glInterleavedArrays(GL_T2F_C3F_V3F, 0, this->buffer);
    } else if (this->hasTexture) {
        glInterleavedArrays(GL_T2F_V3F, 0, this->buffer);
    } else if (this->hasColor) {
        glInterleavedArrays(GL_C3F_V3F, 0, this->buffer);
    } else {
        glInterleavedArrays(GL_V3F, 0, this->buffer);
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

    this->clear();
}

void Tesselator::clear(void) {
    this->vertices = 0;
    this->p = 0;
}

void Tesselator::init(void) {
    this->clear();
    this->hasColor = false;
    this->hasTexture = false;
}

void Tesselator::tex(float u, float v) {
    if (!this->hasTexture) {
        this->len += 2;
    }

    this->hasTexture = true;
    this->u = u;
    this->v = v;
}

void Tesselator::color(int r, int g, int b) {
    if (!this->hasColor) {
        this->len += 3;
    }

    this->hasColor = true;
    this->r = float(r) / 255.0;
    this->g = float(g) / 255.0;
    this->b = float(b) / 255.0;
}

void Tesselator::vertexUV(float x, float y, float z, float u, float v) {
    this->tex(u, v);
    this->vertex(x, y, z);
}

void Tesselator::vertex(float x, float y, float z) {
    if (this->hasTexture) {
        this->buffer[this->p++] = this->u;
        this->buffer[this->p++] = this->v;
    }
    if (this->hasColor) {
        this->buffer[this->p++] = this->r;
        this->buffer[this->p++] = this->g;
        this->buffer[this->p++] = this->b;
    }

    this->buffer[this->p++] = x;
    this->buffer[this->p++] = y;
    this->buffer[this->p++] = z;
    this->vertices++;
    if (this->vertices % 4 == 0 && this->p >= MAX_FLOATS - this->len * 4) {
        flush();
    }
}

void Tesselator::color(int rgb) {
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8)  & 0xFF;
    rgb &= 0xFF;
    this->color(r, g, b);
}

void Tesselator::noColor() {
    this->hasNoColor = true;
}