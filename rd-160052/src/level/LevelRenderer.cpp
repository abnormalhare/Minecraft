#include "level/LevelRenderer.hpp"

LevelRenderer::LevelRenderer(void) {}

LevelRenderer::LevelRenderer(std::shared_ptr<Level>& level) : level(level) {
    this->level->addListener(this);

    this->xChunks = level->width / CHUNK_SIZE;
    this->yChunks = level->depth / CHUNK_SIZE;
    this->zChunks = level->height / CHUNK_SIZE;

    this->chunks.resize(xChunks * yChunks * zChunks);
    for (int x = 0; x < xChunks; x++) {
        for (int y = 0; y < yChunks; y++) {
            for (int z = 0; z < zChunks; z++) {
                int x0 = x * CHUNK_SIZE;
                int y0 = y * CHUNK_SIZE;
                int z0 = z * CHUNK_SIZE;
                int x1 = (x + 1) * CHUNK_SIZE;
                int y1 = (y + 1) * CHUNK_SIZE;
                int z1 = (z + 1) * CHUNK_SIZE;
                
                if (x1 > level->width) x1 = level->width;
                if (y1 > level->depth) y1 = level->depth;
                if (z1 > level->height) z1 = level->height;

                this->chunks[(x + y * xChunks) * this->zChunks + z] = std::make_unique<Chunk>(this->level, x0, y0, z0, x1, y1, z1);
            }
        }
    }
}

std::vector<std::shared_ptr<Chunk>> LevelRenderer::getAllDirtyChunks() {
    std::vector<std::shared_ptr<Chunk>> dirty = std::vector<std::shared_ptr<Chunk>>();
    Frustum* frustum = Frustum::getFrustum();

    for (int i = 0; i < this->chunks.size(); i++) {
        std::shared_ptr<Chunk> chunk = this->chunks[i];
        if (chunk->isDirty()) {
            dirty.push_back(chunk);
        }
    }

    return dirty;
}

void LevelRenderer::render(std::shared_ptr<Player>& player, std::int32_t layer) {
    glEnable(GL_TEXTURE_2D);
    int id = Textures::loadTexture("terrain.png", GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, id);
    Frustum* frustum = Frustum::getFrustum();

    for (int i = 0; i < xChunks * yChunks * zChunks; i++) {
        if (frustum->isVisible(this->chunks[i]->aabb)) {
            this->chunks[i]->render(layer);
        }
    }

    glDisable(GL_TEXTURE_2D);
}

bool comp(int a, int b){return a > b;}

void LevelRenderer::updateDirtyChunks(std::unique_ptr<Player>& player) {
    std::vector<std::shared_ptr<Chunk>> dirty = this->getAllDirtyChunks();

    if (dirty.size() != 0) {
        std::sort(dirty.begin(), dirty.end(), DirtyChunkSorter::compare);

        for (int i = 0; i < 8 && i < dirty.size(); i++) {
            dirty.at(i)->rebuild();
        }
    }
}

void LevelRenderer::pick(std::shared_ptr<Player>& player) {
    std::shared_ptr<Tesselator> t = Tesselator::instance;
    float r = 3.0f;
    AABB box = player->bb.grow(r, r, r);

    int x0 = (int)(box.x0);
    int x1 = (int)(box.x1 + 1.0f);
    int y0 = (int)(box.y0);
    int y1 = (int)(box.y1 + 1.0f);
    int z0 = (int)(box.z0);
    int z1 = (int)(box.z1 + 1.0f);

    glInitNames();
    for (int x = x0; x < x1; x++) {
        glPushName(x);
        for (int y = y0; y < y1; y++) {
            glPushName(y);
            for (int z = z0; z < z1; z++) {
                glPushName(z);
                
                Tile* tile = Tile::tiles[this->level->getTile(x, y, z)];
                if (tile != nullptr) {
                    glPushName(0);
                    for (int i = 0; i < 6; i++) {
                        glPushName(i);
                        t->init();
                        Tile::rock->renderFace(t, x, y, z, i);
                        t->flush();
                        glPopName();
                    }
                    glPopName();
                }
                glPopName();
            }
            glPopName();
        }
        glPopName();
    }
}

void LevelRenderer::renderHit(std::unique_ptr<HitResult>& h) {
    std::shared_ptr<Tesselator> t = Tesselator::instance;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    int time = Timer::getTimeInMilliSeconds();      // for some reason this doesnt work
    float alpha = sin(time / 100.0) * 0.2f + 0.4f; // unless i split it up like this
    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    t->init();
    Tile::rock->renderFace(t, h->x, h->y, h->z, h->f);
    t->flush();
    glDisable(GL_BLEND);
}

void LevelRenderer::setDirty(std::int32_t x0, std::int32_t y0, std::int32_t z0, std::int32_t x1, std::int32_t y1, std::int32_t z1) {
    x0 /= CHUNK_SIZE;
    x1 /= CHUNK_SIZE;
    y0 /= CHUNK_SIZE;
    y1 /= CHUNK_SIZE;
    z0 /= CHUNK_SIZE;
    z1 /= CHUNK_SIZE;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 >= xChunks) x1 = xChunks - 1;
    if (y1 >= yChunks) y1 = yChunks - 1;
    if (z1 >= zChunks) z1 = zChunks - 1;

    for (int x = x0; x <= x1; x++) {
        for (int y = y0; y <= y1; y++) {
            for (int z = z0; z <= z1; z++) {
                this->chunks[(x + y * xChunks) * zChunks + z]->setDirty();
            }
        }
    }
}

void LevelRenderer::tileChanged(std::int32_t x, std::int32_t y, std::int32_t z) {
    setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void LevelRenderer::lightColumnChanged(std::int32_t x, std::int32_t z, std::int32_t y0, std::int32_t y1) {
    setDirty(x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
}

void LevelRenderer::allChanged(void) {
    setDirty(0, 0, 0, this->level->width, this->level->height, this->level->depth);
}