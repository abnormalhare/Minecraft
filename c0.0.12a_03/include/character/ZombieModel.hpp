#pragma once

#include "character/Cube.hpp"
#include "math.h"

class ZombieModel {
    public:
        Cube head;
        Cube body;
        Cube arm0;
        Cube arm1;
        Cube leg0;
        Cube leg1;

        ZombieModel();

        void render(double time);
};