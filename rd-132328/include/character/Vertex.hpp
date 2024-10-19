#pragma once

#include "base.hpp"

#include "character/Vec3.hpp"

class Vertex {
    public:
        Vec3 pos;
        float u;
        float v;

        Vertex(float x, float y, float z, float u, float v);
        Vertex remap(float u, float v);
        Vertex(Vertex* vertex, float u, float v);
        Vertex(Vec3 pos, float u, float v);
        
        Vertex(void);
};