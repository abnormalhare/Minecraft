#ifndef TEXTURES_H
#define TEXTURES_H

#include "types.h"

#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include "java/ArrayList.h"
#include "java/IntBuffer.h"
#include "java/HashMap.h"

void initIdMap(void);
s32 loadTexture(String resourceName, s32 mode);
void texturesBind(int id);

#endif