#include "level/DirtyChunkSorter.hpp"

std::shared_ptr<Player> dirty_chunk_player;
Frustum* frustum;
std::int64_t now;

void chunkSorterInit(std::shared_ptr<Player>& p, Frustum* f) {
    dirty_chunk_player = p;
    frustum = f;
    now = Timer::getTimeInMilliSeconds();
}

bool chunkCompare(const std::shared_ptr<Chunk>& c0, const std::shared_ptr<Chunk>& c1) {
    bool i0 = frustum->isVisible(c0->aabb);
    bool i1 = frustum->isVisible(c1->aabb);

    if (i0 && !i1) return false;
    else if ((!i1 || i0) && c0->distanceToSqr(dirty_chunk_player) < c1->distanceToSqr(dirty_chunk_player)) return false;
    else return true;
}