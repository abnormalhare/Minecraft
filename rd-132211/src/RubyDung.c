#include "RubyDung.h"

bool FULLSCREEN_MODE = FALSE;

bool rbInit(RubyDung* this) {
    // non-code
    HINSTANCE hInstance = GetModuleHandle(NULL);

    intBufferInit(&this->viewportBuffer, 16);
    intBufferInit(&this->selectBuffer, 2000);
    this->hitResult = NULL;
    //

    s32 col = 0x0E0B0A;
    float fr = 0.5f;
    float fg = 0.8f;
    float fb = 1.0f;

    floatBufferInit(&this->fogColor, 4);
    floatBufferPut(&this->fogColor, 0, ((col >> 16) & 0xFF) / 255.0f);
    floatBufferPut(&this->fogColor, 1, ((col >> 8 ) & 0xFF) / 255.0f);
    floatBufferPut(&this->fogColor, 2, ((col      ) & 0xFF) / 255.0f);
    floatBufferPut(&this->fogColor, 3, 1.0f);
    floatBufferFlip(&this->fogColor);

    this->width = 1024;
    this->height = 768;
    if (setDisplayMode(this->width, this->height)) return 1;
    mouseCreate();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(fr, fg, fb, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    this->level = newLevel(256, 256, 64);
    this->levelRenderer = newLevelRenderer(this->level);
    this->player = newPlayer(this->level);

    setMouseGrabbed(TRUE);

    return 0;
}

void rbDestroy(RubyDung* this) {
    save(this->level);
    displayDestroy();
    mouseDestroy();
}

void rbRun(RubyDung* this) {
    if (rbInit(this)) {
        MessageBox(NULL, "Failed to Start RubyDung", "ERROR", MB_OK | MB_ICONSTOP);
        return;
    }

    s64 lastTime = currentTimeMillis();
    s32 frames = 0;

    while (!GetAsyncKeyState(VK_ESCAPE) && !isCloseRequested()) {
        advanceTime(this->timer);
        for (s32 i = 0; i < this->timer->ticks; i++) {
            tick(this->player);
        }
        
        rbRender(this, this->timer->a);
        frames++;
        printf("player: %f, %f, %f\n", this->player->x, this->player->y, this->player->z);
        while (currentTimeMillis() >= lastTime + 1000) {
            printf("%d fps, %d\n", frames, chunkUpdates);
            chunkUpdates = 0;
            lastTime += 1000;
            frames = 0;
        }
    }

    rbDestroy(this);
}

void moveCameraToPlayer(RubyDung* this, float a) {
    glTranslatef(0.0f, 0.0f, -0.3f);
    glRotatef(this->player->xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(this->player->yRot, 0.0f, 1.0f, 0.0f);
    float x = this->player->xo + (this->player->x - this->player->xo) * a;
    float y = this->player->yo + (this->player->y - this->player->yo) * a;
    float z = this->player->zo + (this->player->z - this->player->zo) * a;
    glTranslatef(-x, -y, -z);
}

void setupCamera(RubyDung* this, float a) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f, this->width / this->height, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(this, a);
}

void setupPickCamera(RubyDung* this, float a, s32 x, s32 y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    intBufferClear(&this->viewportBuffer);
    glGetIntegerv(GL_VIEWPORT, this->viewportBuffer.data);
    intBufferFlip(&this->viewportBuffer);
    intBufferLimit(&this->viewportBuffer, 16);
    gluPickMatrix(x, y, 5.0f, 5.0f, this->viewportBuffer.data);
    gluPerspective(70.0f, this->width / this->height, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(this, a);
}

void rbPick(RubyDung* this, float a) {
    intBufferClear(&this->selectBuffer);
    glSelectBuffer(2000, this->selectBuffer.data);
    glRenderMode(GL_SELECT);
    setupPickCamera(this, a, this->width / 2, this->height / 2);
    lrPick(this->levelRenderer, this->player);

    s32 hits = glRenderMode(GL_RENDER);
    intBufferFlip(&this->selectBuffer);
    intBufferLimit(&this->selectBuffer, this->selectBuffer.capacity);
    s64 closest = 0;
    s32 names[10];
    s32 hitNameCount = 0;
    for (s32 i = 0; i < hits; i++) {
        s32 nameCount = intBufferGet(&this->selectBuffer);
        s32 minZ = intBufferGet(&this->selectBuffer);
        intBufferGet(&this->selectBuffer);
        
        s32 dist = minZ;
        if (dist < closest || i == 0) {
            closest = dist;
            hitNameCount = nameCount;
            for (s32 j = 0; j < nameCount; j++) {
                names[j] = intBufferGet(&this->selectBuffer);
            }
        } else {
            for (s32 j = 0; j < nameCount; j++) {
                intBufferGet(&this->selectBuffer);
            }
        }
    }

    if (hitNameCount > 0) {
        this->hitResult = newHitResult(names[0], names[1], names[2], names[3], names[4]);
    } else {
        this->hitResult = NULL;
    }
}

void rbRender(RubyDung* this, float a) {
    float xo = mouseGetDX();
    float yo = mouseGetDY();
    turn(this->player, xo, yo);
    rbPick(this, a);
    
    glfwPollEvents();

    if (mouse->button == GLFW_MOUSE_BUTTON_LEFT && this->hitResult)
        setTile(this->level, this->hitResult->x, this->hitResult->y, this->hitResult->z, 0);
    if (mouse->button == GLFW_MOUSE_BUTTON_RIGHT && this->hitResult) {
        s32 x = this->hitResult->x;
        s32 y = this->hitResult->y;
        s32 z = this->hitResult->z;

        switch (this->hitResult->f) {
            case 0: y--; break;
            case 1: y++; break;
            case 2: z--; break;
            case 3: z++; break;
            case 4: x--; break;
            case 5: x++; break;
        }

        setTile(this->level, x, y, z, 1);
    }

    if (GetAsyncKeyState(VK_RETURN))
        save(this->level);
    glClear(0x4100);
    setupCamera(this, a);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, 2048);
    glFogf(GL_FOG_DENSITY, 0.2f);
    glFogfv(GL_FOG_COLOR, this->fogColor.data);
    glDisable(GL_FOG);
    lrRender(this->levelRenderer, this->player, 0);
    glEnable(GL_FOG);
    lrRender(this->levelRenderer, this->player, 1);
    glDisable(GL_TEXTURE_2D);
    if (this->hitResult)
        lrRenderHit(this->levelRenderer, this->hitResult);
    
    glDisable(GL_FOG);
    displayUpdate();
}

void main(void) {
    RubyDung* game = malloc(sizeof(RubyDung));

    floatBufferInit(&game->fogColor, 4);
    game->timer = newTimer(60.0f);

    initIdMap();
    initTiles();
    initChunk();

    printf("Running!\n");

    rbRun(game);
}