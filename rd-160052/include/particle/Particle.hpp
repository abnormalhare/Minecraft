#pragma once

#include "base.hpp"

#include <GLFW/glfw3.h>
#include "math.h"
#include "Entity.hpp"

class Particle : public Entity {
    private:
        float xd, yd, zd;
        float uo, vo;

    public:
        int tex;

        Particle(std::shared_ptr<Level>& level, GLFWwindow* window, float x, float y, float z, float xa, float ya, float za, int tex);

        void tick();
        void render();
}