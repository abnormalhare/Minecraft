#pragma once

#include "base.hpp"
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

#include "level/LevelListener.hpp"
#include "phys/AABB.hpp"

class Level : public std::enable_shared_from_this<Level> {
    private:
        static const int TILE_UPDATE_INTERVAL = 400;
        char* blocks;
        std::int32_t* lightDepths;
        std::vector<LevelListener*> levelListeners;
        std::mt19937 random;
        int unprocessed;

        void generateMap();
        
    public:
        const std::int32_t width;
        const std::int32_t height;
        const std::int32_t depth;

        Level(std::int32_t w, std::int32_t h, std::int32_t d);
        bool load();
        void save();
        void calcLightDepths(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1);
        void addListener(LevelListener* listener);
        void removeListener(LevelListener* listener);
        bool isLightBlocker(std::int32_t x, std::int32_t y, std::int32_t z);
        std::vector<std::shared_ptr<AABB>> getCubes(AABB& aabb);
        bool setTile(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type);
        bool isLit(std::int32_t x, std::int32_t y, std::int32_t z);
        int getTile(std::int32_t x, std::int32_t y, std::int32_t z);
        bool isSolidTile(std::int32_t x, std::int32_t y, std::int32_t z);
        void tick();
};