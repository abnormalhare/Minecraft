#ifndef TEXTURES_H
#define TEXTURES_H

#include "types.h"

#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include "java/ArrayList.h"
#include "java/IntBuffer.h"

typedef struct _item {
    String key;
    s32 value;
} item;

typedef struct _hashMap {
    item* items;
    size_t size;
} HashMap;

HashMap* newHashMap(void);
void freeHashMap(HashMap* map);
void addHash(HashMap* map, item value);
item* getHash(HashMap* map, String key);
void removeHash(HashMap* map, item value);
void initIdMap(void);
s32 loadTexture(String resourceName, s32 mode);
void texturesBind(int id);

#endif