#include "level/Level.hpp"

Level::Level(std::int32_t w, std::int32_t h, std::int32_t d) : width(w), height(h), depth(d) {
    blocks = new char[w * h * d];
    lightDepths = new std::int32_t[w * h];

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < d; y++) {
            for (int z = 0; z < h; z++) {
                int i = (y * this->height + z) * this->width + x;
                this->blocks[i] = (y <= (d * 2 / 3)) ? 1 : 0;
            }
        }
    }

    calcLightDepths(0, 0, w, h);
    load();
}

void Level::load(void) {
    try {
        std::ifstream dis("level.dat", std::ios::binary);
        dis.read(this->blocks, this->width * this->height * this->depth);
        calcLightDepths(0, 0, this->width, this->height);

        for (size_t i = 0; i < this->levelListeners->size(); i++) {
            this->levelListeners->at(i)->allChanged();
        }
        dis.close();
        
    } catch (std::exception& e) {
        std::cerr << "Failed to load level: " << e.what() << std::endl;
    }
}

void Level::save(void) {
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
                for (size_t i = 0; i < this->levelListeners->size(); i++) {
                    this->levelListeners->at(i)->lightColumnChanged(x, z, yl0, yl1);
                }
            }
        }
    }
}

void Level::addListener(LevelListener* listener) {
    this->levelListeners->push_back(listener);
}

void Level::removeListener(LevelListener* listener) {
    this->levelListeners->erase(std::remove(
        this->levelListeners->begin(),
        this->levelListeners->end(),
        listener
    ));
}

bool Level::isTile(std::int32_t x, std::int32_t y, std::int32_t z) {
    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height) {
        return false;
    } else {
        return this->blocks[(y * this->height + z) * this->width + x] == 1;
    }
}

bool Level::isSolidTile(std::int32_t x, std::int32_t y, std::int32_t z) {
    return isTile(x, y, z);
}

bool Level::isLightBlocker(std::int32_t x, std::int32_t y, std::int32_t z) {
    return isSolidTile(x, y, z);
}

std::vector<AABB>* Level::getCubes(AABB* aabb) {
    std::vector<AABB>* aABBs = new std::vector<AABB>();
    int x0 = (int)aabb->x0;
    int x1 = (int)(aabb->x1 + 1.0f);
    int y0 = (int)aabb->y0;
    int y1 = (int)(aabb->y1 + 1.0f);
    int z0 = (int)aabb->z0;
    int z1 = (int)(aabb->z1 + 1.0f);

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 > this->width) x1 = this->width;
    if (y1 > this->depth) y1 = this->depth;
    if (z1 > this->height) z1 = this->height;

    for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            for (int z = z0; z < z1; z++) {
                if (isSolidTile(x, y, z)) {
                    aABBs->push_back(AABB(
                        x, y, z,
                        x + 1, y + 1, z + 1
                    ));
                }
            }
        }
    }

    return aABBs;
}

float Level::getBrightness(std::int32_t x, std::int32_t y, std::int32_t z) {
    float dark = 0.8f;
    float light = 1.0f;

    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height) {
        return light;
    }
    if (y < this->lightDepths[x + z * this->width]) {
        return dark;
    }
    return light;
}

void Level::setTile(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t type) {
    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height) {
        return;
    }
    
    this->blocks[(y * this->height + z) * this->width + x] = type;
    calcLightDepths(x, z, 1, 1);

    for (size_t i = 0; i < this->levelListeners->size(); i++) {
        this->levelListeners->at(i)->tileChanged(x, y, z);
    }
}