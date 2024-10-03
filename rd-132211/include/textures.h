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

void initIdMap(void);
s32 loadTexture(String resourceName, s32 mode);
void texturesBind(int id);

#endif