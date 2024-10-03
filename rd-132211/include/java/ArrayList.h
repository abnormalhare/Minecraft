#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "level/levelListener.h"

ArrayList* newArrayList(size_t initialCapacity);
void freeArrayList(ArrayList* list);

#endif