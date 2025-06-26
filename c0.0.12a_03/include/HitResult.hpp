#pragma once

#include "base.hpp"

#include "Player.hpp"

class HitResult {
    public:
        std::int32_t type;
        std::int32_t x;
        std::int32_t y;
        std::int32_t z;
        std::int32_t f;

        HitResult(std::int32_t type, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t f);
        
        float distanceTo(std::shared_ptr<Player>& p, std::int32_t unk);
};