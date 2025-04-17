#pragma once

#include "level/Chunk.hpp"
#include "level/Frustum.hpp"
#include "Player.hpp"
#include "Timer.hpp"

class DirtyChunkSorter {
    private:
        std::shared_ptr<Player>& player;
        Frustum* frustum;
        std::int64_t now;

    public:
        DirtyChunkSorter(std::shared_ptr<Player>& player, Frustum* frustum);
        
        int compare(std::unique_ptr<Chunk>& c0, std::unique_ptr<Chunk>& c1);
};