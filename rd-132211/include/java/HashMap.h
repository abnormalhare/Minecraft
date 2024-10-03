#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "types.h"
#include <stdio.h>

typedef struct _item {
    String key;
    s32 value;
} item;

typedef struct _hashMap {
    item* items;
    s32 size;
    s32 capacity;
} HashMap;

HashMap* newHashMap(void);
void freeHashMap(HashMap* map);
void addHash(HashMap* map, item value);
item* getHash(HashMap* map, String key);
void removeHash(HashMap* map, item value);
bool containsKey(HashMap* map, String key);

#endif