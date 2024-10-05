#include "level/tile.h"

Tile* rock;
Tile* grass;

void initTiles(void) {
    rock = malloc(sizeof(Tile));
    if (!rock) {
        fprintf(stderr, "Failed to allocate rock");
    } else {
        rock->tex = 0;
    }
    
    grass = malloc(sizeof(Tile));
    if (!grass) {
        fprintf(stderr, "Failed to allocate grass");
    } else {
        grass->tex = 1;
    }
}

static inline bool check(float br, float c1, s32 layer) {
    return (br == c1) ^ (layer == 1);
}

void tileRender(Tile* tile, Tesselator* t, Level* level, s32 layer, s32 x, s32 y, s32 z) {
    float u0 = tile->tex / 16.0f;
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

    // bottom
    if (!isSolidTile(level, x, y - 1, z)) {
        float br = getBrightness(level, x, y - 1, z) * c1;

        if (check(br, c1, layer)) {
            color(t, br, br, br);
            tex(t, u0, v1);
            vertex(t, x0, y0, z1);
            tex(t, u0, v0);
            vertex(t, x0, y0, z0);
            tex(t, u1, v0);
            vertex(t, x1, y0, z0);
            tex(t, u1, v1);
            vertex(t, x1, y0, z1);
        }
    }

    // top
    if (!isSolidTile(level, x, y + 1, z)) {
        float br = getBrightness(level, x, y - 1, z) * c1;

        if (check(br, c1, layer)) {
            color(t, br, br, br);
            tex(t, u1, v1);
            vertex(t, x1, y1, z1);
            tex(t, u1, v0);
            vertex(t, x1, y1, z0);
            tex(t, u0, v0);
            vertex(t, x0, y1, z0);
            tex(t, u0, v1);
            vertex(t, x0, y1, z1);
        }
    }

    // back
    if (!isSolidTile(level, x, y, z - 1)) {
        float br = getBrightness(level, x, y, z - 1) * c2;

        if (check(br, c1, layer)) {
            color(t, br, br, br);
            tex(t, u1, v0);
            vertex(t, x0, y1, z0);
            tex(t, u0, v0);
            vertex(t, x1, y1, z0);
            tex(t, u0, v1);
            vertex(t, x1, y0, z0);
            tex(t, u1, v1);
            vertex(t, x0, y0, z0);
        }
    }

    // front
    if (!isSolidTile(level, x, y, z + 1)) {
        float br = getBrightness(level, x, y, z + 1) * c2;

        if (check(br, c1, layer)) {
            color(t, br, br, br);
            tex(t, u0, v0);
            vertex(t, x0, y1, z1);
            tex(t, u0, v1);
            vertex(t, x0, y0, z1);
            tex(t, u1, v1);
            vertex(t, x1, y0, z1);
            tex(t, u1, v0);
            vertex(t, x1, y1, z1);
        }
    }

    // left
    if (!isSolidTile(level, x - 1, y, z)) {
        float br = getBrightness(level, x - 1, y, z) * c3;

        if (check(br, c1, layer)) {
            color(t, br, br, br);
            tex(t, u1, v0);
            vertex(t, x0, y1, z1);
            tex(t, u0, v0);
            vertex(t, x0, y1, z0);
            tex(t, u0, v1);
            vertex(t, x0, y0, z0);
            tex(t, u1, v1);
            vertex(t, x0, y0, z1);
        }
    }

    // right
    if (!isSolidTile(level, x + 1, y, z)) {
        float br = getBrightness(level, x + 1, y, z) * c3;

        if (check(br, c1, layer)) {
            color(t, br, br, br);
            tex(t, u0, v1);
            vertex(t, x1, y0, z1);
            tex(t, u1, v1);
            vertex(t, x1, y0, z0);
            tex(t, u1, v0);
            vertex(t, x1, y1, z0);
            tex(t, u0, v0);
            vertex(t, x1, y1, z1);
        }
    }
}

void tileRenderFace(Tile* tile, Tesselator* t, s32 x, s32 y, s32 z, s32 face) {
    float x0 = x + 0.0f;
    float x1 = x + 1.0f;
    float y0 = y + 0.0f;
    float y1 = y + 1.0f;
    float z0 = z + 0.0f;
    float z1 = z + 1.0f;

    if (face == 0) {
        vertex(t, x0, y0, z1);
        vertex(t, x0, y0, z0);
        vertex(t, x1, y0, z0);
        vertex(t, x1, y0, z1);
    }
    if (face == 1) {
        vertex(t, x1, y1, z1);
        vertex(t, x1, y1, z0);
        vertex(t, x0, y1, z0);
        vertex(t, x0, y1, z1);
    }
    if (face == 2) {
        vertex(t, x0, y1, z0);
        vertex(t, x1, y1, z0);
        vertex(t, x1, y0, z0);
        vertex(t, x0, y0, z0);
    }
    if (face == 3) {
        vertex(t, x0, y1, z1);
        vertex(t, x0, y0, z1);
        vertex(t, x1, y0, z1);
        vertex(t, x1, y1, z1);
    }
    if (face == 4) {
        vertex(t, x0, y1, z1);
        vertex(t, x0, y1, z0);
        vertex(t, x0, y0, z0);
        vertex(t, x0, y0, z1);
    }
    if (face == 5) {
        vertex(t, x1, y0, z1);
        vertex(t, x1, y0, z0);
        vertex(t, x1, y1, z0);
        vertex(t, x1, y1, z1);
    }
}