#ifndef PLAYER_H
#define PLAYER_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include "level/level.h"
#include "phys/AABB.h"
#include "java/ArrayList.h"

typedef struct _Player {
    Level* level;
    float xo, yo, zo;
    float x, y, z;
    float xd, yd, zd;
    float yRot, xRot;
    AABB* bb;
    bool onGround;
} Player;

Player* newPlayer(Level* level);
void resetPos(Player* player);
void setPos(Player* player, float x, float y, float z);
void turn(Player* player, float xo, float yo);
void tick(Player* player);
void playerMove(Player* player, float xa, float ya, float za);
void playerMoveRelative(Player* player, float xa, float za, float speed);

#endif