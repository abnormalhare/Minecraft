#pragma once

#include <GLFW/glfw3.h>
#include "base.hpp"
#include "level/Level.hpp"
#include "render/Tesselator.hpp"
#include "level/tile/Tile.hpp"
#include "phys/AABB.hpp"
#include "Player.hpp"
#include "render/Textures.hpp"
#include "Timer.hpp"

class Chunk {
    private:
        bool dirty;
        int lists;
        static std::shared_ptr<Tesselator> t;
        static std::int64_t totalTime;
        static std::int32_t totalUpdates;

        void rebuild(std::int32_t layer);

    public:
        AABB aabb;
        std::shared_ptr<Level> level;
        const std::int32_t x0, y0, z0;
        const std::int32_t x1, y1, z1;
        const float x, y, z;
        static std::int32_t updates;
        std::int64_t dirtiedTime;

        
        Chunk(std::shared_ptr<Level>& level, int x0, int y0, int z0, int x1, int y1, int z1);
        void rebuild();
        void render(std::int32_t layer);
        void setDirty();
        bool isDirty();
        float distanceToSqr(std::shared_ptr<Player>& player);
        void reset();
};