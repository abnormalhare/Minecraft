#pragma once

#include "base.hpp"

#include "timer.hpp"
#include "level/Level.hpp"
#include "level/LevelRenderer.hpp"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);