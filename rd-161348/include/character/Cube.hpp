#pragma once

#include "base.hpp"
#include "character/Vertex.hpp"
#include "character/Polygon.hpp"
#include <GL/gl.h>
#include <array>
#include <vector>

class Cube {
    private:
        std::array<Vertex, 8> vertices;
        std::array<PolygonRD, 6> polygons;
        int xTexOffs, yTexOffs;

        bool compiled;
        int list;

        void compile();
    
    public:
        float x, y, z;
        float xRot, yRot, zRot;

        Cube(int xTexOffs = 0, int yTexOffs = 0);
        void setTexOffs(int xTexOffs, int yTexOffs);
        void addBox(float x0, float y0, float z0, int w, int h, int d);
        void setPos(float x, float y, float z);
        void render(void);
};