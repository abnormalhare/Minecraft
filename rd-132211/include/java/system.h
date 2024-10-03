#ifndef SYSTEM_H
#define SYSTEM_H

#include <windows.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

typedef struct _Mouse {
    double currX;
    double currY;
    double prevX;
    double prevY;
    s32 button;
} Mouse;

extern Mouse* mouse;

bool setDisplayMode(s32 width, s32 height);
void displayUpdate(void);
void displayDestroy(void);
bool isCloseRequested(void);

void setMouseGrabbed(bool isGrabbed);
void mouseCreate(void);
float mouseGetDX(void);
float mouseGetDY(void);
void mouseDestroy(void);

#endif