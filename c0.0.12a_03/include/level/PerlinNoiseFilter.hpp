#pragma once

#include <random>
#include <vector>

class PerlinNoiseFilter {
    public:
        std::mt19937 random;
        int seed;
        int levels;
        int fuzz;

        PerlinNoiseFilter(int levels);

        std::vector<int> read(int width, int height);
        
};