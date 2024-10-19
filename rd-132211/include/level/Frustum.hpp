#pragma once

#include "base.hpp"
#include <cmath>
#include <vector>
#include <GLFW/glfw3.h>

#include "phys/AABB.hpp"

class Frustum {
    private:
        static Frustum* frustum;
        float proj[16];
        float modl[16];
        float clip[16];

        void normalizePlane(float frustum[6][4], int side);
        void calculateFrustum(void);
    
    public:
        float m_Frustum[6][4];
        static const std::int32_t RIGHT = 0;
        static const std::int32_t LEFT = 1;
        static const std::int32_t BOTTOM = 2;
        static const std::int32_t TOP = 3;
        static const std::int32_t BACK = 4;
        static const std::int32_t FRONT = 5;
        
        static const std::int32_t A = 0;
        static const std::int32_t B = 1;
        static const std::int32_t C = 2;
        static const std::int32_t D = 3;

        static Frustum* getFrustum();
        bool pointInFrustum(float x, float y, float z);
        bool sphereInFrustum(float x, float y, float z, float radius);
        bool cubeFullyInFrustum(float x1, float y1, float z1, float x2, float y2, float z2);
        bool cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2);
        bool cubeInFrustum(AABB* aabb);
};