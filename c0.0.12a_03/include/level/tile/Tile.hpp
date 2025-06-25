#pragma once

#include "base.hpp"

#include <GLFW/glfw3.h>
#include "level/Level.hpp"
#include "render/Tesselator.hpp"
#include "particle/ParticleEngine.hpp"

class Tile {
    private:
        float x0, y0, z0;
        float x1, y1, z1;
        
        bool shouldRenderFace(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t layer);

    protected:
        Tile(std::int32_t id);
        Tile(std::int32_t id, std::int32_t tex);
        void setTicking(bool isTick);
        void setShape(float x0, float y0, float z0, float x1, float y1, float z1);
        virtual std::int32_t getTexture(std::int32_t face);
    
    public:
        enum TileType {
            EMPTY,
            ROCK,
            GRASS,
            DIRT,
            STONE_BRICK,
            WOOD,
            BUSH,
            BEDROCK,
            WATER,
            CALM_WATER,
            LAVA,
            CALM_LAVA,
        };
        
        static Tile* tiles[256];
        static bool shouldTick[256];

        static Tile* empty;
        static Tile* rock;
        static Tile* grass;
        static Tile* dirt;
        static Tile* stoneBrick;
        static Tile* wood;
        static Tile* bush;
        static Tile* bedrock;
        static Tile* water;
        static Tile* calmWater;
        static Tile* lava;
        static Tile* calmLava;

        const std::int32_t id;
        std::int32_t tex;
        
        virtual void render(std::shared_ptr<Tesselator>& t, std::shared_ptr<Level>& level, std::int32_t layer, std::int32_t x, std::int32_t y, std::int32_t z);
        void renderFace(std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face);
        static void renderFaceNoTexture(std::shared_ptr<Player>& p, std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face);
        const std::shared_ptr<AABB> getTileAABB(std::int32_t x, std::int32_t y, std::int32_t z);
        virtual std::shared_ptr<AABB> getAABB(std::int32_t x, std::int32_t y, std::int32_t z);
        virtual bool blocksLight();
        virtual bool isSolid();
        virtual void tick(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::mt19937 random);
        void destroy(std::shared_ptr<Level>& level, GLFWwindow* window, std::int32_t x, std::int32_t y, std::int32_t z, std::shared_ptr<ParticleEngine>& particleEngine);
        virtual int getLiquidType();
        virtual void neighborChanged(Level *level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type);
};