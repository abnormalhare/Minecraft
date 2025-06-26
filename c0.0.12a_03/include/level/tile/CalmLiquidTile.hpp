#pragma once

#include "base.hpp"

#include "level/tile/LiquidTile.hpp"

class CalmLiquidTile : public LiquidTile {
public:
    CalmLiquidTile(std::int32_t id, std::int32_t tex);

    void tick(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::mt19937 random);
    void neighborChanged(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tile);
};