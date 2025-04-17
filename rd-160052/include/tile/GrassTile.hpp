#pragma once

#include <random>
#include "tile/Tile.hpp"

class GrassTile : public Tile {
    protected:
        std::int32_t getTexture(std::int32_t face) override;
    
    public:
        GrassTile(int id);
        
        void tick(std::shared_ptr<Level>& level, int x, int y, int z, std::mt19937 random);
};