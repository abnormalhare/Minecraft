#include "level/DirtyChunkSorter.hpp"

DirtyChunkSorter::DirtyChunkSorter(std::shared_ptr<Player>& player, Frustum* frustum) : player(player), frustum(frustum) {}

int DirtyChunkSorter::compare(std::unique_ptr<Chunk>& c0, std::unique_ptr<Chunk>& c1) {
    bool i0 = this->frustum->isVisible(c0->aabb);
    bool i1 = this->frustum->isVisible(c1->aabb);

    if (i0 && !i1) return -1;
    else if (i1 && !i0) return 1;
    else {
        int t0 = (this->now - c0->dirtiedTime) / 2000;
        int t1 = (this->now - c1->dirtiedTime) / 2000;

        if (t0 < t1) return -1;
        else if (t0 > t1) return 1;
        else {
            return c0->distanceToSqr(this->player) < c1->distanceToSqr(this->player) ? -1 : 1;
        }
    }
}