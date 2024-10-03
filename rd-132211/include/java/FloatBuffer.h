#ifndef FLOAT_BUFFER_H
#define FLOAT_BUFFER_H

#include "types.h"
#include <stdio.h>

void floatBufferInit(FloatBuffer* buffer, s32 capacity);
void floatBufferFlip(FloatBuffer* buffer);
void floatBufferClear(FloatBuffer* buffer);
bool floatBufferPut(FloatBuffer* buffer, s32 index, float value);
void floatBufferLimit(FloatBuffer* buffer, s32 limit);
float floatBufferGet(FloatBuffer* buffer);
float floatBufferGeti(FloatBuffer* buffer, s32 index);
void floatBufferGetv(FloatBuffer* buffer, float* dst);

#endif