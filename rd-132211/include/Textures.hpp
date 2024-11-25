#pragma once

#include "base.hpp"
#include <unordered_map>
#include <windef.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

class Textures {
    private:
        static std::unordered_map<std::string, std::int32_t>* idMap;
        static std::int32_t lastId;

    public:
        static std::int32_t loadTexture(const char* resourceName, std::int32_t mode);
        static void bind(std::int32_t id);
};