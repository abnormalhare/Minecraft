#pragma once

#include "base.hpp"
#include <GLFW/glfw3.h>
#include <math.h>

#include "level/Level.hpp"

class Entity {
    private:
        Level* level = nullptr;
        GLFWwindow* window = nullptr;
        void setPos(float x, float y, float z);

    protected:
        float heightOffset = 0.0f;
        void resetPos(void);

    public:
        float xo, yo, zo;
        float x, y, z;
        float xd, yd, zd;
        float yRot, xRot;
        AABB bb = AABB(0, 0, 0, 0, 0, 0);
        bool onGround = false;

        Entity(Level* level, GLFWwindow* window);
        void turn(float xo, float yo);
        void tick(void);
        void move(float xa, float ya, float za);
        void moveRelative(float xa, float za, float speed);
};