#include "level/tile/Tile.hpp"

#include "level/tile/DirtTile.hpp"
#include "level/tile/GrassTile.hpp"
#include "level/tile/Bush.hpp"
#include "level/tile/LiquidTile.hpp"
#include "level/tile/CalmLiquidTile.hpp"

bool Tile::shouldTick[256];
Tile* Tile::tiles[256];
Tile* Tile::empty      = nullptr;
Tile* Tile::rock       = new Tile           (Tile::ROCK, 1);
Tile* Tile::grass      = new GrassTile      (Tile::GRASS);
Tile* Tile::dirt       = new DirtTile       (Tile::DIRT, 2);
Tile* Tile::stoneBrick = new Tile           (Tile::STONE_BRICK, 16);
Tile* Tile::wood       = new Tile           (Tile::WOOD, 4);
Tile* Tile::bush       = new Bush           (Tile::BUSH);
Tile* Tile::bedrock    = new Tile           (Tile::BEDROCK, 17);
Tile* Tile::water      = new LiquidTile     (Tile::WATER, 1);
Tile* Tile::calmWater  = new CalmLiquidTile (Tile::CALM_WATER, 1);
Tile* Tile::lava       = new LiquidTile     (Tile::LAVA, 2);
Tile* Tile::calmLava   = new CalmLiquidTile (Tile::CALM_LAVA, 2);

Tile::Tile(std::int32_t id) : id(id) {
    tiles[id] = this;
    this->setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

void Tile::setTicking(bool isTick) {
    this->shouldTick[this->id] = isTick;
}

void Tile::setShape(float x0, float y0, float z0, float x1, float y1, float z1) {
    this->x0 = x0;
    this->y0 = y0;
    this->z0 = z0;
    this->x1 = x1;
    this->y1 = y1;
    this->z1 = z1;
}

Tile::Tile(std::int32_t id, std::int32_t tex) : id(id), tex(tex) {
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
    if (this->shouldRenderFace(level, x, y + 1, z, layer)) {
        t->color(c1, c1, c1);
        this->renderFace(t, x, y, z, 1);
    }
    if (this->shouldRenderFace(level, x, y, z - 1, layer)) {
        t->color(c2, c2, c2);
        this->renderFace(t, x, y, z, 2);
    }
    if (this->shouldRenderFace(level, x, y, z + 1, layer)) {
        t->color(c2, c2, c2);
        this->renderFace(t, x, y, z, 3);
    }
    if (this->shouldRenderFace(level, x - 1, y, z, layer)) {
        t->color(c3, c3, c3);
        this->renderFace(t, x, y, z, 4);
    }
    if (this->shouldRenderFace(level, x + 1, y, z, layer)) {
        t->color(c3, c3, c3);
        this->renderFace(t, x, y, z, 5);
    }
}

bool Tile::shouldRenderFace(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t layer) {
    if (x < 0 && y < 0 && z < 0 && x >= level->width && y >= level->depth && z >= level->height) {
        return false;
    }

    return !level->isSolidTile(x, y, z) && level->isLit(x, y, z) ^ (layer == 1);
}

std::int32_t Tile::getTexture(UNUSED std::int32_t face) {
    return this->tex;
}

void Tile::renderFace(std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face) {
    int _tex = this->getTexture(face);
    float u0 = _tex % 16 / 16.0f;
    float u1 = u0 + (0.999f / 16.0f);
    float v0 = _tex / 16 / 16.0f;
    float v1 = v0 + (0.999f / 16.0f);

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

void Tile::renderBackFace(std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face) {
    int _tex = this->getTexture(face);
    float u0 = _tex % 16 / 16.0f;
    float u1 = u0 + (0.999f / 16.0f);
    float v0 = _tex / 16 / 16.0f;
    float v1 = v0 + (0.999f / 16.0f);

    float x0 = x + 0.0f;
    float x1 = x + 1.0f;
    float y0 = y + 0.0f;
    float y1 = y + 1.0f;
    float z0 = z + 0.0f;
    float z1 = z + 1.0f;

    if (face == 0) {
        t->vertexUV(x1, y0, z1, u1, v1);
        t->vertexUV(x1, y0, z0, u1, v0);
        t->vertexUV(x0, y0, z0, u0, v0);
        t->vertexUV(x0, y0, z1, u0, v1);
    }

    if (face == 1) {
        t->vertexUV(x0, y1, z1, u0, v1);
        t->vertexUV(x0, y1, z0, u0, v0);
        t->vertexUV(x1, y1, z0, u1, v0);
        t->vertexUV(x1, y1, z1, u1, v1);
    }

    if (face == 2) {
        t->vertexUV(x0, y0, z0, u1, v1);
        t->vertexUV(x1, y0, z0, u0, v1);
        t->vertexUV(x1, y1, z0, u0, v0);
        t->vertexUV(x0, y1, z0, u1, v0);
    }

    if (face == 3) {
        t->vertexUV(x1, y1, z1, u1, v0);
        t->vertexUV(x1, y0, z1, u1, v1);
        t->vertexUV(x0, y0, z1, u0, v1);
        t->vertexUV(x0, y1, z1, u0, v0);
    }

    if (face == 4) {
        t->vertexUV(x0, y0, z1, u1, v1);
        t->vertexUV(x0, y0, z0, u0, v1);
        t->vertexUV(x0, y1, z0, u0, v0);
        t->vertexUV(x0, y1, z1, u1, v0);
    }

    if (face == 5) {
        t->vertexUV(x1, y1, z1, u0, v0);
        t->vertexUV(x1, y1, z0, u1, v0);
        t->vertexUV(x1, y0, z0, u1, v1);
        t->vertexUV(x1, y0, z1, u0, v1);
    }
}

void Tile::renderFaceNoTexture(UNUSED std::shared_ptr<Player>& p, std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face) {
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

const std::shared_ptr<AABB> Tile::getTileAABB(std::int32_t x, std::int32_t y, std::int32_t z) {
    return std::make_shared<AABB>(x, y, z, x + 1, y + 1, z + 1);
}

std::shared_ptr<AABB> Tile::getAABB(std::int32_t x, std::int32_t y, std::int32_t z) {
    return std::make_shared<AABB>(x, y, z, x + 1, y + 1, z + 1);
}

bool Tile::blocksLight() { return true; }

bool Tile::isSolid() { return true; }

bool Tile::mayPick() { return true; }

void Tile::tick(UNUSED std::shared_ptr<Level>& level, UNUSED std::int32_t x, UNUSED std::int32_t y, UNUSED std::int32_t z, UNUSED std::mt19937 random) {}

void Tile::destroy(std::shared_ptr<Level>& level, GLFWwindow* window, std::int32_t x, std::int32_t y, std::int32_t z, std::shared_ptr<ParticleEngine>& particleEngine) {
    int SD = 4;

    for (int xx = 0; xx < SD; xx++) {
        for (int yy = 0; yy < SD; yy++) {
            for (int zz = 0; zz < SD; zz++) {
                float xp = x + (xx + 0.5f) / SD;
                float yp = y + (yy + 0.5f) / SD;
                float zp = z + (zz + 0.5f) / SD;
                std::shared_ptr<Particle> p = std::make_shared<Particle>(level, window, xp, yp, zp, xp - x - 0.5f, yp - y - 0.5f, zp - z - 0.5f, this->tex);
                particleEngine->add(p);
            }
        }
    }
}

int Tile::getLiquidType() {
    return 0;
}

void Tile::neighborChanged(UNUSED std::shared_ptr<Level>& level, UNUSED std::int32_t x, UNUSED std::int32_t y, UNUSED std::int32_t z, UNUSED std::int32_t type) {}