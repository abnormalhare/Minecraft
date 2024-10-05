#ifndef TILE_H
#define TILE_H

#include "types.h"
#include "level/level.h"
#include "level/tesselator.h"

typedef struct _Tile {
    s32 tex;
} Tile;

extern Tile* rock;
extern Tile* grass;

void initTiles(void);
void tileRender(Tile* tile, Tesselator* t, Level* level, s32 layer, s32 x, s32 y, s32 z);
void tileRenderFace(Tile* tile, Tesselator* t, s32 x, s32 y, s32 z, s32 face);

#endif