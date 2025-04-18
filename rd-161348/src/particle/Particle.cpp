#include "particle/Particle.hpp"

Particle::Particle(std::shared_ptr<Level>& level, GLFWwindow* window, float x, float y, float z, float xa, float ya, float za, int tex) : Entity(level, window) {
    this->tex = tex;
    this->setSize(0.2f, 0.2f);
    this->heightOffset = this->bbHeight / 2.0f;
    this->setPos(x, y, z);

    this->xd = xa + (randd * 2.0 - 1.0) * 0.4f;
    this->yd = ya + (randd * 2.0 - 1.0) * 0.4f;
    this->zd = za + (randd * 2.0 - 1.0) * 0.4f;
    
    float speed = (randd + randd + 1.0) * 0.15f;
    float dd = sqrt(this->xd * this->xd + this->yd * this->yd + this->zd * this->zd);
    this->xd = this->xd / dd * speed * 0.4f;
    this->yd = this->yd / dd * speed * 0.4f + 0.1f;
    this->zd = this->zd / dd * speed * 0.4f;

    this->uo = randd * 3.0f;
    this->vo = randd * 3.0f;

    this->size = randd * 0.5 + 0.5;
    this->lifetime = 4.0 / (randd * 0.9 + 0.1);
    this->age = 0;
}

void Particle::tick() {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
    if (this->age++ >= this->lifetime) {
        this->remove();
    }

    this->yd -= 0.04;
    this->move(this->xd, this->yd, this->zd);
    
    this->xd *= 0.98f;
    this->yd *= 0.98f;
    this->zd *= 0.98f;
    if (this->onGround) {
        this->xd *= 0.7f;
        this->zd *= 0.7f;
    }
}

void Particle::render(std::shared_ptr<Tesselator>& t, float a, float xa, float ya, float za, float xa2, float za2) {
    float u0 = (this->tex % 16 + this->uo / 4.0f) / 16.0f;
    float u1 = u0 + (1.0f / 64.0f);
    float v0 = (this->tex / 16 + this->vo / 4.0f) / 16.0f;
    float v1 = v0 + (1.0f / 64.0f);

    float r = 0.1f * this->size;
    float x = this->xo + (this->x - this->xo) * a;
    float y = this->yo + (this->y - this->yo) * a;
    float z = this->zo + (this->z - this->zo) * a;

    t->vertexUV((x - xa * r) - (xa2 * r), y - ya * r, (z - za * r) - (za2 * r), u0, v1);
    t->vertexUV((x - xa * r) + (xa2 * r), y + ya * r, (z - za * r) + (za2 * r), u0, v0);
    t->vertexUV((x + xa * r) + (xa2 * r), y + ya * r, (z + za * r) + (za2 * r), u1, v0);
    t->vertexUV((x + xa * r) - (xa2 * r), y - ya * r, (x + xa * r) - (xa2 * r), u1, v1);
}