#pragma once

#include "base.hpp"

#include <random>

class LevelGen {
private:
    float rand_float();

public:
    int width, height, depth;
    std::mt19937 random;

    LevelGen(int w, int h, int d);

    void carve(char *blocks);
};