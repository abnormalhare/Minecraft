#include "level/LevelGen.hpp"

#include "level/tile/Tile.hpp"

LevelGen::LevelGen(int w, int h, int d) {
    std::random_device rd;
    std::mt19937 eng(rd());
    this->random = eng;

    this->width = w;
    this->height = h;
    this->depth = d;
}

float LevelGen::rand_float() {
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(this->random);
}

// this is a guess. it makes sense honestly
void LevelGen::carve(char* blocks) {
    int w = this->width;
    int h = this->height;
    int d = this->depth;

    int layer = w * h * d / 256 / 64;

    UNUSED int var25;
    for (int _idx = 0; _idx < layer; _idx++) {
        float randW = this->rand_float() * w;
        float randD = this->rand_float() * d;
        float randH = this->rand_float() * h;

        float var12 = this->rand_float() + this->rand_float() * 150.0f;

        float randS = this->rand_float() * PI * 2.0;
        float randScomp = 0.0f;
        float randC = this->rand_float() * PI * 2.0;
        float randCcomp = 0.0f;

        int blockIdx;
        for (int i = 0; i < var12; i++) {
            randW += sin(randS) * cos(randC);
            randH += sin(randS) * cos(randC);
            randD += sin(randC);

            randS += randScomp * 0.2f;
            randScomp *= 0.9f;
            randScomp += this->rand_float() - this->rand_float();

            randC += randCcomp * 0.5f;
            randC *= 0.5f;
            randCcomp *= 0.9f;
            randCcomp += this->rand_float() - this->rand_float();

            float radius = sin(i * PI / var12) * 2.5 + 1.0;

            for (int wi = randW - radius; wi <= randW + radius; wi++) {
                for (int di = randD - radius; di <= randD + radius; di++) {
                    for (int hi = randH - radius; hi <= randH + radius; hi++) {
                        float randWI = wi - randW;
                        float randDI = di - randD;
                        float randHI = hi - randH;

                        randHI = randWI * randWI + randDI * randDI * 2.0f + randHI * randHI;
                        if (randHI < radius * radius
                            && wi >= 1 && di >= 1 && hi >= 1
                            && wi < this->width - 1 && di < this->depth - 1 && hi < this->height - 1)
                        {
                            blockIdx = (di * this->height + hi) * this->width + wi;
                            if (blocks[blockIdx] == Tile::rock->id) {
                                blocks[blockIdx] = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}