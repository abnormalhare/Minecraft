#include "level/DirtyChunkSorter.hpp"

std::shared_ptr<Player> player;
Frustum* frustum;
std::int64_t now;

void chunkSorterInit(std::shared_ptr<Player>& p, Frustum* f) {
    player = p;
    frustum = f;
    now = Timer::getTimeInMilliSeconds();
}

bool chunkCompare(const std::shared_ptr<Chunk>& c0, const std::shared_ptr<Chunk>& c1) {
    bool i0 = frustum->isVisible(c0->aabb);
    bool i1 = frustum->isVisible(c1->aabb);

    if (i0 && !i1) return false;
    else if (i1 && !i0) return true;
    else {
        int t0 = (now - c0->dirtiedTime) / 2000;
        int t1 = (now - c1->dirtiedTime) / 2000;

        if (t0 < t1) return false;
        else if (t0 > t1) return true;
        else {
            return c0->distanceToSqr(player) < c1->distanceToSqr(player);
        }
    }
}