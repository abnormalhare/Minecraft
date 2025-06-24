#pragma once

#include "base.hpp"

#include <GLFW/glfw3.h>

#ifdef WIN
#include <windef.h>
#endif

#include <GL/glu.h>
#include "render/Tesselator.hpp"
#include "character/Zombie.hpp"
#include "level/Level.hpp"
#include "level/LevelRenderer.hpp"
#include "level/DirtyChunkSorter.hpp"
#include "particle/ParticleEngine.hpp"
#include "level/tile/Tile.hpp"
#include "gui/Font.hpp"
#include "Timer.hpp"

class Minecraft {
private:
    static const bool FULLSCREEN_MODE = false;
    float fogColor0[4];
    float fogColor1[4];
    Timer timer = Timer(20.0f);
    std::shared_ptr<Level> level = nullptr;
    std::shared_ptr<LevelRenderer> levelRenderer = nullptr;
    std::shared_ptr<Player> player = nullptr;
    int paintTexture = 1;
    std::shared_ptr<ParticleEngine> particleEngine;

    //...
    Textures textureManager;
    Font *font;
    
    std::vector<std::shared_ptr<Zombie>> zombies = std::vector<std::shared_ptr<Zombie>>();
    std::unique_ptr<HitResult> hitResult = nullptr;

    std::int32_t viewportBuffer[16];
    std::uint32_t selectBuffer[2000];

    GLFWwindow* window;
    GLFWcursor* cursor;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

    void setFullscreen(bool isFullscreen);
    float getMouseDX();
    float getMouseDY();

    void moveCameraToPlayer(float a);
    void setupCamera(float a);
    void setupPickCamera(float a, int x, int y);
    void pick(float a);

    void drawGui(UNUSED float a);
    void setupFog(int i);
    float* getBuffer(float a, float b, float c, float d);
    
public:
    int width, height;
    volatile bool pause = false;

    Minecraft() {};
    void mouseButtonCallback(UNUSED GLFWwindow* window, int button, int action, UNUSED int mods);
    void keyCallback(GLFWwindow* window, int key, UNUSED int scancode, int action, UNUSED int mods);
    void init();
    void destroy();
    void run();
    void tick();

    void render(float a);

    void showLoadingScreen(const char* top_text, const char* bottom_text);
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);