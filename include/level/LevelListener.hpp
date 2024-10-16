#pragma once

#include "base.hpp"

class LevelListener {
    public:
        virtual void tileChanged(std::int32_t paramInt1, std::int32_t paramInt2, std::int32_t paramInt3);
        virtual void lightColumnChanged(std::int32_t paramInt1, std::int32_t paramInt2, std::int32_t paramInt3, std::int32_t paramInt4);
        virtual void allChanged(void);
        bool operator==(const LevelListener& listener);
};