#include "RubyDung.hpp"

class RubyDung {
    private:
        static const bool FULLSCREEN_MODE = false;
        int width, height;
        float fogColor[4];
        Timer timer = Timer(60.0f);
        std::shared_ptr<Level> level = nullptr;
        std::shared_ptr<LevelRenderer> levelRenderer = nullptr;
        std::shared_ptr<Player> player = nullptr;
        std::vector<std::shared_ptr<Zombie>> zombies = std::vector<std::shared_ptr<Zombie>>();
        std::unique_ptr<HitResult> hitResult = nullptr;

        std::int32_t viewportBuffer[16];
        std::uint32_t selectBuffer[2000];

        GLFWwindow* window;
        GLFWcursor* cursor;

        void setDisplayMode(std::int32_t width, std::int32_t height) {
            if (!glfwInit()) {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                exit(EXIT_FAILURE);
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

            this->window = glfwCreateWindow(width, height, "Game", NULL, NULL);
            if (!this->window) {
                glfwTerminate();
                std::cerr << "Failed to create window" << std::endl;
                exit(EXIT_FAILURE);
            }
            glfwMakeContextCurrent(this->window);

            glfwSetMouseButtonCallback(this->window, mouse_button_callback);
            glfwSetKeyCallback(this->window, key_callback);
        }

        double lastMouseX = 0.0;
        double lastMouseY = 0.0;

        float getMouseDX(void) {
            double xpos, ypos, mouseDX;
            glfwGetCursorPos(this->window, &xpos, &ypos);

            mouseDX = xpos - lastMouseX;
            lastMouseX = xpos;

            return mouseDX;
        }

        float getMouseDY(void) {
            double xpos, ypos, mouseDY;
            glfwGetCursorPos(this->window, &xpos, &ypos);

            ypos = -ypos;
            mouseDY = ypos - lastMouseY;
            lastMouseY = ypos;

            return mouseDY;
        }

    public:
        void mouseButtonCallback(UNUSED GLFWwindow* window, int button, int action, UNUSED int mods) {
            if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
                if (this->hitResult != nullptr) {
                    this->level->setTile(this->hitResult->x, this->hitResult->y, this->hitResult->z, 0);
                }
            }
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                if (this->hitResult != nullptr) {
                    int x = this->hitResult->x;
                    int y = this->hitResult->y;
                    int z = this->hitResult->z;

                    if (this->hitResult->f == 0) y--;
                    if (this->hitResult->f == 1) y++;
                    if (this->hitResult->f == 2) z--;
                    if (this->hitResult->f == 3) z++;
                    if (this->hitResult->f == 4) x--;
                    if (this->hitResult->f == 5) x++;

                    this->level->setTile(x, y, z, 1);
                }
            }
        }

        void keyCallback(GLFWwindow* window, int key, UNUSED int scancode, int action, UNUSED int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                this->level->save();
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }

        void init(void) {
            std::int32_t col = 0x0E0B0A;
            float fr = 0.5f;
            float fg = 0.8f;
            float fb = 1.0f;

            this->fogColor[0] = ((col >> 16) & 0xFF) / 255.0f;
            this->fogColor[1] = ((col >> 8 ) & 0xFF) / 255.0f;
            this->fogColor[2] = ((col >> 0 ) & 0xFF) / 255.0f;
            this->fogColor[3] = 1.0f;

            this->width = 1024;
            this->height = 768;
            this->setDisplayMode(this->width, this->height);
            cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            glfwSetCursor(this->window, this->cursor);

            glEnable(GL_TEXTURE_2D);
            glShadeModel(GL_SMOOTH);
            glClearColor(fr, fg, fb, 0.0f);
            glClearDepth(1.0f);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);

            this->level = std::make_shared<Level>(256, 256, 64);
            this->levelRenderer = std::make_shared<LevelRenderer>(this->level);
            this->player = std::make_shared<Player>(this->level, this->window);

            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            for (int i = 0; i < 100; i++) {
                std::shared_ptr<Zombie> zombie = std::make_shared<Zombie>(this->level, this->window, 128.0f, 0.0f, 128.0f);
                this->zombies.push_back(zombie);
            }
        }

        void destroy(void) {
            this->level->save();

            glfwDestroyCursor(this->cursor);
            glfwDestroyWindow(this->window);
            glfwTerminate();
        }

        void run(void) {
            try {
                init();
            } catch (const std::exception& e) {
                std::cerr << "Failed to start RubyDung" << std::endl;
                exit(EXIT_SUCCESS);
            }

            std::int64_t lastTime = Timer::getTimeInMilliSeconds();
            int frames = 0;

            try {
                while (!glfwWindowShouldClose(this->window)) {
                    this->timer.advanceTime();
                    for (int i = 0; i < this->timer.ticks; i++) {
                        this->tick();
                    }
                    this->render(this->timer.a);
                    frames++;

                    while (Timer::getTimeInMilliSeconds() >= lastTime + 1000) {
                        std::cout << frames << " fps, " << Chunk::updates << std::endl;
                        Chunk::updates = 0;
                        lastTime += 1000;
                        frames = 0;
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
            this->destroy();
        }

        void tick(void) {
            for (size_t i = 0; i < this->zombies.size(); i++) {
                this->zombies[i]->tick();
            }
            this->player->tick();
        }

    private:
        void moveCameraToPlayer(float a) {
            glTranslatef(0.0f, 0.0f, -0.3f);
            glRotatef(this->player->xRot, 1.0f, 0.0f, 0.0f);
            glRotatef(this->player->yRot, 0.0f, 1.0f, 0.0f);

            float x = this->player->xo + (this->player->x - this->player->xo) * a;
            float y = this->player->yo + (this->player->y - this->player->yo) * a;
            float z = this->player->zo + (this->player->z - this->player->zo) * a;

            glTranslatef(-x, -y, -z);
        }

        void setupCamera(float a) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(70.0f, this->width / (double)this->height, 0.05f, 1000.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            moveCameraToPlayer(a);
        }

        void setupPickCamera(float a, int x, int y) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            std::fill(std::begin(this->viewportBuffer), std::end(this->viewportBuffer), 0);
            glGetIntegerv(GL_VIEWPORT, this->viewportBuffer);
            gluPickMatrix(x, y, 5.0f, 5.0f, this->viewportBuffer);
            gluPerspective(70.0f, this->width / (double)this->height, 0.05f, 1000.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            moveCameraToPlayer(a);
        }

        void pick(float a) {
            std::fill(std::begin(this->selectBuffer), std::end(this->selectBuffer), 0);
            glSelectBuffer(2000, this->selectBuffer);
            glRenderMode(GL_SELECT);
            setupPickCamera(a, this->width / 2, this->height / 2);
            levelRenderer->pick(this->player);
            
            int hits = glRenderMode(GL_RENDER);
            std::int64_t closest = 0;
            std::int32_t names[10];
            int hitNameCount = 0;
            int index = 0;
            for (int i = 0; i < hits; i++) {
                int nameCount = this->selectBuffer[index++];
                std::int64_t minZ = this->selectBuffer[index++];
                index++;

                std::int64_t dist = minZ;
                if (dist < closest || i == 0) {
                    closest = dist;
                    hitNameCount = nameCount;
                    for (int j = 0; j < nameCount; j++) {
                        names[j] = this->selectBuffer[index++];
                    }
                } else {
                    index += nameCount;
                }
            }
            if (hitNameCount > 0) {
                this->hitResult = std::make_unique<HitResult>(names[0], names[1], names[2], names[3], names[4]);
            } else {
                this->hitResult = nullptr;
            }
        }

    public:
        void render(float a) {
            float xo = getMouseDX();
            float yo = getMouseDY();
            this->player->turn(xo, yo);
            pick(a);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            setupCamera(a);
            glEnable(GL_CULL_FACE);
            glEnable(GL_FOG);
            glFogi(GL_FOG_MODE, 2048);
            glFogf(GL_FOG_DENSITY, 0.2f);
            glFogfv(GL_FOG_COLOR, this->fogColor);
            glDisable(GL_FOG);
            this->levelRenderer->render(this->player, 0);
            for (size_t i = 0; i < this->zombies.size(); i++) {
                this->zombies[i]->render(a);
            }
            glEnable(GL_FOG);
            this->levelRenderer->render(this->player, 1);
            glDisable(GL_TEXTURE_2D);
            if (this->hitResult != nullptr) {
                this->levelRenderer->renderHit(this->hitResult);
            }
            glDisable(GL_FOG);
            glfwSwapBuffers(this->window);
            glfwPollEvents();
        }
};

RubyDung rubyDung;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    rubyDung.mouseButtonCallback(window, button, action, mods);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    rubyDung.keyCallback(window, key, scancode, action, mods);
}

int main(void) {
    std::srand(std::time({}));
    rubyDung = RubyDung();
    rubyDung.run();
}