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

        
    protected:
        float heightOffset;
        float bbWidth, bbHeight;

        void setSize(float w, float h);
        void setPos(float x, float y, float z);

    public:
        float xo, yo, zo;
        float x, y, z;
        float xd, yd, zd;
        float yRot, xRot;
        AABB bb;
        bool onGround = false;
        bool removed = false;
        
        Entity(std::shared_ptr<Level>& level, GLFWwindow* window);
        GLFWwindow* getWindow(void);
        void resetPos();
        void remove();
        void turn(float xo, float yo);
        void tick();
        void move(float xa, float ya, float za);
        void moveRelative(float xa, float za, float speed);
        bool isLit();
};