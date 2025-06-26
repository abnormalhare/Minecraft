#include "level/tile/CalmLiquidTile.hpp"

CalmLiquidTile::CalmLiquidTile(std::int32_t id, std::int32_t tex) : LiquidTile(id, tex) {
    this->tileId = id - 1;
    this->calmTileId = id;
    this->setTicking(false);
}

void CalmLiquidTile::tick(UNUSED std::shared_ptr<Level>& level, UNUSED std::int32_t x, UNUSED std::int32_t y, UNUSED std::int32_t z, UNUSED std::mt19937 random) {}

void CalmLiquidTile::neighborChanged(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tile) {
    if (
        level->getTile(x - 1, y, z) == Tile::EMPTY ||
        level->getTile(x + 1, y, z) == Tile::EMPTY ||
        level->getTile(x, y, z - 1) == Tile::EMPTY ||
        level->getTile(x, y, z + 1) == Tile::EMPTY ||
        level->getTile(x, y - 1, z) == Tile::EMPTY)
    {
        level->setTileNoUpdate(x, y, z, this->tileId);
    }

    if (this->liquidType == 1 && tile == Tile::lava->id) {
        level->setTileNoUpdate(x, y, z, Tile::rock->id);
    }

    if (this->liquidType == 2 && tile == Tile::water->id) {
        level->setTileNoUpdate(x, y, z, Tile::rock->id);
    }
}