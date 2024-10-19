#pragma once

#include "base.hpp"

#include "character/Vertex.hpp"
#include "character/Polygon.hpp"

#include <memory>

class Cube {
    private:
        Vertex vertices[8];
        PolygonRD polygons[6];

        std::int32_t xTexOffs, yTexOffs;
    
    public:
        float x, y, z = 0.0f;
        float xRot, yRot, zRot = 0.0f;

        Cube(std::int32_t xTexOffs, std::int32_t yTexOffs);
        void setTexOffs(std::int32_t xTexOffs, std::int32_t yTexOffs);
        void addBox(float x0, float y0, float z0, std::int32_t w, std::int32_t h, std::int32_t d);
        void setPos(float x, float y, float z);
        void render(void);
};