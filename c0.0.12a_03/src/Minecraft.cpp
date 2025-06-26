#include "Minecraft.hpp"

#include "character/Zombie.hpp"

Minecraft::Minecraft(std::int32_t width, std::int32_t height, bool fullscreen) {
    this->width = width;
    this->height = height;
    this->fullscreen = fullscreen;
    this->textureManager = std::shared_ptr<Textures>();
}

void Minecraft::update() {
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

void Minecraft::setFullscreen(bool isFullscreen, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!this->width || !this->height) {
        const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
        this->width = vidMode->width;
        this->height = vidMode->height;
    }

    if (isFullscreen) {
        this->window = glfwCreateWindow(this->width, this->height, title, monitor, NULL);
    } else {
        this->window = glfwCreateWindow(this->width, this->height, title, NULL, NULL);
    }
    if (!this->window) {
        glfwTerminate();
        std::cerr << "Failed to create window" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->window);

    glfwSetMouseButtonCallback(this->window, mouse_button_callback);
    glfwSetKeyCallback(this->window, key_callback);
}

float Minecraft::getMouseDX(void) {
    double xpos, ypos, mouseDX;
    glfwGetCursorPos(this->window, &xpos, &ypos);

    mouseDX = xpos - this->lastMouseX;
    this->lastMouseX = xpos;

    return mouseDX;
}

float Minecraft::getMouseDY(void) {
    double xpos, ypos, mouseDY;
    glfwGetCursorPos(this->window, &xpos, &ypos);

    ypos = -ypos;
    mouseDY = ypos - this->lastMouseY;
    this->lastMouseY = ypos;

    return mouseDY;
}

void Minecraft::grabMouse() {
    if (!this->mouseGrabbed) {
        this->mouseGrabbed = true;
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void Minecraft::releaseMouse() {
    if (!this->mouseGrabbed) {
        this->mouseGrabbed = false;
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Minecraft::mouseButtonCallback(UNUSED GLFWwindow* window, int button, int action, UNUSED int mods) {
    if (!this->mouseGrabbed) {
        this->grabMouse();
        return;
    }
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (this->editMode == 0) {
            if (this->hitResult != nullptr) {
                Tile* oldTile = Tile::tiles[this->level->getTile(this->hitResult->x, this->hitResult->y, this->hitResult->z)];
                bool changed = this->level->setTile(this->hitResult->x, this->hitResult->y, this->hitResult->z, 0);
                if (oldTile != nullptr && changed) {
                    oldTile->destroy(this->level, this->window, this->hitResult->x, this->hitResult->y, this->hitResult->z, this->particleEngine);
                } 
            }
            return;
        }
        
        if (this->hitResult != nullptr) {
            int x = this->hitResult->x;
            int y = this->hitResult->y;
            int z = this->hitResult->z;
            size_t entity;

            if (this->hitResult->f == 0) y--;
            if (this->hitResult->f == 1) y++;
            if (this->hitResult->f == 2) z--;
            if (this->hitResult->f == 3) z++;
            if (this->hitResult->f == 4) x--;
            if (this->hitResult->f == 5) x++;

            std::shared_ptr<AABB> c = Tile::tiles[this->paintTexture]->getAABB(x, y, z);
            if (c != nullptr) {
                // this is 100% its own function, but i dont care enough to find where it should go
                UNUSED bool retValue = false;
                if (this->player->bb.intersects(*c)) {
                    retValue = false;
                } else {
                    entity = 0;
                    while (true) {
                        if (entity >= this->entities.size()) {
                            retValue = true;
                            break;
                        }

                        if (this->entities[entity].bb.intersects(*c)) {
                            retValue = false;
                            break;
                        }

                        entity++;
                    }
                }
            }

            this->level->setTile(x, y, z, this->paintTexture);
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        this->editMode = ~this->editMode;
    }
}

void Minecraft::keyCallback(UNUSED GLFWwindow* window, int key, UNUSED int scancode, int action, UNUSED int mods) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE && !this->fullscreen) {
        this->releaseMouse();
    }

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
    if (key == GLFW_KEY_6) {
        this->paintTexture = 6;
    }
    if (key == GLFW_KEY_Y) {
        this->yMouseAxis = -this->yMouseAxis;
    }
    if (key == GLFW_KEY_G) {
        std::shared_ptr<Zombie> zombie = std::make_shared<Zombie>(this->level, this->window, this->textureManager, this->player->x, this->player->y, this->player->z);
        this->zombies.push_back(zombie);
    }
    if (key == GLFW_KEY_N) {
        this->level->generateMap();
        this->player->resetPos();

        while (this->entities.size() > 0) {
            this->entities.erase(this->entities.begin());
        }
    }
    if (key == GLFW_KEY_F) {
        this->levelRenderer->drawDistance = (this->levelRenderer->drawDistance + 1) % 4;
    }
}

void Minecraft::reportGLError(const char* error) {
    GLenum err = glGetError();
    if (err != 0) {
        const GLubyte* errorStr = gluErrorString(err);
        
        std::cout << "########## GL ERROR ##########" << std::endl
                  << "@ " << error << std::endl
                  << err << ": " << errorStr << std::endl;
        exit(EXIT_SUCCESS);
    }
}

void Minecraft::destroy(void) {
    this->level->save();

    glfwDestroyCursor(this->cursor);
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Minecraft::init(void) {
    UNUSED std::int32_t col0 = 0xFEFBFA;
    std::int32_t col1 = 0x0E0B0A;

    float fr = 0.5f;
    float fg = 0.8f;
    float fb = 1.0f;

    this->fogColor0[0] = fr;
    this->fogColor0[1] = fg;
    this->fogColor0[2] = fb;
    this->fogColor0[3] = 1.0f;

    this->fogColor1[0] = ((col1 >> 16) & 0xFF) / 255.0f;
    this->fogColor1[1] = ((col1 >> 8 ) & 0xFF) / 255.0f;
    this->fogColor1[2] = ((col1 >> 0 ) & 0xFF) / 255.0f;
    this->fogColor1[3] = 1.0f;

    this->width = this->height = 0;
    if (!this->fullscreen) {
        this->setFullscreen(false, "Minecraft 0.0.12a_03");
    } else {
        this->setFullscreen(true, "Minecraft 0.0.12a_03");
    }
    this->cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    glfwSetCursor(this->window, this->cursor);

    this->reportGLError("Pre startup");

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(fr, fg, fb, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    reportGLError("Startup");

    this->font = new Font("default.gif", this->textureManager);
    
    glViewport(0, 0, this->width, this->height);
    this->level = std::make_shared<Level>(this, 256, 256, 64);
    this->levelRenderer = std::make_shared<LevelRenderer>(this->level, this->textureManager);
    this->player = std::make_shared<Player>(this->level, this->window);
    this->particleEngine = std::make_shared<ParticleEngine>(this->level, this->textureManager);

    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    for (int i = 0; i < 10; i++) {
        std::shared_ptr<Zombie> zombie = std::make_shared<Zombie>(this->level, this->window, this->textureManager, 128.0f, 0.0f, 128.0f);
        zombie->resetPos();
        this->zombies.push_back(zombie);
    }

    reportGLError("Post startup");
}

void Minecraft::run(void) {
    this->running = true;

    try {
        this->init();
    } catch (const std::exception& e) {
        std::cerr << "Failed to start Minecraft" << std::endl;
        exit(EXIT_SUCCESS);
    }

    std::int64_t lastTime = Timer::getTimeInMilliSeconds();
    int frames = 0;

    try {
        while (this->running) {
            if (this->pause) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            if (!glfwWindowShouldClose(this->window)) {
                this->running = false;
            }

            this->timer.advanceTime();
            for (int i = 0; i < this->timer.ticks; i++) {
                this->tick();
            }

            reportGLError("Pre render");
            this->render(this->timer.a);
            reportGLError("Post render");

            frames++;
            while (Timer::getTimeInMilliSeconds() >= lastTime + 1000) {
                if (this->fpsString != nullptr) delete this->fpsString;
                char *buffer = new char[0x100];
                sprintf(buffer, "%d fps, %d chunk updates", frames, Chunk::updates);
                this->fpsString = buffer;

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

void Minecraft::tick(void) {
    this->level->tick();
    this->particleEngine->tick();

    // for (size_t i = 0; i < this->zombies.size(); i++) {
    //     this->zombies[i]->tick();
    //     if (this->zombies.at(i)->removed) {
    //         this->zombies.erase(std::next(zombies.begin(), i--));
    //     }
    // }
    
    this->player->tick();
}

void Minecraft::moveCameraToPlayer(float a) {
    glTranslatef(0.0f, 0.0f, -0.3f);
    glRotatef(this->player->xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(this->player->yRot, 0.0f, 1.0f, 0.0f);

    float x = this->player->xo + (this->player->x - this->player->xo) * a;
    float y = this->player->yo + (this->player->y - this->player->yo) * a;
    float z = this->player->zo + (this->player->z - this->player->zo) * a;

    glTranslatef(-x, -y, -z);
}

void Minecraft::setupCamera(float a) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f, this->width / (double)this->height, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->moveCameraToPlayer(a);
}

void Minecraft::setupPickCamera(float a, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    std::fill(std::begin(this->viewportBuffer), std::end(this->viewportBuffer), 0);
    glGetIntegerv(GL_VIEWPORT, this->viewportBuffer);
    gluPickMatrix(x, y, 5.0f, 5.0f, this->viewportBuffer);
    gluPerspective(70.0f, this->width / (double)this->height, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->moveCameraToPlayer(a);
}

void Minecraft::pick(float a) {
    std::fill(std::begin(this->selectBuffer), std::end(this->selectBuffer), 0);
    glSelectBuffer(2000, this->selectBuffer);
    glRenderMode(GL_SELECT);
    this->setupPickCamera(a, this->width / 2, this->height / 2);
    this->levelRenderer->pick(this->player, Frustum::getFrustum());
    
    int hits = glRenderMode(GL_RENDER);
    std::int32_t names[10];
    std::shared_ptr<HitResult> result;
    int index = 0;
    // change because vineflower
    for (int i = 0; i < hits; i++) {
        int nameCount = this->selectBuffer[index++];
        UNUSED std::int64_t minZ = this->selectBuffer[index++];
        index++;
        
        for (int j = 0; j < nameCount; j++) {
            names[j] = this->selectBuffer[index++];
        }

        this->hitResult = std::make_shared<HitResult>(names[0], names[1], names[2], names[3], names[4]);
        if (result != nullptr) {
            if (this->hitResult->distanceTo(this->player, this->editMode) >= result->distanceTo(this->player, this->editMode)) {
                continue;
            }
        }

        result = this->hitResult;
    }
}

void Minecraft::render(float a) {
    if (glfwGetWindowAttrib(this->window, GLFW_FOCUSED)) {
        this->releaseMouse();
    }

    glViewport(0, 0, this->width, this->height);

    UNUSED float yaxis;
    if (this->mouseGrabbed) {
        float xo = getMouseDX();
        float yo = getMouseDY();
        yaxis = yo * this->yMouseAxis;
        this->player->turn(xo, yo);
    }

    this->reportGLError("Set viewport");
    this->pick(a);
    this->reportGLError("Picked");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->setupCamera(a);
    this->reportGLError("Set up camera");

    glEnable(GL_CULL_FACE);
    UNUSED Frustum* frustum = Frustum::getFrustum();
    this->levelRenderer->updateDirtyChunks(this->player);
    this->reportGLError("Update chunks");
    
    this->setupFog(0);
    glEnable(GL_FOG);
    this->levelRenderer->render(this->player, 0);
    this->reportGLError("Rendered level");

    // for (size_t i = 0; i < this->zombies.size(); i++) {
    //     std::shared_ptr<Zombie> zombie = this->zombies.at(i);
    //     if (zombie->isLit() && frustum->isVisible(zombie->bb)) {
    //         this->zombies[i]->render(a);
    //     }
    // }
    this->reportGLError("Rendered entities");
    
    this->particleEngine->render(this->player, a, 0);
    this->reportGLError("Rendered particles");

    this->setupFog(1);
    this->levelRenderer->render(this->player, 1);

    // for (size_t ix = 0; ix < this->zombies.size(); ix++) {
    //     std::shared_ptr<Zombie> zombie = this->zombies.at(ix);
    //     if (!zombie->isLit() && frustum->isVisible(zombie->bb)) {
    //         this->zombies.at(ix)->render(a);
    //     }
    // }
    
    this->particleEngine->render(this->player, a, 1);
    this->levelRenderer->renderSurround(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->setupFog(0);
    this->levelRenderer->renderSurround(1);
    glEnable(GL_BLEND);
    glDepthMask(false);
    this->levelRenderer->render(this->player, 2);
    glDepthMask(true);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    this->reportGLError("Rendered rest");
    
    if (this->hitResult != nullptr) {
        glDisable(GL_ALPHA_TEST);
        this->levelRenderer->renderHit(this->player, this->hitResult, this->editMode, this->paintTexture);
        glEnable(GL_ALPHA_TEST);
    }
    this->reportGLError("Rendered hit");

    this->drawGui(a);

    this->update();
}

void Minecraft::drawGui(UNUSED float a) {
    int screenWidth = this->width * 240 / this->height;
    int screenHeight = this->height * 240 / this->height;
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, screenWidth, screenHeight, 0.0, 100.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -200.0f);
    reportGLError("GUI: Init");

    glPushMatrix();
    glTranslatef(screenWidth - 16, 16.0f, 0.0f);
    
    std::shared_ptr<Tesselator> t = Tesselator::instance;
    glScalef(16.0f, 16.0f, 16.0f);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-1.5f, 0.5f, -0.5f);
    glScalef(-1.0f, -1.0f, 1.0f);

    int id = this->textureManager->loadTexture("terrain.png", GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, id);
    glEnable(GL_TEXTURE_2D);
    t->init();
    Tile::tiles[this->paintTexture]->render(t, this->level, 0, -2, 0, 0);
    t->flush();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    this->reportGLError("GUI: Draw selected");

    this->font->drawShadow("0.0.12a_03", 2, 2, 0xFFFFFF00);
    if (this->fpsString) {
        this->font->drawShadow(this->fpsString, 2, 12, 0xFFFFFF00);
    }
    this->reportGLError("GUI: Draw text");

    int wc = screenWidth / 2;
    int hc = screenHeight / 2;
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    t->init();
    t->vertex(wc + 1, hc - 4, 0.0F);
    t->vertex(wc - 0, hc - 4, 0.0F);
    t->vertex(wc - 0, hc + 5, 0.0F);
    t->vertex(wc + 1, hc + 5, 0.0F);
    t->vertex(wc + 5, hc - 0, 0.0F);
    t->vertex(wc - 4, hc - 0, 0.0F);
    t->vertex(wc - 4, hc + 1, 0.0F);
    t->vertex(wc + 5, hc + 1, 0.0F);
    t->flush();
    this->reportGLError("GUI: Draw crosshair");
    this->reportGLError("Rendered gui");
}

void Minecraft::setupFog(int i) {
    Tile *tile = Tile::tiles[this->level->getTile(this->player->x, this->player->y, this->player->z)];

    if (tile != nullptr && tile->getLiquidType() == 1) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.1f);
        glFogfv(GL_FOG_COLOR, this->getBuffer(0.02f, 0.02f, 0.2f, 1.0f));
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, this->getBuffer(0.3f, 0.3f, 0.5f, 1.0f));
    } else if (tile != nullptr && tile->getLiquidType() == 2) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.2f);
        glFogfv(GL_FOG_COLOR, this->getBuffer(0.5f, 0.3f, 0.0f, 1.0f));
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, this->getBuffer(0.4f, 0.3f, 0.3f, 1.0f));
    } else if (i == 0) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.001f);
        glFogfv(GL_FOG_COLOR, this->fogColor0);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, this->getBuffer(1.0f, 1.0f, 1.0f, 1.0f));
    } else if (i == 1) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.01f);
        glFogfv(GL_FOG_COLOR, this->fogColor1);
        float br = 0.6f;
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, this->getBuffer(br, br, br, 1.0f));
    }

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glEnable(GL_LIGHTING);
}

float* Minecraft::getBuffer(float a, float b, float c, float d) {
    float* temp = new float[4]{a, b, c, d};
    return temp;
}

void Minecraft::showLoadingScreen(const char* top_text, const char* bottom_text) {
    int u = this->width * 240 / this->height;
    int v = this->height * 240 / this->height;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    std::shared_ptr<Tesselator> t = Tesselator::instance;
    glEnable(GL_TEXTURE_2D);

    int dirtTex = this->textureManager->loadTexture("dirt.png", GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, dirtTex);

    t->init();
    t->color(0x808080);

    float texSize = 32.0f;
    t->vertexUV(0.0f, v,    0.0f, 0.0f,        v / texSize);
    t->vertexUV(u,    v,    0.0f, u / texSize, v / texSize);
    t->vertexUV(u,    0.0f, 0.0f, u / texSize, 0.0f);
    t->vertexUV(0.0f, 0.0f, 0.0f, 0.0f,        0.0f);
    t->flush();

    glEnable(GL_TEXTURE_2D);
    this->font->drawShadow(top_text,    (u - this->font->getWidth(top_text))    / 2, v / 2 - 4 - 8, 0xFFFFFF00);
    this->font->drawShadow(bottom_text, (u - this->font->getWidth(bottom_text)) / 2, v / 2 - 4 + 4, 0xFFFFFF00);

    this->update();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}


Minecraft minecraft = Minecraft(640, 480, false);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    minecraft.mouseButtonCallback(window, button, action, mods);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    minecraft.keyCallback(window, key, scancode, action, mods);
}

int main(void) {
    std::srand(std::time({}));
    minecraft.run();
}