#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

HashMap* newHashMap(void) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->items = malloc(sizeof(item));
    map->size = 0;
    
    return map;
}

void freeHashMap(HashMap* map) {
    free(map->items);
    free(map);
}

void addHash(HashMap* map, item value) {
    item* it;
    size_t i;
    for (i = 0; i < map->size; i++) {
        it = &map->items[i];
        if (it->key == value.key) {
            it->value = value.value;
            return;
        }
    }

    map->size++;
    map->items = realloc(map->items, map->size * sizeof(item*));
    map->items[map->size - 1].key = malloc(128);
    map->items[map->size - 1] = value;
}

item* getHash(HashMap* map, String key) {
    for (s32 i = 0; i < map->size; i++) {
        if (map->items[i].key == key) {
            return &map->items[i];
        }
    }

    return NULL;
}

void removeHash(HashMap* map, item value) {
    item it;
    for (s32 i = 0; i < map->size; i++) {
        it = map->items[i];
        if (it.key == value.key) {
            for (s32 j = i; j < map->size - 1; j++) {
                map->items[j] = map->items[j + 1];
            }
            map->size--;
        }
    }
}

bool containsKey(HashMap* map, String key) {
    item it;

    for (s32 i = 0; i < map->size; i++) {
        it = map->items[i];
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