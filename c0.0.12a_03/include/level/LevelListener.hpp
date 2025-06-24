#pragma once

#include "base.hpp"

class LevelListener {
    public:
        virtual void setDirty(std::int32_t x0, std::int32_t y0, std::int32_t z0, std::int32_t x1, std::int32_t y1, std::int32_t z1) = 0;
        virtual void resetChunks() = 0;
        bool operator==(const LevelListener& listener);
};