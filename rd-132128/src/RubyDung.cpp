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
        std::vector<Zombie> zombies = std::vector<Zombie>();
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
        void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
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

        void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
                this->zombies.push_back(Zombie(this->level, 128.0f, 0.0f, 128.0f));
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
            for (int i = 0; i < this->zombies.size(); i++) {
                this->zombies[i].tick();
            }
            this->player->tick();
        }

        void moveCameraToPlayer(float a) {
            glTranslatef(0.0f, 0.0f, -0.3f);
            glRotatef(this->player->xRot, 1.0f, 0.0f, 0.0f);
            glRotatef(this->player->yRot, 0.0f, 1.0f, 0.0f);

            float x = this->player->xo + (this->player->x - this->player->xo) * a;
            float y = this->player->yo + (this->player->y - this->player->yo) * a;
            float z = this->player->zo + (this->player->z - this->player->zo) * a;

            glTranslatef(-x, -y, -z);
        }
};