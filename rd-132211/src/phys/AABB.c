#include "phys/AABB.h"

void addAABB(ArrayList* list, AABB* value) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(AABB*));
    }
    ((AABB**)list->data)[list->size++] = value;
}

void getAABB(ArrayList* list, size_t index, AABB* retVal) {
    if (index < list->size) {
        *retVal = *((AABB**)list->data)[index];
    } else {
        retVal = NULL;
    }
}

void removeAABB(ArrayList* list, AABB* value) {
    for (size_t i = 0; i < list->size; i++) {
        if (((AABB**)list->data)[i] == value) {
            for (size_t j = i; j < list->size - 1; j++) {
                ((AABB**)list->data)[j] = ((AABB**)list->data)[j + 1];
            }
            list->size--;
        }
    }
}

AABB* newAABB(float x0, float y0, float z0, float x1, float y1, float z1) {
    AABB* aabb = malloc(sizeof(AABB));
    if (!aabb) {
        fprintf(stderr, "Failed to allocate to aabb");
        return NULL;
    }

    aabb->x0 = x0; aabb->y0 = y0; aabb->z0 = z0;
    aabb->x1 = x1; aabb->y1 = y1; aabb->z1 = z1;

    return aabb;
}

AABB* expand(AABB* aabb, float xa, float ya, float za) {
    float _x0 = aabb->x0;
    float _y0 = aabb->y0;
    float _z0 = aabb->z0;
    float _x1 = aabb->x1;
    float _y1 = aabb->y1;
    float _z1 = aabb->z1;

    if (xa < 0.0f) {
        _x0 += xa;
    }
    if (xa > 0.0f) {
        _x1 += xa;
    }
    if (ya < 0.0f) {
        _y0 += ya;
    }
    if (ya > 0.0f) {
        _y1 += ya;
    }
    if (za < 0.0f) {
        _z0 += za;
    }
    if (za > 0.0f) {
        _z1 += za;
    }

    return newAABB(_x0, _y0, _z0, _x1, _y1, _z1);
}

AABB* grow(AABB* aabb, float xa, float ya, float za) {
    float _x0 = aabb->x0 - xa;
    float _y0 = aabb->y0 - ya;
    float _z0 = aabb->z0 - za;
    float _x1 = aabb->x1 + xa;
    float _y1 = aabb->y1 + ya;
    float _z1 = aabb->z1 + za;
    return newAABB(_x0, _y0, _z0, _x1, _y1, _z1);
}

float clipXCollide(AABB* aabb, AABB* c, float xa) {
    float max;

    if (c->y1 <= aabb->y0 || c->y0 >= aabb->y1)
        return xa;
    if (c->z1 <= aabb->z0 || c->z0 >= aabb->z1)
        return xa; 
    if (xa > 0.0F && c->x1 <= aabb->x0) {
        max = aabb->x0 - c->x1 - aabb->epsilon;
        if (max < xa)
            xa = max; 
    } 
    if (xa < 0.0F && c->x0 >= aabb->x1) {
        max = aabb->x1 - c->x0 + aabb->epsilon;
        if (max > xa)
            xa = max; 
    } 
    return xa;
}

float clipYCollide(AABB* aabb, AABB* c, float ya) {
    float max;

    if (c->x1 <= aabb->x0 || c->x0 >= aabb->x1)
        return ya; 
    if (c->z1 <= aabb->z0 || c->z0 >= aabb->z1)
        return ya; 
    if (ya > 0.0F && c->y1 <= aabb->y0) {
        max = aabb->y0 - c->y1 - aabb->epsilon;
        if (max < ya)
            ya = max; 
    } 
    if (ya < 0.0F && c->y0 >= aabb->y1) {
        max = aabb->y1 - c->y0 + aabb->epsilon;
        if (max > ya)
            ya = max; 
    } 
    return ya;
}

float clipZCollide(AABB* aabb, AABB* c, float za) {
    float max;

    if (c->x1 <= aabb->x0 || c->x0 >= aabb->x1)
        return za; 
    if (c->y1 <= aabb->y0 || c->y0 >= aabb->y1)
        return za; 
    if (za > 0.0F && c->z1 <= aabb->z0) {
        max = aabb->z0 - c->z1 - aabb->epsilon;
        if (max < za)
            za = max; 
    } 
    if (za < 0.0F && c->y0 >= aabb->y1) {
        max = aabb->z1 - c->z0 + aabb->epsilon;
        if (max > za)
            za = max; 
    } 
    return za;
}

bool intersects(AABB* aabb, AABB* c) {
    if (c->x1 <= aabb->x0 || c->x0 >= aabb->x1)
        return FALSE;
    if (c->y1 <= aabb->y0 || c->y0 >= aabb->y1)
        return FALSE;
    if (c->z1 <= aabb->z0 || c->z0 >= aabb->z1)
        return FALSE;
    return TRUE; 
}

void AABBMove(AABB* aabb, float xa, float ya, float za) {
    aabb->x0 += xa;
    aabb->y0 += ya;
    aabb->z0 += za;
    aabb->x1 += xa;
    aabb->y1 += ya;
    aabb->z1 += za;
}