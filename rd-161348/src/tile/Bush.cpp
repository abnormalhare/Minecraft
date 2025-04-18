#include "tile/Bush.hpp"

Bush::Bush(std::int32_t id) : Tile(id) {
    this->tex = 15;
}

void Bush::tick(std::shared_ptr<Level>& level, int x, int y, int z, UNUSED std::mt19937 random) {
    int below = level->getTile(x, y - 1, z);
    if (!level->isLit(x, y, z) || (below != Tile::dirt->id && below != Tile::grass->id)) {
        level->setTile(x, y, z, 0);
    }
}

void Bush::render(std::shared_ptr<Tesselator>& t, std::shared_ptr<Level>& level, int layer, int x, int y, int z) {
    if (!level->isLit(x, y, z) ^ (layer != 1)) {
        int tex = this->getTexture(15);

        float u0 = tex % 16 / 16.0f;
        float u1 = u0 + (1.0f / 64.0f);
        float v0 = tex / 16 / 16.0f;
        float v1 = v0 + (1.0f / 64.0f);

        int rots = 2;
        t->color(1.0f, 1.0f, 1.0f);
        for (int r = 0; r < rots; r++) {
            float xa = sin(r * PI / rots + (PI / 4)) * 0.5;
            float za = cos(r * PI / rots + (PI / 4)) * 0.5;

            float x0 = x + 0.5f - xa;
            float x1 = x + 0.5f + xa;
            float y0 = y + 0.0f;
            float y1 = y + 1.0f;
            float z0 = z + 0.5f - za;
            float z1 = z + 0.5f + za;

            t->vertexUV(x0, y1, z0, u1, v0);
            t->vertexUV(x1, y1, z1, u0, v0);
            t->vertexUV(x1, y0, z1, u0, v1);
            t->vertexUV(x0, y0, z0, u1, v1);
            t->vertexUV(x1, y1, z1, u0, v0);
            t->vertexUV(x0, y1, z0, u1, v0);
            t->vertexUV(x0, y0, z0, u1, v1);
            t->vertexUV(x1, y0, z1, u0, v1);
        }
    }
}

std::shared_ptr<AABB> Bush::getAABB(UNUSED int x, UNUSED int y, UNUSED int z) {
    return nullptr;
}

bool Bush::blocksLight() {
    return false;
}

bool Bush::isSolid() {
    return false;
}
