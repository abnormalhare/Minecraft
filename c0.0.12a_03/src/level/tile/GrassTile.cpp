#include "level/tile/GrassTile.hpp"

GrassTile::GrassTile(int id) : Tile(id) {
    this->tex = 0;
}

std::int32_t GrassTile::getTexture(std::int32_t face) {
    if (face == 1) return 0;
    else if (face == 0) return 2;
    else return 3;
}

void GrassTile::tick(std::shared_ptr<Level>& level, int x, int y, int z, std::mt19937 random) {
    if (!level->isLit(x, y, z)) {
        level->setTile(x, y, z, Tile::dirt->id);
    } else {
        for (int i = 0; i < 4; i++) {
            std::uniform_int_distribution<> dist1(0, 2);
            int xt = x + dist1(random) - 1;
            std::uniform_int_distribution<> dist2(0, 4);
            int yt = y + dist2(random) - 3;
            std::uniform_int_distribution<> dist3(0, 2);
            int zt = z + dist3(random) - 1;
            if (level->getTile(xt, yt, zt) == Tile::dirt->id && level->isLit(xt, yt, zt)) {
                level->setTile(xt, yt, zt, Tile::grass->id);
            }
        }
    }
}