#include "particle/ParticleEngine.hpp"

ParticleEngine::ParticleEngine(std::shared_ptr<Level>& level) {
    this->level = level;
}

void ParticleEngine::add(Particle& p) {
    this->particles.push_back(p);
}

void ParticleEngine::tick() {
    for (size_t i = 0; i < this->particles.size(); i++) {
        Particle p = this->particles.at(i);
        p.tick();
        if (p.removed) {
            this->particles.erase(std::next(this->particles.begin(), i--));
        }
    }
}

void ParticleEngine::render(std::shared_ptr<Player>& player, float a, int layer) {
    glEnable(GL_TEXTURE_2D);
    int id = Textures::loadTexture("terrain.png", GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, id);

    float xa = -cos(player->yRot * PI / 180.0);
    float za = -sin(player->yRot * PI / 180.0);
    float ya = 1.0f;

    std::shared_ptr<Tesselator> t = Tesselator::instance;
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
    t->init();

    for (size_t i = 0; i < this->particles.size(); i++) {
        Particle p = this->particles.at(i);
        if (p.isLit() ^ (layer == 1)) {
            p.render(t, a, xa, ya, za);
        }
    }

    t->flush();
    glDisable(GL_TEXTURE_2D);
}