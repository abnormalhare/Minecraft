#pragma once

#include "base.hpp"

#include <random>
#include "tile/Tile.hpp"
#include "level/Level.hpp"
#include "level/Tesselator.hpp"

class Bush : public Tile {
    public:
        Bush(std::int32_t id);
        void tick(std::shared_ptr<Level>& level, int x, int y, int z, std::mt19937 random) override;
        void render(std::shared_ptr<Tesselator>& t, std::shared_ptr<Level>& level, int layer, int x, int y, int z) override;
        std::shared_ptr<AABB> getAABB(int x, int y, int z) override;
        bool blocksLight() override;
        bool isSolid() override;
};