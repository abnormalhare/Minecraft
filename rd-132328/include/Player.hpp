#pragma once

#include "base.hpp"
#include <windows.h>
#include <GLFW/glfw3.h>
#include <numbers>

#include "level/Level.hpp"
#include "phys/AABB.hpp"
#include "Entity.hpp"

class Player: public Entity {
    private:
        Level* level = nullptr;
        GLFWwindow* window = nullptr;

    protected:
        float heightOffset;

    public:
        Player(Level* level, GLFWwindow* window);
        void tick(void);
};