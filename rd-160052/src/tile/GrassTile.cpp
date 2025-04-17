#include "tile/GrassTile.hpp"

GrassTile::GrassTile(int id) : id(id) {
    this->tex = 3;
}

GrassTile::getTexture(int face) {
    if (face == 1) return 0;
    else return face == 0 ? 2 : 3;
}

void GrassTile::tick(std::shared_ptr<Level>& level, int x, int y, int z, std::mt19937 random) {
    if (!level->isLit(x, y, z)) {
        level->setTile(x, y, z, Tile::dirt->id);
    } else {
        for (int i = 0; i < 4; i++) {
            std::uniform_int_distribution<> dist(0, 2);
            int xt = x + dist(this->random) - 1;
            std::uniform_int_distribution<> dist(0, 4);
            int yt = y + dist(this->random) - 3;
            std::uniform_int_distribution<> dist(0, 2);
            int zt = z + dist(this->random) - 1;
            if (level->getTile(xt, yt, zt) == Tile::dirt->id && level->isLit(xt, yt, zt)) {
                level->setTile(xt, yt, zt, Tile::grass->id);
            }
        }
    }
}