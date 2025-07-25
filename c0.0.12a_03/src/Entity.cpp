#include "Entity.hpp"

Entity::Entity(std::shared_ptr<Level>& level, GLFWwindow* window) : level(level), window(window) {
    heightOffset = 0.0f;
    bbWidth = 0.6f;
    bbHeight = 1.8f;
    onGround = false;
    removed = false;
    
    resetPos();
}

GLFWwindow* Entity::getWindow(void) {
    return this->window;
}

void Entity::resetPos(void) {
    float x = randd * this->level->width;
    float y = this->level->depth + 10;
    float z = randd * this->level->height;
    setPos(x, y, z);
}

void Entity::remove() {
    this->removed = true;
}

void Entity::setSize(float w, float h) {
    this->bbWidth = w;
    this->bbHeight = h;
}

void Entity::setPos(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    float w = this->bbWidth / 2.0;
    float h = this->bbHeight / 2.0;
    this->bb = AABB(x - w, y - h, z - w, x + w, y + h, z + w);
}

void Entity::turn(float xo, float yo) {
    this->yRot += xo * 0.15;
    this->xRot -= yo * 0.15;

    if (this->xRot < -90.0f) {
        this->xRot = -90.0f;
    }
    if (this->xRot > 90.0f) {
        this->xRot = 90.0f;
    }
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

    AABB aabb = this->bb.expand(xa, ya, za);
    std::vector<std::shared_ptr<AABB>> aabbs = this->level->getCubes(aabb);

    for (size_t i = 0; i < aabbs.size(); i++) {
        ya = aabbs[i]->clipYCollide(this->bb, ya);
    }
    this->bb.move(0.0f, ya, 0.0f);

    for (size_t i = 0; i < aabbs.size(); i++) {
        xa = aabbs[i]->clipXCollide(this->bb, xa);
    }
    this->bb.move(xa, 0.0f, 0.0f);

    for (size_t i = 0; i < aabbs.size(); i++) {
        za = aabbs[i]->clipZCollide(this->bb, za);
    }
    this->bb.move(0.0f, 0.0f, za);

    this->onGround = yaOrg != ya && yaOrg < 0.0f;
    if (xaOrg != xa) this->xd = 0.0f;
    if (yaOrg != ya) this->yd = 0.0f;
    if (zaOrg != za) this->zd = 0.0f;

    this->x = (this->bb.x0 + this->bb.x1) / 2.0f;
    this->y = this->bb.y0 + this->heightOffset;
    this->z = (this->bb.z0 + this->bb.z1) / 2.0f;
}

bool Entity::isInWater() {
    return this->level->containsLiquid(&this->bb, 1);
}

bool Entity::isInLava() {
    return this->level->containsLiquid(&this->bb, 2);
}

void Entity::moveRelative(float xa, float za, float speed) {
    float dist = xa * xa + za * za;
    if (dist < 0.01f) return;
    dist = speed / sqrtf(dist);
    
    xa *= dist;
    za *= dist;

    float _sin = sin(this->yRot * PI / 180.0);
    float _cos = cos(this->yRot * PI / 180.0);

    this->xd += xa * _cos - za * _sin;
    this->zd += za * _cos + xa * _sin;
}

bool Entity::isLit() {
    int xTile = this->x;
    int yTile = this->y;
    int zTile = this->z;
    return this->level->isLit(xTile, yTile, zTile);
}

void Entity::render(UNUSED float a) {}