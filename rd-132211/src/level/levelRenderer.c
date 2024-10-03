#include "level/levelRenderer.h"

s32 CHUNK_SIZE = 16;

void _lrTileChanged(LevelRenderer* this, s32 x, s32 y, s32 z) {
    lrSetDirty(this, x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void _lrLightColumnChanged(LevelRenderer* this, s32 x, s32 z, s32 y0, s32 y1) {
    lrSetDirty(this, x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
}

void _lrAllChanged(LevelRenderer* this) {
    lrSetDirty(this, 0, 0, 0, this->level->width, this->level->depth, this->level->height);
}

LevelRenderer* newLevelRenderer(Level* level) {
    LevelRenderer* lr = malloc(sizeof(LevelRenderer));
    if (!lr) {
        fprintf(stderr, "Failed to allocate to level renderer");
        return NULL;
    }

    lr->t = newTesselator();
    lr->level = level;

    lr->levelListener = malloc(sizeof(LevelListener));
    if (!lr->levelListener) {
        fprintf(stderr, "Failed to allocate to level listener");
        return NULL;
    }
    lr->levelListener->base = lr;
    lr->levelListener->tileChanged = _lrTileChanged;
    lr->levelListener->lightColumnChanged = _lrLightColumnChanged;
    lr->levelListener->allChanged = _lrAllChanged;
    addListener(level, lr->levelListener);
    
    lr->xChunks = level->width / 16;
    lr->yChunks = level->depth / 16;
    lr->zChunks = level->height / 16;
    lr->chunks = malloc(lr->xChunks * lr->yChunks * lr->zChunks * sizeof(Chunk));
    if (!lr->chunks) {
        fprintf(stderr, "Failed to allocate to level chunks");
        return NULL;
    }
    for (s32 x = 0; x < lr->xChunks; x++) {
        for (s32 y = 0; y < lr->yChunks; y++) {
            for (s32 z = 0; z < lr->zChunks; z++) {
                s32 x0 = x * 16;
                s32 y0 = y * 16;
                s32 z0 = z * 16;
                s32 x1 = (x + 1) * 16;
                s32 y1 = (y + 1) * 16;
                s32 z1 = (z + 1) * 16;

                if (x1 > level->width ) x1 = level->width;
                if (y1 > level->depth ) y1 = level->depth;
                if (z1 > level->height) z1 = level->height;

                lr->chunks[(x + y * lr->xChunks) * lr->zChunks + z] = newChunk(level, x0, y0, z0, x1, y1, z1);
            }
        }
    }

    return lr;
}

void lrRender(LevelRenderer* this, Player* player, s32 layer) {
    Frustum* frustum;
    
    s32 chunkLength = this->xChunks * this->yChunks * this->zChunks;

    rebuiltThisFrame = 0;
    frustum = getFrustum();
    for (s32 i = 0; i < chunkLength; i++) {
        if (cubeInFrustumA(frustum, this->chunks[i]->aabb)) {
            chunkRender(this->chunks[i], layer);
        }
    }
}

void lrPick(LevelRenderer* this, Player* player) {
    float r = 3.0f;
    AABB* box = grow(player->bb, r, r, r);
    s32 x0 = box->x0;
    s32 x1 = box->x1 + 1.0f;
    s32 y0 = box->y0;
    s32 y1 = box->y1 + 1.0f;
    s32 z0 = box->z0;
    s32 z1 = box->z1 + 1.0f;
    
    glInitNames();
    for (s32 x = x0; x < x1; x++) {
        glPushName(x);
        for (s32 y = y0; y < y1; y++) {
            glPushName(y);
            for (s32 z = z0; z < z1; z++) {
                glPushName(z);
                if (isSolidTile(this->level, x, y, z)) {
                    glPushName(0);
                    for (s32 i = 0; i < 6; i++) {
                        glPushName(i);
                        tesselatorInit(this->t);
                        tileRenderFace(rock, this->t, x, y, z, i);
                        flush(this->t);
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

void lrRenderHit(LevelRenderer* this, HitResult* h) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0f, 1.0f, 1.0f, sin(currentTimeMillis() / 100.0) * 0.2f + 0.4f);
    tesselatorInit(this->t);
    tileRenderFace(rock, this->t, h->x, h->y, h->z, h->f);
    flush(this->t);
    glDisable(GL_BLEND);
}

void lrSetDirty(LevelRenderer* this, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1) {
    x0 /= 16;
    x1 /= 16;
    y0 /= 16;
    y1 /= 16;
    z0 /= 16;
    z1 /= 16;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 >= this->xChunks) x1 = this->xChunks - 1;
    if (y1 >= this->xChunks) y1 = this->yChunks - 1;
    if (z1 >= this->xChunks) z1 = this->zChunks - 1;

    
    for (s32 x = x0; x < x1; x++) {
        for (s32 y = y0; y < y1; y++) {
            for (s32 z = z0; z < z1; z++) {
                chunkSetDirty(this->chunks[(x + y * this->xChunks) * this->zChunks + z]);
            }
        }
    }
}