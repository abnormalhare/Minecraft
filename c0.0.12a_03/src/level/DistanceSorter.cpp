#include "level/DistanceSorter.hpp"

std::shared_ptr<Player> player;

void distSorterInit(std::shared_ptr<Player>& p) {
    player = p;
}

bool distCompare(const std::shared_ptr<Chunk>& c0, const std::shared_ptr<Chunk>& c1) {
    return c0->distanceToSqr(player) >= c1->distanceToSqr(player);
}