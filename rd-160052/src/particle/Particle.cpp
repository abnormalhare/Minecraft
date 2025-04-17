#include "particle/Particle.hpp"

Particle::Particle(std::shared_ptr<Level>& level, GLFWwindow* window, float x, float y, float z, float xa, float ya, float za, int tex) : Entity(level, window) {
    this->tex = tex;
    this->setSize(0.2f, 0.2f);
    this->heightOffset = this->bbHeight / 2.0f;
    this->setPos(x, y, z);

    this->xd = xa + (randd * 2.0 - 1.0) * 0.4f;
    this->yd = ya + (randd * 2.0 - 1.0) * 0.4f;
    this->zd = za + (randd * 2.0 - 1.0) * 0.4f;
    
    float speed = (random + random + 1.0) * 0.15f;
    float dd = sqrt(this->xd * this->xd + this->yd * this->yd + this->zd * this->zd);
    this->xd = this->xd / dd * speed * 0.7;
    this->yd = this->yd / dd * speed;
    this->zd = this->zd / dd * speed * 0.7;

    this->uo = randd * 3.0f;
    this->vo = randd * 3.0f;
}

void Particle::tick() {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
    if (randd < 0.1) {
        this->remove();
    }

    this->yd -= 0.06;
    this->move(this->xd, this->yd, this->zd);
    
    this->xd *= 0.98f;
    this->yd *= 0.98f;
    this->zd *= 0.98f;
    if (this->onGround) {
        this->xd *= 0.7f;
        this->zd *= 0.7f;
    }
}
