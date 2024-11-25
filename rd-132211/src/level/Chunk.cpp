#include "level/Chunk.hpp"

std::int32_t Chunk::texture = -1;
std::unique_ptr<Tesselator> Chunk::t = std::make_unique<Tesselator>();

std::int32_t Chunk::rebuiltThisFrame = 0;
std::int32_t Chunk::updates = 0;

Chunk::Chunk(std::unique_ptr<Level>& level, int x0, int y0, int z0, int x1, int y1, int z1) : x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1) {
    if (Chunk::texture == -1) {
        Chunk::texture = Textures::loadTexture("terrain.png", GL_NEAREST);
    }
    
    this->level = std::move(level);
    this->aabb = std::make_unique<AABB>(x0, y0, z0, x1, y1, z1);
    this->lists = glGenLists(2);
}

void Chunk::rebuild(std::int32_t layer) {
    if (rebuiltThisFrame == 2) {
        return;
    }
    this->dirty = false;
    updates++;
    rebuiltThisFrame++;

    glNewList(this->lists + layer, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    t->init();
    std::int32_t tiles = 0;
    for (int x = this->x0; x < this->x1; x++) {
        for (int y = this->y0; y < this->y1; y++) {
            for (int z = this->z0; z < this->z1; z++) {
                if (this->level->isTile(x, y, z)) {
                    std::int32_t tex = (y == this->level->depth * 2 / 3) ? 0 : 1;
                    tiles++;

                    if (tex == 0) {
                        Tile::rock->render(t, this->level, layer, x, y, z);
                    } else {
                        Tile::grass->render(t, this->level, layer, x, y, z);
                    }
                }
            }
        }
    }

    t->flush();
    glDisable(GL_TEXTURE_2D);
    glEndList();
}

void Chunk::render(std::int32_t layer) {
    if (this->dirty) {
        rebuild(0);
        rebuild(1);
    }
    glCallList(this->lists + layer);
}

void Chunk::setDirty(void) {
    this->dirty = true;
}