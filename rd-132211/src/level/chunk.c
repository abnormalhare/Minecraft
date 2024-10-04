#include "level/chunk.h"

s32 rebuiltThisFrame = 0;
s32 chunkUpdates = 0;
Tesselator* t = NULL;
s32 texture;

bool initChunk(void) {
    texture = loadTexture("terrain.png", GL_NEAREST);
    if (texture == -1) {
        fprintf(stderr, "Failed to load textures!");
        return 0;
    }
    t = newTesselator();
    return 1;
}

Chunk* newChunk(Level* level, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1) {
    Chunk* chunk = malloc(sizeof(Chunk));
    if (!chunk) {
        fprintf(stderr, "Failed to allocate to chunk");
        return NULL;
    }

    chunk->aabb = newAABB(x0, y0, z0, x1, y1, z1);
    chunk->level = level;
    chunk->x0 = x0; chunk->y0 = y0; chunk->z0 = z0;
    chunk->x1 = x1; chunk->y1 = y1; chunk->z1 = z1;
    chunk->dirty = TRUE;
    chunk->lists = glGenLists(2);

    return chunk;
}

void rebuild(Chunk* this, s32 layer) {
    if (rebuiltThisFrame == 2) return;

    this->dirty = FALSE;
    chunkUpdates++;
    rebuiltThisFrame++;
    glNewList(this->lists + layer, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    tesselatorInit(t);

    s32 tiles = 0;
    for (s32 x = this->x0; x < this->x1; x++) {
        for (s32 y = this->y0; y < this->y1; y++) {
            for (s32 z = this->z0; z < this->z1; z++) {
                if (isTile(this->level, x, y, z)) {
                    s32 tex = (y == this->level->depth * 2 / 3) ? 0 : 1;
                    tiles++;
                    if (tex == 0) {
                        tileRender(rock, t, this->level, layer, x, y, z);
                    } else {
                        tileRender(grass, t, this->level, layer, x, y, z);
                    }
                }
            }
        }
    }

    flush(t);
    glDisable(GL_TEXTURE_2D);
    glEndList();
}

void chunkRender(Chunk* this, s32 layer) {
    if (this->dirty) {
        rebuild(this, 0);
        rebuild(this, 1);
    }
    glCallList(this->lists + layer);
}

void chunkSetDirty(Chunk* this) {
    this->dirty = TRUE;
}