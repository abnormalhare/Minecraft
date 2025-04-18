#pragma once

#include "base.hpp"

#include "level/Chunk.hpp"
#include "level/Frustum.hpp"
#include "level/Level.hpp"
#include "level/Tesselator.hpp"
#include "level/DirtyChunkSorter.hpp"
#include "HitResult.hpp"
#include "Player.hpp"
#include "Timer.hpp"

class LevelRenderer: public LevelListener {
    private:
        std::shared_ptr<Level> level;
        std::vector<std::shared_ptr<Chunk>> chunks;
        std::int32_t xChunks;
        std::int32_t yChunks;
        std::int32_t zChunks;

    public:
        static const std::int32_t MAX_REBUILDS_PER_FRAME = 8;
        static const std::int32_t CHUNK_SIZE = 16;

        LevelRenderer(void);
        LevelRenderer(std::shared_ptr<Level>& level);
        std::vector<std::shared_ptr<Chunk>> getAllDirtyChunks();
        void render(std::shared_ptr<Player>& player, std::int32_t layer);
        void updateDirtyChunks(std::shared_ptr<Player>& player);
        void pick(std::shared_ptr<Player>& player);
        void renderHit(std::unique_ptr<HitResult>& h);
        void setDirty(std::int32_t x0, std::int32_t y0, std::int32_t z0, std::int32_t x1, std::int32_t y1, std::int32_t z1);
        void tileChanged(std::int32_t x, std::int32_t y, std::int32_t z) override;
        void lightColumnChanged(std::int32_t x, std::int32_t z, std::int32_t y0, std::int32_t y1) override;
        void allChanged(void) override;
};