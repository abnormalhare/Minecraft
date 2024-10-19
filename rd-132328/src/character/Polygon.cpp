#include "character/Polygon.hpp"

PolygonRD::PolygonRD(Vertex* vertices, std::int32_t vertCount) {
    this->vertices = vertices;
    this->vertexCount = vertCount;
}

PolygonRD::PolygonRD(Vertex* vertices, std::int32_t vertCount, std::int32_t u0, std::int32_t v0, std::int32_t u1, std::int32_t v1) {
    this->vertices = vertices;
    this->vertexCount = vertCount;
    vertices[0] = vertices[0].remap(u1, v0);
    vertices[1] = vertices[1].remap(u0, v0);
    vertices[2] = vertices[2].remap(u0, v1);
    vertices[3] = vertices[3].remap(u1, v1);
}

void PolygonRD::render(void) {
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 3; i >= 0; i--) {
        Vertex v = this->vertices[i];
        glTexCoord2f(v.u / 64.0f, v.v / 32.0f);
        glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    }
}

PolygonRD::PolygonRD(void)
 : vertices(nullptr), vertexCount(0) {}