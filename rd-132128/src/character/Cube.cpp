#include "character/Cube.hpp"

Cube::Cube(int _xTexOffs, int _yTexOffs)
    : xTexOffs(_xTexOffs), yTexOffs(_yTexOffs) {}

void Cube::setTexOffs(int _xTexOffs, int _yTexOffs) {
    this->xTexOffs = _xTexOffs;
    this->yTexOffs = _yTexOffs;
}

void Cube::addBox(float x0, float y0, float z0, int w, int h, int d) {
    std::vector<Vertex> tempVerts;
    float x1 = x0 + w;
    float y1 = y0 + h;
    float z1 = z0 + d;

    Vertex u0 = Vertex(x0, y0, z0, 0.0f, 0.0f);
    Vertex u1 = Vertex(x1, y0, z0, 0.0f, 8.0f);
    Vertex u2 = Vertex(x1, y1, z0, 8.0f, 8.0f);
    Vertex u3 = Vertex(x0, y1, z0, 8.0f, 0.0f);

    Vertex l0 = Vertex(x0, y0, z1, 0.0f, 0.0f);
    Vertex l1 = Vertex(x1, y0, z1, 0.0f, 8.0f);
    Vertex l2 = Vertex(x1, y1, z1, 8.0f, 8.0f);
    Vertex l3 = Vertex(x0, y1, z1, 8.0f, 0.0f);

    this->vertices[0] = u0;
    this->vertices[1] = u1;
    this->vertices[2] = u2;
    this->vertices[3] = u3;
    this->vertices[4] = l0;
    this->vertices[5] = l1;
    this->vertices[6] = l2;
    this->vertices[7] = l3;

    tempVerts = std::vector<Vertex>{ l1, u1, u2, l2 };
    this->polygons[0] = PolygonRD(tempVerts,
        this->xTexOffs + d + w, this->yTexOffs + d,
        this->xTexOffs + d + w + d, this->yTexOffs + d + h
    );

    tempVerts = std::vector<Vertex>{ u0, l0, l3, u3 };
    this->polygons[1] = PolygonRD(tempVerts,
        this->xTexOffs + 0, this->yTexOffs + d,
        this->xTexOffs + d, this->yTexOffs + d + h
    );

    tempVerts = std::vector<Vertex>{ l1, l0, u0, u1 };
    this->polygons[2] = PolygonRD(tempVerts,
        this->xTexOffs + d, this->yTexOffs + 0,
        this->xTexOffs + d + w, this->yTexOffs + d
    );

    tempVerts = std::vector<Vertex>{ u2, u3, l3, l2 };
    this->polygons[3] = PolygonRD(tempVerts,
        this->xTexOffs + d + w, this->yTexOffs + 0,
        this->xTexOffs + d + w + w, this->yTexOffs + d
    );

    tempVerts = std::vector<Vertex>{ u1, u0, u3, u2 };
    this->polygons[4] = PolygonRD(tempVerts,
        this->xTexOffs + d, this->yTexOffs + d,
        this->xTexOffs + d + w, this->yTexOffs + d + h
    );

    tempVerts = std::vector<Vertex>{ l0, l1, l2, l3 };
    this->polygons[5] = PolygonRD(tempVerts,
        this->xTexOffs + d + w + d, this->yTexOffs + d,
        this->xTexOffs + d + w + d + w, this->yTexOffs + d + h
    );
}

void Cube::setPos(float _x, float _y, float _z) {
    this->x = _x;
    this->y = _y;
    this->z = _z;
}

void Cube::render(void) {
    float c = (180.0 / PI);

    glPushMatrix();
    
    glTranslatef(this->x, this->y, this->z);
    glRotatef(this->zRot * c, 0.0f, 0.0f, 1.0f);
    glRotatef(this->yRot * c, 0.0f, 1.0f, 0.0f);
    glRotatef(this->xRot * c, 1.0f, 0.0f, 0.0f);

    glBegin(7);
    for (size_t i = 0; i < this->polygons.size(); i++) {
        this->polygons[i].render();
    }
    glEnd();

    glPopMatrix();
}