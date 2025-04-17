#include "RubyDung.hpp"

class RubyDung {
    private:
        static const bool FULLSCREEN_MODE = false;
        int width, height;
        float fogColor0[4];
        float fogColor1[4];
        Timer timer = Timer(20.0f);
        std::shared_ptr<Level> level = nullptr;
        std::shared_ptr<LevelRenderer> levelRenderer = nullptr;
        std::shared_ptr<Player> player = nullptr;
        int paintTexture = 1;
        std::shared_ptr<ParticleEngine> particleEngine;
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
            if (action != GLFW_PRESS) return;

            if (key == GLFW_KEY_ENTER) {
                this->level->save();
            }
            if (key == GLFW_KEY_1) {
                this->paintTexture = 1;
            }
            if (key == GLFW_KEY_2) {
                this->paintTexture = 3;
            }
            if (key == GLFW_KEY_3) {
                this->paintTexture = 4;
            }
            if (key == GLFW_KEY_4) {
                this->paintTexture = 5;
            }
            if (key == GLFW_KEY_G) {
                this->zombies.push_back(Zombie(this->level, this->player->x, this->player->y, this->player->z));
            }
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                this->level->save();
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }

        void init(void) {
            std::int32_t col0 = 0xFEFBFA;
            std::int32_t col1 = 0x0E0B0A;
            float fr = 0.5f;
            float fg = 0.8f;
            float fb = 1.0f;

            this->fogColor0[0] = ((col0 >> 16) & 0xFF) / 255.0f;
            this->fogColor0[1] = ((col0 >> 8 ) & 0xFF) / 255.0f;
            this->fogColor0[2] = ((col0 >> 0 ) & 0xFF) / 255.0f;
            this->fogColor0[3] = 1.0f;

            this->fogColor1[0] = ((col1 >> 16) & 0xFF) / 255.0f;
            this->fogColor1[1] = ((col1 >> 8 ) & 0xFF) / 255.0f;
            this->fogColor1[2] = ((col1 >> 0 ) & 0xFF) / 255.0f;
            this->fogColor1[3] = 1.0f;

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
            this->particleEngine = std::make_shared<ParticleEngine>(this->level, this->window);

            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            for (int i = 0; i < 10; i++) {
                Zombie zombie = Zombie(this->level, this->window, 128.0f, 0.0f, 128.0f);
                zombie.resetPos();
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
                this->init();
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
            this->level->tick();
            this->particleEngine->tick();

            for (size_t i = 0; i < this->zombies.size(); i++) {
                this->zombies[i].tick();
                if (this->zombies.at(i).removed) {
                    this->zombies.erase(i--);
                }
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

        void setupOrthoCamera() {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0f, this->width, this->height, 0.0f, 100.0f, 300.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(0.0f, 0.0f, -200.0f)
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
            Frustum* frustum = Frustum::getFrustum();
            this->levelRenderer->updateDirtyChunks(this->player);
            this->setupFog(0);
            glEnable(GL_FOG);
            this->levelRenderer->render(this->player, 0);

            for (size_t i = 0; i < this->zombies.size(); i++) {
                Zombie zombie = this->zombies.at(i);
                if (zombie.isLit() && frustum->isVisible(zombie.bb)) {
                    this->zombies[i].render(a);
                }
            }
            
            this->particleEngine->render(this->player, a, 0);
            this->setupFog(1);
            this->levelRenderer->render(this->player, 1);

            for (int ix = 0; ix < this->zombies.size(); ix++) {
                Zombie zombie = this->zombies.at(ix);
                if (!zombie.isLit() && frustum->isVisible(zombie.bb)) {
                    this->zombies.at(i).render(a);
                }
            }
            
            this->particleEngine->render(this->player, a, 1);
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_FOG);
            if (this->hitResult != nullptr) {
                this->levelRenderer->renderHit(this->hitResult);
            }
            
            this->drawGui(a);
            glfwSwapBuffers(this->window);
            glfwPollEvents();
        }

    private:
        void drawGui(float a) {
            glClear(256);
            this->setupOrthoCamera();
            glPushMatrix();
            glTranslatef(this->width - 48, 48.0f, 0.0f);
            
            std::shared_ptr<Tesselator> t = Tesselator::instance;
            glScalef(48.0f, 48.0f, 48.0f);
            glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
            glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(1.5f, -0.5f, -0.5f);

            int id = Textures::loadTexture("terrain.png", GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, id);
            glEnable(GL_TEXTURE_2D);
            t->init();
            Tile::tiles[this->paintTexture]->render(t, this->level, 0, -2, 0, 0);
            t->flush();
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
            int wc = this->width / 2;
            int hc = this->height / 2;
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            t->init();
            t->vertex(wc + 1, hc - 8, 0.0F);
            t->vertex(wc - 0, hc - 8, 0.0F);
            t->vertex(wc - 0, hc + 9, 0.0F);
            t->vertex(wc + 1, hc + 9, 0.0F);
            t->vertex(wc + 9, hc - 0, 0.0F);
            t->vertex(wc - 8, hc - 0, 0.0F);
            t->vertex(wc - 8, hc + 1, 0.0F);
            t->vertex(wc + 9, hc + 1, 0.0F);
            t->flush();
        }

        void setupFog(int i) {
            if (i == 0) {
                glFogi(GL_FOG_MODE, 2048);
                glFogf(GL_FOG_DENSITY, 0.001f);
                glFogfv(GL_FOG_COLOR, this->fogColor0);
                glDisable(GL_LIGHTING);
            } else if (i == 1) {
                glFogi(GL_FOG_MODE, 2048);
                glFogf(GL_FOG_DENSITY, 0.06f);
                glFogfv(GL_FOG_COLOR, this->fogColor1);
                glEnable(GL_LIGHTING);
                glEnable(GL_COLOR_MATERIAL);
                float br = 0.6f;
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, this->getBuffer(br, br, br, 1.0f));
            }
        }

        float* getBuffer(float a, float b, float c, float d) {
            return float[4](a, b, c, d);
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