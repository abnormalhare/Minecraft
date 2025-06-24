#pragma once

#include <random>
#include <vector>

class PerlinNoiseFilter {
    public:
        std::mt19937 random;
        int levels;
        int fuzz;
        bool flag;

        PerlinNoiseFilter(std::mt19937 random, int levels, bool flag);

        std::vector<int> read(int width, int height);
        
};