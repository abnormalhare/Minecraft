#pragma once

#include "Player.hpp"
#include "level/Chunk.hpp"
#include "render/Frustum.hpp"

void distSorterInit(std::shared_ptr<Player>& p);
bool distCompare(const std::shared_ptr<Chunk>& c0, const std::shared_ptr<Chunk>& c1);