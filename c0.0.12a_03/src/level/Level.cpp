#include "level/Level.hpp"

#include "level/PerlinNoiseFilter.hpp"
#include "level/tile/Tile.hpp"

Level::Level(Minecraft* minecraft, std::int32_t w, std::int32_t h, std::int32_t d) :
    levelListeners(std::vector<LevelListener*>()),
    unprocessed(0),
    width(256), height(256), depth(64)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    this->random = gen;

    this->minecraft = minecraft;
    w = minecraft->width * 240 / minecraft->height;
    h = minecraft->height * 240 / minecraft->height;

    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, h, w, 0.0, 100.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -200.0f);
    minecraft->showLoadingScreen("Loading level", "Reading..");
    
    blocks = new char[this->width * this->height * this->depth];
    lightDepths = new std::int32_t[this->width * this->height];

    bool mapLoaded = this->load();
    if (!mapLoaded) {
        this->generateMap();
    }
}

void Level::generateMap() {
    this->minecraft->showLoadingScreen("Generating level", "Raising..");
    LevelGen *level_gen = new LevelGen(this->width, this->height, this->depth);
    
    int w = level_gen->width;
    int h = level_gen->height;
    int d = level_gen->depth;

    std::vector<int> heightmap1 = PerlinNoiseFilter(level_gen->random, 1, true ).read(w, h);
    std::vector<int> heightmap2 = PerlinNoiseFilter(level_gen->random, 0, true ).read(w, h);
    std::vector<int> cf1        = PerlinNoiseFilter(level_gen->random, 2, false).read(w, h);
    std::vector<int> cf2        = PerlinNoiseFilter(level_gen->random, 4, false).read(w, h);
    std::vector<int> rockMap    = PerlinNoiseFilter(level_gen->random, 1, true ).read(w, h);
    char *blocks    = new char(w * h * d);
    int df = d / 2;

    int dh2;
    int ch2;
    int dh;
    int rh;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < d; y++) {
            for (int z = 0; z < h; z++) {
                int dh1 = heightmap1[x + z * this->width];
                dh2     = heightmap2[x + z * this->width];
                int ch1 = cf1[x + z * this->width];
                ch2     = cf2[x + z * this->width];

                if (ch1 < 128) dh2 = dh1;

                dh = dh1;
                if (dh2 > dh1) dh = dh2;

                dh = (dh - 128) / 8;
                dh = dh + df - 1;
                rh = ((rockMap[x + z * this->width] - 128) / 6 + df + dh) / 2;

                if (ch2 < 92) {
                    dh = (dh / 2) << 1;
                } else if (ch2 < 160) {
                    dh = (dh / 4) << 2;
                }
                
                if (dh < df - 2) rh = (rh - df) / 2 + df;
                if (rh > dh - 2) rh = dh - 2;

                int i = (y * this->height + z) * this->width + x;
                int id = 0;

                if (y == dh && y >= d / 2) id = Tile::grass->id;
                if (y < dh) id = Tile::dirt->id;
                if (y <= rh) id = Tile::rock->id;

                blocks[i] = id;
            }
        }
    }

    this->blocks = blocks;

    this->minecraft->showLoadingScreen("Generating level", "Carving..");
    level_gen->carve(blocks);

    this->minecraft->showLoadingScreen("Generating Level", "Watering..");
    std::int64_t start = Timer::getTimeInNanoSeconds();
    std::int64_t flood = 0;
    std::int32_t water = Tile::calmWater->id;

    for (int i = 0; i < this->width; i++) {
        flood += this->floodFill(i, this->depth / 2 - 1, 0,                0, water);
        flood += this->floodFill(i, this->depth / 2 - 1, this->height - 1, 0, water);
    }

    for (int i = 0; i < this->height; i++) {
        flood += this->floodFill(0,               this->depth / 2 - 1, i, 0, water);
        flood += this->floodFill(this->width - 1, this->depth / 2 - 1, i, 0, water);
    }

    std::int64_t end = Timer::getTimeInNanoSeconds();

    this->minecraft->showLoadingScreen("Generate level", "Melting..");
    int lavaCount = 0;
    for (int i = 0; i < 400; i++) {
        static std::uniform_int_distribution<> dist1(0, this->width);
        int width  = dist1(this->random);
        static std::uniform_int_distribution<> dist2(0, this->depth / 2);
        int depth  = dist2(this->random);
        static std::uniform_int_distribution<> dist3(0, this->height);
        int height = dist3(this->random);

        if (this->getTile(width, depth, height) == 0) {
            lavaCount++;
            flood += this->floodFill(width, depth, height, 0, Tile::calmLava->id);
        }
    }

    std::cout << "LavaCount: " << lavaCount << std::endl;
    std::cout << "Flood filled " << flood << " tiles in " << (end - start) / 1000000.0 << "ms";
    this->calcLightDepths(0, 0, this->width, this->height);

    for (int i = 0; i < this->levelListeners.size(); i++) {
        this->levelListeners[i]->resetChunks();
    }

    delete level_gen;
}

bool Level::load() {
    try {
        std::ifstream dis("level.dat", std::ios::binary);
        if (!dis.is_open()) {
            return false;
        }

        dis.read(this->blocks, this->width * this->height * this->depth);
        dis.close();

        this->calcLightDepths(0, 0, this->width, this->height);

        for (size_t i = 0; i < this->levelListeners.size(); i++) {
            this->levelListeners[i]->resetChunks();
        }

        return true;
    } catch (const std::exception& e) {
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
            for (int y = this->depth - 1; y > 0 && !this->isLightBlocker(x, y, z); y--) {
                Tile *tile = Tile::tiles[this->getTile(x, y, z)];

                if (tile == nullptr ? false : tile->blocksLight()) break; 
            }

            this->lightDepths[x + z * this->width] = y;
            if (oldDepth != y) {
                int yl0 = (oldDepth < y) ? oldDepth : y;
                int yl1 = (oldDepth > y) ? oldDepth : y;

                for (size_t i = 0; i < this->levelListeners.size(); i++) {
                    this->levelListeners[i]->setDirty(x - 1, yl0 - 1, z - 1, x + 1, yl1 + 1, z + 1);
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

std::vector<std::shared_ptr<AABB>> Level::getCubes(AABB& aabb) {
    std::vector<std::shared_ptr<AABB>> aABBs = std::vector<std::shared_ptr<AABB>>();
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
                    std::shared_ptr<AABB> c = tile->getAABB(x, y, z);
                    if (c != nullptr) aABBs.push_back(c);
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
    
    this->updateNeighborAt(x - 1, y, z, type);
    this->updateNeighborAt(x + 1, y, z, type);
    this->updateNeighborAt(x, y - 1, z, type);
    this->updateNeighborAt(x, y + 1, z, type);
    this->updateNeighborAt(x, y, z - 1, type);
    this->updateNeighborAt(x, y, z + 1, type);
    calcLightDepths(x, z, 1, 1);

    for (size_t i = 0; i < this->levelListeners.size(); i++) {
        this->levelListeners.at(i)->setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
    }

    return true;
}

bool Level::setTileNoUpdate(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type) {
    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height) {
        return false;
    }
    
    if (type == this->blocks[(y * this->height + z) * this->width + x]) {
        return false;
    }

    this->blocks[(y * this->height + z) * this->width + x] = type;
    return true;
}

void Level::updateNeighborAt(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type) {
    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height) {
        Tile *tile = Tile::tiles[this->blocks[(y * this->height + z) * this->width + x]];
        if (tile != nullptr) {
            tile->neighborChanged(this, x, y, z, type);
        }
    }
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

bool Level::containsLiquid(AABB *aabb, std::int32_t type) {
    int x0 = floor(aabb->x0 + 0.0f);
    int x1 = floor(aabb->x1 + 1.0f);
    int y0 = floor(aabb->y0 + 0.0f);
    int y1 = floor(aabb->y1 + 1.0f);
    int z0 = floor(aabb->z0 + 0.0f);
    int z1 = floor(aabb->z1 + 1.0f);

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 > this->width ) x1 = this->width;
    if (y1 > this->depth ) y1 = this->depth;
    if (z1 > this->height) z1 = this->height;

    for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            for (int z = z0; z < z1; z++) {
                Tile *tile = Tile::tiles[this->getTile(x, y, z)];
                if (tile != nullptr && tile->getLiquidType() == type) {
                    return true;
                }
            }
        }
    }

    return false;
}

void Level::tick() {
    this->unprocessed = this->unprocessed + this->width * this->height * this->depth;
    int ticks = this->unprocessed / 400;
    this->unprocessed -= ticks * 400;

    for (int i = 0; i < ticks; i++) {
        std::uniform_int_distribution<> dist1(0, this->width);
        int x = dist1(this->random);
        std::uniform_int_distribution<> dist2(0, this->height);
        int y = dist2(this->random);
        std::uniform_int_distribution<> dist3(0, this->depth);
        int z = dist3(this->random);

        Tile* tile = Tile::tiles[this->getTile(x, y, z)];
        if (tile != nullptr) {
            std::shared_ptr<Level> level = this->shared_from_this();
            tile->tick(level, x, y, z, this->random);
        }
    }
}