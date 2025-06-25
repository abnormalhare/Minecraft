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