#include "java/IntBuffer.h"

void intBufferInit(IntBuffer* buffer, s32 capacity) {
    buffer->data = (int*) malloc(capacity * sizeof(int));
    buffer->capacity = capacity;
    buffer->position = 0;
    buffer->limit = capacity;
}

void intBufferFlip(IntBuffer* buffer) {
    buffer->limit = buffer->position;
    buffer->position = 0;
}

void intBufferClear(IntBuffer* buffer) {
    buffer->position = 0;
    buffer->limit = buffer->position;
}

bool intBufferPut(IntBuffer* buffer, s32 index, int value) {
    if (index < buffer->limit) {
        buffer->data[index] = value;
        return TRUE;
    } else {
        return FALSE;
    }
}

void intBufferLimit(IntBuffer* buffer, s32 limit) {
    buffer->limit = limit;
}

s32 intBufferGet(IntBuffer* buffer) {
    return buffer->data[buffer->position++];
}