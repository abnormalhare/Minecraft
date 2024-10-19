#include "level/Tile.hpp"

Tile* Tile::rock = new Tile(0);
Tile* Tile::grass = new Tile(1);

Tile::Tile(std::int32_t tex) : tex(tex) {}

void Tile::render(Tesselator* t, Level* level, std::int32_t layer, std::int32_t x, std::int32_t y, std::int32_t z) {
    float u0 = this->tex / 16.0f;
    float u1 = u0 + 0.0624375f;
    float v0 = 0.0f;
    float v1 = v0 + 0.0624375f;

    float c1 = 1.0f;
    float c2 = 0.8f;
    float c3 = 0.6f;

    float x0 = x + 0.0f;
    float x1 = x + 1.0f;
    float y0 = y + 0.0f;
    float y1 = y + 1.0f;
    float z0 = z + 0.0f;
    float z1 = z + 1.0f;

    if (!level->isSolidTile(x, y - 1, z)) {
        float br = level->getBrightness(x, y - 1, z) * c1;
        if ((br == c1) ^ (layer == 1)) {
            t->color(br, br, br);
            t->tex(u0, v1);
            t->vertex(x0, y0, z1);
            t->tex(u0, v0);
            t->vertex(x0, y0, z0);
            t->tex(u1, v0);
            t->vertex(x1, y0, z0);
            t->tex(u1, v1);
            t->vertex(x1, y0, z1);
        }
    }
    if (!level->isSolidTile(x, y + 1, z)) {
        float br = level->getBrightness(x, y + 1, z) * c1;
        if ((br == c1) ^ (layer == 1)) {
            t->color(br, br, br);
            t->tex(u1, v1);
            t->vertex(x1, y1, z1);
            t->tex(u1, v0);
            t->vertex(x1, y1, z0);
            t->tex(u0, v0);
            t->vertex(x0, y1, z0);
            t->tex(u0, v1);
            t->vertex(x0, y1, z1);
        }
    }
    if (!level->isSolidTile(x, y, z - 1)) {
        float br = level->getBrightness(x, y, z - 1) * c2;
        if ((br == c2) ^ (layer == 1)) {
            t->color(br, br, br);
            t->tex(u1, v0);
            t->vertex(x0, y1, z0);
            t->tex(u0, v0);
            t->vertex(x1, y1, z0);
            t->tex(u0, v1);
            t->vertex(x1, y0, z0);
            t->tex(u1, v1);
            t->vertex(x0, y0, z0);
        }
    }
    if (!level->isSolidTile(x, y, z + 1)) {
        float br = level->getBrightness(x, y, z + 1) * c2;
        if ((br == c2) ^ (layer == 1)) {
            t->color(br, br, br);
            t->tex(u0, v0);
            t->vertex(x0, y1, z1);
            t->tex(u0, v1);
            t->vertex(x0, y0, z1);
            t->tex(u1, v1);
            t->vertex(x1, y0, z1);
            t->tex(u1, v0);
            t->vertex(x1, y1, z1);
        }
    }
    if (!level->isSolidTile(x - 1, y, z)) {
        float br = level->getBrightness(x - 1, y, z) * c3;
        if ((br == c3) ^ (layer == 1)) {
            t->color(br, br, br);
            t->tex(u1, v0);
            t->vertex(x0, y1, z1);
            t->tex(u0, v0);
            t->vertex(x0, y1, z0);
            t->tex(u0, v1);
            t->vertex(x0, y0, z0);
            t->tex(u1, v1);
            t->vertex(x0, y0, z1);
        }
    }
    if (!level->isSolidTile(x + 1, y, z)) {
        float br = level->getBrightness(x + 1, y, z) * c3;
        if ((br == c3) ^ (layer == 1)) {
            t->color(br, br, br);
            t->tex(u0, v1);
            t->vertex(x1, y0, z1);
            t->tex(u1, v1);
            t->vertex(x1, y0, z0);
            t->tex(u1, v0);
            t->vertex(x1, y1, z0);
            t->tex(u0, v0);
            t->vertex(x1, y1, z1);
        }
    }
}

void Tile::renderFace(Tesselator* t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face) {
    float x0 = x + 0.0f;
    float x1 = x + 1.0f;
    float y0 = y + 0.0f;
    float y1 = y + 1.0f;
    float z0 = z + 0.0f;
    float z1 = z + 1.0f;

    if (face == 0) {
        t->vertex(x0, y0, z1);
        t->vertex(x0, y0, z0);
        t->vertex(x1, y0, z0);
        t->vertex(x1, y0, z1);
    }
    if (face == 1) {
        t->vertex(x1, y1, z1);
        t->vertex(x1, y1, z0);
        t->vertex(x0, y1, z0);
        t->vertex(x0, y1, z1);
    }
    if (face == 2) {
        t->vertex(x0, y1, z0);
        t->vertex(x1, y1, z0);
        t->vertex(x1, y0, z0);
        t->vertex(x0, y0, z0);
    }
    if (face == 3) {
        t->vertex(x0, y1, z1);
        t->vertex(x0, y0, z1);
        t->vertex(x1, y0, z1);
        t->vertex(x1, y1, z1);
    }
    if (face == 4) {
        t->vertex(x0, y1, z1);
        t->vertex(x0, y1, z0);
        t->vertex(x0, y0, z0);
        t->vertex(x0, y0, z1);
    }
    if (face == 5) {
        t->vertex(x1, y0, z1);
        t->vertex(x1, y0, z0);
        t->vertex(x1, y1, z0);
        t->vertex(x1, y1, z1);
    }
}