#pragma once

#include "base.hpp"
#include "Entity.hpp"
#include "render/Textures.hpp"
#include "Timer.hpp"
#include "character/Cube.hpp"
#include "character/ZombieModel.hpp"

class Zombie : public Entity {
    private:
        static ZombieModel zombieModel;
        std::shared_ptr<Textures> textureManager;

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

        Zombie(std::shared_ptr<Level>& level, GLFWwindow* window, std::shared_ptr<Textures> &textures, float x, float y, float z);
        void tick(void);
        void render(float a);
};