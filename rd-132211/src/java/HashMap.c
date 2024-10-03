#include "java/HashMap.h"

HashMap* newHashMap(void) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) {
        fprintf(stderr, "Failed to allocate for hashmap");
        return NULL;
    }
    
    map->items = (item*)malloc(sizeof(item));
    if (!map->items) {
        fprintf(stderr, "Failed to allocate for hashmap data");
        return NULL;
    }

    map->items[0].key = malloc(128);
    if (!map->items[0].key) {
        fprintf(stderr, "Failed to allocate for hashmap's first string");
    }
    
    map->size = 0;
    map->capacity = 1;
    
    return map;
}

void freeHashMap(HashMap* map) {
    free(map->items);
    free(map);
}

void addHash(HashMap* map, item value) {
    item* it;
    s32 i;
    for (i = 0; i < map->size; i++) {
        it = &map->items[i];
        if (it->key == value.key) {
            it->value = value.value;
            return;
        }
    }
    
    if (map->size >= map->capacity) {
        map->capacity *= 2;
        map->items = realloc(map->items, map->capacity * sizeof(item));
    }
    map->items[map->size].key = malloc(128);
    map->items[map->size++] = value;
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
