#include "level/PerlinNoiseFilter.hpp"

PerlinNoiseFilter::PerlinNoiseFilter(std::mt19937 random, int levels, bool flag)
    : fuzz(16), random(random), levels(levels), flag(flag) {}

std::vector<int> PerlinNoiseFilter::read(int width, int height) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> tmp = std::vector<int>(width * height);
    int level = this->levels;
    int step = width >> level;

    for (int y = 0; y < height; y += step) {
        for (int x = 0; x < width; x += step) {
            std::uniform_int_distribution<> dist(0,256 - 1);
            tmp[x + y * width] = (dist(gen) - 128) * this->fuzz;

            if (this->flag) {
                if (x != 0 && y != 0) {
                    std::uniform_int_distribution<> dist(0,192 - 1);
                    tmp[x + y * width] = (dist(gen) - 64) * this->fuzz;
                } else {
                    tmp[x + y * width] = 0;
                }
            }
        }
    }

    for (int stepx = width >> level; stepx > 1; stepx /= 2) {
        int val = 256 * stepx >> level;
        int ss = stepx / 2;

        for (int y = 0; y < height; y += stepx) {
            for (int x = 0; x < width; x += stepx) {
                int ul = tmp[(x +     0) % width + ((y +     0) % height) * width];
                int ur = tmp[(x + stepx) % width + ((y +     0) % height) * width];
                int dl = tmp[(x +     0) % width + ((y + stepx) % height) * width];
                int dr = tmp[(x + stepx) % width + ((y + stepx) % height) * width];

                std::uniform_int_distribution<> dist(0, val*2-1);
                int m = (ul + ur + dl + dr) / 4 + dist(gen) - val;
                tmp[x + ss + (y + ss) * width] = m;

                if (this->flag && (x == 0 || y == 0)) {
                    tmp[x + y * width] = 0;
                }
            }
        }

        for (int y = 0; y < height; y += stepx) {
            for (int x = 0; x < width; x += stepx) {
                int c = tmp[x + y * width];
                int r = tmp[(x + stepx) % width + y * width];
                int d = tmp[x + (y + stepx) % width * width];

                int mu = tmp[((x + ss        ) & (width - 1)) + ((y + ss - stepx) & (height - 1)) * width];
                int ml = tmp[((x + ss - stepx) & (width - 1)) + ((y + ss        ) & (height - 1)) * width];
                int m  = tmp[(x + ss         ) % (width     ) + ((y + ss        ) % (height    )) * width];

                std::uniform_int_distribution<> dist(0, val*2-1);
                int u = (c + r + m + mu) / 4 + dist(gen) - val;
                int l = (c + d + m + ml) / 4 + dist(gen) - val;
                tmp[x + ss + y * width] = u;
                tmp[x + (y + ss) * width] = l;
            }
        }
    }

    std::vector<int> result = std::vector<int>(width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[x + y * width] = tmp[x % width + (y % height) * width] / 512 + 128;
        }
    }

    return result;
}