#pragma once

#include "base.hpp"

#include "level/Level.hpp"
#include "level/Tesselator.hpp"

class Tile {
    private:
        Tile(std::int32_t tex);
    
    public:
        static Tile* rock;
        static Tile* grass;
        std::int32_t tex;
        void render(Tesselator* t, Level* level, std::int32_t layer, std::int32_t x, std::int32_t y, std::int32_t z);
        void renderFace(Tesselator* t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t face);
};