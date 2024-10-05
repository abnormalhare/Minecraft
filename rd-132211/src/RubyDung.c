#include "RubyDung.h"

bool FULLSCREEN_MODE = FALSE;

bool rbInit(RubyDung* game) {
    intBufferInit(&game->viewportBuffer, 16);
    intBufferInit(&game->selectBuffer, 2000);
    game->hitResult = NULL;

    s32 col = 0x0E0B0A;
    float fr = 0.5f;
    float fg = 0.8f;
    float fb = 1.0f;

    floatBufferInit(&game->fogColor, 4);
    floatBufferPut(&game->fogColor, 0, ((col >> 16) & 0xFF) / 255.0f);
    floatBufferPut(&game->fogColor, 1, ((col >> 8 ) & 0xFF) / 255.0f);
    floatBufferPut(&game->fogColor, 2, ((col      ) & 0xFF) / 255.0f);
    floatBufferPut(&game->fogColor, 3, 1.0f);
    floatBufferFlip(&game->fogColor);

    game->width = 1024;
    game->height = 768;
    if (setDisplayMode(game->width, game->height)) return 1;
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
    game->level = newLevel(256, 256, 64);
    game->levelRenderer = newLevelRenderer(game->level);
    game->player = newPlayer(game->level);

    setMouseGrabbed(TRUE);

    return 0;
}

void rbDestroy(RubyDung* game) {
    save(game->level);
    displayDestroy();
    mouseDestroy();
}

void rbRun(RubyDung* game) {
    if (rbInit(game)) {
        MessageBox(NULL, "Failed to Start RubyDung", "ERROR", MB_OK | MB_ICONSTOP);
        return;
    }

    s64 lastTime = currentTimeMillis();
    s32 frames = 0;

    while (!GetAsyncKeyState(VK_ESCAPE) && !isCloseRequested()) {
        advanceTime(game->timer);
        for (s32 i = 0; i < game->timer->ticks; i++) {
            tick(game->player);
        }
        
        rbRender(game, game->timer->a);
        frames++;
        while (currentTimeMillis() >= lastTime + 1000) {
            printf("%d fps, %d\n", frames, chunkUpdates);
            chunkUpdates = 0;
            lastTime += 1000;
            frames = 0;
        }
    }

    rbDestroy(game);
}

void moveCameraToPlayer(RubyDung* game, float a) {
    glTranslatef(0.0f, 0.0f, -0.3f);
    glRotatef(game->player->xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(game->player->yRot, 0.0f, 1.0f, 0.0f);
    float x = game->player->xo + (game->player->x - game->player->xo) * a;
    float y = game->player->yo + (game->player->y - game->player->yo) * a;
    float z = game->player->zo + (game->player->z - game->player->zo) * a;
    glTranslatef(-x, -y, -z);
}

void setupCamera(RubyDung* game, float a) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f, game->width / game->height, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(game, a);
}

void setupPickCamera(RubyDung* game, float a, s32 x, s32 y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    intBufferClear(&game->viewportBuffer);
    glGetIntegerv(GL_VIEWPORT, game->viewportBuffer.data);
    intBufferFlip(&game->viewportBuffer);
    intBufferLimit(&game->viewportBuffer, 16);
    gluPickMatrix(x, y, 5.0f, 5.0f, game->viewportBuffer.data);
    gluPerspective(70.0f, game->width / game->height, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(game, a);
}

void rbPick(RubyDung* game, float a) {
    intBufferClear(&game->selectBuffer);
    glSelectBuffer(2000, (u32*)game->selectBuffer.data);
    glRenderMode(GL_SELECT);
    setupPickCamera(game, a, game->width / 2, game->height / 2);
    lrPick(game->levelRenderer, game->player);

    s32 hits = glRenderMode(GL_RENDER);
    intBufferFlip(&game->selectBuffer);
    intBufferLimit(&game->selectBuffer, game->selectBuffer.capacity);
    s64 closest = 0;
    s32 names[10];
    s32 hitNameCount = 0;
    for (s32 i = 0; i < hits; i++) {
        s32 nameCount = intBufferGet(&game->selectBuffer);
        s32 minZ = intBufferGet(&game->selectBuffer);
        intBufferGet(&game->selectBuffer);
        
        s32 dist = minZ;
        if (dist < closest || i == 0) {
            closest = dist;
            hitNameCount = nameCount;
            for (s32 j = 0; j < nameCount; j++) {
                names[j] = intBufferGet(&game->selectBuffer);
            }
        } else {
            for (s32 j = 0; j < nameCount; j++) {
                intBufferGet(&game->selectBuffer);
            }
        }
    }

    if (hitNameCount > 0) {
        game->hitResult = newHitResult(names[0], names[1], names[2], names[3], names[4]);
    } else {
        game->hitResult = NULL;
    }
}

void rbRender(RubyDung* game, float a) {
    float xo = mouseGetDX();
    float yo = mouseGetDY();
    turn(game->player, xo, yo);
    rbPick(game, a);
    
    glfwPollEvents();

    if (mouse->button == GLFW_MOUSE_BUTTON_LEFT && game->hitResult)
        setTile(game->level, game->hitResult->x, game->hitResult->y, game->hitResult->z, 0);
    if (mouse->button == GLFW_MOUSE_BUTTON_RIGHT && game->hitResult) {
        s32 x = game->hitResult->x;
        s32 y = game->hitResult->y;
        s32 z = game->hitResult->z;

        switch (game->hitResult->f) {
            case 0: y--; break;
            case 1: y++; break;
            case 2: z--; break;
            case 3: z++; break;
            case 4: x--; break;
            case 5: x++; break;
        }

        setTile(game->level, x, y, z, 1);
    }

    if (GetAsyncKeyState(VK_RETURN))
        save(game->level);
    glClear(0x4100);
    setupCamera(game, a);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, 2048);
    glFogf(GL_FOG_DENSITY, 0.2f);
    glFogfv(GL_FOG_COLOR, game->fogColor.data);
    glDisable(GL_FOG);
    lrRender(game->levelRenderer, game->player, 0);
    glEnable(GL_FOG);
    lrRender(game->levelRenderer, game->player, 1);
    glDisable(GL_TEXTURE_2D);
    if (game->hitResult)
        lrRenderHit(game->levelRenderer, game->hitResult);
    
    glDisable(GL_FOG);
    displayUpdate();
}

int main(void) {
    RubyDung* game = malloc(sizeof(RubyDung));

    if (!game) {
        fprintf(stderr, "Failed to allocate for game");
        return -1;
    }

    floatBufferInit(&game->fogColor, 4);
    game->timer = newTimer(60.0f);

    initIdMap();
    initTiles();
    if (!initChunk()) return -2;

    printf("Running!\n");

    rbRun(game);

    return 0;
}