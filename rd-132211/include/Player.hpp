#pragma once

#include "base.hpp"
#include <random>
#include <windows.h>
#include <GLFW/glfw3.h>
#include <numbers>

#include "level/Level.hpp"
#include "phys/AABB.hpp"

class Player {
    private:
        Level* level = nullptr;
        GLFWwindow* window = nullptr;
        bool isKeyDown(int key);
        void setPos(float x, float y, float z);

    public:
        float xo, yo, zo;
        float x, y, z;
        float xd, yd, zd;
        float yRot, xRot;
        AABB* bb = nullptr;
        bool onGround = false;

        void resetPos(void);
        Player(Level* level, GLFWwindow* window);
        void turn(float xo, float yo);
        void tick(void);
        void move(float xa, float ya, float za);
        void moveRelative(float xa, float za, float speed);
};