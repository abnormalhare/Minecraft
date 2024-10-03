#include "phys/AABB.h"

AABB* newAABB(float x0, float y0, float z0, float x1, float y1, float z1) {
    AABB* aabb = malloc(sizeof(AABB));
    
    aabb->x0 = x0; aabb->y0 = y0; aabb->z0 = z0;
    aabb->x1 = x1; aabb->y1 = y1; aabb->z1 = z1;

    return aabb;
}

AABB* expand(AABB* this, float xa, float ya, float za) {
    float _x0 = this->x0;
    float _y0 = this->y0;
    float _z0 = this->z0;
    float _x1 = this->x1;
    float _y1 = this->y1;
    float _z1 = this->z1;

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

AABB* grow(AABB* this, float xa, float ya, float za) {
    float _x0 = this->x0 - xa;
    float _y0 = this->y0 - ya;
    float _z0 = this->z0 - za;
    float _x1 = this->x1 + xa;
    float _y1 = this->y1 + ya;
    float _z1 = this->z1 + za;
    return newAABB(_x0, _y0, _z0, _x1, _y1, _z1);
}

float clipXCollide(AABB* this, AABB* c, float xa) {
    float max;

    if (c->y1 <= this->y0 || c->y0 >= this->y1)
        return xa;
    if (c->z1 <= this->z0 || c->z0 >= this->z1)
        return xa; 
    if (xa > 0.0F && c->x1 <= this->x0) {
        max = this->x0 - c->x1 - this->epsilon;
        if (max < xa)
            xa = max; 
    } 
    if (xa < 0.0F && c->x0 >= this->x1) {
        max = this->x1 - c->x0 + this->epsilon;
        if (max > xa)
            xa = max; 
    } 
    return xa;
}

float clipYCollide(AABB* this, AABB* c, float ya) {
    float max;

    if (c->x1 <= this->x0 || c->x0 >= this->x1)
        return ya; 
    if (c->z1 <= this->z0 || c->z0 >= this->z1)
        return ya; 
    if (ya > 0.0F && c->y1 <= this->y0) {
        max = this->y0 - c->y1 - this->epsilon;
        if (max < ya)
            ya = max; 
    } 
    if (ya < 0.0F && c->y0 >= this->y1) {
        max = this->y1 - c->y0 + this->epsilon;
        if (max > ya)
            ya = max; 
    } 
    return ya;
}

float clipZCollide(AABB* this, AABB* c, float za) {
    float max;

    if (c->x1 <= this->x0 || c->x0 >= this->x1)
        return za; 
    if (c->y1 <= this->y0 || c->y0 >= this->y1)
        return za; 
    if (za > 0.0F && c->z1 <= this->z0) {
        max = this->z0 - c->z1 - this->epsilon;
        if (max < za)
            za = max; 
    } 
    if (za < 0.0F && c->y0 >= this->y1) {
        max = this->z1 - c->z0 + this->epsilon;
        if (max > za)
            za = max; 
    } 
    return za;
}

bool intersects(AABB* this, AABB* c) {
    if (c->x1 <= this->x0 || c->x0 >= this->x1)
        return FALSE;
    if (c->y1 <= this->y0 || c->y0 >= this->y1)
        return FALSE;
    if (c->z1 <= this->z0 || c->z0 >= this->z1)
        return FALSE;
    return TRUE; 
}

void AABBMove(AABB* this, float xa, float ya, float za) {
    this->x0 += xa;
    this->y0 += ya;
    this->z0 += za;
    this->x1 += xa;
    this->y1 += ya;
    this->z1 += za;
}