#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "level/levelListener.h"

ArrayList* newArrayList(size_t initialCapacity, callable add, callable get, callable remove);
void freeArrayList(ArrayList* list);

#endif