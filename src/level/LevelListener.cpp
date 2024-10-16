#include "level/LevelListener.hpp"

bool LevelListener::operator==(const LevelListener& listener) {
    return (this == &listener);
}


void LevelListener::tileChanged(std::int32_t paramInt1, std::int32_t paramInt2, std::int32_t paramInt3) {}
void LevelListener::lightColumnChanged(std::int32_t paramInt1, std::int32_t paramInt2, std::int32_t paramInt3, std::int32_t paramInt4) {}
void LevelListener::allChanged(void) {}