#include "java/ArrayList.h"

ArrayList* newArrayList(size_t initialCapacity) {
    ArrayList* list = (ArrayList*)malloc(sizeof(ArrayList));
    list->data = malloc(initialCapacity * sizeof(void*));
    list->size = 0;
    list->capacity = initialCapacity;
    
    return list;
}

void freeArrayList(ArrayList* list) {
    free(list->data);
    free(list);
}