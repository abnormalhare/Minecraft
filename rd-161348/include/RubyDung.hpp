#pragma once

#include "base.hpp"

#include <GLFW/glfw3.h>

#ifdef WIN
#include <windef.h>
#endif

#include <GL/glu.h>
#include "level/Tesselator.hpp"
#include "character/Zombie.hpp"
#include "level/Level.hpp"
#include "level/LevelRenderer.hpp"
#include "level/DirtyChunkSorter.hpp"
#include "particle/ParticleEngine.hpp"
#include "tile/Tile.hpp"
#include "Timer.hpp"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);