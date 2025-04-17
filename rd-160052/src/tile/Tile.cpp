#include "tile/Tile.hpp"

Tile* empty = nullptr;
Tile* rock = new Tile(1, 1);
Tile* grass = new GrassTile(2);
Tile* dirt = new DirtTile(3, 2);
Tile* stoneBrick = new Tile(4, 16);
Tile* wood = new Tile(5, 4);

Tile::Tile(std::int32_t id) : id(id) {
    tiles[id] = this;
}

Tile::Tile(std::int32_t id, std::int32_t id) : id(id), tex(tex) {
    tiles[id] = this;
}

void Tile::render(std::shared_ptr<Tesselator>& t, std::shared_ptr<Level>& level, std::int32_t layer, std::int32_t x, std::int32_t y, std::int32_t z) {
    float c1 = 1.0f;
    float c2 = 0.8f;
    float c3 = 0.6f;

    if (this->shouldRenderFace(level, x, y - 1, z, layer)) {
        t->color(c1, c1, c1);
        this->renderFace(t, x, y, z, 0);
    }
}

bool Tile::shouldRenderFace(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t layer) {
    return !level->isSolidTile(x, y, z) && level->isLit(x, y, z) ^ layer == 1;
}

std::int32_t getTexture(std::int32_t face) {
    return this->tex;
}

void Tile::renderFace(std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face) {
    int tex = this->getTexture(face);
    float u0 = tex % 16 / 16.0f;
    float u1 = u0 + 0.0624375f;
    float v0 = tex / 16 / 16.0f;
    float v1 = v0 + 0.0624375f;

    float x0 = x + 0.0f;
    float x1 = x + 1.0f;
    float y0 = y + 0.0f;
    float y1 = y + 1.0f;
    float z0 = z + 0.0f;
    float z1 = z + 1.0f;

    if (face == 0) {
        t->vertexUV(x0, y0, z1, u0, v1);
        t->vertexUV(x0, y0, z0, u0, v0);
        t->vertexUV(x1, y0, z0, u1, v0);
        t->vertexUV(x1, y0, z1, u1, v1);
    }

    if (face == 1) {
        t->vertexUV(x1, y1, z1, u1, v1);
        t->vertexUV(x1, y1, z0, u1, v0);
        t->vertexUV(x0, y1, z0, u0, v0);
        t->vertexUV(x0, y1, z1, u0, v1);
    }

    if (face == 2) {
        t->vertexUV(x0, y1, z0, u1, v0);
        t->vertexUV(x1, y1, z0, u0, v0);
        t->vertexUV(x1, y0, z0, u0, v1);
        t->vertexUV(x0, y0, z0, u1, v1);
    }

    if (face == 3) {
        t->vertexUV(x0, y1, z1, u0, v0);
        t->vertexUV(x0, y0, z1, u0, v1);
        t->vertexUV(x1, y0, z1, u1, v1);
        t->vertexUV(x1, y1, z1, u1, v0);
    }

    if (face == 4) {
        t->vertexUV(x0, y1, z1, u1, v0);
        t->vertexUV(x0, y1, z0, u0, v0);
        t->vertexUV(x0, y0, z0, u0, v1);
        t->vertexUV(x0, y0, z1, u1, v1);
    }

    if (face == 5) {
        t->vertexUV(x1, y0, z1, u0, v1);
        t->vertexUV(x1, y0, z0, u1, v1);
        t->vertexUV(x1, y1, z0, u1, v0);
        t->vertexUV(x1, y1, z1, u0, v0);
    }
}

void Tile::renderFaceNoTexture(std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face) {
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

std::shared_ptr<AABB> getAABB(std::int32_t x, std::int32_t y, std::int32_t z) {
    return std::make_shared<AABB>(x, y, z, x + 1, y + 1, z + 1);
}

bool blocksLight() { return true; } // cant wait for this one

bool isSolid() { return true; } // ...whats the difference?

void tick(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::mt19937 random) {}

void destroy(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::shared_ptr<ParticleEngine>& particleEngine) {
    int SD = 4;

    for (int xx = 0; xx < SD; xx++) {
        for (int yy = 0; yy < SD; yy++) {
            for (int zz = 0; zz < SD; zz++) {
                float xp = x + (xx + 0.5f) / SD;
                float yp = y + (yy + 0.5f) / SD;
                float zp = z + (zz + 0.5f) / SD;
                particleEngine->add(Particle(level, xp, yp, zp, xp - x - 0.5f, yp - y - 0.5f, zp - z - 0.5f, this->tex));
            }
        }
    }
}