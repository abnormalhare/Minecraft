#include "level/Level.hpp"

Level::Level(std::int32_t w, std::int32_t h, std::int32_t d) :
    width(w), height(h), depth(d),
    levelListeners(std::vector<LevelListener*>()),
    unprocessed(0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    this->random = gen;
    
    blocks = new char[w * h * d];
    lightDepths = new std::int32_t[w * h];

    bool mapLoaded = this->load();
    if (!mapLoaded) {
        this->generateMap();
    }

    calcLightDepths(0, 0, w, h);
}

void Level::generateMap() {
    int w = this->width;
    int h = this->height;
    int d = this->depth;

    std::vector<int> heightmap1 = PerlinNoiseFilter(0).read(w, h);
    std::vector<int> heightmap2 = PerlinNoiseFilter(0).read(w, h);
    std::vector<int> cf = PerlinNoiseFilter(1).read(w, h);
    std::vector<int> rockMap = PerlinNoiseFilter(1).read(w, h);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < d; y++) {
            for (int z = 0; z < h; z++) {
                int dh1 = heightmap1[x + z * this->width];
                int dh2 = heightmap2[x + z * this->width];
                int cfh = cf[x + z * this->width];

                if (cfh < 128) dh2 = dh1;

                int dh = dh1;
                if (dh2 > dh1) dh = dh2;

                dh = dh / 8 + d / 3;
                int rh = rockMap[x + z * this->width] / 8 + d / 3;
                if (rh > dh - 2) rh = dh - 2;

                int i = (y * this->height + z) * this->width + x;
                int id = 0;
                if (y == dh) id = Tile::grass->id;
                if (y < dh) id = Tile::dirt->id;
                if (y <= rh) id = Tile::rock->id;

                this->blocks[i] = id;
            }
        }
    }
}

bool Level::load() {
    try {
        std::ifstream dis("level.dat", std::ios::binary);
        dis.read(this->blocks, this->width * this->height * this->depth);
        calcLightDepths(0, 0, this->width, this->height);

        for (size_t i = 0; i < this->levelListeners.size(); i++) {
            this->levelListeners.at(i)->allChanged();
        }
        dis.close();
        return true;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

void Level::save() {
    try {
        std::ofstream dos("level.dat", std::ios::binary);
        dos.write(this->blocks, this->width * this->height * this->depth);
        dos.close();
        
    } catch (std::exception& e) {
        std::cerr << "Failed to save level: " << e.what() << std::endl;
    }
}

void Level::calcLightDepths(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1) {
    for (int x = x0; x < x0 + x1; x++) {
        for (int z = y0; z < y0 + y1; z++) {
            std::int32_t oldDepth = this->lightDepths[x + z * this->width];
            std::int32_t y = this->depth - 1;

            while (y > 0 && !isLightBlocker(x, y, z)) {
                y--;
            }
            this->lightDepths[x + z * this->width] = y;

            if (oldDepth != y) {
                int yl0 = (oldDepth < y) ? oldDepth : y;
                int yl1 = (oldDepth > y) ? oldDepth : y;
                for (size_t i = 0; i < this->levelListeners.size(); i++) {
                    this->levelListeners.at(i)->lightColumnChanged(x, z, yl0, yl1);
                }
            }
        }
    }
}

void Level::addListener(LevelListener* listener) {
    this->levelListeners.push_back(listener);
}

void Level::removeListener(LevelListener* listener) {
    this->levelListeners.erase(std::remove(
        this->levelListeners.begin(),
        this->levelListeners.end(),
        listener
    ));
}

bool Level::isLightBlocker(std::int32_t x, std::int32_t y, std::int32_t z) {
    Tile* tile = Tile::tiles[this->getTile(x, y, z)];
    return tile == nullptr ? false : tile->blocksLight();
}

std::vector<AABB> Level::getCubes(AABB& aabb) {
    std::vector<AABB> aABBs = std::vector<AABB>();
    int x0 = (int)aabb.x0;
    int x1 = (int)(aabb.x1 + 1.0f);
    int y0 = (int)aabb.y0;
    int y1 = (int)(aabb.y1 + 1.0f);
    int z0 = (int)aabb.z0;
    int z1 = (int)(aabb.z1 + 1.0f);

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 > this->width) x1 = this->width;
    if (y1 > this->depth) y1 = this->depth;
    if (z1 > this->height) z1 = this->height;

    for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            for (int z = z0; z < z1; z++) {
                Tile* tile = Tile::tiles[this->getTile(x, y, z)];
                if (tile != nullptr) {
                    aABBs.push_back(tile->getAABB(x, y, z));
                }
            }
        }
    }

    return aABBs;
}

bool Level::setTile(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type) {
    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height) {
        return false;
    }
    
    if (type == this->blocks[(y * this->height + z) * this->width + x]) {
        return false;
    }

    this->blocks[(y * this->height + z) * this->width + x] = type;
    calcLightDepths(x, z, 1, 1);

    for (size_t i = 0; i < this->levelListeners.size(); i++) {
        this->levelListeners.at(i)->tileChanged(x, y, z);
    }

    return true;
}

bool Level::isLit(std::int32_t x, std::int32_t y, std::int32_t z) {
    return x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height ? true : y >= this->lightDepths[x + z * this->width];
}

int Level::getTile(std::int32_t x, std::int32_t y, std::int32_t z) {
    return x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height ? this->blocks[(y * this->height + z) * this->width + x] : 0;
}

bool Level::isSolidTile(std::int32_t x, std::int32_t y, std::int32_t z) {
    Tile* tile = Tile::tiles[this->getTile(x, y, z)];
    return tile == nullptr ? false : tile->isSolid();
}

void Level::tick() {
    this->unprocessed = this->unprocessed + this->width * this->height * this->depth;
    int ticks = this->unprocessed / 400;
    this->unprocessed -= ticks * 400;

    for (int i = 0; i < ticks; i++) {
        std::uniform_int_distribution<> dist(0, this->width);
        int x = dist(this->random);
        std::uniform_int_distribution<> dist(0, this->height);
        int y = dist(this->random);
        std::uniform_int_distribution<> dist(0, this->depth);
        int z = dist(this->random);

        Tile* tile = Tile::tiles[this->getTile(x, y, z)];
        if (tile != nullptr) {
            tile->tick(this, x, y, z, this->random);
        }
    }
}