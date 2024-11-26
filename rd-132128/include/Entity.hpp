#pragma once

#include <cmath>
#include <GLFW/glfw3.h>
#include "base.hpp"
#include "level/Level.hpp"
#include "phys/AABB.hpp"

class Entity {
    private:
        std::shared_ptr<Level> level;
        GLFWwindow* window;

        void setPos(float x, float y, float z);
        
    protected:
        float heightOffset = 0.0f;

        void resetPos(void);

    public:
        float xo, yo, zo;
        float x, y, z;
        float xd, yd, zd;
        float yRot, xRot;
        AABB bb;
        bool onGround = false;
        
        Entity(std::shared_ptr<Level>& level, GLFWwindow* window);
        GLFWwindow* getWindow(void);
        void turn(float xo, float yo);
        void tick(void);
        void move(float xa, float ya, float za);
        void moveRelative(float xa, float za, float speed);
};