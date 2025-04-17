#pragma once

#include "base.hpp"

class AABB {
    private:
        float epsilon = 0.0f;
        
    public:
        float x0, y0, z0;
        float x1, y1, z1;

        AABB(float x0, float y0, float z0, float x1, float y1, float z1);
        AABB(void);
        AABB expand(float xa, float ya, float za);
        AABB grow(float xa, float ya, float za);
        float clipXCollide(AABB& c, float xa);
        float clipYCollide(AABB& c, float ya);
        float clipZCollide(AABB& c, float za);
        bool intersects(AABB& c);
        void move(float xa, float ya, float za);
};