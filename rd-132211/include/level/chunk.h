#ifndef CHUNK_H
#define CHUNK_H

#include <GLFW/glfw3.h>
#include "types.h"
#include "textures.h"
#include "phys/AABB.h"
#include "level/level.h"
#include "level/tesselator.h"
#include "level/tile.h"

typedef struct _Chunk {
    AABB* aabb;
    Level* level;
    s32 x0, y0, z0;
    s32 x1, y1, z1;
    bool dirty;
    s32 lists;
} Chunk;

void initChunk(void);
Chunk* newChunk(Level* level, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1);
void rebuild(Chunk* this, s32 layer);
void chunkRender(Chunk* this, s32 layer);
void chunkSetDirty(Chunk* this);

extern s32 rebuiltThisFrame;
extern s32 chunkUpdates;

#endif