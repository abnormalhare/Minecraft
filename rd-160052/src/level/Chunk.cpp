#include "level/Chunk.hpp"

std::shared_ptr<Tesselator> Chunk::t = Tesselator::instance;
std::int32_t Chunk::updates = 0;
std::int64_t Chunk::totalTime = 0;
std::int32_t Chunk::totalUpdates = 0;

Chunk::Chunk(std::shared_ptr<Level>& level, int x0, int y0, int z0, int x1, int y1, int z1)
    : aabb(x0, y0, z0, x1, y1, z1), level(level),
    x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1),
    x((x0+x1)/2.0f), y((y0+y1)/2.0f), z((z0+z1)/2.0f)
{
    dirty = true;
    dirtiedTime = 0;
    lists = glGenLists(2);
}

void Chunk::rebuild(std::int32_t layer) {
    this->dirty = false;
    this->updates++;
    std::int64_t before = Timer::getTimeInNanoSeconds();
    glNewList(this->lists + layer, GL_COMPILE);
    t->init();

    int tiles = 0;
    for (int x = this->x0; x < this->x1; x++) {
        for (int y = this->y0; y < this->y1; y++) {
            for (int z = this->z0; z < this->z1; z++) {
                int tileId = this->level->getTile(x, y, z);
                if (tileId > 0) {
                    Tile::tiles[tileId]->render(t, this->level, layer, x, y, z);
                    tiles++;
                }
            }
        }
    }

    t->flush();
    glEndList();

    std::int64_t after = Timer::getTimeInNanoSeconds();
    if (tiles > 0) {
        this->totalTime += after - before;
        this->totalUpdates++;
    }
}

void Chunk::rebuild() {
    this->rebuild(0);
    this->rebuild(1);
}

void Chunk::render(std::int32_t layer) {
    glCallList(this->lists + layer);
}

void Chunk::setDirty(void) {
    if (!this->dirty) {
        this->dirtiedTime = Timer::getTimeInMilliSeconds();
    }

    this->dirty = true;
}

bool Chunk::isDirty() {
    return this->dirty;
}

float Chunk::distanceToSqr(std::shared_ptr<Player>& player) {
    float xd = player->x - this->x;
    float yd = player->y - this->y;
    float zd = player->z - this->z;
    return xd * xd + yd * yd + zd * zd;
}