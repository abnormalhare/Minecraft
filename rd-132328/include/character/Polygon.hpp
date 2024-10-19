#pragma once

#include "base.hpp"
#include <GLFW/glfw3.h>

#include "character/Vertex.hpp"

class PolygonRD {
    public:
        Vertex* vertices;
        std::int32_t vertexCount = 0;

        PolygonRD(Vertex* vertices, std::int32_t vertCount);
        PolygonRD(Vertex* vertices, std::int32_t vertCount, std::int32_t u0, std::int32_t v0, std::int32_t u1, std::int32_t v1);
        void render(void);

        PolygonRD(void);
};