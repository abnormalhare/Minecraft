#include "Entity.hpp"

Entity::Entity(Level* level, GLFWwindow* window) {
    this->level = level;
    this->window = window;
    resetPos();
}

bool Entity::isKeyDown(int key) {
    int state = glfwGetKey(this->window, key);
    return state == GLFW_PRESS;
}

void Entity::resetPos(void) {
    float x = randf * this->level->width;
    float y = this->level->depth + 10;
    float z = randf * this->level->height;

    setPos(x, y, z);
}

void Entity::setPos(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;

    float w = 0.3f;
    float h = 0.9f;

    this->bb = AABB(x - w, y - h, z - w, x + w, y + h, z + w);
}

void Entity::turn(float xo, float yo) {
    this->yRot += xo * 0.15;
    this->xRot -= yo * 0.15;

    if (this->xRot < -90.0f) this->xRot = -90.0f;
    if (this->xRot > 90.0f) this->xRot = 90.0f;
}

void Entity::tick(void) {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
}

void Entity::move(float xa, float ya, float za) {
    float xaOrg = xa;
    float yaOrg = ya;
    float zaOrg = za;

    std::vector<AABB>* cubes = this->level->getCubes(this->bb.expand(xa, ya, za));

    size_t i;
    for (i = 0; i < cubes->size(); i++) {
        ya = cubes->at(i).clipYCollide(&this->bb, ya);
    }
    this->bb.move(0.0f, ya, 0.0f);
    for (i = 0; i < cubes->size(); i++) {
        xa = cubes->at(i).clipXCollide(&this->bb, xa);
    }
    this->bb.move(xa, 0.0f, 0.0f);
    for (i = 0; i < cubes->size(); i++) {
        za = cubes->at(i).clipZCollide(&this->bb, za);
    }
    this->bb.move(0.0f, 0.0f, za);

    this->onGround = yaOrg != ya && yaOrg < 0.0f;

    if (xaOrg != xa) this->xd = 0.0f;
    if (yaOrg != ya) this->yd = 0.0f;
    if (zaOrg != za) this->zd = 0.0f;

    this->x = (this->bb.x0 + this->bb.x1) / 2.0f;
    this->y = this->bb.y0 + 1.62f;
    this->z = (this->bb.z0 + this->bb.z1) / 2.0f;

    delete cubes;
}

void Entity::moveRelative(float xa, float za, float speed) {
    float dist = xa * xa + za * za;

    if (dist < 0.01f) return;
    dist = speed / sqrt(dist);
    xa *= dist;
    za *= dist;

    float sin = sinf(this->yRot * PI / 180.0f);
    float cos = cosf(this->yRot * PI / 180.0f);

    this->xd += xa * cos - za * sin;
    this->zd += za * cos + xa * sin;
}