#include "level/LevelRenderer.hpp"

LevelRenderer::LevelRenderer(void) {}

LevelRenderer::LevelRenderer(std::shared_ptr<Level>& level, std::shared_ptr<Textures>& textureManager)
    : level(level), textureManager(textureManager), X(0.0f), Y(0.0f), Z(0.0f), drawDistance(0)
{
    this->level->addListener(this);

    this->xChunks = (level->width + 16 - 1) / CHUNK_SIZE;
    this->yChunks = (level->depth + 16 - 1) / CHUNK_SIZE;
    this->zChunks = (level->height + 16 - 1) / CHUNK_SIZE;

    this->chunks.resize(xChunks * yChunks * zChunks);
    this->sortedChunks.resize(xChunks * yChunks * zChunks);
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

                this->chunks      [(x + y * xChunks) * this->zChunks + z] = std::make_unique<Chunk>(this->level, x0, y0, z0, x1, y1, z1);
                this->sortedChunks[(x + y * xChunks) * this->zChunks + z] = this->chunks[(x + y * this->xChunks) * this->zChunks + z];
            }
        }
    }

    this->surroundLists = glGenLists(2);
    glNewList(this->surroundLists, GL_COMPILE);
    this->compileSurroundingGround();
    glEndList();
    glNewList(this->surroundLists + 1, GL_COMPILE);
    this->compileSurroundingGround();
    glEndList();
}

std::vector<std::shared_ptr<Chunk>> LevelRenderer::getAllDirtyChunks() {
    std::vector<std::shared_ptr<Chunk>> dirty = std::vector<std::shared_ptr<Chunk>>();

    for (size_t i = 0; i < this->chunks.size(); i++) {
        std::shared_ptr<Chunk> chunk = this->chunks[i];
        if (chunk->isDirty()) {
            dirty.push_back(chunk);
        }
    }

    return dirty;
}

void LevelRenderer::render(UNUSED std::shared_ptr<Player>& player, std::int32_t layer) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->textureManager->loadTexture("texture.png", GL_NEAREST));
    
    float x = player->x - this->X;
    float y = player->y - this->Y;
    float z = player->z - this->Z;
    if (x * x + y * y + z * z > 64.0f) {
        distSorterInit(player);
        this->X = player->x;
        this->Y = player->y;
        this->Z = player->z;
        std::sort(this->sortedChunks.begin(), this->sortedChunks.end(), distCompare);
    }

    for (int i = 0; i < this->sortedChunks.size(); i++) {
        if (this->sortedChunks[i]->visible) {
            y = 256 / (1 << this->drawDistance);
            if (this->drawDistance == 0 || this->sortedChunks[i]->distanceToSqr(player)) {
                this->sortedChunks[i]->render(layer);
            }
        }
    }

    glDisable(GL_TEXTURE_2D);
}



void LevelRenderer::updateDirtyChunks(std::shared_ptr<Player>& player) {
    std::vector<std::shared_ptr<Chunk>> dirty = this->getAllDirtyChunks();

    if (dirty.size() != 0) {
        chunkSorterInit(player, Frustum::getFrustum());
        std::sort(dirty.begin(), dirty.end(), chunkCompare);

        for (size_t i = 0; i < 8 && i < dirty.size(); i++) {
            dirty.at(i)->rebuild();
        }
    }
}

void LevelRenderer::pick(std::shared_ptr<Player>& player, Frustum* frustum) {
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
    glPushName(0);
    glPushName(0);
    for (int x = x0; x < x1; x++) {
        glLoadName(x);
        glPushName(0);
        for (int y = y0; y < y1; y++) {
            glLoadName(y);
            glPushName(0);
            for (int z = z0; z < z1; z++) {
                Tile* tile = Tile::tiles[this->level->getTile(x, y, z)];
                const std::shared_ptr<AABB> aabb = tile->getTileAABB(x, y, z);
                if (frustum->isVisible(*aabb)) {
                    if (tile == nullptr) continue;
                    glLoadName(z);
                    glPushName(0);

                    for (int i = 0; i < 6; i++) {
                        glLoadName(i);
                        t->init();
                        tile->renderFaceNoTexture(player, t, x, y, z, i);
                        t->flush();
                    }
                    glPopName();
                }
            }
            glPopName();
        }
        glPopName();
    }
    glPopName();
    glPopName();
}

void LevelRenderer::compileSurroundingGround() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->textureManager->loadTexture("rock.png", GL_NEAREST));
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    std::shared_ptr<Tesselator> t = Tesselator::instance;
    float std_y = 30.0f;
    t->init();
    
    for (int x = -640; x < this->level->width + 640; x += 128) {
        for (int z = -640; z < this->level->height + 640; z += 128) {
            float y = std_y;
            if (x >= 0 && z >= 0 && x < this->level->width && z < this->level->height) {
                y = 0.0f;
            }

            t->vertexUV(x,       y, z + 128, 0.0f,   128.0f);
            t->vertexUV(x + 128, y, z + 128, 128.0f, 128.0f);
            t->vertexUV(x + 128, y, z,       128.0f, 0.0f  );
            t->vertexUV(x,       y, z,       0.0f,   0.0f  );
        }
    }

    t->flush();

    glBindTexture(GL_TEXTURE_2D, this->textureManager->loadTexture("rock.png", GL_NEAREST));
    glColor3f(0.8f, 0.8f, 0.8f);
    t->init();

    for (int x = 0; x < this->level->width; x += 128) {
        t->vertexUV(x,          0.0f,  0.0f, 0.0f,   0.0f );
        t->vertexUV(x + 128.0f, 0.0f,  0.0f, 128.0f, 0.0f );
        t->vertexUV(x + 128.0f, std_y, 0.0f, 128.0f, std_y);
        t->vertexUV(x,          std_y, 0.0f, 0.0f,   std_y);

        t->vertexUV(x,          std_y, this->level->height, 0.0f,   std_y);
        t->vertexUV(x + 128.0f, std_y, this->level->height, 128.0f, std_y);
        t->vertexUV(x + 128.0f, 0.0f , this->level->height, 128.0f, 0.0f );
        t->vertexUV(x,          0.0f , this->level->height, 0.0f,   0.0f );
    }

    glColor3f(0.6f, 0.6f, 0.6f);

    for (int z = 0; z < this->level->height; z += 128) {
        t->vertexUV(0.0f, std_y, z,          0.0f,   0.0f );
        t->vertexUV(0.0f, std_y, z + 128.0f, 128.0f, 0.0f );
        t->vertexUV(0.0f, 0.0f,  z + 128.0f, 128.0f, std_y);
        t->vertexUV(0.0f, 0.0f,  z,          0.0f,   std_y);

        t->vertexUV(this->level->width, 0.0f , z,          0.0f,   std_y);
        t->vertexUV(this->level->width, 0.0f , z + 128.0f, 128.0f, std_y);
        t->vertexUV(this->level->width, std_y, z + 128.0f, 128.0f, 0.0f );
        t->vertexUV(this->level->width, std_y, z,          0.0f,   0.0f );
    }

    t->flush();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void LevelRenderer::compileSurroundingWater() {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, this->textureManager->loadTexture("water.png", GL_NEAREST));

    float base_y = 32.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::shared_ptr<Tesselator> t = Tesselator::instance;
    t->init();
    
    for (int x = -640; x < this->level->width + 640; x += 128) {
        for (int z = -640; z < this->level->height + 640; z += 128) {
            float y = base_y - 0.1f;

            if (x < 0 || z < 0 || x >= this->level->width || z >= this->level->height) {
                t->vertexUV(x,       y, z + 128, 0.0f,   128.0f);
                t->vertexUV(x + 128, y, z + 128, 128.0f, 128.0f);
                t->vertexUV(x + 128, y, z,       128.0f, 0.0f  );
                t->vertexUV(x      , y, z,       0.0f  , 0.0f  );

                t->vertexUV(x      , y, z,       0.0f  , 0.0f  );
                t->vertexUV(x + 128, y, z,       128.0f, 0.0f  );
                t->vertexUV(x + 128, y, z + 128, 128.0f, 128.0f);
                t->vertexUV(x,       y, z + 128, 0.0f,   128.0f);
            }
        }
    }

    t->flush();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void LevelRenderer::renderHit(std::shared_ptr<Player>& p, std::unique_ptr<HitResult>& h, std::int32_t unk, std::int32_t tile) {
    std::shared_ptr<Tesselator> t = Tesselator::instance;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    int64_t time = Timer::getTimeInMilliSeconds();         // for some reason this doesnt work
    float alpha = (sin(time / 100.0) * 0.2f + 0.4f) * 0.5; // unless i split it up like this
    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    if (unk == 0) {
        t->init();
        for (int face = 0; face < 6; face++) {
            Tile::renderFaceNoTexture(p, t, h->x, h->y, h->z, face);
        }
        t->flush();
    } else {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        int64_t time = Timer::getTimeInMilliSeconds();         // for some reason this doesnt work
        float color = (sin(time / 100.0) * 0.2f + 0.4f) * 0.5; // unless i split it up like this
        float alpha = sin(time / 200.0) * 0.2f + 0.5f;
        glColor4f(color, color, color, alpha);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->textureManager->loadTexture("terrain.png", GL_NEAREST));
        
        int x = h->x;
        int y = h->y;
        int z = h->z;
        switch (h->f) {
            case 0: y--; break;
            case 1: y++; break;
            case 2: z--; break;
            case 3: z++; break;
            case 4: x--; break;
            case 5: x++; break;
        }

        t->init();
        t->noColor();
        Tile::tiles[tile]->render(t, this->level, 0, x, y, z);
        Tile::tiles[tile]->render(t, this->level, 1, x, y, z);
        t->flush();
        glDisable(GL_TEXTURE_2D);
    }

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

void LevelRenderer::resetChunks() {
    for (int i = 0; i < this->chunks.size(); i++) {
        this->chunks[i].reset();
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