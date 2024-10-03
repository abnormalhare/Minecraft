#ifndef LEVEL_RENDERER_H
#define LEVEL_RENDERER_H

#include "types.h"

#include <GLFW/glfw3.h>
#include "hitResult.h"
#include "player.h"
#include "phys/AABB.h"
#include "level/chunk.h"
#include "level/frustum.h"
#include "level/tesselator.h"
#include "level/level.h"
#include "level/tile.h"
#include "java/time.h"

typedef struct {
    Level* level;
    Chunk** chunks;
    s32 xChunks;
    s32 yChunks;
    s32 zChunks;
    Tesselator* t;
    LevelListener* levelListener;
} LevelRenderer;

LevelRenderer* newLevelRenderer(Level* level);
void lrRender(LevelRenderer* this, Player* player, s32 layer);
void lrPick(LevelRenderer* this, Player* player);
void lrRenderHit(LevelRenderer* this, HitResult* h);
void lrSetDirty(LevelRenderer* this, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1);


#endif