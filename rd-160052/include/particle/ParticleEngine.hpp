#pragma once

#include "particle/Particle.hpp"
#include "level/Tesselator.hpp"
#include "Textures.hpp"
#include "Player.hpp"

class ParticleEngine {
    private:
        std::vector<Particle> particles = std::vector<Particle>();

    protected:
        std::shared_ptr<Level> level;

    public:
        ParticleEngine(std::shared_ptr<Level>& level);

        void add(Particle& p);
        void tick();
        void render(std::shared_ptr<Player>& player, float a, int layer);
};