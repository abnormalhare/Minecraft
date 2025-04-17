#pragma once

#include <random>
#include "tile/Tile.hpp"

class GrassTile : public Tile {
    protected:
        GrassTile(int id);
        int getTexture(int face);
    
    public:
        int tick(std::shared_ptr<Level>& level, int x, int y, int z, std::mt19937 random);
}