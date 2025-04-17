#pragma once

#include "level/Chunk.hpp"
#include "level/Frustum.hpp"
#include "Player.hpp"
#include "Timer.hpp"

void chunkSorterInit(std::shared_ptr<Player>& p, Frustum* f);
bool chunkCompare(const std::shared_ptr<Chunk>& c0, const std::shared_ptr<Chunk>& c1);