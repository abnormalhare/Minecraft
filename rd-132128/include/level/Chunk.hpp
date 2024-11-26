#pragma once

#include <GLFW/glfw3.h>
#include "base.hpp"
#include "level/Level.hpp"
#include "level/Tesselator.hpp"
#include "level/Tile.hpp"
#include "phys/AABB.hpp"
#include "Textures.hpp"

class Chunk {
    private:
        bool dirty = true;
        int lists = -1;
        static std::unique_ptr<Tesselator> t;

        void rebuild(std::int32_t layer);

    public:
        AABB aabb;
        std::shared_ptr<Level> level;
        const std::int32_t x0, y0, z0;
        const std::int32_t x1, y1, z1;
        static std::int32_t rebuiltThisFrame;
        static std::int32_t updates;
        
        Chunk(std::shared_ptr<Level>& level, int x0, int y0, int z0, int x1, int y1, int z1);
        void render(std::int32_t layer);
        void setDirty(void);
};