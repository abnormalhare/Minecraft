#pragma once

#include "base.hpp"
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

#include "level/LevelListener.hpp"
#include "level/LevelGen.hpp"
#include "phys/AABB.hpp"

class Level : public std::enable_shared_from_this<Level> {
    private:
        static const int TILE_UPDATE_INTERVAL = 400;

        std::int32_t* lightDepths;
        std::vector<LevelListener*> levelListeners;
        std::mt19937 random;
        std::int32_t randValue;
        class Minecraft* minecraft;
        int unprocessed;
        int* coords;

        bool load();
        void calcLightDepths(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1);
        void updateNeighborAt(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type);
        std::int64_t floodFill(std::int32_t width, std::int32_t depth, std::int32_t height, UNUSED std::int32_t unk, std::int32_t tile);
        
    public:
        const std::int32_t width;
        const std::int32_t height;
        const std::int32_t depth;
        char* blocks;

        Level(class Minecraft *minecraft, std::int32_t w, std::int32_t h, std::int32_t d);
        void generateMap();
        void save();
        void addListener(LevelListener* listener);
        void removeListener(LevelListener* listener);
        bool isLightBlocker(std::int32_t x, std::int32_t y, std::int32_t z);
        bool setTile(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type);
        bool setTileNoUpdate(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type);
        std::vector<std::shared_ptr<AABB>> getCubes(AABB& aabb);
        bool isLit(std::int32_t x, std::int32_t y, std::int32_t z);
        int getTile(std::int32_t x, std::int32_t y, std::int32_t z);
        bool isSolidTile(std::int32_t x, std::int32_t y, std::int32_t z);
        bool containsLiquid(AABB *aabb, std::int32_t type);
        void tick();
};