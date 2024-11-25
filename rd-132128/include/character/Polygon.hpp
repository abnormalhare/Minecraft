#pragma once

#include "base.hpp"
#include "character/Vertex.hpp"
#include <GLFW/glfw3.h>
#include <vector>

class PolygonRD {
    public:
        std::vector<Vertex> vertices;
        int vertexCount = 0;

        PolygonRD(void);
        PolygonRD(std::vector<Vertex>& vertices);
        PolygonRD(std::vector<Vertex>& vertices, int u0, int v0, int u1, int v1);
        void render(void);
};