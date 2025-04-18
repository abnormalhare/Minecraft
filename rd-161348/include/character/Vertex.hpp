#pragma once

#include "base.hpp"
#include "character/Vec3.hpp"

class Vertex {
    public:
        Vec3 pos;
        float u, v;

        Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f, float u = 0, float v = 0);
        Vertex remap(float u, float v);
        Vertex(Vertex& vertex, float u, float v);
        Vertex(Vec3& pos, float u, float v);
};