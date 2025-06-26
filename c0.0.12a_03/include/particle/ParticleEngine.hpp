#pragma once

#include "particle/Particle.hpp"
#include "render/Tesselator.hpp"
#include "render/Textures.hpp"
#include "Player.hpp"

class ParticleEngine {
    private:
        std::vector<std::shared_ptr<Particle>> particles;
        std::shared_ptr<Textures> textureManager;

    protected:
        std::shared_ptr<Level> level;

    public:
        ParticleEngine(std::shared_ptr<Level>& level, std::shared_ptr<Textures>& textureManager);

        void add(std::shared_ptr<Particle>& p);
        void tick();
        void render(std::shared_ptr<Player>& player, float a, int layer);
};