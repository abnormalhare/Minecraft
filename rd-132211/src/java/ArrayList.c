#include "java/ArrayList.h"

ArrayList* newArrayList(size_t initialCapacity, callable add, callable get, callable remove) {
    ArrayList* list = (ArrayList*)malloc(sizeof(ArrayList));
    list->data = malloc(initialCapacity * sizeof(void*));
    list->size = 0;
    list->capacity = initialCapacity;
    
    list->add = add;
    list->get = get;
    list->remove = remove;
    return list;
}

void freeArrayList(ArrayList* list) {
    free(list->data);
    free(list);
}