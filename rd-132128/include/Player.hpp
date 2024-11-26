#pragma once

#include <GLFW/glfw3.h>
#include "base.hpp"
#include "Entity.hpp"
#include "level/Level.hpp"

class Player : public Entity {
    private:
        bool isKeyDown(int key);
    public:
        Player(std::shared_ptr<Level>& level, GLFWwindow* window);
        void tick(void);
};