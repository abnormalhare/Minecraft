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
void load(Level* this);
void save(Level* this);
void calcLightDepths(Level* this, s32 x0, s32 y0, s32 x1, s32 y1);
void addListener(Level* this, LevelListener* levelListener);
void removeListener(Level* this, LevelListener* levelListener);
bool isTile(Level* this, s32 x, s32 y, s32 z);
bool isSolidTile(Level* this, s32 x, s32 y, s32 z);
bool isLightBlocker(Level* this, s32 x, s32 y, s32 z);
ArrayList* getCubes(Level* this, AABB* aABB);
float getBrightness(Level* this, s32 x, s32 y, s32 z);
void setTile(Level* this, s32 x, s32 y, s32 z, s32 type);

#endif