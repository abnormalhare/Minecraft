#include "player.h"

Player* newPlayer(Level* level) {
    Player* player = malloc(sizeof(Player));
    if (!player) {
        fprintf(stderr, "Failed to allocate for player");
        return NULL;
    }


    player->level = level;
    player->onGround = FALSE;
    resetPos(player);

    return player;
}

#define RANDOM_FLOAT ((double)rand() / (double)RAND_MAX)

void resetPos(Player* player) {
    float x = RANDOM_FLOAT * (float)player->level->width;
    float y = player->level->depth + 10;
    float z = RANDOM_FLOAT * player->level->height;
    setPos(player, x, y, z);
}

void setPos(Player* player, float x, float y, float z) {
    player->x = x;
    player->y = y;
    player->z = z;
    float w = 0.3f;
    float h = 0.9f;
    player->bb = newAABB(x - w, y - h, z - w, x + w, y + h, z + w);
}

void turn(Player* player, float xo, float yo) {
    player->yRot += xo * 0.15;
    player->xRot -= yo * 0.15;
    if (player->xRot < -90.0f)
        player->xRot = -90.0f;
    if (player->xRot > 90.0f)
        player->xRot = 90.0f;
}

void tick(Player* player) {
    player->xo = player->x;
    player->yo = player->y;
    player->zo = player->z;

    float xa = 0.0f;
    float ya = 0.0f;
    if (GetAsyncKeyState('R')) {
        resetPos(player);
    }
    if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) {
        ya--;
    }
    if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) {
        ya++;
    }
    if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
        xa--;
    }
    if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
        xa++;
    }
    if (GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_LWIN)) {
        if (player->onGround)
            player->yd = 0.12f;
    }
    playerMoveRelative(player, xa, ya, player->onGround ? 0.02f : 0.005f);
    player->yd -= 0.005;
    playerMove(player, player->xd, player->yd, player->zd);
    
    player->xd *= 0.91f;
    player->yd *= 0.98f;
    player->zd *= 0.91f;
    if (player->onGround) {
        player->xd *= 0.8f;
        player->zd *= 0.8f;
    }
    printf("player: %f, %f, %f\n", player->x, player->y, player->z);
}

void playerMove(Player* player, float xa, float ya, float za) {
    AABB aABB;
    float xaOrg = xa;
    float yaOrg = ya;
    float zaOrg = za;
    ArrayList* aABBs = getCubes(player->level, expand(player->bb, xa, ya, za));

    s32 i;
    for (i = 0; i < aABBs->size; i++) {
        getAABB(aABBs, i, &aABB);
        ya = clipYCollide(&aABB, player->bb, ya);
    }
    AABBMove(player->bb, 0.0f, ya, 0.0f);
    for (i = 0; i < aABBs->size; i++) {
        getAABB(aABBs, i, &aABB);
        xa = clipXCollide(&aABB, player->bb, xa);
    }
    AABBMove(player->bb, xa, 0.0f, 0.0f);
    for (i = 0; i < aABBs->size; i++) {
        getAABB(aABBs, i, &aABB);
        za = clipZCollide(&aABB, player->bb, za);
    }
    AABBMove(player->bb, 0.0f, 0.0f, za);

    player->onGround = yaOrg != ya && yaOrg < 0.0f;

    if (xaOrg != xa) player->xd = 0.0f;
    if (yaOrg != ya) player->yd = 0.0f;
    if (zaOrg != za) player->zd = 0.0f;

    player->x = (player->bb->x0 + player->bb->x1) / 2.0f;
    player->y = player->bb->y0 + 1.62f;
    player->z = (player->bb->z0 + player->bb->z1) / 2.0f;
}

void playerMoveRelative(Player* player, float xa, float za, float speed) {
    float dist = xa * xa + za * za;
    if (dist < 0.01f) return;
    dist = speed / sqrtf(dist);
    xa *= dist;
    za *= dist;
    float rotSin = sin(player->yRot * M_PI / 180.0);
    float rotCos = cos(player->yRot * M_PI / 180.0);
    player->xd += xa * rotCos - za * rotSin;
    player->zd += za * rotCos + xa * rotSin;
}