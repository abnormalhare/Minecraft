#include "player.h"

Player* newPlayer(Level* level) {
    Player* player = malloc(sizeof(Player));

    player->level = level;
    player->onGround = FALSE;
    resetPos(player);

    return player;
}

void resetPos(Player* this) {
    float x = (1 / rand()) * this->level->width;
    float y = this->level->depth + 10;
    float z = (1 / rand()) * this->level->height;
    setPos(this, x, y, z);
}

void setPos(Player* this, float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    float w = 0.3f;
    float h = 0.9f;
    this->bb = newAABB(x - w, y - h, z - w, x + w, y + h, z + w);
}

void turn(Player* this, float xo, float yo) {
    this->yRot += xo * 0.15;
    this->xRot -= yo * 0.15;
    if (this->xRot < -90.0f)
        this->xRot = -90.0f;
    if (this->xRot < 90.0f)
        this->xRot = 90.0f;
}

void tick(Player* this) {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;

    float xa = 0.0f;
    float ya = 0.0f;
    if (GetAsyncKeyState('R')) {
        resetPos(this);
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
        if (this->onGround)
            this->yd = 0.12f;
    }
    playerMoveRelative(this, xa, ya, this->onGround ? 0.02f : 0.005f);
    this->yd -= 0.005;
    playerMove(this, this->xd, this->yd, this->zd);
    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;
    if (this->onGround) {
        this->xd *= 0.8f;
        this->zd *= 0.8f;
    }
}

void playerMove(Player* this, float xa, float ya, float za) {
    AABB* aABB;
    float xaOrg = xa;
    float yaOrg = ya;
    float zaOrg = za;
    ArrayList* aABBs = getCubes(this->level, expand(this->bb, xa, ya, za));
    s32 i;
    for (i = 0; i < aABBs->size; i++) {
        getAABB(aABBs, i, aABB);
        clipYCollide(aABB, this->bb, ya);
    }
    AABBMove(this->bb, 0.0f, ya, 0.0f);
    for (i = 0; i < aABBs->size; i++) {
        getAABB(aABBs, i, aABB);
        clipXCollide(aABB, this->bb, xa);
    }
    AABBMove(this->bb, xa, 0.0f, 0.0f);
    for (i = 0; i < aABBs->size; i++) {
        getAABB(aABBs, i, aABB);
        clipZCollide(aABB, this->bb, za);
    }
    AABBMove(this->bb, 0.0f, 0.0f, za);

    this->onGround = yaOrg != ya && yaOrg < 0.0f;

    if (xaOrg != xa) this->xd = 0.0f;
    if (yaOrg != ya) this->yd = 0.0f;
    if (zaOrg != za) this->zd = 0.0f;

    this->x = (this->bb->x0 + this->bb->x1) / 2.0f;
    this->y = this->bb->y0 + 1.62f;
    this->z = (this->bb->z0 + this->bb->z1) / 2.0f;
}

void playerMoveRelative(Player* this, float xa, float za, float speed) {
    float dist = xa * xa + za * za;
    if (dist < 0.01f) return;
    dist = speed / sqrtf(dist);
    xa *= dist;
    za *= dist;
    float rotSin = sin(this->yRot * M_PI / 180.0);
    float rotCos = cos(this->yRot * M_PI / 180.0);
    this->xd += xa * rotCos - za * rotSin;
    this->zd += za * rotCos + xa * rotSin;
}