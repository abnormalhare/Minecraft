#pragma once

#include "base.hpp"

#include <GLFW/glfw3.h>
#include "level/Level.hpp"
#include "level/Tesselator.hpp"
#include "particle/ParticleEngine.hpp"

class Tile {
    private:
        bool shouldRenderFace(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t layer);

    protected:
        Tile(std::int32_t id);
        Tile(std::int32_t id, std::int32_t tex);
        virtual std::int32_t getTexture(std::int32_t face);
    
    public:
        
        static Tile* tiles[256];
        static Tile* empty;
        static Tile* rock;
        static Tile* grass;
        static Tile* dirt;
        static Tile* stoneBrick;
        static Tile* wood;

        const std::int32_t id;
        std::int32_t tex;
        
        void render(std::shared_ptr<Tesselator>& t, std::shared_ptr<Level>& level, std::int32_t layer, std::int32_t x, std::int32_t y, std::int32_t z);
        void renderFace(std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face);
        void renderFaceNoTexture(std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face);
        std::shared_ptr<AABB> getAABB(std::int32_t x, std::int32_t y, std::int32_t z);
        bool blocksLight();
        bool isSolid();
        void tick(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::mt19937 random);
        void destroy(std::shared_ptr<Level>& level, GLFWwindow* window, std::int32_t x, std::int32_t y, std::int32_t z, std::shared_ptr<ParticleEngine>& particleEngine);
};