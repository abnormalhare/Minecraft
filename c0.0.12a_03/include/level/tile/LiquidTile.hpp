#pragma once

#include "base.hpp"

#include "level/tile/Tile.hpp"

class LiquidTile : public Tile {
private:
    std::int32_t spreadSpeed;

    bool updateWater(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tick);
    bool checkWater(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tick);

protected:
    std::int32_t liquidType;
    std::int32_t calmTileId;
    std::int32_t tileId;

    LiquidTile(std::int32_t id, std::int32_t tex);
    bool shouldRenderFace(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t id);

public:
    void tick(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tick);
    void renderFace(Tesselator *t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tex);

    bool mayPick();
    std::shared_ptr<AABB> getAABB(std::int32_t x, std::int32_t y, std::int32_t z);
    bool blocksLight();
    bool isSolid();
    int getLiquidType();
    void neighborChanged(Level *level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type);

};