#pragma once

#include "base.hpp"
#include "Entity.hpp"
#include "Textures.hpp"
#include "Timer.hpp"
#include "character/Cube.hpp"

class Zombie : public Entity {
    public:
        Cube head;
        Cube body;
        Cube arm0;
        Cube arm1;
        Cube leg0;
        Cube leg1;

        float rot;
        float timeOffs;
        float speed;
        float rotA;

        Zombie(std::shared_ptr<Level>& level, GLFWwindow* window, float x, float y, float z);
        void tick(void);
        void render(float a);
};