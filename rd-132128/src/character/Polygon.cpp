#include "character/Polygon.hpp"

PolygonRD::PolygonRD(void) {}

PolygonRD::PolygonRD(std::vector<Vertex>& vertices)
    : vertices(vertices), vertexCount(vertices.size()) {}

PolygonRD::PolygonRD(std::vector<Vertex>& vertices, int u0, int v0, int u1, int v1)
    : vertices(vertices), vertexCount(vertices.size()) {

    vertices[0] = vertices[0].remap(u1, v0);
    vertices[1] = vertices[1].remap(u0, v0);
    vertices[2] = vertices[2].remap(u0, v1);
    vertices[3] = vertices[3].remap(u1, v1);
}

void PolygonRD::render(void) {
    Vertex v;
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 3; i >= 0; i--) {
        v = vertices[i];
        glTexCoord2f(v.u / 64.0f, v.v / 32.0f);
        glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    }
}