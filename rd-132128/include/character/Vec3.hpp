#pragma once

#include "base.hpp"

class Vec3 {
    public:
        float x;
        float y;
        float z;

        Vec3(float x = 0, float y = 0, float z = 0);
        Vec3 interpolateTo(Vec3& t, float p);
        void set(float x, float y, float z);
};