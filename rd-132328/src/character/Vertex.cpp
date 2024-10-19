#include "character/Vertex.hpp"

Vertex::Vertex(float x, float y, float z, float u, float v)
 : pos(Vec3(x, y, z)), u(u), v(v) {}

Vertex Vertex::remap(float u, float v) {
    return Vertex(this, u, v);
}

Vertex::Vertex(Vertex* vertex, float u, float v)
 : pos(vertex->pos), u(u), v(v) {}

Vertex::Vertex(Vec3 pos, float u, float v)
 : pos(pos), u(u), v(v) {}

Vertex::Vertex(void)
 : pos(Vec3(0, 0, 0)), u(0), v(0) {}