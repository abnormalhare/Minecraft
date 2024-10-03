#ifndef int_BUFFER_H
#define int_BUFFER_H

#include "types.h"
#include <stdio.h>

void intBufferInit(IntBuffer* buffer, s32 capacity);
void intBufferFlip(IntBuffer* buffer);
void intBufferClear(IntBuffer* buffer);
bool intBufferPut(IntBuffer* buffer, s32 index, int value);
void intBufferLimit(IntBuffer* buffer, s32 limit);
s32 intBufferGet(IntBuffer* buffer);

#endif