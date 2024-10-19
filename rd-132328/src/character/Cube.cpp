#include "character/Cube.hpp"

Cube::Cube(std::int32_t xTexOffs, std::int32_t yTexOffs) 
 : xTexOffs(xTexOffs), yTexOffs(yTexOffs) {
}

void Cube::setTexOffs(std::int32_t xTexOffs, std::int32_t yTexOffs) {
    this->xTexOffs = xTexOffs;
    this->yTexOffs = yTexOffs;
}

#define SET_VERTICES(v, v0, v1, v2, v3) v[0] = v0; v[1] = v1; v[2] = v2; v[3] = v3;

void Cube::addBox(float x0, float y0, float z0, std::int32_t w, std::int32_t h, std::int32_t d) {
    float x1 = x0 + w;
    float y1 = y0 + h;
    float z1 = z0 + d;

    Vertex u0 = Vertex( x0, y0, z0, 0.0f, 0.0f );
    Vertex u1 = Vertex( x1, y0, z0, 0.0f, 8.0f );
    Vertex u2 = Vertex( x1, y1, z0, 8.0f, 8.0f );
    Vertex u3 = Vertex( x0, y1, z0, 8.0f, 0.0f );
    Vertex l0 = Vertex( x0, y0, z1, 0.0f, 0.0f );
    Vertex l1 = Vertex( x1, y0, z1, 0.0f, 8.0f );
    Vertex l2 = Vertex( x1, y1, z1, 8.0f, 8.0f );
    Vertex l3 = Vertex( x0, y1, z1, 8.0f, 0.0f );

    this->vertices[0] = u0;
    this->vertices[1] = u1;
    this->vertices[2] = u2;
    this->vertices[3] = u3;
    this->vertices[4] = l0;
    this->vertices[5] = l1;
    this->vertices[6] = l2;
    this->vertices[7] = l3;

    Vertex verts[4] = { l1, u1, u2, l2 };
    this->polygons[0] = PolygonRD(verts, 4, this->xTexOffs + d + w, this->yTexOffs + d, this->xTexOffs + d + w + d, this->yTexOffs + d + h);
    SET_VERTICES(vertices, u0, l0, l3, u3);
    this->polygons[1] = PolygonRD(verts, 4, this->xTexOffs + 0, this->yTexOffs + d, this->xTexOffs + d, this->yTexOffs + d + h);
    SET_VERTICES(vertices, l1, l0, u0, u1);
    this->polygons[2] = PolygonRD(verts, 4, this->xTexOffs + d, this->yTexOffs + 0, this->xTexOffs + d + w, this->yTexOffs + d);
    SET_VERTICES(vertices, u2, u3, l3, l2);
    this->polygons[3] = PolygonRD(verts, 4, this->xTexOffs + d + w, this->yTexOffs + 0, this->xTexOffs + d + w + w, this->yTexOffs + d);
    SET_VERTICES(vertices, u1, u0, u3, u2);
    this->polygons[4] = PolygonRD(verts, 4, this->xTexOffs + d, this->yTexOffs + d, this->xTexOffs + d + w, this->yTexOffs + d + h);
    SET_VERTICES(vertices, l0, l1, l2, l3);
    this->polygons[5] = PolygonRD(verts, 4, this->xTexOffs + d + w + d, this->yTexOffs + d, this->xTexOffs + d + w + d + w, this->yTexOffs + d + h);
}

void Cube::setPos(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Cube::render(void) {
    float c = 180.0f / PI;

    glPushMatrix();
    glTranslatef(this->x, this->y, this->z);
    glRotatef(this->zRot * c, 0.0f, 0.0f, 1.0f);
    glRotatef(this->yRot * c, 0.0f, 1.0f, 0.0f);
    glRotatef(this->xRot * c, 1.0f, 0.0f, 0.0f);

    glBegin(7);
    for (int i = 0; i < 6; i++)  { // length of polygons
        this->polygons[i].render();
    }
    glEnd();
    
    glPopMatrix();
}