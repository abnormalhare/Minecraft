#pragma once

#include "base.hpp"

#include "level/Tile.hpp"
#include "level/Level.hpp"
#include "level/Tesselator.hpp"
#include "phys/AABB.hpp"
#include "Textures.hpp"

class Chunk {
    private:
        bool dirty = true;
        std::int32_t lists = -1;
        static std::int32_t texture;
        static Tesselator* t;

        void rebuild(std::int32_t layer);
    public:
        AABB* aabb;
        Level* level;
        const std::int32_t x0, y0, z0;
        const std::int32_t x1, y1, z1;
        static std::int32_t rebuiltThisFrame;
        static std::int32_t updates;
        
        Chunk(Level* level, int x0, int y0, int z0, int x1, int y1, int z1);
        void render(std::int32_t layer);
        void setDirty(void);
};