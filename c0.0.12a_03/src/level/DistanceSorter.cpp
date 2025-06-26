#include "level/DistanceSorter.hpp"

std::shared_ptr<Player> dist_player;

void distSorterInit(std::shared_ptr<Player>& p) {
    dist_player = p;
}

bool distCompare(const std::shared_ptr<Chunk>& c0, const std::shared_ptr<Chunk>& c1) {
    return c0->distanceToSqr(dist_player) >= c1->distanceToSqr(dist_player);
}