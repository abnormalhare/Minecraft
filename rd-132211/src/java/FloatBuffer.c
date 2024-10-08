#include "java/FloatBuffer.h"

void floatBufferInit(FloatBuffer* buffer, s32 capacity) {
    buffer->data = (float*)malloc(capacity * sizeof(float));
    if (!buffer->data) {
        fprintf(stderr, "Failed to allocate for floatBuffer data");
        return;
    }
    
    buffer->capacity = capacity;
    buffer->position = 0;
    buffer->limit = capacity;
}

void floatBufferFlip(FloatBuffer* buffer) {
    buffer->limit = buffer->position;
    buffer->position = 0;
}

void floatBufferClear(FloatBuffer* buffer) {
    buffer->position = 0;
    buffer->limit = buffer->position;
}

bool floatBufferPut(FloatBuffer* buffer, s32 index, float value) {
    if (index < buffer->limit) {
        buffer->data[index] = value;
        return TRUE;
    } else {
        return FALSE;
    }
}

void floatBufferLimit(FloatBuffer* buffer, s32 limit) {
    buffer->limit = limit;
}

float floatBufferGet(FloatBuffer* buffer) {
    return buffer->data[buffer->position++];
}

float floatBufferGeti(FloatBuffer* buffer, s32 index) {
    buffer->position = index;
    return floatBufferGet(buffer);
}

void floatBufferGetv(FloatBuffer* buffer, float* dst) {
    for (s32 i = 0; i < buffer->limit; i++) {
        dst[i] = buffer->data[i];
    }
}