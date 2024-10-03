#include "java/ArrayList.h"

ArrayList* newArrayList(size_t initialCapacity) {
    ArrayList* list = (ArrayList*)malloc(sizeof(ArrayList));
    if (!list) {
        fprintf(stderr, "Failed to allocate for arraylist");
        return NULL;
    }
    
    list->data = malloc(initialCapacity * sizeof(void*));
    if (!list->data) {
        fprintf(stderr, "Failed to allocate for arraylist data");
        return NULL;
    }
    
    list->size = 0;
    list->capacity = initialCapacity;
    
    return list;
}

void freeArrayList(ArrayList* list) {
    free(list->data);
    free(list);
}