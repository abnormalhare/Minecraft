#include "character/Polygon.hpp"

PolygonRD::PolygonRD(void) : vertexCount(0) {}

PolygonRD::PolygonRD(std::vector<Vertex>& _vertices)
    : vertices(_vertices), vertexCount(_vertices.size()) {}

PolygonRD::PolygonRD(std::vector<Vertex>& _vertices, int u0, int v0, int u1, int v1)
    : vertexCount(_vertices.size()) {

    vertices.push_back(_vertices[0].remap(u1, v0));
    vertices.push_back(_vertices[1].remap(u0, v0));
    vertices.push_back(_vertices[2].remap(u0, v1));
    vertices.push_back(_vertices[3].remap(u1, v1));
}

void PolygonRD::render(void) {
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 3; i >= 0; i--) {
        Vertex v = vertices[i];
        glTexCoord2f(v.u / 64.0f, v.v / 32.0f);
        glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    }
}