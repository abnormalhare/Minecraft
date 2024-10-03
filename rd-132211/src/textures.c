#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void addHash(ArrayList* list, item value) {
    item it;
    for (size_t i = 0; i < list->size; i++) {
        it = ((item*)list->data)[i];
        if (it.key == value.key) {
            it.value = value.value;
        }
    }
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(item*));
    }
    ((item*)list->data)[list->size++] = value;
}

void getHash(ArrayList* list, size_t index, item* retVal) {
    if (index < list->size) {
        *retVal = ((item*)list->data)[index];
    } else {
        retVal = NULL;
    }
}

void removeHash(ArrayList* list, item value) {
    item it;
    for (size_t i = 0; i < list->size; i++) {
        it = ((item*)list->data)[i];
        if (it.key == value.key) {
            for (size_t j = i; j < list->size - 1; j++) {
                ((item*)list->data)[j] = ((item*)list->data)[j + 1];
            }
            list->size--;
        }
    }
}

bool containsKey(ArrayList* map, String key) {
    item it;

    for (s32 i = 0; i < map->size; i++) {
        getHash(map, i, &it);
        if (it.key != NULL && strcmp(it.key, key) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

void getRGB(u8* img, s32 startX, s32 startY, s32 width, s32 height, u8* pixels, s32 offset, s32 scansize) {
    for (s32 x = startX; x < width; x++) {
        for (s32 y = startY; y < height; y++) {
            pixels[x * y] = img[offset + (y-startY)*scansize + (x-startX)];
        }
    }
}

ArrayList* idMap;
s32 lastId = -9999999;

void initIdMap(void) {
    idMap = newArrayList(10);
}

s32 loadTexture(String resourceName, s32 mode) {
    item it;

    if (containsKey(idMap, resourceName)) {
        getHash(idMap, resourceName, &it);
        return it.value;
    }

    IntBuffer ib;
    intBufferInit(&ib, 1);
    
    glGenTextures(1, ib.data);
    s32 id = ib.data[0];
    texturesBind(id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

    s32 w, h, channels;
    u8* img = stbi_load(resourceName, &w, &h, &channels, 0);
    if (!img) return -1;

    char pixels[w * h * 4];
    getRGB(img, 0, 0, w, h, pixels, 0, w);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    return id;
}

void texturesBind(int id) {
    if (id != lastId) {
        glBindTexture(GL_TEXTURE_2D, id);
        lastId = id;
    }
}