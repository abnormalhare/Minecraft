#ifndef LEVEL_H
#define LEVEL_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib/zlib.h"

#include "phys/AABB.h"
#include "level/levelListener.h"
#include "java/ArrayList.h"

typedef struct _Level {
    s32 width;
    s32 height;
    s32 depth;
    char* blocks;
    s32* lightDepths;
    ArrayList* LevelListeners;
} Level;

Level* newLevel(s32 w, s32 h, s32 d);
void load(Level* level);
void save(Level* level);
void calcLightDepths(Level* level, s32 x0, s32 y0, s32 x1, s32 y1);
void addListener(Level* level, LevelListener* levelListener);
void removeListener(Level* level, LevelListener* levelListener);
bool isTile(Level* level, s32 x, s32 y, s32 z);
bool isSolidTile(Level* level, s32 x, s32 y, s32 z);
bool isLightBlocker(Level* level, s32 x, s32 y, s32 z);
ArrayList* getCubes(Level* level, AABB* aABB);
float getBrightness(Level* level, s32 x, s32 y, s32 z);
void setTile(Level* level, s32 x, s32 y, s32 z, s32 type);

#endif