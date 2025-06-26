#include "level/tile/LiquidTile.hpp"

LiquidTile::LiquidTile(std::int32_t id, std::int32_t tex) : Tile(id) {
    this->liquidType = tex;
    this->tex = 14;
    if (tex == 2) {
        this->tex = 30;
        this->spreadSpeed = 2;
    } else if (tex == 1) {
        this->spreadSpeed = 8;
    }

    this->tileId = id;
    this->calmTileId = id + 1;

    float one_tenth = 0.1f;
    this->setShape(0.0f, 0.0f - one_tenth, 0.0f, 1.0f, 1.0f - one_tenth, 1.0f);
    this->setTicking(true);
}

void LiquidTile::tick(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::mt19937 random) {
    this->updateWater(level, x, y, z, 0);
}

bool LiquidTile::updateWater(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tick) {
    bool setCalmWaterTile = false;
    bool tileIsSet = false;

    while (tileIsSet && this->liquidType != 2) {
        if (level->getTile(x, --y, z) != 0) break;
        
        tileIsSet = level->setTile(x, y, z, this->tileId);
        if (tileIsSet) {
            setCalmWaterTile = true;
        }
    }

    y++;
    if (this->liquidType == 1 || !setCalmWaterTile) {
        setCalmWaterTile |= this->checkWater(level, x - 1, y, z, tick);
        setCalmWaterTile |= this->checkWater(level, x + 1, y, z, tick);
        setCalmWaterTile |= this->checkWater(level, x, y, z - 1, tick);
        setCalmWaterTile |= this->checkWater(level, x, y, z + 1, tick);
    }

    if (!setCalmWaterTile) {
        level->setTileNoUpdate(x, y, z, this->calmTileId);
    }

    return setCalmWaterTile;
}

bool LiquidTile::checkWater(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tick) {
    bool waterUpdated = false;

    int tile = level->getTile(x, y, z);
    if (tile == 0) {
        bool tileIsSet = level->setTile(x, y, z, this->tileId);

        if (tileIsSet && tick < this->spreadSpeed) {
            waterUpdated = this->updateWater(level, x, y, z, tick + 1);
        }
    }

    return waterUpdated;
}

bool LiquidTile::shouldRenderFace(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t id) {
    if (id != 2) {
        return false;
    } else {
        id = level->getTile(x, y, z);
        if (id != this->tileId && id != this->calmTileId) {
            return this->Tile::shouldRenderFace(level, x, y, z, -1);
        } else {
            return false;
        }
    }
}

void LiquidTile::renderFace(std::shared_ptr<Tesselator>& t, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tex) {
    this->Tile::renderFace(t, x, y, z, tex);
    this->Tile::renderBackFace(t, x, y, z, tex);
}

bool LiquidTile::mayPick() {
    return false;
}

std::shared_ptr<AABB> LiquidTile::getAABB(UNUSED std::int32_t x, UNUSED std::int32_t y, UNUSED std::int32_t z) {
    return nullptr;
}

bool LiquidTile::blocksLight() {
    return true;
}

bool LiquidTile::isSolid() {
    return false;
}

int LiquidTile::getLiquidType() {
    return this->liquidType;
}

void LiquidTile::neighborChanged(std::shared_ptr<Level>& level, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t tile) {
    if (this->liquidType == 1 && tile == Tile::lava->id) {
        level->setTileNoUpdate(x, y, z, Tile::rock->id);
    }

    if (this->liquidType == 2 && tile == Tile::water->id) {
        level->setTileNoUpdate(x, y, z, Tile::rock->id);
    }
}