#pragma once

#include "base.hpp"

class Vec3 {
    public:
        float x;
        float y;
        float z;

        Vec3(void);
        Vec3(float x, float y, float z);
        Vec3 interpolateTo(Vec3& t, float p);
        void set(float x, float y, float z);
};