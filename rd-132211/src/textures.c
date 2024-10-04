#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

HashMap* idMap;
s32 lastId = -9999999;

void initIdMap(void) {
    idMap = newHashMap();
}

s32 loadTexture(String resourceName, s32 mode) {
    item* it;

    if (containsKey(idMap, resourceName)) {
        it = getHash(idMap, resourceName);
        return it->value;
    }

    IntBuffer ib;
    intBufferInit(&ib, 1);
    
    glGenTextures(256, ib.data);
    s32 id = ib.data[0];
    texturesBind(id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

    s32 w, h, channels;
    u8* img = stbi_load(resourceName, &w, &h, &channels, 4);
    if (!img) return -1;

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
    return id;
}

void texturesBind(int id) {
    if (id != lastId) {
        glBindTexture(GL_TEXTURE_2D, id);
        lastId = id;
    }
}