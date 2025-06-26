#include "particle/ParticleEngine.hpp"

ParticleEngine::ParticleEngine(std::shared_ptr<Level>& level, std::shared_ptr<Textures>& textureManager) {
    this->level = level;
    this->particles = std::vector<std::shared_ptr<Particle>>();
    this->textureManager = textureManager;
}

void ParticleEngine::add(std::shared_ptr<Particle>& p) {
    this->particles.push_back(p);
}

void ParticleEngine::tick() {
    for (size_t i = 0; i < this->particles.size(); i++) {
        std::shared_ptr<Particle> p = this->particles.at(i);
        p->tick();
        if (p->removed) {
            this->particles.erase(std::next(this->particles.begin(), i--));
        }
    }
}

void ParticleEngine::render(std::shared_ptr<Player>& player, float a, int layer) {
    if (this->particles.size() == 0) return;
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->textureManager->loadTexture("terrain.png", GL_NEAREST));

    float xa = -cos(player->yRot * PI / 180.0);
    float za = -sin(player->yRot * PI / 180.0);
    float xa2 = -za * sin(player->xRot * PI / 180.0);
    float za2 = xa * sin(player->xRot * PI / 180.0);
    float ya = cos(player->xRot * PI / 180.0);

    std::shared_ptr<Tesselator> t = Tesselator::instance;
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
    t->init();

    for (size_t i = 0; i < this->particles.size(); i++) {
        std::shared_ptr<Particle> p = this->particles.at(i);
        if (p->isLit() ^ (layer == 1)) {
            p->render(t, a, xa, ya, za, xa2, za2);
        }
    }

    t->flush();
    glDisable(GL_TEXTURE_2D);
}