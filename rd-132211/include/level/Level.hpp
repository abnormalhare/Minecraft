#pragma once

#include "base.hpp"
#include <vector>
#include <fstream>
#include <algorithm>

#include "level/LevelListener.hpp"
#include "phys/AABB.hpp"

class Level {
    private:
        char* blocks;
        std::int32_t* lightDepths;
        std::vector<LevelListener*> levelListeners = std::vector<LevelListener*>();
        
    public:
        const std::int32_t width;
        const std::int32_t height;
        const std::int32_t depth;

        Level(std::int32_t w, std::int32_t h, std::int32_t d);
        void load(void);
        void save(void);
        void calcLightDepths(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1);
        void addListener(LevelListener* listener);
        void removeListener(LevelListener* listener);
        bool isTile(std::int32_t x, std::int32_t y, std::int32_t z);
        bool isSolidTile(std::int32_t x, std::int32_t y, std::int32_t z);
        bool isLightBlocker(std::int32_t x, std::int32_t y, std::int32_t z);
        std::vector<AABB> getCubes(AABB& aabb);
        float getBrightness(std::int32_t x, std::int32_t y, std::int32_t z);
        void setTile(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type);
};