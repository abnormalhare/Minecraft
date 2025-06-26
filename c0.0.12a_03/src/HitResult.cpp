#include "HitResult.hpp"

HitResult::HitResult(std::int32_t type, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t f) :
            type(type), x(x), y(y), z(z), f(f) {}

float HitResult::distanceTo(std::shared_ptr<Player>& p, std::int32_t unk) {
    int x = this->x;
    int y = this->y;
    int z = this->z;

    if (unk == 1) {
        if (this->f == 0) --y;
        if (this->f == 1) ++y;
        if (this->f == 2) --z;
        if (this->f == 3) ++z;
        if (this->f == 4) --x;
        if (this->f == 5) ++x;
    }

    float xf = x - this->x;
    float yf = y - this->y;
    float zf = z - this->z;

    return xf * xf + yf * yf + zf * zf;
}