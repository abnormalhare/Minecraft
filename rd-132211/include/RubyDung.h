#ifndef RUBY_DUNG_H
#define RUBY_DUNG_H

#include "types.h"
#include <windows.h>
#include <stdio.h>
#include "java/FloatBuffer.h"
#include "java/IntBuffer.h"
#include "java/system.h"
#include "level/frustum.h"
#include "level/level.h"
#include "level/levelRenderer.h"
#include "level/tile.h"
#include "timer.h"
#include "player.h"

typedef struct _RubyDung {
    s32 width;
    s32 height;
    FloatBuffer fogColor;
    Timer* timer;
    Level* level;
    LevelRenderer* levelRenderer;
    Player* player;
    IntBuffer viewportBuffer;
    IntBuffer selectBuffer;
    HitResult* hitResult;
} RubyDung;

s32 rbInit(RubyDung* this);
void rbRun(RubyDung* this);
void rbDestroy(RubyDung* this);
void rbRender(RubyDung* this, float a);
void main(void);

#endif