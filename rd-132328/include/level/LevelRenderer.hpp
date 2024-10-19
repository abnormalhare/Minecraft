#pragma once

#include "base.hpp"

#include "level/Chunk.hpp"
#include "level/Frustum.hpp"
#include "level/Level.hpp"
#include "level/Tesselator.hpp"
#include "HitResult.hpp"
#include "Player.hpp"
#include "Timer.hpp"

class LevelRenderer: public LevelListener {
    private:
        static const std::int32_t CHUNK_SIZE = 16;
        Level* level;
        Chunk** chunks;
        std::int32_t xChunks;
        std::int32_t yChunks;
        std::int32_t zChunks;
        Tesselator* t;

    public:
        LevelRenderer(Level* level);
        void render(Player* player, std::int32_t layer);
        void pick(Player* player);
        void renderHit(HitResult* h);
        void setDirty(std::int32_t x0, std::int32_t y0, std::int32_t z0, std::int32_t x1, std::int32_t y1, std::int32_t z1);
        void tileChanged(std::int32_t x, std::int32_t y, std::int32_t z) override;
        void lightColumnChanged(std::int32_t x, std::int32_t z, std::int32_t y0, std::int32_t y1) override;
        void allChanged(void) override;
};