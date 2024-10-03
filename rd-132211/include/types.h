#ifndef _TYPES_H
#define _TYPES_H

#include <windows.h>

typedef long long s64;
typedef unsigned long long u64;
typedef int s32;
typedef unsigned int u32;
typedef short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;
typedef const char* String;
typedef int bool;

typedef void (*callable)();

typedef struct _FloatBuffer {
    float* data;
    s32 capacity;
    s32 position;
    s32 limit;
} FloatBuffer;

typedef struct _IntBuffer {
    int* data;
    s32 capacity;
    s32 position;
    s32 limit;
} IntBuffer;

typedef struct _ArrayList {
    void *data;
    size_t size;
    size_t capacity;
} ArrayList;

#endif