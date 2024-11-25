#pragma once

#include "base.hpp"

class AABB {
    private:
        float epsilon = 0.0f;
        
    public:
        float x0, y0, z0;
        float x1, y1, z1;

        AABB(float x0, float y0, float z0, float x1, float y1, float z1);
        std::unique_ptr<AABB> expand(float xa, float ya, float za);
        std::unique_ptr<AABB> grow(float xa, float ya, float za);
        float clipXCollide(std::unique_ptr<AABB>& c, float xa);
        float clipYCollide(std::unique_ptr<AABB>& c, float ya);
        float clipZCollide(std::unique_ptr<AABB>& c, float za);
        bool intersects(std::unique_ptr<AABB>& c);
        void move(float xa, float ya, float za);
};